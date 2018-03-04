// Object3D.cpp: implementation of the Object3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Object3D.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace ThreeDimLib {

void Object3D::DrawObject3D(HDC hdc, Matrix3D & stereoLeft, Matrix3D & stereoRight, Matrix3D & CameraModification, double cutSurfaceInterval)
{
	static int i;
	static POINT pt[4];
	static Vector3D temp;
	static bool skip1, skip2, skip3, skip4;
	static POINT p;
	static double a,b;
	static double c,d;
	a=stereoLeft[0][2];
	b=stereoLeft[3][2];
	c=stereoRight[0][2];
	d=stereoRight[3][2];

	
	static HPEN oldPen; 

	int old = SetROP2(hdc, R2_MERGEPEN);

	for ( i=0; i < size;  ) {
		if ( selected )
			oldPen = (HPEN) SelectObject(hdc, leftSelectedObjectPen);
		else
			oldPen = (HPEN) SelectObject(hdc, leftObjectPen);

		temp = points[i]*(CameraModification*Modification);
		skip1 = false;
		if ( temp[2] < cutSurfaceInterval )
			skip1 = true;

		temp[0] += temp[2]*a;
		temp[3] = temp[2]*b+1;
		pt[0].x = (long)(temp[0]/temp[3]);
		pt[0].y = (long)(temp[1]/temp[3]);

		++i;

		temp = points[i]*(CameraModification*Modification);
		skip2 = false;
		if ( temp[2] < cutSurfaceInterval )
			skip2 = true;

		temp[0] += temp[2]*a;
		temp[3] = temp[2]*b+1;
		pt[1].x = (long)(temp[0]/temp[3]);
		pt[1].y = (long)(temp[1]/temp[3]);
		++i;

		temp = points[i]*(CameraModification*Modification);
		skip3 = false;
		if ( temp[2] < cutSurfaceInterval )
			skip3 = true;

		temp[0] += temp[2]*a;
		temp[3] = temp[2]*b+1;
		pt[2].x = (long)(temp[0]/temp[3]);
		pt[2].y = (long)(temp[1]/temp[3]);
		++i;

		temp = points[i]*(CameraModification*Modification);
		skip4 = false;
		if ( temp[2] < cutSurfaceInterval )
			skip4 = true;

		temp[0] += temp[2]*a;
		temp[3] = temp[2]*b+1;
		pt[3].x = (long)(temp[0]/temp[3]);
		pt[3].y = (long)(temp[1]/temp[3]);
		++i;

		if ( !skip1 && !skip2 ) {
  			MoveToEx(hdc, pt[0].x, pt[0].y, &p);
			LineTo(hdc, pt[1].x, pt[1].y);  			
		} 
		if ( !skip2 && !skip3 ) {
  			MoveToEx(hdc, pt[1].x, pt[1].y, &p);
			LineTo(hdc, pt[2].x, pt[2].y);  			
		} 
		if ( !skip3 && !skip4 ) {
  			MoveToEx(hdc, pt[2].x, pt[2].y, &p);
			LineTo(hdc, pt[3].x, pt[3].y);  			
		} 
		if ( !skip1 && !skip4 ) {
  			MoveToEx(hdc, pt[3].x, pt[3].y, &p);
			LineTo(hdc, pt[0].x, pt[0].y);  			
		}


		// right
		i-=4;
		if ( selected ) 
			SelectObject(hdc, rightSelectedObjectPen);
		else
			SelectObject(hdc, rightObjectPen);

		temp = points[i]*(CameraModification*Modification);
		skip1 = false;
		if ( temp[2] < cutSurfaceInterval )
			skip1 = true;

		temp[0] += temp[2]*c;
		temp[3] = temp[2]*d+1;
		pt[0].x = (long)(temp[0]/temp[3]);
		pt[0].y = (long)(temp[1]/temp[3]);

		++i;

		temp = points[i]*(CameraModification*Modification);
		skip2 = false;
		if ( temp[2] < cutSurfaceInterval )
			skip2 = true;

		temp[0] += temp[2]*c;
		temp[3] = temp[2]*d+1;
		pt[1].x = (long)(temp[0]/temp[3]);
		pt[1].y = (long)(temp[1]/temp[3]);
		++i;

		temp = points[i]*(CameraModification*Modification);
		skip3 = false;
		if ( temp[2] < cutSurfaceInterval )
			skip3 = true;

		temp[0] += temp[2]*c;
		temp[3] = temp[2]*d+1;
		pt[2].x = (long)(temp[0]/temp[3]);
		pt[2].y = (long)(temp[1]/temp[3]);
		++i;

		temp = points[i]*(CameraModification*Modification);
		skip4 = false;
		if ( temp[2] < cutSurfaceInterval )
			skip4 = true;

		temp[0] += temp[2]*c;
		temp[3] = temp[2]*d+1;
		pt[3].x = (long)(temp[0]/temp[3]);
		pt[3].y = (long)(temp[1]/temp[3]);
		++i;

		if ( !skip1 && !skip2 ) {
  			MoveToEx(hdc, pt[0].x, pt[0].y, &p);
			LineTo(hdc, pt[1].x, pt[1].y);  			
		} 
		if ( !skip2 && !skip3 ) {
  			MoveToEx(hdc, pt[1].x, pt[1].y, &p);
			LineTo(hdc, pt[2].x, pt[2].y);  			
		} 
		if ( !skip3 && !skip4 ) {
  			MoveToEx(hdc, pt[2].x, pt[2].y, &p);
			LineTo(hdc, pt[3].x, pt[3].y);  			
		} 
		if ( !skip1 && !skip4 ) {
  			MoveToEx(hdc, pt[3].x, pt[3].y, &p);
			LineTo(hdc, pt[0].x, pt[0].y);  			
		}


	}
	SetROP2(hdc, old);
	SelectObject(hdc, oldPen);
}

