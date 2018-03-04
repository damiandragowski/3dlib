// Torus3D.h: interface for the Torus3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TORUS3D_H__D245D717_281F_495F_935B_112B6A0C9F26__INCLUDED_)
#define AFX_TORUS3D_H__D245D717_281F_495F_935B_112B6A0C9F26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object3D.h"

namespace ThreeDimLib {

class Torus3D : public Object3D  
{
	double	ray0, ray1;
	int		du, dv;
public:
	Torus3D(double _r0, double _r1, int _du, int _dv);
	virtual ~Torus3D();
	virtual void Generate();
};

}

#endif // !defined(AFX_TORUS3D_H__D245D717_281F_495F_935B_112B6A0C9F26__INCLUDED_)
