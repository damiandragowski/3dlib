// Sphere3D.cpp: implementation of the Sphere3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Sphere3D.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace ThreeDimLib {

Sphere3D::Sphere3D(double _r0=100.0, int _du=20, int _dv=20):ray0(_r0), du(_du), dv(_dv)
{

}

Sphere3D::~Sphere3D()
{

}

void Sphere3D::Generate()
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
		 x = ray0 * sin(theta) * cos(phi) ;
		 y = ray0 * cos(theta);
		 z = ray0 * sin(phi) * sin(theta);
		 points[i++].setVector(x,y,z);

		 theta = (u+du) * PIDIV180;
		 phi   = (v) * PIDIV180;
		 x = ray0 * sin(theta) * cos(phi) ;
		 y = ray0 * cos(theta);
		 z = ray0 * sin(phi) * sin(theta);
		 points[i++].setVector(x,y,z);

		 theta = (u+du) * PIDIV180;
		 phi   = (v+dv) * PIDIV180;
		 x = ray0 * sin(theta) * cos(phi) ;
		 y = ray0 * cos(theta);
		 z = ray0 * sin(phi) * sin(theta);
		 points[i++].setVector(x,y,z);

		 theta = (u) * PIDIV180;
		 phi   = (v+dv) * PIDIV180;
		 x = ray0 * sin(theta) * cos(phi) ;
		 y = ray0 * cos(theta);
		 z = ray0 * sin(phi) * sin(theta);
		 points[i++].setVector(x,y,z);

	  }
	}
}



}