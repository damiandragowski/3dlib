// ParamDialogBox.h: interface for the ParamDialogBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARAMDIALOGBOX_H__3D9FB6FA_4E62_4293_83CE_80122940FFEA__INCLUDED_)
#define AFX_PARAMDIALOGBOX_H__3D9FB6FA_4E62_4293_83CE_80122940FFEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ParamDialogBox  
{
	HWND		m_hParent;
	HWND		m_hWnd;
public:
	int			u,v;
	ParamDialogBox(HWND hWnd);
	DWORD Exec(HINSTANCE hInst);
	virtual ~ParamDialogBox();
	friend static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
	static ParamDialogBox *_this;
};

#endif // !defined(AFX_PARAMDIALOGBOX_H__3D9FB6FA_4E62_4293_83CE_80122940FFEA__INCLUDED_)
