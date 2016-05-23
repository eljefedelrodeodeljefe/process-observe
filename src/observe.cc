#include <nan.h>
#include <stdio.h>
#include <stdlib.h>

#include "process-observe.h"


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

  info.GetReturnValue().Set(obj);
}

void Info(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  v8::Local<v8::Object> obj = Nan::New<v8::Object>();
  double pid = info[0]->NumberValue();

  p_obsrv_info_t data;

  int err = process_info(pid, &data);
  if (err) {
    Nan::ThrowError("Error excuting getting children");
  }

  obj->Set(Nan::New("pid").ToLocalChecked(), Nan::New<v8::Number>(data.pid));
  obj->Set(Nan::New("parent_pid").ToLocalChecked(), Nan::New<v8::Number>(data.ppid));
  obj->Set(Nan::New("children_count").ToLocalChecked(), Nan::New<v8::Number>(data.children_count));


  v8::Local<v8::Array> process_arr = Nan::New<v8::Array>(data.children_count);
  for (size_t i = 0; i < (size_t)data.children_count; i++) {
    process_arr->Set(i, Nan::New<v8::Number>(data.children[i]));
  }
  obj->Set(Nan::New("children").ToLocalChecked(), process_arr);

  info.GetReturnValue().Set(obj);
}

void Init(v8::Local<v8::Object> exports) {
  exports->Set(Nan::New("children").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(GetChildren)->GetFunction());
  exports->Set(Nan::New("info").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(Info)->GetFunction());
}

NODE_MODULE(addon, Init)
