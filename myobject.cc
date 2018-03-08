// myobject.cc
#include "myobject.h"


namespace demo {
using v8::Exception;
using v8::Context;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::Persistent;
using v8::String;
using v8::Value;

Persistent<Function> MyObject::constructor;

MyObject::MyObject(double value) : value_(value) {
  card=-1;
  linked=false;
}

MyObject::~MyObject() {
}

void MyObject::Init(Local<Object> exports) {
  Isolate* isolate = exports->GetIsolate();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "MyObject"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "plusOne", PlusOne);
  NODE_SET_PROTOTYPE_METHOD(tpl, "link", Link);//getchanelVolt
  NODE_SET_PROTOTYPE_METHOD(tpl, "getchanelVolt", GetchanelVolt);
  constructor.Reset(isolate, tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "MyObject"),
               tpl->GetFunction());
}

void MyObject::New(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.IsConstructCall()) {
    // Invoked as constructor: `new MyObject(...)`
    double value = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
    MyObject* obj = new MyObject(value);
    obj->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
  } else {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    Local<Context> context = isolate->GetCurrentContext();
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    Local<Object> result =
        cons->NewInstance(context, argc, argv).ToLocalChecked();
    args.GetReturnValue().Set(result);
  }
}

void MyObject::PlusOne(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  MyObject* obj = ObjectWrap::Unwrap<MyObject>(args.Holder());
  obj->value_ += 1;

  args.GetReturnValue().Set(Number::New(isolate, obj->value_));
}
void MyObject::Link(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    MyObject* obj = ObjectWrap::Unwrap<MyObject>(args.Holder());
    I16 card;
    I16  err, card_num=0;
    //getch();
    printf("card=%d\n", obj->card);
    card=Register_Card (PCI_9111HR, card_num);
    if (card <0 ) {
        printf("Register_Card error=%d", card);
        obj->linked=false;
        return;
    }
    else{
        printf("card=%d", card);
        obj->card=card;
    }
    err = AI_9111_Config(card, TRIG_INT_PACER, 0, 1024);
    if (err!=0) {
       printf("AI_9111_Config error=%d", err);
       obj->linked=false;
       return;
    }
    obj->linked=true;
    args.GetReturnValue().Set(Number::New(isolate,obj->card));
}
void MyObject::GetchanelVolt(const FunctionCallbackInfo<Value>& args) {
  printf("GetchanelVolt\n");
  Isolate* isolate = args.GetIsolate();
  MyObject* obj = ObjectWrap::Unwrap<MyObject>(args.Holder());
  // Check the number of arguments passed.
  if (args.Length() < 1) {
    printf("<1");
    // Throw an Error that is passed back to JavaScript
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }

  // Check the argument types
  if (!args[0]->IsNumber() ) {
    printf("not number");
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }
  U16 Channel = args[0]->NumberValue() ;
  printf("card=%d", obj->card);
  printf("Channel=%d\n", Channel);
  F64 voltage;
  U16 AdRange=AD_B_10_V;
  I16 err=AI_VReadChannel(obj->card, Channel, AdRange , &voltage);
  if (err!=0) {
       printf("read error=%d\n", err);
       return;
    }
  printf("volt=%f\n", voltage);
}

}  // namespace demo
//I16 __stdcall AI_VReadChannel(U16 CardNumber, U16 Channel, U16 AdRange, F64 *voltage);