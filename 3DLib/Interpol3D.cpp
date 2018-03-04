// Interpol3D.cpp: implementation of the Interpol3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Interpol3D.h"
#include "Mesh3D.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


namespace ThreeDimLib {

void Interpol3D::DrawObject3D(HDC hdc, Matrix3D & stereoLeft, Matrix3D & stereoRight, Matrix3D & CameraModification, double cutSurfaceInterval)
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
		} else { 
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

void Interpol3D::Generate(int accuracy, vector<Object3D *> & selectedPoints)
{
	static int i;
	Mesh3D * mesh = (Mesh3D *)world;

	this->accuracy = accuracy;
	if ( points ) 
		delete [] points;


	int sellen=selectedPoints.size();


	if ( sellen >= 2 ) {
		BezierRef = selectedPoints;
		for ( i=0; i < selectedPoints.size(); ++i )
		{
			((Point3D*)selectedPoints[i])->user.push_back(this);
		}

		int n = BezierRef.size()-1;
		int i,j;

		vector<double> gorna(n+1);
		vector<double> diagonalna(n+1);
		vector<double> dolna(n+1);

		size = (accuracy+1)*n;
		points = new Vector3D[size+1];


		vector<Vector3D> D(n+1);
		vector<Vector3D> Macierz(n+1);
		vector<Vector3D> y(n+1);
		vector<double>   h(n+1);
		vector<Vector3D> A(n+1);
		vector<Vector3D> B(n+1);
		vector<Vector3D> C(n+1);
		vector<Vector3D> D1(n+1);
		
		for (i=0; i < n+1; ++i)
			A[i] = ( ((Point3D*)BezierRef[i])->getVector() );

		// counting h[]
		h[0] = 0.0f;
		for (i=1; i < n+1; ++i) 
			h[i] = (A[i] - A[i-1]).L2Norm();



		gorna[0] = 0;
		dolna[n] = 0;
		for (i = 0; i< diagonalna.size(); ++i)
			diagonalna[i] = 2.0;

		for (i = 1; i < n; ++i) {
			gorna[i]=h[i+1]/(h[i]+h[i+1]);
			dolna[i] = 1 - gorna[i];
			D[i] = (((A[i+1]-A[i])/h[i+1]) - ((A[i]-A[i-1])/h[i])) * (6.0f/(h[i]+h[i+1]));
		}

		for (i = 0; i < diagonalna.size()-1; ++i) {
			gorna[i] /= diagonalna[i];
			diagonalna[i+1] -= gorna[i]*dolna[i+1];
		}

		y[0] = D[0]/diagonalna[0];
		for (i = 1; i < n+1; ++i)
			y[i] = (D[i] - (y[i-1]*dolna[i]))/diagonalna[i];
		Macierz[n] = y[n];
		for (j = n-1; j >= 0; --j)
			Macierz[j] = (y[j] - (Macierz[j+1]*gorna[j]));


		
		static double t;

		points[0]=A[0];
		int d=1;
		for (j=0;j<n;++j) {
			C[j] = Macierz[j]/2.0f;
			B[j] = (A[j+1]-A[j])/h[j+1] - ((Macierz[j]*2.0+Macierz[j+1])/6.0f) * h[j+1];
			D1[j] = (Macierz[j+1]-Macierz[j])/(6.0f*h[j+1]);
			for (int i=1; i<=accuracy+1; ++i) {
				t = (double)(i*h[j+1])/(double)accuracy;
				points[d] = A[j] + B[j]*t + C[j]*t*t + D1[j]*t*t*t;
				points[d].set4();
				++d;
			}
		}
	}
}

void Interpol3D::ReCalculate()
{
	static int i;

	if ( !changed )
		return;
	if ( BezierRef.size() == 0 ) 
		return;
	if ( points ) 
		delete [] points;
	

	Mesh3D * mesh = (Mesh3D *)world;


	int sellen=BezierRef.size();

	if ( sellen >= 2 ) {
		int n = BezierRef.size()-1;
		int i,j;

		vector<double> up(n+1);
		vector<double> diag(n+1);
		vector<double> down(n+1);

		vector<Vector3D> D(n+1);
		vector<Vector3D> M(n+1);
		vector<Vector3D> y(n+1);
		vector<double>   h(n+1);
		vector<Vector3D> alfa(n+1),
						 beta(n+1),
						 gamma(n+1),
						 delta(n+1);
		
		for (i=0; i < n+1; ++i)
			alfa[i] = ( ((Point3D*)BezierRef[i])->getVector() );

		// counting h[]
		h[0] = 0.0f;
		for (i=1; i < n+1; ++i) 
			h[i] = (alfa[i] - alfa[i-1]).L2Norm();



		up[0] = 0;
		down[n] = 0;
		for (i = 0; i< diag.size(); ++i)
			diag[i] = 2.0;

		for (i = 1; i < n; ++i) {
			up[i]=h[i+1]/(h[i]+h[i+1]);
			down[i] = 1 - up[i];
			D[i] = (((alfa[i+1]-alfa[i])/h[i+1]) - ((alfa[i]-alfa[i-1])/h[i])) * (6.0f/(h[i]+h[i+1]));
		}

		for (i = 0; i < diag.size()-1; ++i) {
			up[i] /= diag[i];
			diag[i+1] -= up[i]*down[i+1];
		}

		y[0] = D[0]/diag[0];
		for (i = 1; i < n+1; ++i)
			y[i] = (D[i] - (y[i-1]*down[i]))/diag[i];
		M[n] = y[n];
		for (j = n-1; j >= 0; --j)
			M[j] = (y[j] - (M[j+1]*up[j]));


		
		static double t;

		size = (accuracy+1)*n;
		points = new Vector3D[size+1];
		points[0]=alfa[0];
		int d=1;
		for (j=0;j<n;++j) {
			gamma[j] = M[j]/2.0f;
			beta[j] = (alfa[j+1]-alfa[j])/h[j+1] - ((M[j]*2.0+M[j+1])/6.0f) * h[j+1];
			delta[j] = (M[j+1]-M[j])/(6.0f*h[j+1]);
			for (int i=1; i<=accuracy+1; ++i) {
				t = (double)(i*h[j+1])/(double)accuracy;
				points[d] = alfa[j] + beta[j]*t + gamma[j]*t*t + delta[j]*t*t*t;
				points[d].set4();
				++d;
			}
		}
	}
}


Interpol3D::Interpol3D(void * _this):accuracy(15), world(_this)
{

}

Interpol3D::~Interpol3D()
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