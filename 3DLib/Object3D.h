// Object3D.h: interface for the Object3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECT3D_H__BAECB757_37A4_4C7E_8377_AC2F45A8CF92__INCLUDED_)
#define AFX_OBJECT3D_H__BAECB757_37A4_4C7E_8377_AC2F45A8CF92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "matrix3d.h"

namespace ThreeDimLib {


class Object3D  
{
protected:
	Matrix3D	Modification;
	Matrix3D	InvertModification;
	Vector3D *	points;
	int			size;
	int			ObjectWireSize;
	HPEN		ObjectPen;
	COLORREF	ObjectColor;
	HPEN		leftObjectPen;
	COLORREF	leftObjectColor;
	HPEN		rightObjectPen;
	COLORREF	rightObjectColor;

	// selected
	HPEN		leftSelectedObjectPen;
	COLORREF	leftSelectedObjectColor;

	HPEN		rightSelectedObjectPen;
	COLORREF	rightSelectedObjectColor;

public:
	bool		selected;
	bool		deleteTHIS;
	bool		changed;

	virtual void userUnion(vector<class Object3D *> & user)
	{
	}


	Object3D():points(0), size(0), ObjectWireSize(1), ObjectColor(RGB(0,255,0)), leftObjectColor(RGB(0,150,0)),
		rightObjectColor(RGB(150,0,0)), selected(false), leftSelectedObjectColor(RGB(0,255,0)), 
		rightSelectedObjectColor(RGB(255,0,0)),deleteTHIS(false), changed(false)
	{ 
		Modification.LoadIdentity();
		InvertModification.LoadIdentity();
		ObjectPen = CreatePen(PS_SOLID, ObjectWireSize, ObjectColor);
		leftObjectPen = CreatePen(PS_SOLID, ObjectWireSize, leftObjectColor);
		rightObjectPen = CreatePen(PS_SOLID, ObjectWireSize, rightObjectColor);

		leftSelectedObjectPen = CreatePen(PS_SOLID, ObjectWireSize, leftSelectedObjectColor);
		rightSelectedObjectPen = CreatePen(PS_SOLID, ObjectWireSize, rightSelectedObjectColor);

	}
	virtual void ReplaceRef(Object3D * t1, Object3D * t2)
	{
		
	}
	virtual void ChangeMode() { }
	virtual void ClearUse() {  } 

	virtual double operator[] ( int index ) {
		if ( points )
			return 0.0;
		return 0.0;
	}

	virtual bool isNear(Vector3D & temp)
	{
		Vector3D tmp;
		if ( points )
			for ( register int i = 0; i < size; ++i ) { 
				Vector3D t1=points[i]*Modification;
				tmp = t1-temp;
				if ( tmp.L1Norm() < 10 ) 
					return true;
			}
		return false;
	}

	virtual void ReCalculate() { }
	// set methods
	inline void setWireColor(COLORREF & color) { 
		ObjectColor = color; 
		DeleteObject(ObjectPen);
		ObjectPen = CreatePen(PS_SOLID, ObjectWireSize, ObjectColor);
	}
	inline void setLeftWireColor(COLORREF & color) { 
		leftObjectColor = color; 
		DeleteObject(leftObjectPen);
		ObjectPen = CreatePen(PS_SOLID, ObjectWireSize, leftObjectColor);
	}
	inline void setRightWireColor(COLORREF & color) { 
		rightObjectColor = color; 
		DeleteObject(rightObjectPen);
		ObjectPen = CreatePen(PS_SOLID, ObjectWireSize, rightObjectColor);
	}
	inline void setWireSize(int size) { 
		ObjectWireSize = size; 
		DeleteObject(ObjectPen);
		ObjectPen = CreatePen(PS_SOLID, ObjectWireSize, ObjectColor);
	}

	// get methods
	inline COLORREF getLeftWireColor(void) const { return leftObjectColor; }
	inline COLORREF getRightWireColor(void) const { return rightObjectColor; }
	inline COLORREF getWireColor(void) const { return ObjectColor; }
	inline int getWireSize(void) const { return ObjectWireSize; }

