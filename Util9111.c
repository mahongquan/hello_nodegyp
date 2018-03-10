#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "dask.h"
#include "resource.h"

#define IDM_EXIT        110
#define IDM_SETUP       101
#define IDC_SCROLL      207
#define IDC_SPIN        301
#define IDC_START       208
#define IDC_STOP        209
#define IDC_PAUSE       210
#define IDC_EDIT        211
#define IDC_GAIN        212
#define IDC_DA          400
#define TIMER1            1
#define NONE              0

int PASCAL      WinMain(HANDLE, HANDLE, LPSTR, int);
long  PASCAL MainWndProc(HWND, UINT, WPARAM, LPARAM);

HANDLE  hInst;
HWND hAddrComboBox;
HWND hCardComboBox;
HWND hADScroll;
HWND hEdit;
HWND MainhWnd;

I16 card=-1, card_number = 0;
I16 input=0, output=0;
I16 oldinput=0,analog_input[16],adinput[16],old_adinput1[16],old_adinput2[16];
U8 input_lo=0, input_hi=0;
int Card_Type=PCI_9111DG;
I16 card_value[2] = {PCI_9111DG, PCI_9111HR};
LPSTR Card_Name[2]={"PCI9111DG", "PCI9111HR"};

LPSTR AD_Range[5]={"+/-10V","+/-5V","+/-2.5V","+/-1.25V","+/-0.625V"};
I16 range_value[5] = {AD_B_10_V, AD_B_5_V, AD_B_2_5_V, AD_B_1_25_V, AD_B_0_625_V};

HBRUSH  hLTGreenBrush,hGreenBrush,hBlueBrush,hYellowBrush,hRedBrush,hBlackBrush,hWhiteBrush,hLTRedBrush,hLTBlueBrush;
HPEN    hRedPen,hYellowPen,hGreenPen,hBluePen,hLTBluePen,hLTRedPen,hLTGreenPen,hWhitePen;

I16 sample_rate=10, time_interval, range=AD_B_5_V;

POINT input_pos[16]= {{470, 280},  {440, 280},
                       {410, 280},  {380, 280},
                       {350, 280},  {320, 280},
                       { 290, 280},  {260, 280},
                       {230, 280},  {200, 280},
                       {170, 280},  {140, 280},
                       {110, 280},  {80, 280},
                       { 50, 280},  { 20, 280},
                      };
POINT output_pos[16] ={
                       {470, 340},  {440, 340},
                       {410, 340},  {380, 340},
                       {350, 340},  {320, 340},
                       { 290, 340},  {260, 340},
                       {230, 340},  {200, 340},
                       {170, 340},  {140, 340},
                       {110, 340},  {80, 340},
                       { 50, 340},  { 20, 340},
                      };

POINT io_pos[16] ={
                       {395, 40},  {370, 40},
                       {345, 40},  {320, 40},
                       {295, 40},  {270, 40},
                       { 245, 40},  {220, 40},
                       {195, 40},  {170, 40},
                       {145, 40},  {120, 40},
                       {95, 40},  {70, 40},
                       { 45, 40},  { 20, 40},
                      };

BOOL bCheck1[8],bCheck2[8],bStart=FALSE,bStop=FALSE,bPause=FALSE;
BOOL InitApplication(HANDLE);
BOOL InitInstance(HANDLE, int);

int PASCAL WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow)
HANDLE hInstance, hPrevInstance;
LPSTR lpCmdLine;
int nCmdShow;
{
  MSG msg;

  if (!hPrevInstance)
     if (!InitApplication(hInstance))  return(FALSE);
  if (!InitInstance(hInstance, nCmdShow)) return(FALSE);
  while (GetMessage(&msg, NULL, NULL, NULL)) {
     TranslateMessage(&msg);
     DispatchMessage(&msg);
  }
  return(msg.wParam);
}

BOOL InitApplication(hInstance)
HANDLE hInstance;
{
  WNDCLASS wc;

  /* Main Window Class */
  wc.style = NULL;
  wc.lpfnWndProc = (WNDPROC)MainWndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = hInstance;
  wc.hIcon = LoadIcon(hInstance, "MyIcon");
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = GetStockObject(LTGRAY_BRUSH);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = "GenericWClass";
  if (!RegisterClass(&wc)) return(FALSE);
}


