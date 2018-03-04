// Bezier3D.cpp: implementation of the Bezier3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Bezier3D.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


namespace ThreeDimLib {

void Bezier3D::DrawObject3D(HDC hdc, Matrix3D & stereoLeft, Matrix3D & stereoRight, Matrix3D & CameraModification, double cutSurfaceInterval)
{
	if ( !isValidate() )
		return;
	static int i;
	static POINT pt[4];
	static Vector3D temp;
	static bool skip1, skip2, skip3, skip4;
	static POINT p;
	static double a,b;
	static double c,d;
	a=stereoLeft[0][2];
	b=stereoLeft[3][2];
	c=stereoRight[0][2];
	d=stereoRight[3][2];

	
	static HPEN oldPen; 

	int old = SetROP2(hdc, R2_MERGEPEN);

	temp = points[0]*(CameraModification);
	skip1 = false;
	if ( temp[2] < cutSurfaceInterval )
		skip1 = true;

	temp[0] += temp[2]*a;
	temp[3] = temp[2]*b+1;
	pt[0].x = (long)(temp[0]/temp[3]);
	pt[0].y = (long)(temp[1]/temp[3]);
  	MoveToEx(hdc, pt[0].x, pt[0].y, &p);

	if ( selected )
		oldPen = (HPEN) SelectObject(hdc, leftSelectedObjectPen);
	else
		oldPen = (HPEN) SelectObject(hdc, leftObjectPen);


	for ( i=0; i < size; ++i ) {

		temp = points[i]*(CameraModification);
		skip1 = false;
		if ( temp[2] < cutSurfaceInterval )
			skip1 = true;

		temp[0] += temp[2]*a;
		temp[3] = temp[2]*b+1;
		pt[0].x = (long)(temp[0]/temp[3]);
		pt[0].y = (long)(temp[1]/temp[3]);
		if ( skip1 ) {
			skip2=true;
			MoveToEx(hdc, pt[0].x, pt[0].y, &p);  			
		}
		else { 
			if ( skip2 ) {
				MoveToEx(hdc, pt[0].x, pt[0].y, &p);  			
				skip2=false;
			} else {
				LineTo(hdc, pt[0].x, pt[0].y);  			
			}
		}
	}

	if ( selected )
		oldPen = (HPEN) SelectObject(hdc, rightSelectedObjectPen);
	else
		oldPen = (HPEN) SelectObject(hdc, rightObjectPen);

	temp = points[0]*(CameraModification);
	skip1 = false;
	if ( temp[2] < cutSurfaceInterval )
		skip1 = true;

	temp[0] += temp[2]*c;
	temp[3] = temp[2]*d+1;
	pt[0].x = (long)(temp[0]/temp[3]);
	pt[0].y = (long)(temp[1]/temp[3]);
  	MoveToEx(hdc, pt[0].x, pt[0].y, &p);

	for ( i=0; i < size; ++i ) {

		temp = points[i]*(CameraModification);
		skip1 = false;
		if ( temp[2] < cutSurfaceInterval )
			skip1 = true;

		temp[0] += temp[2]*c;
		temp[3] = temp[2]*d+1;
		pt[0].x = (long)(temp[0]/temp[3]);
		pt[0].y = (long)(temp[1]/temp[3]);
		if ( skip1 ) {
			skip2=true;
			MoveToEx(hdc, pt[0].x, pt[0].y, &p);  			
		} else {
			if ( skip2 ) {
				MoveToEx(hdc, pt[0].x, pt[0].y, &p);  			
				skip2=false;
			} else {
				LineTo(hdc, pt[0].x, pt[0].y);  			
			}
		}
	}

	SetROP2(hdc, old);
	SelectObject(hdc, oldPen);
}

void Bezier3D::CalculateBezier(double t, Object3D & P1, Object3D & P2, Object3D & P3, Object3D & P4, Vector3D & output)
{
	
	static double x,y,z;
	static double g;
	g=1-t;

	x = P1[0]*g*g*g + t*(3*P2[0]*g*g + t*(3*P3[0]*g + t*(P4[0])));
	y = P1[1]*g*g*g + t*(3*P2[1]*g*g + t*(3*P3[1]*g + t*(P4[1])));				
	z = P1[2]*g*g*g + t*(3*P2[2]*g*g + t*(3*P3[2]*g + t*(P4[2])));

	output.setVector(x,y,z);


}

void Bezier3D::Generate(int accuracy, vector<Object3D *> & selectedPoints)
{
	this->accuracy = accuracy;
	if ( points ) 
		delete [] points;
	

	int sellen=selectedPoints.size();

	if ( sellen == 4 ) {
		sellen=1;
		size = (accuracy+1)*(sellen);
		points = new Vector3D[size];
		double d = 1.0/(double)accuracy;

		int j=0;
		for ( int g=0; g < selectedPoints.size(); ++g )
		{
			((Point3D*)selectedPoints[g])->user.push_back(this);
		}

		BezierRef = selectedPoints;

		for ( double t=0.0; t < 1.0; t+=d) {
			CalculateBezier(t, (*selectedPoints[0]), (*selectedPoints[1]), 
				(*selectedPoints[2]), (*selectedPoints[3]), points[j]);
			++j;
		}
	} else if ( sellen > 4 ) {
		int temp = sellen-4;
		sellen = (temp/3)+1;

		size = (accuracy+1)*(sellen);
		points = new Vector3D[size];
		double d = 1.0/(double)accuracy;

		int j=0;
		BezierRef = selectedPoints;

		for ( int i=0; i < sellen; ++i ) {
			for ( int g=i; g < i*3+4; ++g )
			{
				((Point3D*)selectedPoints[g])->user.push_back(this);
			}

			for ( double t=0.0; t < 1.0; t+=d) {
				CalculateBezier(t, (*selectedPoints[3*i]), (*selectedPoints[3*i+1]), 
					(*selectedPoints[3*i+2]), (*selectedPoints[3*i+3]), points[j]);
				++j;
			}
		}
	}
}

void Bezier3D::ReCalculate()
{
	if ( !changed )
		return;
	if ( BezierRef.size() == 0 ) 
		return;
	if ( points ) 
		delete [] points;
	
	int sellen=BezierRef.size();

	if ( sellen == 4 ) {
		sellen=1;
		size = (accuracy+1)*(sellen);
		points = new Vector3D[size];
		double d = 1.0/(double)accuracy;
		int j=0;

		for ( double t=0.0; t < 1.0; t+=d) {
			CalculateBezier(t, (*BezierRef[0]), (*BezierRef[1]), 
				(*BezierRef[2]), (*BezierRef[3]), points[j]);
			++j;
		}
	} else if ( sellen > 4 ) {
		int temp = sellen-4;
		sellen = (temp/3)+1;

		size = (accuracy+1)*(sellen);
		points = new Vector3D[size];
		double d = 1.0/(double)accuracy;
		int j=0;

		for ( int i=0; i < sellen; ++i ) {
			for ( double t=0.0; t < 1.0; t+=d) {
				CalculateBezier(t, (*BezierRef[3*i]), (*BezierRef[3*i+1]), 
					(*BezierRef[3*i+2]), (*BezierRef[3*i+3]), points[j]);
				++j;
			}
		}
	}
}


Bezier3D::Bezier3D():accuracy(15)
{

}

Bezier3D::~Bezier3D()
{
	for ( int i=0; i < BezierRef.size(); ++i ) {
		for ( int j = 0 ; j  < ((Point3D*)BezierRef[i])->user.size(); ++j ) {
			if ( ((Point3D*)BezierRef[i])->user[j] == this ) {
				((Point3D*)BezierRef[i])->user[j]=0;
			}
		}
	}
	BezierRef.clear();	
}

}