	virtual ~Object3D();
	virtual void DrawObject(HDC hdc, Matrix3D & Projection, Matrix3D & CameraModification, double cutSurfaceInterval);
	virtual void DrawObject3D(HDC hdc, Matrix3D & stereoLeft, Matrix3D & stereoRight, Matrix3D & CameraModification, double cutSurfaceInterval);
	virtual void Generate()=0;

	virtual void RotZ(double alfa)
	{
		static double x;
		static double y;
		static double a11,a21,a31,a41;

		x = cos(alfa);
		y = sin(alfa);
		a11 = Modification.matrix[0][0] * x - Modification.matrix[0][1] * y;
		a21 = Modification.matrix[1][0] * x - Modification.matrix[1][1] * y;
		a31 = Modification.matrix[2][0] * x - Modification.matrix[2][1] * y;
		a41 = Modification.matrix[3][0] * x - Modification.matrix[3][1] * y;

		Modification.matrix[0][1] = Modification.matrix[0][0] * y + Modification.matrix[0][1] * x;
		Modification.matrix[1][1] = Modification.matrix[1][0] * y + Modification.matrix[1][1] * x; 
		Modification.matrix[2][1] = Modification.matrix[2][0] * y + Modification.matrix[2][1] * x;
		Modification.matrix[3][1] = Modification.matrix[3][0] * y + Modification.matrix[3][1] * x; 

		Modification.matrix[0][0] = a11;
		Modification.matrix[1][0] = a21;
		Modification.matrix[2][0] = a31;
		Modification.matrix[3][0] = a41;

		x = cos(-alfa);
		y = sin(-alfa);
		a11 = InvertModification.matrix[0][0] * x + InvertModification.matrix[1][0] * y;
		a21 = InvertModification.matrix[0][1] * x + InvertModification.matrix[1][1] * y;
		a31 = InvertModification.matrix[0][2] * x + InvertModification.matrix[1][2] * y;
		a41 = InvertModification.matrix[0][3] * x + InvertModification.matrix[1][3] * y;

		InvertModification.matrix[1][0] = InvertModification.matrix[1][0] * x - InvertModification.matrix[0][0] * y;
		InvertModification.matrix[1][1] = InvertModification.matrix[1][1] * x - InvertModification.matrix[0][1] * y; 
		InvertModification.matrix[1][2] = InvertModification.matrix[1][2] * x - InvertModification.matrix[0][2] * y;
		InvertModification.matrix[1][3] = InvertModification.matrix[1][3] * x - InvertModification.matrix[0][3] * y; 

		InvertModification.matrix[0][0] = a11;
		InvertModification.matrix[0][1] = a21;
		InvertModification.matrix[0][2] = a31;
		InvertModification.matrix[0][3] = a41;

		/*
		Matrix3D temp;
		temp.Diagonal(1.0, cos(alfa), cos(alfa), 1.0);

		temp[2][1] = -sin(alfa);
		temp[1][2] = sin(alfa);
		Modification = temp * Modification;

		temp[2][1] = sin(alfa);
		temp[1][2] = -sin(alfa);
		InvertModification= InvertModification*temp;


		Matrix3D t1 = Modification*InvertModification;*/
	}

