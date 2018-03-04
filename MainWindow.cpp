// MainWindow.cpp: implementation of the MainWindow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "MainWindow.h"
#include "3Dlib/BSpline.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(HINSTANCE hInst):m_hInstance(hInst),leftbuttondown(false),Controls(), GDIObjects()
						,Mouse3DActive(false), rightbuttondown(false),CameraMode(false), RotMode(false), BezierMode(false), BSplineMode(false), InterpolMode(false)
{
	z=0.0;
	memset(keys, false, sizeof(bool)*256);
	m_bCanDraw = false;
	GDIObjects.pen = RGB(0,255,0);
	GDIObjects.background = RGB(0,0,0);

	LoadString(hInst, IDS_CLASSNAME, m_szClassName, MAX_LOADSTRING);
	LoadString(hInst, IDS_TITLENAME, m_szTitleName, MAX_LOADSTRING);

	WNDCLASSEX m_cWndClass;
	m_cWndClass.cbSize			= sizeof(m_cWndClass);
	m_cWndClass.style			= 0;
	m_cWndClass.lpfnWndProc		= MainWindow::WndProc;
	m_cWndClass.cbClsExtra		= 0;
	m_cWndClass.cbWndExtra		= 0;
	m_cWndClass.hInstance		= m_hInstance;
	m_cWndClass.hIcon			= LoadIcon(hInst, (LPCTSTR)IDI_MAINICON);
	m_cWndClass.hCursor			= LoadCursor(0, (LPCTSTR)IDC_ARROW);
	m_cWndClass.hbrBackground	= (HBRUSH) GetStockObject(BLACK_BRUSH);
	m_cWndClass.lpszMenuName	= (const char *) 0;
	m_cWndClass.lpszClassName	= m_szClassName;
	m_cWndClass.hIconSm			= LoadIcon(hInst, (LPCTSTR)IDI_MAINICON);

	if ( RegisterClassEx(&m_cWndClass) == 0 ) {
		PostQuitMessage(0);
		return;
	}

	m_hWnd = CreateWindow(m_szClassName, m_szTitleName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,	CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInst, (LPVOID)this);
	if ( m_hWnd == 0 ) {
		PostQuitMessage(0);
		return;
	}
	SetWindowLong(m_hWnd, GWL_USERDATA, (long)(this));	
	ShowWindow(m_hWnd, SW_SHOW);
}

MainWindow::~MainWindow()
{
	UnregisterClass(m_szClassName, m_hInstance);
}

LRESULT CALLBACK MainWindow::WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	MainWindow * _this = (MainWindow *) GetWindowLong(hWnd, GWL_USERDATA);
	switch (iMsg)
	{
		case WM_CREATE:
			_this = (MainWindow *) ((CREATESTRUCT*)lParam)->lpCreateParams;
			_this->InitGDIObject(hWnd);
			_this->InitControls(hWnd);
			_this->onCreate(wParam, lParam);
			break;
		case WM_LBUTTONDOWN:
			_this->onLeftButtonDown(wParam, lParam);
			break;
		case WM_RBUTTONUP:
			_this->onRightButtonUp(wParam, lParam);
			break;
		case WM_RBUTTONDOWN:
			_this->onRightButtonDown(wParam, lParam);
			break;
		case WM_LBUTTONUP:
			_this->onLeftButtonUp(wParam, lParam);
			break;
		case WM_KEYDOWN:
			_this->keys[wParam] = true;
			_this->onKeyDown();
			break;
		case WM_MOUSEMOVE:
			_this->onMouseMove(wParam, lParam);
			break;
		case WM_KEYUP:
			_this->keys[wParam] = false;
			_this->onKeyUp();
			break;
		case WM_SIZE:
			_this->onSize(wParam, lParam);
			break;
		case WM_PAINT:
			_this->onPaint(wParam, lParam);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	};
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}



void MainWindow::onCreate(WPARAM wParam, LPARAM lParam)
{
//	InitControls();
///	InitGDIObject(hWnd);
}

void MainWindow::InitControls(HWND hWnd)
{
	const int d[] = { 100, 200, 400, 500 };

	Controls.StatusBar  = CreateStatusWindow(WS_CHILD|WS_VISIBLE, "Damian", hWnd, IDC_STATUSBAR);
	SendMessage(Controls.StatusBar , SB_SETPARTS, 4, (long)d);
	SendMessage(Controls.StatusBar , SB_SETTEXT, 0, (long)"Normal Mode");
	SendMessage(Controls.StatusBar , SB_SETTEXT, 1, (long)" ");
	SendMessage(Controls.StatusBar , SB_SETTEXT, 2, (long)" ");
	SendMessage(Controls.StatusBar , SB_SETTEXT, 3, (long)" ");
}

