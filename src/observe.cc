#include <nan.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysctl.h>
#include <unistd.h>
#include <inttypes.h>


extern "C" {

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

}


void GetChildren(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> obj = Nan::New<v8::Object>();
  double pid = info[0]->NumberValue();

  int pcount;
  uint32_t* plist;
  int err = process_children(pid, &plist, &pcount);
  if (err) {
    Nan::ThrowError("Error excuting getting children");
  }

  v8::Local<v8::Number> process_count = Nan::New(pcount);
  v8::Local<v8::Array> process_arr = Nan::New<v8::Array>(pcount);

  for (size_t i = 0; i < (size_t)pcount; i++) {
    process_arr->Set(i, Nan::New<v8::Number>(plist[i]));
  }

  obj->Set(Nan::New("pids").ToLocalChecked(), process_arr);
  obj->Set(Nan::New("count").ToLocalChecked(), process_count);

  free(plist);
  info.GetReturnValue().Set(obj);
}

void Init(v8::Local<v8::Object> exports) {
  exports->Set(Nan::New("children").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(GetChildren)->GetFunction());
}

NODE_MODULE(addon, Init)