BOOL InitInstance(hInstance, nCmdShow)
HANDLE hInstance;
int nCmdShow;
{
  HWND hWnd;
  HMENU hSampleMenu;

  hInst = hInstance;
  hSampleMenu = LoadMenu(hInstance, "U7120Menu");
  hWnd = CreateWindow("GenericWClass",
                      (LPCSTR)"PCI-9111",
                      WS_OVERLAPPEDWINDOW,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      530,
                      480,
                      NULL,
                      hSampleMenu,
                      hInstance,
                      NULL);
  MainhWnd=hWnd;
  if (!hWnd) return(FALSE);
  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);
  return(TRUE);
}

BOOL PASCAL setup(HWND hWnd, unsigned message, WPARAM wParam, LPARAM lParam)
{
  int i,k;
  char n[16];
  char s[50];
      switch (message) {
         case WM_INITDIALOG:
              for (i=0;i<2;i++) {
                 SendMessage(GetDlgItem(hWnd,IDC_COMBO1), CB_ADDSTRING, NULL, (LONG)Card_Name[i]);
			  }
              SendMessage(GetDlgItem(hWnd,IDC_COMBO1) , CB_SETCURSEL, 0, 0L);
              for (i=0;i<16;i++) {
                wsprintf(n, "%d", i);
                _strupr(n);
                SendMessage(GetDlgItem(hWnd,IDC_COMBO2), CB_ADDSTRING, NULL, (LONG)(LPSTR) n);
              }
              SendMessage(GetDlgItem(hWnd,IDC_COMBO2) , CB_SETCURSEL, 0, 0L);
              break;
         case WM_COMMAND:
              switch(LOWORD(wParam)) {

                case IDC_COMBO1:
                     if (HIWORD(wParam) == CBN_CLOSEUP) {
                        i = (int)SendMessage(GetDlgItem(hWnd,IDC_COMBO1), CB_GETCURSEL, 0, 0L);
                        SendMessage(GetDlgItem(hWnd,IDC_COMBO1), CB_GETLBTEXT, i, (LPARAM)((LPCSTR)n));
						Card_Type = card_value[i];
                     }
                     return(TRUE);
                case IDC_COMBO2:
                     if (HIWORD(wParam) == CBN_CLOSEUP) {
                        i = (int)SendMessage(GetDlgItem(hWnd,IDC_COMBO2), CB_GETCURSEL, 0, 0L);
                        SendMessage(GetDlgItem(hWnd,IDC_COMBO2), CB_GETLBTEXT, i, (LPARAM)((LPCSTR)n));
                        card_number = i;
                     }
                     return(TRUE);
                case IDOK:
                     if ((card=Register_Card(Card_Type, card_number)) < 0) {
                        wsprintf(s, "Register_Card Error = %d\n", card);
                        MessageBox(hWnd, s, "Error", MB_OK);
                        SendMessage(hWnd, WM_CLOSE, 0, 0L);
                        break;
                     }
                     EndDialog(hWnd, TRUE);
                     InvalidateRect(hWnd, NULL, FALSE);
                     return (TRUE);
                case IDCANCEL:
                     EndDialog(hWnd, TRUE);
                     return (TRUE);
              }
              break;
      }
      return (FALSE);
}