void MainWindow::InitGDIObject(HWND hWnd)
{
	m_hdc = GetDC(hWnd);

	// creating objects
	GDIObjects.compatibleDC = CreateCompatibleDC(m_hdc); 
	GDIObjects.compatibleBitmap = CreateCompatibleBitmap(m_hdc, 0, 0);
	GDIObjects.mainBrush = CreateSolidBrush(GDIObjects.background);
	GDIObjects.mainPen = CreatePen(PS_SOLID, 1, GDIObjects.pen);
	// select new brush, pen and bitmap
	SelectObject(GDIObjects.compatibleDC, GDIObjects.compatibleBitmap);
	SelectObject(GDIObjects.compatibleDC, GDIObjects.mainBrush);
	SelectObject(GDIObjects.compatibleDC, GDIObjects.mainPen);

	ReleaseDC(hWnd, m_hdc);
}

void MainWindow::onPaint(WPARAM wParam, LPARAM lParam)
{
	RECT rt;

	m_hdc = BeginPaint(m_hWnd, &GDIObjects.ps);
	GetClientRect(m_hWnd, &rt);

	Rectangle(GDIObjects.compatibleDC, (-rt.right>>1)-1, (-rt.bottom>>1)-1, (rt.right>>1)+1, (rt.bottom>>1)+1);

	m_clsMesh.DrawObjects3D(GDIObjects.compatibleDC);	
	if ( Mouse3DActive )
		m_clsMesh.DrawCursor3D(GDIObjects.compatibleDC);	

	BitBlt(m_hdc, 0, 0, m_iCurrentWidth, m_iCurrentHeight, GDIObjects.compatibleDC, -rt.right>>1, -rt.bottom>>1, SRCCOPY);

	EndPaint(m_hWnd, &GDIObjects.ps);
}

void MainWindow::onSize(WPARAM wParam, LPARAM lParam)
{
	SendMessage(Controls.StatusBar, WM_SIZE, 0, 0 );

	m_iCurrentWidth = LOWORD(lParam);
	m_iCurrentHeight = HIWORD(lParam);

	// Refresh Context Devices
	RefreshGDIObjects();	
}

void MainWindow::RefreshGDIObjects()
{
	POINT oldPoint;

	m_hdc = GetDC(m_hWnd);

	GDIObjects.~_GDIObjects();
	GDIObjects.compatibleDC = CreateCompatibleDC(m_hdc);
	GDIObjects.compatibleBitmap = CreateCompatibleBitmap(m_hdc, m_iCurrentWidth, m_iCurrentHeight);
	GDIObjects.mainBrush = CreateSolidBrush(GDIObjects.background);
	GDIObjects.mainPen = CreatePen(PS_SOLID, 1, GDIObjects.pen);

	SetViewportOrgEx(GDIObjects.compatibleDC, m_iCurrentWidth>>1, m_iCurrentHeight>>1, &oldPoint);
	SetViewportOrgEx(m_hdc, m_iCurrentWidth>>1, m_iCurrentHeight>>1, &oldPoint);

	SelectObject(GDIObjects.compatibleDC, GDIObjects.compatibleBitmap);
	SelectObject(GDIObjects.compatibleDC, GDIObjects.mainBrush);
	SelectObject(GDIObjects.compatibleDC, GDIObjects.mainPen);

	ReleaseDC(m_hWnd, m_hdc);

}

