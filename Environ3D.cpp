// Environ3D.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MainWindow.h"
#include "resource.h"


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR     lpCmdLine, int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	InitCommonControls();


	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDR_ENVIRON3DACC);
	MainWindow window(hInstance);
	window.Initialize();

	while (GetMessage(&msg, 0, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return 0;
}



