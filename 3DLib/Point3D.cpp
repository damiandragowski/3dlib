// Point3D.cpp: implementation of the Point3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Point3D.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace ThreeDimLib {

Point3D::Point3D(double x, double y, double z):point(x,y,z),user(0)
{
	hollow = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
}

Point3D::~Point3D()
{
	for ( int i = 0 ; i < user.size(); ++i )
		if ( user[i] ) 
			user[i]->ClearUse();
	user.clear();
}

void Point3D::DrawObject(HDC hdc, Matrix3D & Projection, Matrix3D & CameraModification, double cutSurfaceInterval)
{
	static POINT pt;
	static Vector3D temp;
	static HPEN oldPen;
	static HBRUSH oldBrush;

	temp = point*(CameraModification);

	if ( temp[2] <= cutSurfaceInterval )
		return;

	oldPen = (HPEN) SelectObject(hdc, ObjectPen);
	oldBrush = (HBRUSH) SelectObject(hdc, hollow);


	temp[3] = temp[2]*Projection[3][2]+1.0;
	pt.x = (long)(temp[0]/temp[3]);
	pt.y = (long)(temp[1]/temp[3]);
	
	Ellipse(hdc, pt.x-5, pt.y-5, pt.x+5, pt.y+5);

	SelectObject(hdc, oldPen);
	SelectObject(hdc, oldBrush);
}

void Point3D::Generate()
{
}

void Point3D::DrawObject3D(HDC hdc, Matrix3D & stereoLeft, Matrix3D & stereoRight, Matrix3D & CameraModification, double cutSurfaceInterval)
{
	static POINT pt;
	static Vector3D temp;
	static HPEN oldPen;
	static HBRUSH oldBrush;
	static int old;

	old = SetROP2(hdc, R2_MERGEPEN);
	oldBrush = (HBRUSH) SelectObject(hdc, hollow);
	temp = point*(CameraModification);

	if ( temp[2] <= cutSurfaceInterval ) {
		SelectObject(hdc, oldBrush);
		SelectObject(hdc, oldPen);
		SetROP2(hdc, old);
		return;
	}



	if ( selected )
		oldPen = (HPEN) SelectObject(hdc, leftSelectedObjectPen);
	else
		oldPen = (HPEN) SelectObject(hdc, leftObjectPen);

	temp[0] += temp[2]*stereoLeft[0][2];
	temp[3] = temp[2]*stereoLeft[3][2]+1;
	pt.x = (long)(temp[0]/temp[3]);
	pt.y = (long)(temp[1]/temp[3]);
	
	Ellipse(hdc, pt.x-5, pt.y-5, pt.x+5, pt.y+5);
	temp = point*(CameraModification);

	if ( temp[2] <= cutSurfaceInterval ) {
		SelectObject(hdc, oldBrush);
		SelectObject(hdc, oldPen);
		SetROP2(hdc, old);
		return;
	}

	if ( selected ) 
		SelectObject(hdc, rightSelectedObjectPen);
	else
		SelectObject(hdc, rightObjectPen);

	temp[0] += temp[2]*stereoRight[0][2];
	temp[3] = temp[2]*stereoRight[3][2]+1;
	pt.x = (long)(temp[0]/temp[3]);
	pt.y = (long)(temp[1]/temp[3]);
	
	Ellipse(hdc, pt.x-5, pt.y-5, pt.x+5, pt.y+5);

	SelectObject(hdc, oldBrush);
	SelectObject(hdc, oldPen);
	SetROP2(hdc, old);
}


}