void Object3D::DrawObject(HDC hdc, Matrix3D & Projection, Matrix3D & CameraModification, double cutSurfaceInterval)
{
	static int i;
	static POINT pt[4];
	static Vector3D temp;
	static bool skip1, skip2, skip3, skip4;
	static POINT p;
	
	static HPEN oldPen;
	oldPen = (HPEN) SelectObject(hdc, ObjectPen);
	for ( i=0; i < size;  ) {
		temp = points[i]*(Modification*CameraModification);
		skip1 = false;
		if ( temp[2] <= cutSurfaceInterval )
			skip1 = true;

		temp[3] = temp[2]*Projection[3][2]+1;
		pt[0].x = (long)(temp[0]/temp[3]);
		pt[0].y = (long)(temp[1]/temp[3]);
		++i;

		temp = points[i]*(Modification*CameraModification);
		skip2 = false;
		if ( temp[2] <= cutSurfaceInterval )
			skip2 = true;

		temp[3] = temp[2]*Projection[3][2]+1;
		pt[1].x = (long)(temp[0]/temp[3]);
		pt[1].y = (long)(temp[1]/temp[3]);
		++i;

		temp = points[i]*(Modification*CameraModification);
		skip3 = false;
		if ( temp[2] <= cutSurfaceInterval )
			skip3 = true;

		temp[3] = temp[2]*Projection[3][2]+1;
		pt[2].x = (long)(temp[0]/temp[3]);
		pt[2].y = (long)(temp[1]/temp[3]);
		++i;

		temp = points[i]*(Modification*CameraModification);
		skip4 = false;
		if ( temp[2] <= cutSurfaceInterval )
			skip4 = true;

		temp[3] = temp[2]*Projection[3][2]+1;
		pt[3].x = (long)(temp[0]/temp[3]);
		pt[3].y = (long)(temp[1]/temp[3]);
		++i;

		if ( !skip1 && !skip2 ) {
  			MoveToEx(hdc, pt[0].x, pt[0].y, &p);
			LineTo(hdc, pt[1].x, pt[1].y);  			
		} 
		if ( !skip2 && !skip3 ) {
  			MoveToEx(hdc, pt[1].x, pt[1].y, &p);
			LineTo(hdc, pt[2].x, pt[2].y);  			
		} 
		if ( !skip3 && !skip4 ) {
  			MoveToEx(hdc, pt[2].x, pt[2].y, &p);
			LineTo(hdc, pt[3].x, pt[3].y);  			
		} 
		if ( !skip1 && !skip4 ) {
  			MoveToEx(hdc, pt[3].x, pt[3].y, &p);
			LineTo(hdc, pt[0].x, pt[0].y);  			
		}
	}
	SelectObject(hdc, oldPen);
}


Object3D::~Object3D()
{
	DeleteObject(ObjectPen);
	DeleteObject(leftObjectPen);
	DeleteObject(rightObjectPen);
	if ( points )
		delete [] points;
}

}