void MainWindow::Initialize()
{
	/*
	m_clsMesh.AddPoint(-100, -100, 0);	
	m_clsMesh.AddPoint(-50, -100, 0);	
	m_clsMesh.AddPoint(0, -100, 0);	
	m_clsMesh.AddPoint(50, -100, 0);	
	m_clsMesh.AddPoint(100, -100, 0);	
	m_clsMesh.AddPoint(150, -100, 0);	
	m_clsMesh.AddPoint(200, -100, 0);	

	m_clsMesh.AddPoint(-100, -50, 50);	
	m_clsMesh.AddPoint(-50, -50, 50);	
	m_clsMesh.AddPoint(0, -50, 0);	
	m_clsMesh.AddPoint(50, -50, 0);	
	m_clsMesh.AddPoint(100, -50, 0);	
	m_clsMesh.AddPoint(150, -50, 0);	
	m_clsMesh.AddPoint(200, -50, 0);	

	m_clsMesh.AddPoint(-100, 0, 0);	
	m_clsMesh.AddPoint(-50, 0, -50);	
	m_clsMesh.AddPoint(0, 0, -50);	
	m_clsMesh.AddPoint(50, 0, 0);	
	m_clsMesh.AddPoint(100, 0, 0);	
	m_clsMesh.AddPoint(150, 0, 0);	
	m_clsMesh.AddPoint(200, 0, 0);	


	m_clsMesh.AddPoint(-100, 50, 0);	
	m_clsMesh.AddPoint(-50, 50, 0);	
	m_clsMesh.AddPoint(0, 50, 0);	
	m_clsMesh.AddPoint(50, 50, 0);	
	m_clsMesh.AddPoint(100, 50, 0);	
	m_clsMesh.AddPoint(150, 50, 0);	
	m_clsMesh.AddPoint(200, 50, 0);	

	m_clsMesh.AddPoint(-100, 100, 0);	
	m_clsMesh.AddPoint(-50, 100, 0);	
	m_clsMesh.AddPoint(0, 100, 0);	
	m_clsMesh.AddPoint(50, 100, 0);	
	m_clsMesh.AddPoint(100, 100, 0);	
	m_clsMesh.AddPoint(150, 100, 0);	
	m_clsMesh.AddPoint(200, 100, 0);	

	m_clsMesh.AddPoint(-100, 150, 0);	
	m_clsMesh.AddPoint(-50, 150, 0);	
	m_clsMesh.AddPoint(0, 150, 0);	
	m_clsMesh.AddPoint(50, 150, 0);	
	m_clsMesh.AddPoint(100, 150, 0);	
	m_clsMesh.AddPoint(150, 150, 0);	
	m_clsMesh.AddPoint(200, 150, 0);	

	m_clsMesh.AddPoint(-100, 200, 0);	
	m_clsMesh.AddPoint(-50, 200, 0);	
	m_clsMesh.AddPoint(0, 200, 0);	
	m_clsMesh.AddPoint(50, 200, 0);	
	
	m_clsMesh.AddPoint(100, 200, 0);	
	m_clsMesh.AddPoint(150, 200, 0);	
	m_clsMesh.AddPoint(200, 200, 0);	




	m_clsMesh.selectAll();
	*/
	// m_clsMesh.AddSphere(200, 10, 10);
	//m_clsMesh.AddTorus();

	m_clsMesh.setEyeWide(10);
	m_clsMesh.setCutSurface(-800);
	m_clsMesh.setDepthInterval(800);
}

