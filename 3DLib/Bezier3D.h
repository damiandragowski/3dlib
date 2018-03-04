// Bezier3D.h: interface for the Bezier3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BEZIER3D_H__B2F32C46_9996_4AAF_B876_C1EFC4228CFB__INCLUDED_)
#define AFX_BEZIER3D_H__B2F32C46_9996_4AAF_B876_C1EFC4228CFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object3D.h"
#include "Point3D.h"
#include <vector>
#include <list>
using namespace std;

namespace ThreeDimLib {

class Bezier3D : public Object3D  
{
	vector<Object3D *> BezierRef;
	int				  accuracy;
public:
	virtual void ReplaceRef(Object3D * t1, Object3D * t2)
	{
		for ( int i=0; i < BezierRef.size(); ++i ) {
			if ( BezierRef[i] == t1 )
				BezierRef[i] = t2;
		}
		ReCalculate();
	}

	Bezier3D();
	virtual ~Bezier3D();
	virtual void Generate() {
	}
	virtual void ClearUse() 
	{
		for ( int i=0; i < BezierRef.size(); ++i ) {
			for ( int j=0 ; j < ((Point3D *)BezierRef[i])->user.size(); ++j ) {
				if ( ((Point3D*)BezierRef[i])->user[j] == this ) {
					((Point3D*)BezierRef[i])->user[j]=0;
				}
			}
		}
		BezierRef.clear();	
		deleteTHIS=true;
	}
	virtual void Generate(int accuracy, vector<Object3D *> & selectedPoints);
	virtual void ReCalculate();
	virtual void DrawObject3D(HDC hdc, Matrix3D & stereoLeft, Matrix3D & stereoRight, Matrix3D & CameraModification, double cutSurfaceInterval);
	bool isValidate() 
	{
		if ( BezierRef.size() >= 4 )
			return true;
		return false;
	}
	void AddRef(Object3D * object)
	{
		changed=true;
		((Point3D*)object)->user.push_back(this);
		BezierRef.push_back(object);
		ReCalculate();
	}

	void CalculateBezier(double t, Object3D & P1, Object3D & P2, Object3D & P3, Object3D & P4, Vector3D & output);
};

}

#endif // !defined(AFX_BEZIER3D_H__B2F32C46_9996_4AAF_B876_C1EFC4228CFB__INCLUDED_)
