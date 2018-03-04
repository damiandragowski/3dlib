// MainWindow.h: interface for the MainWindow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINWINDOW_H__CE4E3B47_08A2_43F8_9864_FB90842BDBA7__INCLUDED_)
#define AFX_MAINWINDOW_H__CE4E3B47_08A2_43F8_9864_FB90842BDBA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "3DLib/Mesh3D.h"
#include "ParamDialogBox.h"

using namespace  ThreeDimLib;

class MainWindow  
{
	// Mesh3D Variable
	Mesh3D	m_clsMesh;

	// keyboard press table
	bool	keys[256];
	// mouse keys
	bool	leftbuttondown;
	bool	rightbuttondown;
	bool	Mouse3DActive;
	POINT	oldMousePos;
	POINT	oldMousePos2;
	double	z;

	Vector3D	cursorPosition;
	Vector3D	oldcursorPosition;

	bool	CameraMode;
	bool	RotMode;
	bool	BezierMode;
	bool	BSplineMode;
	bool	InterpolMode;

	// Controls
	struct _Controls
	{
		// status bar
		HWND	StatusBar;
		string	statusBarTitle;
		_Controls() { }
	}Controls;

	// GDI Objects struct
	struct _GDIObjects
	{
		// Colors
		COLORREF background;
		COLORREF pen;
		// Objects
		HDC		compatibleDC;
		HBITMAP compatibleBitmap;
		HPEN	mainPen;
		HBRUSH	mainBrush;
		PAINTSTRUCT ps;
		~_GDIObjects() {
			DeleteObject(mainPen);
			DeleteObject(mainBrush);
			DeleteObject(compatibleBitmap);
			DeleteObject(compatibleDC);
		}
	} GDIObjects;
	// current window size
	int			m_iCurrentWidth;
	int			m_iCurrentHeight;


	bool		m_bCanDraw;
	HDC			m_hdc;
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;
	TCHAR		m_szClassName[MAX_LOADSTRING];
	TCHAR		m_szTitleName[MAX_LOADSTRING];
public:

	void Initialize();
	MainWindow(HINSTANCE hInst);
	virtual ~MainWindow();
	friend static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

private:
	void onRightButtonDown(WPARAM wParam, LPARAM lParam);
	void onRightButtonUp(WPARAM wParam, LPARAM lParam);
	void onMouseMove(WPARAM wParam, LPARAM lParam);
	// windows message methods
	void onSize(WPARAM wParam, LPARAM lParam);
	void onCreate(WPARAM, LPARAM);
	void onPaint(WPARAM wParam, LPARAM lParam);
	void onLeftButtonDown(WPARAM wParam, LPARAM lParam);
	void onLeftButtonUp(WPARAM wParam, LPARAM lParam);
	void onKeyUp();
	void onKeyDown();

	void RefreshGDIObjects();
	void InitGDIObject(HWND);
	void InitControls(HWND);
};

#endif // !defined(AFX_MAINWINDOW_H__CE4E3B47_08A2_43F8_9864_FB90842BDBA7__INCLUDED_)
