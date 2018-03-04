// ursor3D.h: interface for the Cursor3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_URSOR3D_H__BCFF8A0C_6F8F_4B4D_B0F8_C747E2D9B70D__INCLUDED_)
#define AFX_URSOR3D_H__BCFF8A0C_6F8F_4B4D_B0F8_C747E2D9B70D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object3D.h"


namespace ThreeDimLib {

class Cursor3D : public Object3D  
{
	Vector3D point;
public:
	inline void setX(double x) { point[0]=x; }
	inline void setY(double y) { point[1]=y; }
	inline void setZ(double z) { point[2]=z; }

	virtual void DrawObject(HDC hdc, Matrix3D & Projection, Matrix3D & CameraModification, double cutSurfaceInterval);
	virtual void DrawObject3D(HDC hdc, Matrix3D & stereoLeft, Matrix3D & stereoRight, Matrix3D & CameraModification, double cutSurfaceInterval);
	virtual void Generate();

	Cursor3D(double =0, double =0, double =0);
	virtual ~Cursor3D();

};

}

#endif // !defined(AFX_URSOR3D_H__BCFF8A0C_6F8F_4B4D_B0F8_C747E2D9B70D__INCLUDED_)