void MainWindow::onKeyDown()
{
	if ( keys[0x4A] ) {
		keys[0x4A]=false;
		int count = m_clsMesh.getSelectedCount();
		if ( count < 3 ) {
			MessageBox(m_hWnd, TEXT("Powinieneœ zaznczyæ wiecej punktów kontrolnych"), TEXT("Informacja"), 0);
			return;
		}
		ParamDialogBox param(m_hWnd);
		if ( count == 3 ) {
			// tu dodajemy plaszczyzne generowana
			if( param.Exec(m_hInstance) == IDOK ) 
				m_clsMesh.GenerateSurfaceC0(param.u, param.v);
		} else {
			// tu dodajemy plaszczyzne z zaznaczonych punktow;
			if( param.Exec(m_hInstance) == IDOK ) {
				if ( count == (3*param.u+1) * (3*param.v+1) ) {
					m_clsMesh.AddBezierPatch(param.u, param.v);
				} else  {
					MessageBox(m_hWnd, TEXT("Powinieneœ zaznczyæ wiecej lub mniej punktów kontrolnych"), TEXT("Informacja"), 0);
					return;
				}
			} else 
				return;

		}
		RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
		InvalidateRect(m_hWnd, &rect, false);
	}

	if ( keys[0x4B] ) {
		keys[0x4B]=false;
		int count = m_clsMesh.getSelectedCount();
		if ( count < 3 ) {
			MessageBox(m_hWnd, TEXT("Powinieneœ zaznczyæ wiecej punktów kontrolnych"), TEXT("Informacja"), 0);
			return;
		}
		ParamDialogBox param(m_hWnd);
		if ( count == 3 ) {
			// tu dodajemy plaszczyzne generowana
			if( param.Exec(m_hInstance) == IDOK ) 
				m_clsMesh.GenerateSurfaceBSplinePoints(param.u, param.v);
		} else {
			// tu dodajemy plaszczyzne z zaznaczonych punktow;
			if( param.Exec(m_hInstance) == IDOK ) {
				if ( count == (3+param.u) * (3+param.v) ) {
					m_clsMesh.AddBezierPatch(param.u, param.v);
				} else  {
					MessageBox(m_hWnd, TEXT("Powinieneœ zaznczyæ wiecej lub mniej punktów kontrolnych"), TEXT("Informacja"), 0);
					return;
				}
			} else 
				return;

		}
		RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
		InvalidateRect(m_hWnd, &rect, false);
	}

	if ( keys[0x4C]) {
		keys[0x4C]=false;
		ParamDialogBox param(m_hWnd);
		// tu dodajemy plaszczyzne generowana
		if( param.Exec(m_hInstance) == IDOK ) 
		{
			if ( true )
				m_clsMesh.GenerateSurfaceBSplinePipe(param.u, param.v, true);
			else
				m_clsMesh.GenerateSurfaceBSplinePipe(param.u, param.v, false);
		}
		RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
		InvalidateRect(m_hWnd, &rect, false);
	}

	if ( keys[0x35]) {
		keys[0x35]=false;
		m_clsMesh.GenerateDryerObject();
		RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
		InvalidateRect(m_hWnd, &rect, false);
	}
	if ( keys[0x36]) {
		keys[0x36]=false;
		m_clsMesh.RefreshDryerObject();
		RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
		InvalidateRect(m_hWnd, &rect, false);
	}


	if ( keys[VK_F3] ) {
		keys[VK_F3]=false;
		m_clsMesh.PointsUnion();
		m_clsMesh.Refresh();
		RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
		InvalidateRect(m_hWnd, &rect, false);
	}

	if ( keys[VK_F1] ) {
		keys[VK_F1]=false;
		m_clsMesh.deselectAll();
		RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
		InvalidateRect(m_hWnd, &rect, false);
	}
	if ( keys[VK_F2] ) {
		keys[VK_F2]=false;
		m_clsMesh.selectAll();
		RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
		InvalidateRect(m_hWnd, &rect, false);
	}


	if ( keys[0x42] ) {
		keys[0x42]=false;
		m_clsMesh.AddBezier();
		RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
		InvalidateRect(m_hWnd, &rect, false);
	}
	if ( keys[0x4E] ) {
		keys[0x4E]=false;
		m_clsMesh.AddBSpline();
		RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
		InvalidateRect(m_hWnd, &rect, false);
	}

	if ( keys[0x4D] ) {
		keys[0x4D]=false;
		m_clsMesh.AddInterpol();
		RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
		InvalidateRect(m_hWnd, &rect, false);
	}

	if ( keys[0x4F] ) {
		keys[0x4F]=false;
		m_clsMesh.ChangeBSplineMode();
		m_clsMesh.Refresh();
		RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
		InvalidateRect(m_hWnd, &rect, false);
	}

	if ( keys[VK_SPACE] ) {
		Mouse3DActive = !Mouse3DActive;
		ShowCursor(!Mouse3DActive);
		InvalidateRect(m_hWnd, 0, false);
	} 
	if ( keys[0x43] ) {
		CameraMode = !CameraMode;
		if ( CameraMode ) 
			SendMessage(Controls.StatusBar, SB_SETTEXT, 0, (long)"Camera Mode");
		else {
			if ( RotMode ) 
				SendMessage(Controls.StatusBar, SB_SETTEXT, 0, (long)"Rot Model Mode");
			else 
				SendMessage(Controls.StatusBar, SB_SETTEXT, 0, (long)"Normal Mode");
		}
	} 

	if ( keys[0x31] ) {
		BezierMode = !BezierMode;
		if ( BezierMode )
			m_clsMesh.CreateBezier();
		else
			m_clsMesh.AddBezierToObject();
	} 

	if ( keys[0x32] ) {
		BSplineMode = !BSplineMode;
		if ( BSplineMode )
			m_clsMesh.CreateBSpline();
		else
			m_clsMesh.AddBSplineToObject();
	} 

	if ( keys[0x33] ) {
		InterpolMode = !InterpolMode;
		if ( InterpolMode )
			m_clsMesh.CreateInterpol();
		else
			m_clsMesh.AddInterpolToObject();
	} 



	if ( keys[0x56] ) {
		RotMode = !RotMode;
		if ( RotMode ) 
			SendMessage(Controls.StatusBar, SB_SETTEXT, 0, (long)"Rot Model Mode");
		else {
			if ( CameraMode ) 
				SendMessage(Controls.StatusBar, SB_SETTEXT, 0, (long)"Camera Mode");
			else
				SendMessage(Controls.StatusBar, SB_SETTEXT, 0, (long)"Normal Mode");
		}
	} 

	if ( !BSplineMode && !InterpolMode && !BezierMode && keys[VK_DELETE] ) {
		m_clsMesh.deleteSelected();
		RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
		InvalidateRect(m_hWnd, &rect, false);
	}

	if ( Mouse3DActive ) {
		std::ostringstream buffer2;		
		if ( keys[0x57] ) { // W
			z+=TRANSSTEP5;
			cursorPosition = m_clsMesh.CountPointer3D(oldMousePos.x-(m_iCurrentWidth>>1), oldMousePos.y-(m_iCurrentHeight>>1), z);
			buffer2 << "position3D: " << cursorPosition[0] << ", " << cursorPosition[1] << ", " << cursorPosition[2];
			Controls.statusBarTitle=buffer2.str();
			SendMessage(Controls.StatusBar, SB_SETTEXT, 2, (long)Controls.statusBarTitle.c_str());

			RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
			InvalidateRect(m_hWnd, &rect, false);

		}
		if ( keys[0x53] ) { // S
			z-=TRANSSTEP5;
			cursorPosition = m_clsMesh.CountPointer3D(oldMousePos.x-(m_iCurrentWidth>>1), oldMousePos.y-(m_iCurrentHeight>>1), z);
			buffer2 << "position3D: " << cursorPosition[0] << ", " << cursorPosition[1] << ", " << cursorPosition[2];
			Controls.statusBarTitle=buffer2.str();
			SendMessage(Controls.StatusBar, SB_SETTEXT, 2, (long)Controls.statusBarTitle.c_str());

			RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
			InvalidateRect(m_hWnd, &rect, false);
		}
	} else {


		if ( CameraMode ) {
			if ( keys[0x5A]  ) { // Z
				m_clsMesh.setEyeWide(m_clsMesh.getEyeWide()+1.0);
				RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
				InvalidateRect(m_hWnd, &rect, false);
			}
			if ( keys[0x58]  ) { // X
				m_clsMesh.setEyeWide(m_clsMesh.getEyeWide()-1.0);
				RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
				InvalidateRect(m_hWnd, &rect, false);
			}
			if ( keys[0x51]  ) { // Q
				m_clsMesh.RotZ(-PI75);
				RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
				InvalidateRect(m_hWnd, &rect, false);
			}
			if ( keys[0x45]  ) { // E
				m_clsMesh.RotZ(PI75);
				RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
				InvalidateRect(m_hWnd, &rect, false);
			}

			if ( keys[0x57] ) { // W
				m_clsMesh.TransZ(-TRANSSTEP);
				RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
				InvalidateRect(m_hWnd, &rect, false);
			}
			if ( keys[0x53] ) { // S
				m_clsMesh.TransZ(TRANSSTEP);
				RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
				InvalidateRect(m_hWnd, &rect, false);
			}

			if ( keys[0x52] ) { // R
				m_clsMesh.TransY(-TRANSSTEP);
				RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
				InvalidateRect(m_hWnd, &rect, false);
			}
			if ( keys[0x46] ) { // F
				m_clsMesh.TransY(TRANSSTEP);
				RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
				InvalidateRect(m_hWnd, &rect, false);
			}


			if ( keys[0x41] ) { // A
				m_clsMesh.TransX(-TRANSSTEP);
				RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
				InvalidateRect(m_hWnd, &rect, false);
			}
			if ( keys[0x44] ) { // D
				m_clsMesh.TransX(TRANSSTEP);
				RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
				InvalidateRect(m_hWnd, &rect, false);
			}
		} // end CameraMode


		if ( RotMode ) {
			if ( keys[0x51]  ) { // Q
				m_clsMesh.RotZSelected(-PI75);
				m_clsMesh.Refresh();
				RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
				InvalidateRect(m_hWnd, &rect, false);
			}
			if ( keys[0x45]  ) { // E
				m_clsMesh.RotZSelected(PI75);
				m_clsMesh.Refresh();
				RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
				InvalidateRect(m_hWnd, &rect, false);
			}

			if ( keys[0x57] ) { // W
				m_clsMesh.TransSelected(0,0,-TRANSSTEP);
				m_clsMesh.Refresh();
				RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
				InvalidateRect(m_hWnd, &rect, false);
			}
			if ( keys[0x53] ) { // S
				m_clsMesh.TransSelected(0,0,TRANSSTEP);
				m_clsMesh.Refresh();
				RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
				InvalidateRect(m_hWnd, &rect, false);
			}

			if ( keys[0x52] ) { // R
				m_clsMesh.TransSelected(0,-TRANSSTEP,0);
				m_clsMesh.Refresh();
				RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
				InvalidateRect(m_hWnd, &rect, false);
			}
			if ( keys[0x46] ) { // F
				m_clsMesh.TransSelected(0,TRANSSTEP, 0);
				m_clsMesh.Refresh();
				RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
				InvalidateRect(m_hWnd, &rect, false);
			}


			if ( keys[0x41] ) { // A
				m_clsMesh.TransSelected(-TRANSSTEP, 0, 0);
				m_clsMesh.Refresh();
				RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
				InvalidateRect(m_hWnd, &rect, false);
			}
			if ( keys[0x44] ) { // D
				m_clsMesh.TransSelected(TRANSSTEP, 0, 0);
				m_clsMesh.Refresh();
				RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
				InvalidateRect(m_hWnd, &rect, false);
			}
		} // end RotMode


	}

	if ( RotMode ) {
		if ( keys[0x51]  ) { // Q
			m_clsMesh.RotZSelected(-PI75);
			m_clsMesh.Refresh();
			RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
			InvalidateRect(m_hWnd, &rect, false);
		}
		if ( keys[0x45]  ) { // E
			m_clsMesh.RotZSelected(PI75);
			m_clsMesh.Refresh();
			RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
			InvalidateRect(m_hWnd, &rect, false);
		}

		if ( keys[0x57] ) { // W
			m_clsMesh.TransSelected(0,0,-TRANSSTEP);
			m_clsMesh.Refresh();
			RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
			InvalidateRect(m_hWnd, &rect, false);
		}
		if ( keys[0x53] ) { // S
			m_clsMesh.TransSelected(0,0,TRANSSTEP);
			m_clsMesh.Refresh();
			RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
			InvalidateRect(m_hWnd, &rect, false);
		}

		if ( keys[0x52] ) { // R
			m_clsMesh.TransSelected(0,-TRANSSTEP,0);
			m_clsMesh.Refresh();
			RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
			InvalidateRect(m_hWnd, &rect, false);
		}
		if ( keys[0x46] ) { // F
			m_clsMesh.TransSelected(0,TRANSSTEP, 0);
			m_clsMesh.Refresh();
			RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
			InvalidateRect(m_hWnd, &rect, false);
		}


		if ( keys[0x41] ) { // A
			m_clsMesh.TransSelected(-TRANSSTEP, 0, 0);
			m_clsMesh.Refresh();
			RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
			InvalidateRect(m_hWnd, &rect, false);
		}
		if ( keys[0x44] ) { // D
			m_clsMesh.TransSelected(TRANSSTEP, 0, 0);
			m_clsMesh.Refresh();
			RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
			InvalidateRect(m_hWnd, &rect, false);
		}
	} // end RotMode


}

