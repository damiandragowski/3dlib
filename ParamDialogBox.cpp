// ParamDialogBox.cpp: implementation of the ParamDialogBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ParamDialogBox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ParamDialogBox * ParamDialogBox::_this;

ParamDialogBox::ParamDialogBox(HWND hWnd):u(2),v(2)
{
	m_hParent = hWnd;
}

DWORD ParamDialogBox::Exec(HINSTANCE hInst)
{
	_this=this;
	return DialogBox(hInst, (LPCTSTR)IDD_DIALOG1, m_hParent, (DLGPROC)WndProc);
}

ParamDialogBox::~ParamDialogBox()
{

}


LRESULT CALLBACK ParamDialogBox::WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//ParamDialogBox * _this = (MainWindow *) GetWindowLong(hWnd, GWL_USERDATA);
	
	switch (iMsg)
	{
		case WM_INITDIALOG:
				//SetWindowLong(hWnd, GWL_USERDATA, (long)(this));	
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK) 
			{
				BOOL	Translate;
				int i=0;
				i = GetDlgItemInt(hWnd, IDC_EDIT1, &Translate, true);

				if ( Translate == FALSE || i <= 0 || i >= 17 ) {
					MessageBox(hWnd, TEXT("Podaj numer od 1 do 16"), TEXT("Error"), MB_OK);
					return TRUE;
				}
				_this->u=i;
				i = GetDlgItemInt(hWnd, IDC_EDIT2, &Translate, true);

				if ( Translate == FALSE || i <= 0 || i >= 17 ) {
					MessageBox(hWnd, TEXT("Podaj numer od 1 do 16"), TEXT("Error"), MB_OK);
					return TRUE;
				}

				_this->v=i;

				EndDialog(hWnd, LOWORD(wParam));
				return TRUE;
			}
			if ( LOWORD(wParam) == IDCANCEL )
			{
				EndDialog(hWnd, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
