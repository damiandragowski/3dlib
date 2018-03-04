// Bezier3D.h: interface for the Bezier3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BEZIER3D_H__B2F32C46_9996_4AAF_B876_C1EFC4228CAB__INCLUDED_)
#define AFX_BEZIER3D_H__B2F32C46_9996_4AAF_B876_C1EFC4228CAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object3D.h"
#include "Point3D.h"
#include <vector>
#include <list>
using namespace std;

namespace ThreeDimLib {

class BezierPatchC0 : public Object3D  
{
	vector<Object3D *> PointsRef;

	int				   accuracy;
	int				   u,v;
	double			 * B1[4];
	double			 * B2[4];

public:
	virtual void ReplaceRef(Object3D * t1, Object3D * t2)
	{
		for ( int i=0; i < PointsRef.size(); ++i ) {
			if ( PointsRef[i] == t1 )
				PointsRef[i] = t2;
		}
		ReCalculate();
	}
	BezierPatchC0();
	virtual ~BezierPatchC0();
	virtual void Generate() {
	}
	virtual void ClearUse() 
	{
		for ( int i=0; i < PointsRef.size(); ++i ) {
			for ( int j=0 ; j < ((Point3D *)PointsRef[i])->user.size(); ++j ) {
				if ( ((Point3D*)PointsRef[i])->user[j] == this ) {
					((Point3D*)PointsRef[i])->user[j]=0;
				}
			}
		}
		PointsRef.clear();	
		deleteTHIS=true;
	}
	virtual void Generate(vector<Object3D *> & selectedPoints,int _u, int _v);
	virtual void ReCalculate();
	bool isValidate() 
	{
		if ( points )
			return true;
		return false;
	}
	inline const double B(int x, double u)
	{
		if ( x==0 ) {
			return u*u*u;
		}
		if ( x==1 ) {
			return 3*u*u*(1-u);
		}
		if ( x==2 ) {
			return 3*u*(1-u)*(1-u);
		}
		if ( x==3 ) {
			return (1-u)*(1-u)*(1-u);
		}
		return 0;
	}

	void DrawObject3D(HDC hdc, Matrix3D & stereoLeft, Matrix3D & stereoRight, Matrix3D & CameraModification, double cutSurfaceInterval);
};

}

#endif // !defined(AFX_BEZIER3D_H__B2F32C46_9996_4AAF_B876_C1EFC4228CAB__INCLUDED_)
