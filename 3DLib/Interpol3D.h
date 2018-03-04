// Interpol.h: interface for the Interpol class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Interpol_H__B2F32C46_9996_4AAF_B876_C1EFC4228CFB__INCLUDED_)
#define AFX_Interpol_H__B2F32C46_9996_4AAF_B876_C1EFC4228CFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object3D.h"
#include "Point3D.h"
#include <vector>
#include <list>
using namespace std;

namespace ThreeDimLib {


class Interpol3D : public Object3D  
{
	vector<Object3D *> BezierRef;
	int				  accuracy;
	void *			  world;
public:
	virtual void ReplaceRef(Object3D * t1, Object3D * t2)
	{
		for ( int i=0; i < BezierRef.size(); ++i ) {
			if ( BezierRef[i] == t1 ) {
				BezierRef[i] = t2;

			}
		}
		ReCalculate();
	}
	Interpol3D(void * _this);
	virtual ~Interpol3D();
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
		if ( points )
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
};

}

#endif // !defined(AFX_Interpol_H__B2F32C46_9996_4AAF_B876_C1EFC4228CFB__INCLUDED_)
