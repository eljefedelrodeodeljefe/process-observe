#ifdef _WIN32

int uv_process_children(DWORD ppid, uint32_t** proc_list, int* proc_count) {
  HANDLE process_snap;
  PROCESSENTRY32 pe32;
  uint32_t* temp = NULL;

  *proc_list = NULL;
  *proc_count = 0;
  /* snapshot of all processes */
  process_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if(process_snap == INVALID_HANDLE_VALUE)
    return 1;

  pe32.dwSize = sizeof(PROCESSENTRY32);
  /* retrieve information for root */
  if(!Process32First(process_snap, &pe32)) {
    CloseHandle(process_snap);
    return 1;
  }
  /* walk and push to results */
  do {
    if (pe32.th32ParentProcessID == ppid) {
      temp = uv__realloc(temp, (*proc_count + 1) * sizeof(uint32_t));
      if (temp == NULL) {
        CloseHandle(process_snap);
        return -ENOMEM;
      }
      temp[*proc_count] = (uint32_t)pe32.th32ProcessID;
      (*proc_count)++;
    }
  } while (Process32Next(process_snap, &pe32));

  CloseHandle(process_snap);

  *proc_list = temp;
  uv__free(temp);
  return 0;
}

#endif