void MainWindow::onKeyUp()
{

}

void MainWindow::onLeftButtonDown(WPARAM wParam, LPARAM lParam)
{
	if ( Mouse3DActive ) {
		if ( !InterpolMode && !BSplineMode && !BezierMode && keys[VK_SHIFT] ) {
			m_clsMesh.AddPoint(cursorPosition[0], cursorPosition[1], cursorPosition[2]);
		} else if (!InterpolMode && !BezierMode && !BSplineMode)
			m_clsMesh.selectObject(cursorPosition);
		if ( BezierMode ) {
			m_clsMesh.AddPointToBezier(cursorPosition[0], cursorPosition[1], cursorPosition[2]);
		} 
		if ( BSplineMode ) {
			m_clsMesh.AddPointToBSpline(cursorPosition[0], cursorPosition[1], cursorPosition[2]);
		} 
		if ( InterpolMode ) {
			m_clsMesh.AddPointToInterpol(cursorPosition[0], cursorPosition[1], cursorPosition[2]);
		} 
		
	}
	leftbuttondown=true;
	oldMousePos.x = LOWORD(lParam);
	oldMousePos.y = HIWORD(lParam);
	SetCapture(m_hWnd);
}

void MainWindow::onLeftButtonUp(WPARAM wParam, LPARAM lParam)
{
	leftbuttondown=false;
	ReleaseCapture();
}

