#ifndef PROCESS_OBSERVE_H
#define PROCESS_OBSERVE_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/sysctl.h>
#include <unistd.h>
#include <inttypes.h>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
  Data
 */

typedef struct p_obsrv_info_s p_obsrv_info_t;

struct p_obsrv_info_s {
  uint32_t    pid;
  uint32_t    ppid; // getppid on BSD
  char*       title;
  uint32_t    children_count;
  uint32_t*   children; /*array of pids */
  uint16_t*   group_id;
  uint16_t*   session_id;
  uint16_t*   tty_nr;
  uint16_t*   foreground_id;
};

/*
  API functions
 */

extern int process_children(pid_t ppid, uint32_t** proc_list, int* proc_count);
extern int process_info(pid_t ppid, p_obsrv_info_t* data);


#ifdef __cplusplus
}
#endif
#endif /* PROCESS_OBSERVE_H */
