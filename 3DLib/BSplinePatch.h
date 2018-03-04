// Bezier3D.h: interface for the Bezier3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BEZIER3D_H__B2F32C46_9996_4AAF_B876_C1EFC4228CAF__INCLUDED_)
#define AFX_BEZIER3D_H__B2F32C46_9996_4AAF_B876_C1EFC4228CAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object3D.h"
#include "Point3D.h"
#include <vector>
#include <list>
using namespace std;

namespace ThreeDimLib {

struct _TParam{
	_TParam(double _u=0, double _v=0) { u=_u; v=_v; }
	double u,v;
	static inline bool lessDictX (const _TParam & u,const _TParam & v) {
		bool ret = (u.u == v.u) ? (u.v < v.v) : (u.u < v.u);
		return ret;
	} 

	static inline bool lessDictY (const _TParam & u,const _TParam & v) {
		bool ret = (u.v == v.v) ? (u.u < v.u) : (u.v < v.v);
		return ret;
	}

};

class PreciseMesh
{
public:
	_TParam  uv[21][21];
	Vector3D coordinates[21][21];
};

class TrimmingCurve
{
public:
	vector<_TParam> uv;
	void clear() { uv.clear(); }
};

class BSplinePatch : public Object3D  
{
	bool			   podstawa;
	vector<Object3D *> PointsRef;
	TrimmingCurve	   trimcurve;
	int				   accuracy;
	int				   u,v;
	double			 * B1[4];
	double			 * B2[4];
	bool			   trimmed;

public:
	

	Vector3D derivativeU(double u, double v) 
	{
		return (value(u+1e-5,v)-value(u,v))/1e-5;		
	}
	Vector3D derivativeV(double u, double v) 
	{
		return (value(u,v+1e-5)-value(u,v))/1e-5;
	}

	void MakeMesh(double minu, double maxu, double minv, double maxv, PreciseMesh & mesh);
	virtual void ReplaceRef(Object3D * t1, Object3D * t2)
	{
		for ( int i=0; i < PointsRef.size(); ++i ) {
			if ( PointsRef[i] == t1 )
				PointsRef[i] = t2;
		}
		ReCalculate();
	}
	BSplinePatch();
	virtual ~BSplinePatch();
	virtual void Generate() {
	}
	Matrix3D countJacobi( BSplinePatch& bs2, double u, double v, double s, double t,Vector3D tau,double d);
	Vector3D countF(BSplinePatch& bs2,double u, double v,double u0, double v0, double s, double t,Vector3D tau, double d);
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
	Vector3D value(double _u, double _v);
	virtual void Generate(vector<Object3D *> & selectedPoints,int _u, int _v);
	virtual void ReCalculate();
	bool isValidate() 
	{
		if ( points )
			return true;
		return false;
	}
	bool startingPoint(BSplinePatch & bs);
	void Trim(BSplinePatch & bs);
	inline const double B(int x, double u)
	{
		if ( x==0 ) {
			return ((1-u)*(1-u)*(1-u))/6.0;
		}
		if ( x==1 ) {
			return (3.0*u*u*u-6.0*u*u+4.0)/6.0;
		}
		if ( x==2 ) {
			return (-3.0f*u*u*u + 3.0f*u*u + 3.0f*u + 1.0f)/6.0f  ;
		}
		if ( x==3 ) {
			return u*u*u/6.0f;
		}
		return 0;
	}
	void DrawObject3D(HDC hdc, Matrix3D & stereoLeft, Matrix3D & stereoRight, Matrix3D & CameraModification, double cutSurfaceInterval);
};

class BSplinePipe:public BSplinePatch
{
public:
	
};

}

#endif // !defined(AFX_BEZIER3D_H__B2F32C46_9996_4AAF_B876_C1EFC4228CAF__INCLUDED_)