	virtual void RotX(double alfa)
	{
		static double x;
		static double y;
		static double a11,a21,a31,a41;

		x = cos(alfa);
		y = sin(alfa);
		a11 = Modification.matrix[0][1] * x - Modification.matrix[0][2] * y;
		a21 = Modification.matrix[1][1] * x - Modification.matrix[1][2] * y;
		a31 = Modification.matrix[2][1] * x - Modification.matrix[2][2] * y;
		a41 = Modification.matrix[3][1] * x - Modification.matrix[3][2] * y;

		Modification.matrix[0][2] = Modification.matrix[0][1] * y + Modification.matrix[0][2] * x;
		Modification.matrix[1][2] = Modification.matrix[1][1] * y + Modification.matrix[1][2] * x; 
		Modification.matrix[2][2] = Modification.matrix[2][1] * y + Modification.matrix[2][2] * x;
		Modification.matrix[3][2] = Modification.matrix[3][1] * y + Modification.matrix[3][2] * x; 

		Modification.matrix[0][1] = a11;
		Modification.matrix[1][1] = a21;
		Modification.matrix[2][1] = a31;
		Modification.matrix[3][1] = a41;

		x = cos(-alfa);
		y = sin(-alfa);
		a11 = InvertModification.matrix[1][0] * x + InvertModification.matrix[2][0] * y;
		a21 = InvertModification.matrix[1][1] * x + InvertModification.matrix[2][1] * y;
		a31 = InvertModification.matrix[1][2] * x + InvertModification.matrix[2][2] * y;
		a41 = InvertModification.matrix[1][3] * x + InvertModification.matrix[2][3] * y;

		InvertModification.matrix[2][0] = InvertModification.matrix[2][0] * x - InvertModification.matrix[1][0] * y;
		InvertModification.matrix[2][1] = InvertModification.matrix[2][1] * x - InvertModification.matrix[1][1] * y;
		InvertModification.matrix[2][2] = InvertModification.matrix[2][2] * x - InvertModification.matrix[1][2] * y;
		InvertModification.matrix[2][3] = InvertModification.matrix[2][3] * x - InvertModification.matrix[1][3] * y; 

		InvertModification.matrix[1][0] = a11;
		InvertModification.matrix[1][1] = a21;
		InvertModification.matrix[1][2] = a31;
		InvertModification.matrix[1][3] = a41;

//Matrix3D t1 = Modification*InvertModification;
	}

	virtual void RotY(double alfa)
	{
		static double x;
		static double y;
		static double a11,a21,a31,a41;

		x = cos(alfa);
		y = sin(alfa);
		a11 = Modification.matrix[0][0] * x + Modification.matrix[0][2] * y;
		a21 = Modification.matrix[1][0] * x + Modification.matrix[1][2] * y;
		a31 = Modification.matrix[2][0] * x + Modification.matrix[2][2] * y;
		a41 = Modification.matrix[3][0] * x + Modification.matrix[3][2] * y;

		Modification.matrix[0][2] = Modification.matrix[0][2] * x - Modification.matrix[0][0] * y;
		Modification.matrix[1][2] = Modification.matrix[1][2] * x - Modification.matrix[1][0] * y; 
		Modification.matrix[2][2] = Modification.matrix[2][2] * x - Modification.matrix[2][0] * y;
		Modification.matrix[3][2] = Modification.matrix[3][2] * x - Modification.matrix[3][0] * y; 

		Modification.matrix[0][0] = a11;
		Modification.matrix[1][0] = a21;
		Modification.matrix[2][0] = a31;
		Modification.matrix[3][0] = a41;

		x = cos(-alfa);
		y = sin(-alfa);
		a11 = InvertModification.matrix[0][0] * x - InvertModification.matrix[2][0] * y;
		a21 = InvertModification.matrix[0][1] * x - InvertModification.matrix[2][1] * y;
		a31 = InvertModification.matrix[0][2] * x - InvertModification.matrix[2][2] * y;
		a41 = InvertModification.matrix[0][3] * x - InvertModification.matrix[2][3] * y;

		InvertModification.matrix[2][0] = InvertModification.matrix[0][0] * y + InvertModification.matrix[2][0] * x;
		InvertModification.matrix[2][1] = InvertModification.matrix[0][1] * y + InvertModification.matrix[2][1] * x; 
		InvertModification.matrix[2][2] = InvertModification.matrix[0][2] * y + InvertModification.matrix[2][2] * x;
		InvertModification.matrix[2][3] = InvertModification.matrix[0][3] * y + InvertModification.matrix[2][3] * x; 

		InvertModification.matrix[0][0] = a11;
		InvertModification.matrix[0][1] = a21;
		InvertModification.matrix[0][2] = a31;
		InvertModification.matrix[0][3] = a41;

//   Matrix3D Identity = InvertModification*Modification;
	}
	virtual void TransX(double pos)
	{
		Modification.matrix[0][0] += Modification.matrix[0][3] * pos;
		Modification.matrix[1][0] += Modification.matrix[1][3] * pos;
		Modification.matrix[2][0] += Modification.matrix[2][3] * pos;
		Modification.matrix[3][0] += Modification.matrix[3][3] * pos;

		pos = -pos;
		InvertModification.matrix[3][0] += InvertModification.matrix[0][0] * pos;
		InvertModification.matrix[3][1] += InvertModification.matrix[0][1] * pos;
		InvertModification.matrix[3][2] += InvertModification.matrix[0][2] * pos;
		InvertModification.matrix[3][3] += InvertModification.matrix[0][3] * pos;

		//Matrix3D temp = InvertModification*Modification;
	}
	virtual void TransY(double pos)
	{
		Modification.matrix[0][1] += Modification.matrix[0][3] * pos;
		Modification.matrix[1][1] += Modification.matrix[1][3] * pos;
		Modification.matrix[2][1] += Modification.matrix[2][3] * pos;
		Modification.matrix[3][1] += Modification.matrix[3][3] * pos;

		pos = -pos;
		InvertModification.matrix[3][0] += InvertModification.matrix[1][0] * pos;
		InvertModification.matrix[3][1] += InvertModification.matrix[1][1] * pos;
		InvertModification.matrix[3][2] += InvertModification.matrix[1][2] * pos;
		InvertModification.matrix[3][3] += InvertModification.matrix[1][3] * pos;
		//Matrix3D temp = InvertModification*Modification;
	}

