// Torus3D.cpp: implementation of the Torus3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Torus3D.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace ThreeDimLib {


void Torus3D::Generate()
{
	int u,v;
	double theta,phi;
	double x,y,z;

	if ( points ) 
		delete [] points;

	size = (360/dv)*(360/du)*4;
	points = new Vector3D[size];

	int i=0;
	for (u=0;u<360;u+=du) {
	  for (v=0;v<360;v+=dv) {
		 theta = (u) * PIDIV180;
		 phi   = (v) * PIDIV180;
		 x = cos(theta) * ( ray0 + ray1 * cos(phi) );
		 y = sin(theta) * ( ray0 + ray1 * cos(phi) );
		 z = ray1 * sin(phi);
		 points[i++].setVector(x,y,z);

		 theta = (u+du) * PIDIV180;
		 phi   = (v) * PIDIV180;
		 x = cos(theta) * ( ray0 + ray1 * cos(phi) );
		 y = sin(theta) * ( ray0 + ray1 * cos(phi) );
		 z = ray1 * sin(phi);
		 points[i++].setVector(x,y,z);

		 theta = (u+du) * PIDIV180;
		 phi   = (v+dv) * PIDIV180;
		 x = cos(theta) * ( ray0 + ray1 * cos(phi) );
		 y = sin(theta) * ( ray0 + ray1 * cos(phi) );
		 z = ray1 * sin(phi);
		 points[i++].setVector(x,y,z);

		 theta = (u) * PIDIV180;
		 phi   = (v+dv) * PIDIV180;
		 x = cos(theta) * ( ray0 + ray1 * cos(phi) );
		 y = sin(theta) * ( ray0 + ray1 * cos(phi) );
		 z = ray1 * sin(phi);
		 points[i++].setVector(x,y,z);

	  }
	}

}


Torus3D::Torus3D(double _r0=100.0, double _r1=20.0, int _du=20, int _dv=20)
	:ray0(_r0), ray1(_r1), du(_du), dv(_dv)
{

}

Torus3D::~Torus3D()
{

}


}