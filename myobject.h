// myobject.h
#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <node.h>
#include <node_object_wrap.h>
#include <windows.h>
#include "dask.h"

namespace demo
{

    class MyObject : public node::ObjectWrap
    {
    public:
        static void Init(v8::Local<v8::Object> exports);

    private:
        explicit MyObject(double value = 0);
        ~MyObject();

        static void New(const v8::FunctionCallbackInfo<v8::Value> &args);
        static void PlusOne(const v8::FunctionCallbackInfo<v8::Value> &args);
        static v8::Persistent<v8::Function> constructor;
        static void Link(const v8::FunctionCallbackInfo<v8::Value> &args);
        static void GetchanelVolt(const v8::FunctionCallbackInfo<v8::Value> &args);
        static void PutchanelVolt(const v8::FunctionCallbackInfo<v8::Value> &args);
        //static void PutD(const v8::FunctionCallbackInfo<v8::Value> &args);
        static void GetDO(const v8::FunctionCallbackInfo<v8::Value> &args);
        static void GetDI(const v8::FunctionCallbackInfo<v8::Value> &args);
        static void GetDOBit(const v8::FunctionCallbackInfo<v8::Value> &args);
        static void GetDIBit(const v8::FunctionCallbackInfo<v8::Value> &args);
        static void OpenVLV(const v8::FunctionCallbackInfo<v8::Value> &args);
        static void CloseVLV(const v8::FunctionCallbackInfo<v8::Value> &args);
        static void outputdo(MyObject *o);
        double value_;
        I16 card;
        U32 DO_Value;
        U32 DI_Value;
        I16 card6208;
        bool linked;
    };

}  // namespace demo
#endif
// Private Function CCard_openVLV2440(v As Integer) As Boolean
// If v >= 0 And v <= 7 Then
//     b1 = BitSetChar(b1, v)
//     'Call do9111(b1 + b2 * 256)
//     'Call newDO(0, b1)
//     Call c9111.newDO(0, b1 + CLng(b2) * 256)
//     CCard_openVLV2440 = True
// ElseIf v >= 8 And v <= 15 Then
//     b2 = BitSetChar(b2, v - 8)
//     'Call do9111(b1 + b2 * 256)
//     'Call newDO(1, b2)
//     Call c9111.newDO(0, b1 + CLng(b2) * 256)
//     CCard_openVLV2440 = True
// ElseIf v >= 16 And v <= 23 Then
//     b2 = BitSetChar(b2, v - 16)
//     'Call do9111(b1 + b2 * 256)
//     Call newDO(2, b2)
//     CCard_openVLV2440 = True
// ElseIf v >= 24 And v <= 31 Then
//     b2 = BitSetChar(b2, v - 24)
//     'Call do9111(b1 + b2 * 256)
//     Call newDO(3, b2)
//     CCard_openVLV2440 = True
// End If
// Sleep 2
// End Function
//clare Function AI_VReadChannel Lib "Pci-Dask.dll" (ByVal CardNumber As Integer, ByVal Channel As Integer, ByVal AdRange As Integer, Voltage As Double) As Integer
//I16 __stdcall AI_VReadChannel(U16 CardNumber, U16 Channel, U16 AdRange, F64 *voltage);
// Public Sub newDO(chanel As Byte, v As Long)
// If Not DEMO Then
//     chanel = P9111_CHANNEL_DO
//     v = DO_WritePort(card, chanel, v)
// End If
// End Sub
// Public Sub newDO(chanel As Byte, v As Byte)
//   c9111.newDO chanel, CLng(v)
// End Sub
// Public Sub Di(Port As Integer, v As Long)
//     c9111.Di Port, v
// End Sub
// Public Sub Di(Port As Integer, v As Long)
// If Not DEMO Then
//     Call DI_ReadPort(card, Port, v)
// End If
// End Sub
