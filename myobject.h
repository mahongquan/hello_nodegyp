// myobject.h
#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <node.h>
#include <node_object_wrap.h>
#include <windows.h>
#include "dask.h"

namespace demo {

class MyObject : public node::ObjectWrap {
 public:
  static void Init(v8::Local<v8::Object> exports);

 private:
  explicit MyObject(double value = 0);
  ~MyObject();

  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void PlusOne(const v8::FunctionCallbackInfo<v8::Value>& args);
  static v8::Persistent<v8::Function> constructor;
  static void Link(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void GetchanelVolt(const v8::FunctionCallbackInfo<v8::Value>& args);
  double value_;
  I16 card;
  bool linked;
};

}  // namespace demo
#endif
//clare Function AI_VReadChannel Lib "Pci-Dask.dll" (ByVal CardNumber As Integer, ByVal Channel As Integer, ByVal AdRange As Integer, Voltage As Double) As Integer
//I16 __stdcall AI_VReadChannel(U16 CardNumber, U16 Channel, U16 AdRange, F64 *voltage);