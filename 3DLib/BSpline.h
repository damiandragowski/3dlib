// BSpline3D.h: interface for the BSpline3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BSpline3D_H__B2F32C46_9996_4AAF_B876_C1EFC4228CFB__INCLUDED_)
#define AFX_BSpline3D_H__B2F32C46_9996_4AAF_B876_C1EFC4228CFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object3D.h"
#include "Point3D.h"
#include <vector>
#include <list>
using namespace std;

namespace ThreeDimLib {

class BSpline3D : public Object3D  
{
	friend class Mesh3D;
	void			*  world;
	vector<Object3D *> BezierRef;
	vector<Object3D *> deBoorRef;
	int				  accuracy;
	bool			   BezierMode;
public:
	virtual void ReplaceRef(Object3D * t1, Object3D * t2)
	{
		for ( int i=0; i < BezierRef.size(); ++i ) {
			if ( BezierRef[i] == t1 )
				BezierRef[i] = t2;
		}
		for (i=0; i < deBoorRef.size(); ++i ) {
			if ( deBoorRef[i] == t1 )
				deBoorRef[i] = t2;
		}
		ReCalculate();
	}
	BSpline3D(void * scene);
	virtual ~BSpline3D();
	virtual void Generate() {
	}
	virtual void ChangeMode() { BezierMode = !BezierMode;}
	virtual void ClearUse() 
	{
		for ( int i=0; i < deBoorRef.size(); ++i ) {
			for ( int j=0 ; j < ((Point3D *)deBoorRef[i])->user.size(); ++j ) {
				if ( ((Point3D*)deBoorRef[i])->user[j] == this ) {
					((Point3D*)deBoorRef[i])->user[j]=0;
				}
			}
		}
		deBoorRef.clear();	

		for (i=0; i < BezierRef.size(); ++i ) {
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
		if ( BezierMode ) {
			if ( BezierRef.size() >= 4 )
				return true;
		} else {
			if ( deBoorRef.size() >= 4 )
				return true;
		}
		return false;
	}

	void AddBeziers(Object3D & P1, Object3D & P2, Object3D & P3, Object3D & P4,bool skip);
	void CalcBeziers(Object3D & P1, Object3D & P2, Object3D & P3, Object3D & P4,int i, bool skip);
	void AdddeBoor(Object3D & P1, Object3D & P2, Object3D & P3, Object3D & P4);
	void CalcdeBoor(Object3D & P1, Object3D & P2, Object3D & P3, Object3D & P4,int i, bool skip);

	void CreateOtherPoints(Point3D * object);

	void AddBezierRef(Object3D * object)
	{
		if ( BezierRef.size() >= 4 ) // must be 4 in other case won;t work
		{
			changed=true;
			((Point3D*)object)->user.push_back(this);
			CreateOtherPoints((Point3D*)object);
			BezierRef.push_back(object);

			ReCalculate();
		} 
	}

	void AdddeBoorRef(Object3D * object)
	{
		changed=true;
		((Point3D*)object)->user.push_back(this);
		deBoorRef.push_back(object);
		ReCalculate();
	}



	void CalculatedeBoor(double t, Object3D & P1, Object3D & P2, Object3D & P3, Object3D & P4, Vector3D & output);
	void CalculateBezier(double t, Object3D & P1, Object3D & P2, Object3D & P3, Object3D & P4, Vector3D & output);
};

}

#endif // !defined(AFX_BSpline3D_H__B2F32C46_9996_4AAF_B876_C1EFC4228CFB__INCLUDED_)
