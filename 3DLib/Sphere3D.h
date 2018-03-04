// Sphere3D.h: interface for the Sphere3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPHERE3D_H__AA0CFE8B_CCE1_4763_8CE9_8BE85D400569__INCLUDED_)
#define AFX_SPHERE3D_H__AA0CFE8B_CCE1_4763_8CE9_8BE85D400569__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object3D.h"

namespace ThreeDimLib {

class Sphere3D : public Object3D  
{
	double	ray0;
	int		du, dv;
public:
	Sphere3D(double _r0, int _du, int _dv);
	virtual ~Sphere3D();
	virtual void Generate();
};

}

#endif // !defined(AFX_SPHERE3D_H__AA0CFE8B_CCE1_4763_8CE9_8BE85D400569__INCLUDED_)
