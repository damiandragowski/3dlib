// Point3D.h: interface for the Point3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINT3D_H__ADB02E0F_F45E_4942_852B_893F2D1F7409__INCLUDED_)
#define AFX_POINT3D_H__ADB02E0F_F45E_4942_852B_893F2D1F7409__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object3D.h"
#include <vector>
using namespace std;


namespace ThreeDimLib {

class Point3D : public Object3D  
{
	friend class BSpline3D;
	friend class Interpol3D;
	Vector3D point;
	HBRUSH	 hollow;
public:
	vector<Object3D *> user;
	Vector3D getVector() const { return point; }
	Point3D(double=0, double=0, double=0);
	Point3D(const Vector3D & rhs) { point=rhs; }
	virtual ~Point3D();
	virtual void DrawObject(HDC hdc, Matrix3D & Projection, Matrix3D & CameraModification, double cutSurfaceInterval);
	virtual void DrawObject3D(HDC hdc, Matrix3D & stereoLeft, Matrix3D & stereoRight, Matrix3D & CameraModification, double cutSurfaceInterval);
	virtual void Generate();
	virtual bool isNear(Vector3D & temp)
	{
		Vector3D tmp=point*Modification;
		tmp = tmp-temp;
		if ( tmp.L1Norm() < 10 ) 
			return true;
		return false;
	}
	virtual void ReplaceRef(Object3D * t1, Object3D * t2)
	{
		for ( int i=0; i < user.size(); ++i ) {
			user[i]->ReplaceRef(t1, t2);
		}
		t2->userUnion(user);
		user.clear();
	}
	virtual void userUnion(vector<Object3D *> & user)
	{
		vector<Object3D *>::iterator j;
		for ( int i=0;i<user.size();++i) {
			j=find(this->user.begin(), this->user.end(), user[i]);
			if ( j == this->user.end() )
				this->user.push_back(user[i]);
		}
	}

	virtual double operator[] ( int index ) {
		return point[index];
	}

	void changed()
	{
		for ( int i=0;i < user.size(); ++i )
			if ( user[i] != 0 )
				user[i]->changed=true;
	}

	virtual void RotZ(double alfa)
	{
		Modification.Diagonal(cos(alfa), cos(alfa), 1.0, 1.0);
		Modification[0][1] = sin(alfa);
		Modification[1][0] = -sin(alfa);
		point = point * Modification;
		changed();
	}

	virtual void RotX(double alfa)
	{
		Modification.Diagonal(1.0, cos(alfa), cos(alfa), 1.0);
		Modification[2][1] = -sin(alfa);
		Modification[1][2] = sin(alfa);
		point = point * Modification;
		changed();
	}

	virtual void RotY(double alfa)
	{
		Modification.Diagonal(cos(alfa), 1.0, cos(alfa), 1.0);
		Modification[0][2] = -sin(alfa);
		Modification[2][0] = sin(alfa);
		point = point * Modification;
		changed();
	}
	virtual void TransX(double pos)
	{
		point[0]+=pos;
		changed();
	}
	virtual void TransY(double pos)
	{
		point[1]+=pos;
		changed();
	}

	virtual void TransZ(double pos)
	{
		point[2]+=pos;
		changed();
	}

	virtual void ScaleX(double scale)
	{
		point[0]*=scale;
		changed();
	}

	virtual void ScaleY(double scale)
	{
		point[1]*=scale;	
		changed();
	}
	virtual void ScaleZ(double scale)
	{
		point[2]*=scale;	
		changed();
	}

};

}

#endif // !defined(AFX_POINT3D_H__ADB02E0F_F45E_4942_852B_893F2D1F7409__INCLUDED_)
