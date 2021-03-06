#include "../process-observe.h"
#if defined(__APPLE__)    || \
    defined(__linux__)    || \
    defined(__NetBSD__)   || \
    defined(__OpenBSD__)  || \
    defined(__FreeBSD__)  || \
    defined(__DragonFly__)

#include <stdio.h>
#include <stdlib.h>
#include <sys/sysctl.h>
#include <unistd.h>
#include <inttypes.h>
#include <errno.h>


int process_children(pid_t ppid, uint32_t** proc_list, int* proc_count) {
  uint32_t* temp = NULL;

#if defined(__APPLE__)
  struct kinfo_proc *p_list = NULL;
  int ret, p_count, i;
  size_t len = 0;
  /* ref:
     http://unix.superglobalmegacorp.com/Net2/newsrc/sys/kinfo_proc.h.html */
  static const int name[] = { CTL_KERN, KERN_PROC, KERN_PROC_ALL, 0 };

  *proc_list = NULL;
  *proc_count = 0;
  /* get number of total processes for subsequent calls */
  ret = sysctl((int *)name, (sizeof(name) / sizeof(*name)) - 1, NULL, &len, NULL, 0);
  if (ret) return 1;

  p_list = (struct kinfo_proc*)malloc(len);
  if (!p_list) return 1;
  /* get the process list */
  ret = sysctl((int *)name, (sizeof(name) / sizeof(*name)) - 1, p_list, &len, NULL, 0);
  if (ret) {
    free(p_list);
    return 1;
  }


  p_count = len / sizeof(struct kinfo_proc);
  /* iterate though whole p_list */
  for (i = 0; i < p_count; i++) {
   /* if parent is ppid, push pid to array; increase counter */
   if ((uint32_t) p_list[i].kp_eproc.e_ppid == (uint32_t) ppid) {
     temp = (uint32_t*)realloc(temp, (*proc_count + 1) * sizeof(uint32_t));
     if (temp == NULL) {
       free(p_list);
       return -ENOMEM;
     }
     temp[*proc_count] = (uint32_t)p_list[i].kp_proc.p_pid;

     (*proc_count)++;
   }
  }
  free(p_list);
  *proc_list = temp;
  free(temp);

#elif defined(__linux__)
  char proc_p[256] = {0};
  FILE *fp = NULL;
  int match_pid;
  int count = 0;

  *proc_list = NULL;
  *proc_count = 0;
  /* Rationale: children are defined in thread with sames ID of process -> read,
   * check line endings, count, build array, return */
  sprintf(proc_p, "/proc/%u/task/%u/children", ppid, ppid);
  fp = fopen(proc_p, "r");
  if (fp == NULL)
    return 127;

  while (fscanf(fp, "%d", &match_pid) > 0) {
     temp = (uint32_t*)realloc(temp, (*proc_count + 1) * sizeof(uint32_t));
     if (temp == NULL) {
       fclose(fp);
       return -ENOMEM;
     }
     temp[*proc_count] = (uint32_t) match_pid;
     (*proc_count)++;
     count++;
  }
  fclose(fp);
  *proc_list = temp;
#endif

  return 0;
}


int process_info(pid_t pid, p_obsrv_info_t* data) {
  int pcount;
  uint32_t* plist;

  data->pid = pid;
  data->ppid = getppid();

  int err = process_children(pid, &plist, &pcount);
  if (err) {
    return 1;
  }
  data->children = plist;
  data->children_count = (uint32_t)pcount;
  return 0;
}

#endif