void MainWindow::onMouseMove(WPARAM wParam, LPARAM lParam)
{
	POINT mousePos;
	std::ostringstream buffer;


	mousePos.x=LOWORD(lParam);
	mousePos.y=HIWORD(lParam);
	buffer << "position: " << mousePos.x-(m_iCurrentWidth>>1) << ", " << mousePos.y-(m_iCurrentHeight>>1);
	Controls.statusBarTitle=buffer.str();
	SendMessage(Controls.StatusBar, SB_SETTEXT, 1, (long)Controls.statusBarTitle.c_str());



	if ( Mouse3DActive ) {

		if ( keys[VK_CONTROL] ) {
			if ( oldMousePos.y-HIWORD(lParam) < 0 ) 
				z+=5;
			else if ( oldMousePos.y-HIWORD(lParam) > 0 ) 
				z-=5;
		} 

		std::ostringstream buffer2;
		oldcursorPosition = m_clsMesh.CountPointer3D(oldMousePos.x-(m_iCurrentWidth>>1), oldMousePos.y-(m_iCurrentHeight>>1), z);
		cursorPosition = m_clsMesh.CountPointer3D(mousePos.x-(m_iCurrentWidth>>1), mousePos.y-(m_iCurrentHeight>>1), z);

		buffer2 << "position3D: " << cursorPosition[0] << ", " << cursorPosition[1] << ", " << cursorPosition[2];
		Controls.statusBarTitle=buffer2.str();
		SendMessage(Controls.StatusBar, SB_SETTEXT, 2, (long)Controls.statusBarTitle.c_str());

		RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
		InvalidateRect(m_hWnd, &rect, false);
	}

	if ( CameraMode ) {
		if ( leftbuttondown ) {
			if ( oldMousePos.x-LOWORD(lParam) ) {
				double beta = (oldMousePos.x-LOWORD(lParam))*0.02;
				m_clsMesh.RotY(beta);
				m_clsMesh.Refresh();
			} 
			if ( oldMousePos.y-HIWORD(lParam) ) {
				double beta = (oldMousePos.y-HIWORD(lParam))*0.02;
				m_clsMesh.RotX(-beta);				
				m_clsMesh.Refresh();
			}
			RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
			InvalidateRect(m_hWnd, &rect, false);
		}
	}

	if ( RotMode ) {
		if ( leftbuttondown ) {
			if ( oldMousePos.x-LOWORD(lParam) ) {
				double beta = (oldMousePos.x-LOWORD(lParam))*0.02;
				m_clsMesh.RotYSelected(beta);
				m_clsMesh.Refresh();
			} 
			if ( oldMousePos.y-HIWORD(lParam) ) {
				double beta = (oldMousePos.y-HIWORD(lParam))*0.02;
				m_clsMesh.RotXSelected(-beta);				
				m_clsMesh.Refresh();		
			}
			RECT rect={0, 0, m_iCurrentWidth , m_iCurrentHeight-20};
			InvalidateRect(m_hWnd, &rect, false);
		}
	}

	if ( rightbuttondown && Mouse3DActive ) {
		Vector3D temp = oldcursorPosition-cursorPosition;
		m_clsMesh.TransSelected(-temp[0], -temp[1], -temp[2]);
		m_clsMesh.Refresh();
	}
	oldMousePos2.x = oldMousePos.x = LOWORD(lParam);
	oldMousePos2.y = oldMousePos.y = HIWORD(lParam);
}

void MainWindow::onRightButtonUp(WPARAM wParam, LPARAM lParam)
{
	rightbuttondown=false;

	oldMousePos2.x = LOWORD(lParam);
	oldMousePos2.y = HIWORD(lParam);

	ReleaseCapture();
}

void MainWindow::onRightButtonDown(WPARAM wParam, LPARAM lParam)
{
	rightbuttondown=true;

	SetCapture(m_hWnd);
}
