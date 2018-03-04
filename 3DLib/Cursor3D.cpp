// Cursor3D.cpp: implementation of the Cursor3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Cursor3D.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


namespace ThreeDimLib {

Cursor3D::Cursor3D(double x, double y, double z):point(x,y,z)
{

}


void Cursor3D::DrawObject(HDC hdc, Matrix3D & Projection, Matrix3D & CameraModification, double cutSurfaceInterval)
{
	static POINT pt;
	static Vector3D temp;
	static HPEN oldPen;

	temp = point*(CameraModification);

	if ( temp[2] <= cutSurfaceInterval )
		return;

	oldPen = (HPEN) SelectObject(hdc, ObjectPen);


	temp[3] = temp[2]*Projection[3][2]+1.0;
	pt.x = (long)(temp[0]/temp[3]);
	pt.y = (long)(temp[1]/temp[3]);
	
	Ellipse(hdc, pt.x-5, pt.y-5, pt.x+5, pt.y+5);

	SelectObject(hdc, oldPen);
}

void Cursor3D::Generate()
{
}

void Cursor3D::DrawObject3D(HDC hdc, Matrix3D & stereoLeft, Matrix3D & stereoRight, Matrix3D & CameraModification, double cutSurfaceInterval)
{
	static POINT pt;
	static Vector3D temp;
	static HPEN oldPen;
	static int old;
	static POINT tempPT;

	old = SetROP2(hdc, R2_MERGEPEN);

	temp = point*(CameraModification);

	if ( temp[2] <= cutSurfaceInterval ) {
		SelectObject(hdc, oldPen);
		SetROP2(hdc, old);
		return;
	}



	oldPen = (HPEN) SelectObject(hdc, leftSelectedObjectPen);

	temp[0] += temp[2]*stereoLeft[0][2];
	temp[3] = temp[2]*stereoLeft[3][2]+1;
	pt.x = (long)(temp[0]/temp[3]);
	pt.y = (long)(temp[1]/temp[3]);
	

	MoveToEx(hdc, pt.x, pt.y-10, &tempPT);
	LineTo(hdc, pt.x, pt.y+10);
	MoveToEx(hdc, pt.x-10, pt.y, &tempPT);
	LineTo(hdc, pt.x+10, pt.y);

//	Ellipse(hdc, pt.x-5, pt.y-5, pt.x+5, pt.y+5);

	temp = point*(CameraModification);

	if ( temp[2] <= cutSurfaceInterval ) {
		SelectObject(hdc, oldPen);
		SetROP2(hdc, old);
		return;
	}

	SelectObject(hdc, rightSelectedObjectPen);

	temp[0] += temp[2]*stereoRight[0][2];
	temp[3] = temp[2]*stereoRight[3][2]+1;
	pt.x = (long)(temp[0]/temp[3]);
	pt.y = (long)(temp[1]/temp[3]);
	
	MoveToEx(hdc, pt.x, pt.y-10, &tempPT);
	LineTo(hdc, pt.x, pt.y+10);
	MoveToEx(hdc, pt.x-10, pt.y, &tempPT);
	LineTo(hdc, pt.x+10, pt.y);

	SelectObject(hdc, oldPen);
	SetROP2(hdc, old);
}



Cursor3D::~Cursor3D()
{

}

}
