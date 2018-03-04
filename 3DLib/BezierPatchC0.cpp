// Bezier3D.cpp: implementation of the Bezier3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BezierPatchC0.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define dim 4

namespace ThreeDimLib {

void BezierPatchC0::DrawObject3D(HDC hdc, Matrix3D & stereoLeft, Matrix3D & stereoRight, Matrix3D & CameraModification, double cutSurfaceInterval)
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

	if ( selected )
		oldPen = (HPEN) SelectObject(hdc, leftSelectedObjectPen);
	else
		oldPen = (HPEN) SelectObject(hdc, leftObjectPen);

	for ( i=0; i < size; i+=accuracy+1 ) {
		temp = points[i]*(CameraModification);
		skip1 = false;
		if ( temp[2] < cutSurfaceInterval )
			skip1 = true;

		temp[0] += temp[2]*a;
		temp[3] = temp[2]*b+1;
		pt[0].x = (long)(temp[0]/temp[3]);
		pt[0].y = (long)(temp[1]/temp[3]);
  		MoveToEx(hdc, pt[0].x, pt[0].y, &p);

		for ( int j = 0 ; j < accuracy+1; ++j ) {
			temp = points[i+j]*(CameraModification);
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
	}

	if ( selected )
		oldPen = (HPEN) SelectObject(hdc, rightSelectedObjectPen);
	else
		oldPen = (HPEN) SelectObject(hdc, rightObjectPen);


	for ( i=0; i < size; i+=accuracy+1 ) {

		temp = points[i]*(CameraModification);
		skip1 = false;
		if ( temp[2] < cutSurfaceInterval )
			skip1 = true;

		temp[0] += temp[2]*c;
		temp[3] = temp[2]*d+1;
		pt[0].x = (long)(temp[0]/temp[3]);
		pt[0].y = (long)(temp[1]/temp[3]);
  		MoveToEx(hdc, pt[0].x, pt[0].y, &p);

		for ( int j=0; j < accuracy+1; ++j) {
			temp = points[i+j]*(CameraModification);
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
	}

	SetROP2(hdc, old);
	SelectObject(hdc, oldPen);
}

void BezierPatchC0::ReCalculate()
{
	if ( !changed )
		return;
	changed=false;
	static int d;
	static Vector3D t1;
	d=0;

	for ( int i=0; i < u; ++i ) 
		for ( int j=0;j < v; ++j ) {
			for ( int ff=0; ff < dim; ++ff ) {
				for ( int h=0; h < accuracy+1; ++h ) {
					t1.ZeroVector();
					for ( int x=0; x < 4; ++x )
						for ( int y=0; y < 4; ++y ) {
							t1 += ((Point3D*)(PointsRef[(i*3+x)*(v*3+1)+(j*3+y)]))->getVector() * B1[x][ff] * B2[y][h];
						}
					points[d] = t1;					
					++d;
				}

			}

			for ( ff=0; ff < dim; ++ff ) {
				for ( int h=0; h < accuracy+1; ++h ) {
					t1.ZeroVector();
					for ( int x=0; x < 4; ++x )
						for ( int y=0; y < 4; ++y ) {
							t1 += ((Point3D*)(PointsRef[(i*3+x)*(v*3+1)+(j*3+y)]))->getVector() * B2[x][h] * B1[y][ff];
						}
					points[d] = t1;					
					++d;
				}
			}

		}

}

void BezierPatchC0::Generate(vector<Object3D *> & selectedPoints,int _u, int _v)
{
	v = _v;
	u = _u;
	changed = true;
	if ( points )
		delete [] points;

	accuracy = 10;



	B1[0]=new double[dim];
	B1[1]=new double[dim];
	B1[2]=new double[dim];
	B1[3]=new double[dim];

	B2[0]=new double[accuracy+1];
	B2[1]=new double[accuracy+1];
	B2[2]=new double[accuracy+1];
	B2[3]=new double[accuracy+1];


	for ( int ff=0; ff < dim; ++ff ) {
		B1[0][ff] = B(0,(double)ff/(dim-1));
		B1[1][ff] = B(1,(double)ff/(dim-1));
		B1[2][ff] = B(2,(double)ff/(dim-1));
		B1[3][ff] = B(3,(double)ff/(dim-1));
	}

	double g1 = 1.0/accuracy;
	double g2 = 0.0;
	for (ff=0; ff < accuracy+1; ++ff ) {
		B2[0][ff] = B(0,g2);
		B2[1][ff] = B(1,g2);
		B2[2][ff] = B(2,g2);
		B2[3][ff] = B(3,g2);
		g2+=g1;
	}


	size = (u+(u-1))*(v+(v-1))*(dim*(accuracy+1))+ (dim*(accuracy+1));
	points = new Vector3D[size];


	PointsRef=selectedPoints;

	for ( int g=0; g < selectedPoints.size(); ++g )
	{
		((Point3D*)selectedPoints[g])->user.push_back(this);
	}


	ReCalculate();	
}

BezierPatchC0::BezierPatchC0():accuracy(15)
{
	B1[0]=0;
	B1[1]=0;
	B1[2]=0;
	B1[3]=0;
	B2[0]=0;
	B2[1]=0;
	B2[2]=0;
	B2[3]=0;
}

BezierPatchC0::~BezierPatchC0()
{
	delete [] B1[0];
	delete [] B1[1];
	delete [] B1[2];
	delete [] B1[3];
	delete [] B2[0];
	delete [] B2[1];
	delete [] B2[2];
	delete [] B2[3];

	for ( int i=0; i < PointsRef.size(); ++i ) {
		for ( int j = 0 ; j  < ((Point3D*)PointsRef[i])->user.size(); ++j ) {
			if ( ((Point3D*)PointsRef[i])->user[j] == this ) {
				((Point3D*)PointsRef[i])->user[j]=0;
			}
		}
	}
	PointsRef.clear();	
}



}