	virtual void TransZ(double pos)
	{
		Modification.matrix[0][2] += Modification.matrix[0][3] * pos;
		Modification.matrix[1][2] += Modification.matrix[1][3] * pos;
		Modification.matrix[2][2] += Modification.matrix[2][3] * pos;
		Modification.matrix[3][2] += Modification.matrix[3][3] * pos;

		pos = -pos;
		InvertModification.matrix[3][0] += InvertModification.matrix[2][0] * pos;
		InvertModification.matrix[3][1] += InvertModification.matrix[2][1] * pos;
		InvertModification.matrix[3][2] += InvertModification.matrix[2][2] * pos;
		InvertModification.matrix[3][3] += InvertModification.matrix[2][3] * pos;
		//Matrix3D temp = InvertModification*Modification;
	}

	virtual void ScaleX(double scale)
	{
		Modification.matrix[0][0] *= scale;
		Modification.matrix[1][0] *= scale;
		Modification.matrix[2][0] *= scale;
		Modification.matrix[3][0] *= scale;

		scale = 1/scale;
		InvertModification.matrix[0][0] *= scale;
		InvertModification.matrix[0][1] *= scale;
		InvertModification.matrix[0][2] *= scale;
		InvertModification.matrix[0][3] *= scale;
//		Matrix3D temp = InvertModification*Modification;
	}

	virtual void ScaleY(double scale)
	{
		Modification.matrix[0][1] *= scale;
		Modification.matrix[1][1] *= scale;
		Modification.matrix[2][1] *= scale;
		Modification.matrix[3][1] *= scale;

		scale = 1/scale;
		InvertModification.matrix[1][0] *= scale;
		InvertModification.matrix[1][1] *= scale;
		InvertModification.matrix[1][2] *= scale;
		InvertModification.matrix[1][3] *= scale;
	}
	virtual void ScaleZ(double scale)
	{
		Modification.matrix[0][2] *= scale;
		Modification.matrix[1][2] *= scale;
		Modification.matrix[2][2] *= scale;
		Modification.matrix[3][2] *= scale;

		scale = 1/scale;
		InvertModification.matrix[2][0] *= scale;
		InvertModification.matrix[2][1] *= scale;
		InvertModification.matrix[2][2] *= scale;
		InvertModification.matrix[2][3] *= scale;
//		Matrix3D temp = InvertModification*Modification;
	}
};

}

#endif // !defined(AFX_OBJECT3D_H__BAECB757_37A4_4C7E_8377_AC2F45A8CF92__INCLUDED_)