long  PASCAL MainWndProc(hWnd, message, wParam, lParam)
HWND hWnd;
UINT message;
WPARAM wParam;
LPARAM lParam;
{
  HDC hDC;
  PAINTSTRUCT ps;
  int i, k, px, py;
  char n[15];
  double Pi = 3.1415926535;
  static U16 pos=0;

  switch(message) {
    case WM_CREATE:
           DialogBox(hInst,IDD_DIALOG2, hWnd, setup);
 		   if(card<0) {
		      SendMessage(hWnd, WM_CLOSE, 0, 0L);
		      break;
		   }
           for (i=0;i<8;i++) {
                  bCheck1[i]=FALSE;
                  bCheck2[i]=FALSE;
                  old_adinput1[i]=-1;
                  old_adinput2[i]=-1;
           }
           hAddrComboBox = CreateWindow("ComboBox", NULL,
               CBS_DROPDOWNLIST | WS_CHILD | WS_VISIBLE | WS_VSCROLL,
               output_pos[4].x+90, io_pos[0].y+70, 85, 120, hWnd, IDC_GAIN, hInst, NULL);
           for (i=0;i<5;i++) {
              SendMessage(hAddrComboBox, CB_ADDSTRING, NULL, (LONG)AD_Range[i]);
           }
           SendMessage(hAddrComboBox ,CB_SETCURSEL, 1, 0L);
           hADScroll=CreateWindow("SCROLLBAR", NULL,
                      WS_CHILD | WS_VISIBLE | SBS_HORZ,
                      output_pos[8].x+10, io_pos[15].y+200,190, 18, hWnd, IDC_SCROLL, hInst, NULL);
           SetScrollRange(hADScroll,SB_CTL,0,4095,TRUE);
           SetScrollPos(hADScroll,SB_CTL,0,TRUE);

           CreateWindow("BUTTON","Start" ,
                      WS_CHILD | WS_VISIBLE ,
                      output_pos[10].x+110, output_pos[10].y+50,50, 30, hWnd, IDC_START, hInst, NULL);

           CreateWindow("BUTTON","Stop"   ,
                      WS_CHILD | WS_VISIBLE ,
                      output_pos[10].x+170, output_pos[10].y+50,50, 30, hWnd, IDC_STOP, hInst, NULL);

           CreateWindow("BUTTON","Pause"   ,
                      WS_CHILD | WS_VISIBLE ,
                      output_pos[10].x+230, output_pos[10].y+50,50, 30, hWnd, IDC_PAUSE, hInst, NULL);

           CreateWindow("BUTTON","0"       ,
                      WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON | BS_LEFTTEXT ,
                      output_pos[4].x+50, io_pos[0].y-5,25,15, hWnd,IDC_DA, hInst, NULL);

           CreateWindow("BUTTON","1"       ,
                      WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON | BS_LEFTTEXT,
                      output_pos[4].x+50, io_pos[0].y+13,25,15, hWnd,IDC_DA+1, hInst, NULL);

           CreateWindow("BUTTON","2"       ,
                      WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON | BS_LEFTTEXT,
                      output_pos[4].x+50, io_pos[0].y+31,25,15, hWnd,IDC_DA+2, hInst, NULL);

           CreateWindow("BUTTON","3"       ,
                      WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON | BS_LEFTTEXT,
                      output_pos[4].x+50, io_pos[0].y+49,25,15, hWnd,IDC_DA+3, hInst, NULL);

           CreateWindow("BUTTON","4"       ,
                      WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON | BS_LEFTTEXT,
                      output_pos[4].x+100, io_pos[0].y-5,25,15, hWnd,IDC_DA+4, hInst, NULL);

           CreateWindow("BUTTON","5"       ,
                      WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON | BS_LEFTTEXT,
                      output_pos[4].x+100, io_pos[0].y+13,25,15, hWnd, IDC_DA+5, hInst, NULL);

           CreateWindow("BUTTON","6"       ,
                      WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON | BS_LEFTTEXT,
                      output_pos[4].x+100, io_pos[0].y+31,25,15, hWnd,IDC_DA+6, hInst, NULL);

           CreateWindow("BUTTON","7"       ,
                      WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON | BS_LEFTTEXT,
                      output_pos[4].x+100, io_pos[0].y+49,25,15, hWnd, IDC_DA+7, hInst, NULL);

           CreateWindow("BUTTON","0"       ,
                      WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON | BS_LEFTTEXT,
                      output_pos[4].x+50, io_pos[0].y+97,25,15, hWnd, IDC_DA+8, hInst, NULL);

           CreateWindow("BUTTON","1"       ,
                      WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON | BS_LEFTTEXT,
                      output_pos[4].x+50, io_pos[0].y+115,25,15, hWnd, IDC_DA+9, hInst, NULL);

           CreateWindow("BUTTON","2"       ,
                      WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON | BS_LEFTTEXT,
                      output_pos[4].x+50, io_pos[0].y+133,25,15, hWnd, IDC_DA+10, hInst, NULL);

           CreateWindow("BUTTON","3"       ,
                      WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON | BS_LEFTTEXT,
                      output_pos[4].x+50, io_pos[0].y+151,25,15, hWnd, IDC_DA+11, hInst, NULL);

           CreateWindow("BUTTON","4"       ,
                      WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON | BS_LEFTTEXT,
                      output_pos[4].x+100, io_pos[0].y+97,25,15, hWnd, IDC_DA+12, hInst, NULL);

           CreateWindow("BUTTON","5"       ,
                      WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON | BS_LEFTTEXT,
                      output_pos[4].x+100, io_pos[0].y+115,25,15, hWnd, IDC_DA+13, hInst, NULL);

           CreateWindow("BUTTON","6"       ,
                      WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON | BS_LEFTTEXT,
                      output_pos[4].x+100, io_pos[0].y+133,25,15, hWnd, IDC_DA+14, hInst, NULL);

           CreateWindow("BUTTON","7"       ,
                      WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON | BS_LEFTTEXT,
                      output_pos[4].x+100, io_pos[0].y+151,25,15, hWnd, IDC_DA+15, hInst, NULL);

           hLTGreenBrush = CreateSolidBrush(RGB(0, 255, 0));
           hGreenBrush = CreateSolidBrush(RGB(0, 128, 0));
           hBlueBrush = CreateSolidBrush(RGB(0, 0, 255));
           hRedBrush = CreateSolidBrush(RGB(255, 0, 0));
           hYellowBrush= CreateSolidBrush(RGB(255,255,0));
           hBlackBrush= CreateSolidBrush(RGB(0,0,0));
           hLTBlueBrush= CreateSolidBrush(RGB(0,255,255));
           hLTRedBrush= CreateSolidBrush(RGB(255,128,0));
           hWhiteBrush= CreateSolidBrush(RGB(255,255,255));
           hRedPen= CreatePen(PS_SOLID,1,RGB(255,0,0));
           hYellowPen= CreatePen(PS_SOLID,1,RGB(255,255,0));
           hBluePen= CreatePen(PS_SOLID,1,RGB(0,0,255));
           hGreenPen= CreatePen(PS_SOLID,1,RGB(0,255,0));
           hLTBluePen= CreatePen(PS_SOLID,1,RGB(0,128,255));
           hLTRedPen= CreatePen(PS_SOLID,1,RGB(255,128,0));
           hLTGreenPen= CreatePen(PS_SOLID,1,RGB(0,128,0));
           hWhitePen= CreatePen(PS_SOLID,1,RGB(255,255,255));
           break;

    case WM_PAINT:
           hDC = BeginPaint(hWnd, &ps);

           // Digital Input  Box
           SelectObject(hDC, hLTGreenBrush);
           Rectangle(hDC, input_pos[15].x-10, input_pos[15].y-14, input_pos[0].x+26, input_pos[0].y+40);
           SetTextColor(hDC,RGB(255,255,255));
           SetBkColor(hDC, RGB(0, 0, 255));
           TextOut(hDC, input_pos[15].x, input_pos[15].y-18, "Digital Input",13);

           // Digital Output Box
           SelectObject(hDC, hGreenBrush);
           Rectangle(hDC, output_pos[15].x-10, output_pos[15].y-14, output_pos[0].x+26, output_pos[0].y+40);
           SetTextColor(hDC,RGB(255,0,0));
           SetBkColor(hDC, RGB(0, 0, 255));
           TextOut(hDC, output_pos[15].x, output_pos[15].y-20, "Digital Output", 14);

           // Analog Input Box
           SelectObject(hDC, GetStockObject(GRAY_BRUSH));
           Rectangle(hDC, output_pos[15].x-10, io_pos[15].y-24, output_pos[0].x+26, io_pos[0].y+175);
           SetTextColor(hDC,RGB(255,255,0));
           SetBkColor(hDC, RGB(0, 0, 255));
           TextOut(hDC, io_pos[15].x, io_pos[15].y-35, "Analog Input(A/D)", 17);

           SelectObject(hDC, hBlackBrush);
           Rectangle(hDC, output_pos[15].x+35, io_pos[15].y-2, output_pos[4].x+41, io_pos[0].y+83);
           Rectangle(hDC, output_pos[15].x+35, io_pos[15].y+87, output_pos[4].x+41, io_pos[0].y+172);

           // Analog Output Box
           SelectObject(hDC, GetStockObject(GRAY_BRUSH));
           Rectangle(hDC, output_pos[15].x-10, io_pos[15].y+180, output_pos[0].x+26, io_pos[0].y+225);
           SetTextColor(hDC,RGB(255,255,0));
           SetBkColor(hDC, RGB(0, 0, 255));
           TextOut(hDC, io_pos[15].x, io_pos[15].y+175, "Analog Output(D/A)", 18);

           // Channel Label
           SetTextColor(hDC,RGB(0,0,0));
           SetBkColor(hDC, RGB(0, 255, 0));
           TextOut(hDC, input_pos[15].x, input_pos[15].y+20,   "15                                                                                                               0", 114);
           SetBkColor(hDC, RGB(0, 128, 0));
           TextOut(hDC, output_pos[15].x, output_pos[15].y+20, "15                                                                                                               0", 114);
           SetBkColor(hDC, RGB(0, 255, 0));

           SelectObject(hDC,hYellowBrush );
           Rectangle(hDC,output_pos[4].x+78, io_pos[0].y-5,output_pos[4].x+90, io_pos[0].y+10);
           Rectangle(hDC,output_pos[4].x+78, io_pos[0].y+97,output_pos[4].x+90, io_pos[0].y+112);
           SelectObject(hDC, hGreenBrush);
           Rectangle(hDC,output_pos[4].x+78, io_pos[0].y+13,output_pos[4].x+90, io_pos[0].y+28);
           Rectangle(hDC,output_pos[4].x+78, io_pos[0].y+115,output_pos[4].x+90, io_pos[0].y+130);
           SelectObject(hDC, hBlueBrush);
           Rectangle(hDC,output_pos[4].x+78, io_pos[0].y+31,output_pos[4].x+90, io_pos[0].y+46);
           Rectangle(hDC,output_pos[4].x+78, io_pos[0].y+133,output_pos[4].x+90, io_pos[0].y+148);
           SelectObject(hDC, hRedBrush);
           Rectangle(hDC,output_pos[4].x+78, io_pos[0].y+49,output_pos[4].x+90, io_pos[0].y+64);
           Rectangle(hDC,output_pos[4].x+78, io_pos[0].y+151,output_pos[4].x+90, io_pos[0].y+166);
           SelectObject(hDC, hLTBlueBrush);
           Rectangle(hDC,output_pos[4].x+128, io_pos[0].y-5,output_pos[4].x+140, io_pos[0].y+10);
           Rectangle(hDC,output_pos[4].x+128, io_pos[0].y+97,output_pos[4].x+140, io_pos[0].y+112);
           SelectObject(hDC, hLTRedBrush);
           Rectangle(hDC,output_pos[4].x+128, io_pos[0].y+13,output_pos[4].x+140, io_pos[0].y+28);
           Rectangle(hDC,output_pos[4].x+128, io_pos[0].y+115,output_pos[4].x+140, io_pos[0].y+130);
           SelectObject(hDC, hLTGreenBrush);
           Rectangle(hDC,output_pos[4].x+128, io_pos[0].y+31,output_pos[4].x+140, io_pos[0].y+46);
           Rectangle(hDC,output_pos[4].x+128, io_pos[0].y+133,output_pos[4].x+140, io_pos[0].y+148);
           SelectObject(hDC, hWhiteBrush);
           Rectangle(hDC,output_pos[4].x+128, io_pos[0].y+49,output_pos[4].x+140, io_pos[0].y+64);
           Rectangle(hDC,output_pos[4].x+128, io_pos[0].y+151,output_pos[4].x+140, io_pos[0].y+166);

           SetBkMode(hDC,TRANSPARENT);

           TextOut(hDC,output_pos[15].x, output_pos[15].y+56,"Card",4);
           TextOut(hDC,output_pos[15].x, output_pos[15].y+69,"Type",4);

           SetBkMode(hDC,TRANSPARENT);
           TextOut(hDC,output_pos[8].x+20, io_pos[15].y+182,"-10    -6     -2    2     6    10",33);
           TextOut(hDC,output_pos[4].x+50, io_pos[0].y+67,"Input",5);
           TextOut(hDC,output_pos[4].x+50, io_pos[0].y+80,"Range",5);
           TextOut(hDC,input_pos[15].x, io_pos[15].y-19,"Volts",5);
           switch (range) {
             case AD_B_10_V:
                       TextOut(hDC,input_pos[15].x-5, io_pos[15].y-5,"10.0",4);
                       TextOut(hDC,input_pos[15].x-8, io_pos[15].y+67,"-10.0",5);
                       TextOut(hDC,input_pos[15].x-5, io_pos[15].y+87,"10.0",4);
                       TextOut(hDC,input_pos[15].x-8, io_pos[15].y+159,"-10.0",5);
                       break;
             case AD_B_5_V:
                       TextOut(hDC,input_pos[15].x-5, io_pos[15].y-5,"5.00",4);
                       TextOut(hDC,input_pos[15].x-8, io_pos[15].y+67,"-5.00",5);
                       TextOut(hDC,input_pos[15].x-5, io_pos[15].y+87,"5.00",4);
                       TextOut(hDC,input_pos[15].x-8, io_pos[15].y+159,"-5.00",5);
                       break;
             case AD_B_2_5_V:
                       TextOut(hDC,input_pos[15].x-5, io_pos[15].y-5,"2.50",4);
                       TextOut(hDC,input_pos[15].x-8, io_pos[15].y+67,"-2.50",5);
                       TextOut(hDC,input_pos[15].x-5, io_pos[15].y+87,"2.50",4);
                       TextOut(hDC,input_pos[15].x-8, io_pos[15].y+159,"-2.50",5);
                       break;
             case AD_B_1_25_V:
                       TextOut(hDC,input_pos[15].x-5, io_pos[15].y-5,"1.25",4);
                       TextOut(hDC,input_pos[15].x-8, io_pos[15].y+67,"-1.25",5);
                       TextOut(hDC,input_pos[15].x-5, io_pos[15].y+87,"1.25",4);
                       TextOut(hDC,input_pos[15].x-8, io_pos[15].y+159,"-1.25",5);
                       break;
             case AD_B_0_625_V:
                       TextOut(hDC,input_pos[15].x-5, io_pos[15].y-5,"0.625",5);
                       TextOut(hDC,input_pos[15].x-8, io_pos[15].y+67,"-0.625",6);
                       TextOut(hDC,input_pos[15].x-5, io_pos[15].y+87,"0.625",5);
                       TextOut(hDC,input_pos[15].x-8, io_pos[15].y+159,"-0.625",6);
                       break;

             case AD_U_10_V:
                       TextOut(hDC,input_pos[15].x-5, io_pos[15].y-5,"10.0",4);
                       TextOut(hDC,input_pos[15].x-5, io_pos[15].y+67," 0.0",4);
                       TextOut(hDC,input_pos[15].x-5, io_pos[15].y+87,"10.0",4);
                       TextOut(hDC,input_pos[15].x-5, io_pos[15].y+159," 0.0",4);
                       break;
             case AD_U_5_V:
                       TextOut(hDC,input_pos[15].x-5, io_pos[15].y-5,"5.00",4);
                       TextOut(hDC,input_pos[15].x-5, io_pos[15].y+67,"0.00",4);
                       TextOut(hDC,input_pos[15].x-5, io_pos[15].y+87,"5.00",4);
                       TextOut(hDC,input_pos[15].x-5, io_pos[15].y+159,"0.00",4);
                       break;
             case AD_U_2_5_V:
                       TextOut(hDC,input_pos[15].x-5, io_pos[15].y-5,"2.50",4);
                       TextOut(hDC,input_pos[15].x-5, io_pos[15].y+67,"0.00",4);
                       TextOut(hDC,input_pos[15].x-5, io_pos[15].y+87,"2.50",4);
                       TextOut(hDC,input_pos[15].x-5, io_pos[15].y+159,"0.00",4);
                       break;
             case AD_U_1_25_V:
                       TextOut(hDC,input_pos[15].x-5, io_pos[15].y-5,"1.25",4);
                       TextOut(hDC,input_pos[15].x-5, io_pos[15].y+67,"0.00",4);
                       TextOut(hDC,input_pos[15].x-5, io_pos[15].y+87,"1.25",4);
                       TextOut(hDC,input_pos[15].x-5, io_pos[15].y+159,"0.00",4);
                       break;
           }

           TextOut(hDC,input_pos[10].x, io_pos[10].y-19,"Signals",7);
           TextOut(hDC,output_pos[4].x+50, io_pos[0].y-20,"Channel",7);
           SetBkMode(hDC,OPAQUE);

           // Detect Channel Input
           for (i=0; i<16; i++) {
              if ((input>>i)&0x01)
                 SelectObject(hDC, hRedBrush);
              else
                 SelectObject(hDC, GetStockObject(WHITE_BRUSH));
              Ellipse(hDC, input_pos[i].x, input_pos[i].y, input_pos[i].x+20, input_pos[i].y+20);
           }

           // Channel Output
           for (i=0; i<16; i++) {
              if ((output>>i)&0x01)
                 SelectObject(hDC, hRedBrush);
              else
                 SelectObject(hDC, GetStockObject(WHITE_BRUSH));
              Ellipse(hDC, output_pos[i].x, output_pos[i].y, output_pos[i].x+20, output_pos[i].y+20);
           }

           EndPaint(hWnd, &ps);
           break;

    case WM_HSCROLL:
           switch(LOWORD(wParam)) {
             case SB_THUMBPOSITION :
                  pos=HIWORD(wParam);
                  break;
           }
           SetScrollPos(lParam,SB_CTL,pos,TRUE);
           break;

    case WM_COMMAND:
        switch(LOWORD(wParam)) {
           case IDM_EXIT:
                SendMessage(hWnd, WM_CLOSE, 0, 0L);
                break;

           case IDC_START:
                time_interval=1000/sample_rate;
                SetTimer(hWnd, TIMER1, time_interval, NULL);
                srand((unsigned)time(NULL));
                bStop=FALSE;
                bPause=FALSE;
                bStart=TRUE;
                break;
           case IDC_STOP:
                KillTimer(hWnd, TIMER1);
                bStop=TRUE;
                bPause=FALSE;
                bStart=FALSE;
                hDC = GetDC(hWnd);
                SelectObject(hDC, hBlackBrush);
                Rectangle(hDC, output_pos[15].x+35, io_pos[15].y-2, output_pos[4].x+41, io_pos[0].y+83);
                Rectangle(hDC, output_pos[15].x+35, io_pos[15].y+87, output_pos[4].x+41, io_pos[0].y+172);
                ReleaseDC(hWnd, hDC);
                break;
           case IDC_PAUSE:
                KillTimer(hWnd, TIMER1);
                bStop=FALSE;
                bPause=TRUE;
                bStart=FALSE;
                break;
           case IDC_DA:
           case IDC_DA+1:
           case IDC_DA+2:
           case IDC_DA+3:
           case IDC_DA+4:
           case IDC_DA+5:
           case IDC_DA+6:
           case IDC_DA+7:
                k=LOWORD(wParam)-IDC_DA;
                if (bCheck1[k]) {
                    SendMessage(GetDlgItem(hWnd,wParam), BM_SETCHECK, 0, 0);
                    bCheck1[k]=FALSE;
                }
                else {
                    SendMessage(GetDlgItem(hWnd,wParam), BM_SETCHECK, 1, 0);
                    bCheck1[k]=TRUE;
                }
                break;
           case IDC_DA+8:
           case IDC_DA+9:
           case IDC_DA+10:
           case IDC_DA+11:
           case IDC_DA+12:
           case IDC_DA+13:
           case IDC_DA+14:
           case IDC_DA+15:
                k=LOWORD(wParam)-IDC_DA-8;
                if (bCheck2[k]) {
                         SendMessage(GetDlgItem(hWnd,wParam), BM_SETCHECK, 0, 0);
                         bCheck2[k]=FALSE;
                }
                else {
                         SendMessage(GetDlgItem(hWnd,wParam), BM_SETCHECK, 1, 0);
                         bCheck2[k]=TRUE;
                }
                break;
           case IDC_GAIN:
                if (HIWORD(wParam) == CBN_CLOSEUP) {
                    if ((i=(int)SendMessage(GetDlgItem(hWnd,IDC_GAIN),CB_GETCURSEL, 0, 0L))>=0 ) {
                       SendMessage(GetDlgItem(hWnd,IDC_GAIN), CB_GETLBTEXT, i, (LPARAM)((LPCSTR)n));
                       range = range_value[i];
                       InvalidateRect(hWnd, NULL, FALSE);
                    }
                }
                break;
           default:
                return(DefWindowProc(hWnd, message, wParam, lParam));
        }
        break;
    case WM_TIMER:
        if (card >= 0) {
           /* Digital Input */
           DI_ReadPort(card,0,&input);
           hDC = GetDC(hWnd);
           for (i=0; i<16; i++) {
               if ( ((input>>i)&0x01) != ((oldinput>>i)&0x01) ) {
                  if ((input>>i)&0x01)
                     SelectObject(hDC, hRedBrush);
                  else
                     SelectObject(hDC, GetStockObject(WHITE_BRUSH));
                  Ellipse(hDC, input_pos[i].x, input_pos[i].y, input_pos[i].x+20, input_pos[i].y+20);
               }
           }
           oldinput=input;
           /* Analog Input */
           BitBlt(hDC,output_pos[15].x+39,io_pos[15].y-2,331,85,hDC,output_pos[15].x+35,io_pos[15].y-2,SRCCOPY);
           for (i=0;i<8;i++ ) {
              if (bCheck1[i]) {
                  AI_ReadChannel(card, i, range, &analog_input[i]);
                  switch(i) {
                    case 0:
                         SelectObject(hDC, hYellowPen);
                         break;
                    case 1:
                         SelectObject(hDC, hGreenPen);
                         break;
                    case 2:
                         SelectObject(hDC, hBluePen);
                         break;
                    case 3:
                         SelectObject(hDC, hRedPen);
                         break;
                    case 4:
                         SelectObject(hDC, hLTBluePen);
                         break;
                    case 5:
                         SelectObject(hDC, hLTRedPen);
                         break;
                    case 6:
                         SelectObject(hDC, hLTGreenPen);
                         break;
                    case 7:
                         SelectObject(hDC, hWhitePen);
                         break;
                  }
                  if (Card_Type == PCI_9111DG) {
                   analog_input[i] = analog_input[i] >> 4;
                   adinput[i]=io_pos[0].y+42-(43*analog_input[i])/2048;
                  }
                  else {
                   adinput[i]=io_pos[0].y+42-(43*analog_input[i])/32767;
                  }
                  if (old_adinput1[i]!=-1 ) {
                     MoveToEx(hDC,output_pos[15].x+39,adinput[i],NULL );
                     LineTo(hDC,output_pos[15].x+43,old_adinput1[i]);
                  }
                  old_adinput1[i]=adinput[i];
              }
           }
           BitBlt(hDC,output_pos[15].x+39,io_pos[15].y+87,331,85,hDC,output_pos[15].x+35,io_pos[15].y+87,SRCCOPY);
           for (i=0;i<8;i++ ) {
              if (bCheck2[i]) {
                  AI_ReadChannel(card, i+8, range, &analog_input[i+8]);
                  switch(i) {
                    case 0:
                         SelectObject(hDC, hYellowPen);
                         break;
                    case 1:
                         SelectObject(hDC, hGreenPen);
                         break;
                    case 2:
                         SelectObject(hDC, hBluePen);
                         break;
                    case 3:
                         SelectObject(hDC, hRedPen);
                         break;
                    case 4:
                         SelectObject(hDC, hLTBluePen);
                         break;
                    case 5:
                         SelectObject(hDC, hLTRedPen);
                         break;
                    case 6:
                         SelectObject(hDC, hLTGreenPen);
                         break;
                    case 7:
                         SelectObject(hDC, hWhitePen);
                         break;
                  }

                  if (Card_Type == PCI_9111DG) {
                   analog_input[i+8] = analog_input[i+8] >> 4;
                   adinput[i+8]=io_pos[0].y+130-(43*analog_input[i+8])/2048;
                  }
                  else {
                   adinput[i+8]=io_pos[0].y+130-(43*analog_input[i+8])/32767;
                  }
                  if (old_adinput2[i]!=-1 ) {
                     MoveToEx(hDC,output_pos[15].x+39,adinput[i+8],NULL);
                     LineTo(hDC,output_pos[15].x+43,old_adinput2[i]);
                  }
                  old_adinput2[i]=adinput[i+8];
              }
           }
           ReleaseDC(hWnd, hDC);

           AO_WriteChannel(card, 0, pos);
        }
        break;
    case WM_DESTROY:
        if (card >= 0) Release_Card(card);
        DeleteObject(hRedBrush);
        DeleteObject(hGreenBrush);
        DeleteObject(hLTGreenBrush);
        DeleteObject(hYellowBrush);
        DeleteObject(hBlueBrush) ;
        DeleteObject(hBlackBrush);
        DeleteObject(hRedPen);
        DeleteObject(hYellowPen);
        DeleteObject(hBluePen);
        DeleteObject(hGreenPen);
        DeleteObject(hLTBluePen);
        DeleteObject(hLTRedPen);
        DeleteObject(hLTGreenPen);
        DeleteObject(hWhitePen);
        PostQuitMessage(0);
        break;
    case WM_LBUTTONDOWN:
        if (card >= 0) {
           px = LOWORD(lParam);
           py = HIWORD(lParam);
           for (i=0; i<16; i++) {
               if (px>output_pos[i].x && px<output_pos[i].x+17 && py>output_pos[i].y && py<output_pos[i].y+17) {
                  hDC = GetDC(hWnd);
                  if ((output>>i)&0x01) {
                     output -= (((unsigned int)1)<<i);
                     SelectObject(hDC, GetStockObject(WHITE_BRUSH));
                  }
                  else {
                     output += (((unsigned int)1)<<i);
                     SelectObject(hDC, hRedBrush);
                  }
                  Ellipse(hDC, output_pos[i].x, output_pos[i].y, output_pos[i].x+20, output_pos[i].y+20);
                                  DO_WritePort(card,0, output);
                  ReleaseDC(hWnd, hDC);
                  break;
               }
           }
        }
        break;
    default:
        return(DefWindowProc(hWnd, message, wParam, lParam));
  }
  return(NULL);
}
