// BSpline3D.cpp: implementation of the BSpline3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BSpline.h"
#include "Vector3D.h"
#include "Mesh3D.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


namespace ThreeDimLib {

void BSpline3D::DrawObject3D(HDC hdc, Matrix3D & stereoLeft, Matrix3D & stereoRight, Matrix3D & CameraModification, double cutSurfaceInterval)
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

void BSpline3D::CalculateBezier(double t, Object3D & P1, Object3D & P2, Object3D & P3, Object3D & P4, Vector3D & output)
{
	
	static double x,y,z;
	static double g;
	g=1-t;

	x = P1[0]*g*g*g + t*(3*P2[0]*g*g + t*(3*P3[0]*g + t*(P4[0])));
	y = P1[1]*g*g*g + t*(3*P2[1]*g*g + t*(3*P3[1]*g + t*(P4[1])));				
	z = P1[2]*g*g*g + t*(3*P2[2]*g*g + t*(3*P3[2]*g + t*(P4[2])));

	output.setVector(x,y,z);


}



void BSpline3D::CalculatedeBoor(double t, Object3D & P1, Object3D & P2, Object3D & P3, Object3D & P4, Vector3D & output)
{
	static double p1,p2,p3,p4;
	static double x,y,z;
	t+=3.0;
	p1 = P1[0];
	p2 = P2[0];
	p3 = P3[0];
	p4 = P4[0];

	x = (4-t) * ((1.0-(t-2.0)/2.0)* ((1.0-(t-1.0)/3.0)*p1+((t-1.0)/3.0)*p2) + ((t-2.0)/2.0) * ((1.0-(t-2.0)/3.0)*p2+((t-2.0)/3.0)*p3)) 
  + (t-3) * ((1.0-(t-3.0)/2.0)* ((1.0-(t-2.0)/3.0)*p2+((t-2.0)/3.0)*p3) + ((t-3.0)/2.0) * ((1.0-(t-3.0)/3.0)*p3+((t-3.0)/3.0)*p4));

	p1 = P1[1];
	p2 = P2[1];
	p3 = P3[1];
	p4 = P4[1];

	y = (4-t) * ((1.0-(t-2.0)/2.0)* ((1.0-(t-1.0)/3.0)*p1+((t-1.0)/3.0)*p2) + ((t-2.0)/2.0) * ((1.0-(t-2.0)/3.0)*p2+((t-2.0)/3.0)*p3)) 
  + (t-3) * ((1.0-(t-3.0)/2.0)* ((1.0-(t-2.0)/3.0)*p2+((t-2.0)/3.0)*p3) + ((t-3.0)/2.0) * ((1.0-(t-3.0)/3.0)*p3+((t-3.0)/3.0)*p4));

	p1 = P1[2];
	p2 = P2[2];
	p3 = P3[2];
	p4 = P4[2];

	z = (4-t) * ((1.0-(t-2.0)/2.0)* ((1.0-(t-1.0)/3.0)*p1+((t-1.0)/3.0)*p2) + ((t-2.0)/2.0) * ((1.0-(t-2.0)/3.0)*p2+((t-2.0)/3.0)*p3)) 
  + (t-3) * ((1.0-(t-3.0)/2.0)* ((1.0-(t-2.0)/3.0)*p2+((t-2.0)/3.0)*p3) + ((t-3.0)/2.0) * ((1.0-(t-3.0)/3.0)*p3+((t-3.0)/3.0)*p4));

	output.setVector(x,y,z);
}

void BSpline3D::CalcdeBoor(Object3D & P1, Object3D & P2, Object3D & P3, Object3D & P4,int i, bool skip)
{
	double x,y,z;
	if ( skip ) {
		x = 6.0*((Point3D&)P1)[0] - 7.0*((Point3D&)P2)[0] + 2.0*((Point3D&)P3)[0];
		y = 6.0*((Point3D&)P1)[1] - 7.0*((Point3D&)P2)[1] + 2.0*((Point3D&)P3)[1];
		z = 6.0*((Point3D&)P1)[2] - 7.0*((Point3D&)P2)[2] + 2.0*((Point3D&)P3)[2];
		((Point3D*)deBoorRef[i])->point.setVector(x,y,z);
	}

	x = 2.0*((Point3D&)P2)[0] - ((Point3D&)P3)[0];
	y = 2.0*((Point3D&)P2)[1] - ((Point3D&)P3)[1];
	z = 2.0*((Point3D&)P2)[2] - ((Point3D&)P3)[2];
	((Point3D*)deBoorRef[i+1])->point.setVector(x,y,z);

	x = 2.0*((Point3D&)P3)[0] - ((Point3D&)P2)[0];
	y = 2.0*((Point3D&)P3)[1] - ((Point3D&)P2)[1];
	z = 2.0*((Point3D&)P3)[2] - ((Point3D&)P2)[2];
	((Point3D*)deBoorRef[i+2])->point.setVector(x,y,z);
	

	x = 6.0*((Point3D&)P4)[0] + 2.0*((Point3D&)P2)[0] - 7.0*((Point3D&)P3)[0];
	y = 6.0*((Point3D&)P4)[1] + 2.0*((Point3D&)P2)[1] - 7.0*((Point3D&)P3)[1];
	z = 6.0*((Point3D&)P4)[2] + 2.0*((Point3D&)P2)[2] - 7.0*((Point3D&)P3)[2];
	((Point3D*)deBoorRef[i+3])->point.setVector(x,y,z);
}


void BSpline3D::CalcBeziers(Object3D & P1, Object3D & P2, Object3D & P3, Object3D & P4,int i, bool skip)
{
	double x,y,z;
	if ( skip ) {
		x = 1.0/6.0*((Point3D&)P1)[0] + 2.0/3.0*((Point3D&)P2)[0] + 1.0/6.0*((Point3D&)P3)[0];
		y = 1.0/6.0*((Point3D&)P1)[1] + 2.0/3.0*((Point3D&)P2)[1] + 1.0/6.0*((Point3D&)P3)[1];
		z = 1.0/6.0*((Point3D&)P1)[2] + 2.0/3.0*((Point3D&)P2)[2] + 1.0/6.0*((Point3D&)P3)[2];
		((Point3D*)BezierRef[i])->point.setVector(x,y,z);
	}


	x = 2.0/3.0*((Point3D&)P2)[0] + 1.0/3.0*((Point3D&)P3)[0];
	y = 2.0/3.0*((Point3D&)P2)[1] + 1.0/3.0*((Point3D&)P3)[1];
	z = 2.0/3.0*((Point3D&)P2)[2] + 1.0/3.0*((Point3D&)P3)[2];
	((Point3D*)BezierRef[i+1])->point.setVector(x,y,z);

	x = 1.0/3.0*((Point3D&)P2)[0] + 2.0/3.0*((Point3D&)P3)[0];
	y = 1.0/3.0*((Point3D&)P2)[1] + 2.0/3.0*((Point3D&)P3)[1];
	z = 1.0/3.0*((Point3D&)P2)[2] + 2.0/3.0*((Point3D&)P3)[2];
	((Point3D*)BezierRef[i+2])->point.setVector(x,y,z);
	

	x = 1.0/6.0*((Point3D&)P2)[0] + 2.0/3.0*((Point3D&)P3)[0] + 1.0/6.0*((Point3D&)P4)[0];
	y = 1.0/6.0*((Point3D&)P2)[1] + 2.0/3.0*((Point3D&)P3)[1] + 1.0/6.0*((Point3D&)P4)[1];
	z = 1.0/6.0*((Point3D&)P2)[2] + 2.0/3.0*((Point3D&)P3)[2] + 1.0/6.0*((Point3D&)P4)[2];
	((Point3D*)BezierRef[i+3])->point.setVector(x,y,z);
}

void BSpline3D::CreateOtherPoints(Point3D * object)
{
	// calculate C2 points 
	// we have "one" ( end of the last bezier ) 
	// and "two" ,last one we wanna add.
	// we have to compute two others in the middle of bezier :)
	Mesh3D * _this = (Mesh3D *)world;

	int last = BezierRef.size();
	Vector3D t1 = ((Point3D*)BezierRef[last-1])->getVector(); // last one
	Vector3D t2 = ((Point3D*)BezierRef[last-2])->getVector(); 
	Vector3D t3 = ((Point3D*)BezierRef[last-3])->getVector();

	Vector3D temp1 = t1 + ( t1-t2 );
	Vector3D temp2 = t2 + ( t2-t3 );
	Vector3D tt = temp1 + ( temp1 - temp2 );

	temp1[3]=1;
	tt[3]=1;

	Point3D * ob = new Point3D(temp1);
	ob->user.push_back(this);
	BezierRef.push_back(ob);
	_this->AddPoint(ob);
	
	ob = new Point3D(tt);
	ob->user.push_back(this);
	BezierRef.push_back(ob);
	_this->AddPoint(ob);
}

void BSpline3D::AdddeBoor(Object3D & P1, Object3D & P2, Object3D & P3, Object3D & P4)
{
	double x,y,z;
	Mesh3D * _this = (Mesh3D*)world;
	x = 6.0*((Point3D&)P4)[0] + 2.0*((Point3D&)P2)[0] - 7.0*((Point3D&)P3)[0];
	y = 6.0*((Point3D&)P4)[1] + 2.0*((Point3D&)P2)[1] - 7.0*((Point3D&)P3)[1];
	z = 6.0*((Point3D&)P4)[2] + 2.0*((Point3D&)P2)[2] - 7.0*((Point3D&)P3)[2];
	Point3D * object = new Point3D(x,y,z);
	object->user.push_back(this);
	_this->AddPoint(object);
	deBoorRef.push_back(object);
}

void BSpline3D::AddBeziers(Object3D & P1, Object3D & P2, Object3D & P3, Object3D & P4,bool skip)
{
	double x,y,z;
	Mesh3D * _this=(Mesh3D*)world;
	Point3D * object;

	if ( skip ) {
		x = 1.0/6.0*((Point3D&)P1)[0] + 2.0/3.0*((Point3D&)P2)[0] + 1.0/6.0*((Point3D&)P3)[0];
		y = 1.0/6.0*((Point3D&)P1)[1] + 2.0/3.0*((Point3D&)P2)[1] + 1.0/6.0*((Point3D&)P3)[1];
		z = 1.0/6.0*((Point3D&)P1)[2] + 2.0/3.0*((Point3D&)P2)[2] + 1.0/6.0*((Point3D&)P3)[2];
		object = new Point3D(x,y,z);
		object->user.push_back(this);
		BezierRef.push_back(object);
		_this->AddPoint(object);
	}

	x = 2.0/3.0*((Point3D&)P2)[0] + 1.0/3.0*((Point3D&)P3)[0];
	y = 2.0/3.0*((Point3D&)P2)[1] + 1.0/3.0*((Point3D&)P3)[1];
	z = 2.0/3.0*((Point3D&)P2)[2] + 1.0/3.0*((Point3D&)P3)[2];
	object = new Point3D(x,y,z);
	object->user.push_back(this);
	BezierRef.push_back(object);
	_this->AddPoint(object);
	

	x = 1.0/3.0*((Point3D&)P2)[0] + 2.0/3.0*((Point3D&)P3)[0];
	y = 1.0/3.0*((Point3D&)P2)[1] + 2.0/3.0*((Point3D&)P3)[1];
	z = 1.0/3.0*((Point3D&)P2)[2] + 2.0/3.0*((Point3D&)P3)[2];
	object = new Point3D(x,y,z);
	object->user.push_back(this);
	BezierRef.push_back(object);
	_this->AddPoint(object);
	

	x = 1.0/6.0*((Point3D&)P2)[0] + 2.0/3.0*((Point3D&)P3)[0] + 1.0/6.0*((Point3D&)P4)[0];
	y = 1.0/6.0*((Point3D&)P2)[1] + 2.0/3.0*((Point3D&)P3)[1] + 1.0/6.0*((Point3D&)P4)[1];
	z = 1.0/6.0*((Point3D&)P2)[2] + 2.0/3.0*((Point3D&)P3)[2] + 1.0/6.0*((Point3D&)P4)[2];
	object = new Point3D(x,y,z);
	object->user.push_back(this);
	BezierRef.push_back(object);
	_this->AddPoint(object);
}

void BSpline3D::Generate(int accuracy, vector<Object3D *> & selectedPoints)
{
	this->accuracy = accuracy;
	if ( points ) 
		delete [] points;
	
	if ( BezierMode ) {
		// bezier3 C2 mode
		MessageBox(0, TEXT("Wylacz BezierMode litera \"M\""), TEXT("Informacja"), 0);
	} else {
		// deBoor C2 mode
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

			deBoorRef = selectedPoints;

			for ( double t=0.0; t < 1.0; t+=d) {
				CalculatedeBoor(t, (*selectedPoints[0]), (*selectedPoints[1]), 
					(*selectedPoints[2]), (*selectedPoints[3]), points[j]);
				++j;
			}
			AddBeziers((*selectedPoints[0]), (*selectedPoints[1]), (*selectedPoints[2]), (*selectedPoints[3]),true);
		} else if ( sellen > 4 ) {
			int temp = sellen-4;
			sellen = temp+1;

			size = (accuracy+1)*(sellen);
			points = new Vector3D[size];
			double d = 1.0/(double)accuracy;

			int j=0;
			deBoorRef = selectedPoints;

			for ( int i=0; i < selectedPoints.size(); ++i )
			{
				((Point3D*)selectedPoints[i])->user.push_back(this);
			}

			for ( i=0; i < sellen; ++i ) {
				AddBeziers((*selectedPoints[i]), (*selectedPoints[i+1]), (*selectedPoints[i+2]), (*selectedPoints[i+3]),(i==0));
				for ( double t=0.0; t < 1.0; t+=d) {
					CalculatedeBoor(t, (*selectedPoints[i]), (*selectedPoints[i+1]), 
						(*selectedPoints[i+2]), (*selectedPoints[i+3]), points[j]);
					++j;
				}
			}
		}

	}
}

void BSpline3D::ReCalculate()
{
	if ( !changed )
		return;
	if ( BezierMode ) {
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
			CalcdeBoor((*BezierRef[0]), (*BezierRef[1]), (*BezierRef[2]), (*BezierRef[3]),0, true);
		} else if ( sellen > 4 ) {
			int temp = sellen-4;
			sellen = (temp/3)+1;

			size = (accuracy+1)*(sellen);
			points = new Vector3D[size];
			double d = 1.0/(double)accuracy;
			int j=0;

			for ( int i=0; i < sellen; ++i ) {
				if ( deBoorRef.size() != 4+(sellen-1)) 
					AdddeBoor((*BezierRef[3*i]), (*BezierRef[3*i+1]), (*BezierRef[3*i+2]), (*BezierRef[3*i+3]));
				else
					CalcdeBoor((*BezierRef[3*i]), (*BezierRef[3*i+1]), (*BezierRef[3*i+2]), (*BezierRef[3*i+3]), i, (i==0));
				for ( double t=0.0; t < 1.0; t+=d) {
					CalculateBezier(t, (*BezierRef[3*i]), (*BezierRef[3*i+1]), 
						(*BezierRef[3*i+2]), (*BezierRef[3*i+3]), points[j]);
					++j;
				}
			}
		}
	} else {
		if ( deBoorRef.size() == 0 ) 
			return;
		if ( points ) 
			delete [] points;
		
		int sellen=deBoorRef.size();

		if ( sellen == 4 ) {
			sellen=1;
			size = (accuracy+1)*(sellen);
			points = new Vector3D[size];
			double d = 1.0/(double)accuracy;
			int j=0;

			for ( double t=0.0; t < 1.0; t+=d) {
				CalculatedeBoor(t, (*deBoorRef[0]), (*deBoorRef[1]), 
					(*deBoorRef[2]), (*deBoorRef[3]), points[j]);
				++j;
			}
			if ( BezierRef.size() == 0 )
				AddBeziers((*deBoorRef[0]), (*deBoorRef[1]), (*deBoorRef[2]), (*deBoorRef[3]),true);
			else
				CalcBeziers((*deBoorRef[0]), (*deBoorRef[1]), (*deBoorRef[2]), (*deBoorRef[3]),0, true);
		} else if ( sellen > 4 ) {
			int temp = sellen-4;
			sellen = temp+1;

			size = (accuracy+1)*(sellen);
			points = new Vector3D[size];
			double d = 1.0/(double)accuracy;
			int j=0;

			for ( int i=0; i < sellen; ++i ) {
				if ( BezierRef.size() != (temp*3)+4) 
					AddBeziers((*deBoorRef[i]), (*deBoorRef[i+1]), (*deBoorRef[i+2]), (*deBoorRef[i+3]), false);
				else
					CalcBeziers((*deBoorRef[i]), (*deBoorRef[i+1]), (*deBoorRef[i+2]), (*deBoorRef[i+3]),i*3, i==0);
				for ( double t=0.0; t < 1.0; t+=d) {
					CalculatedeBoor(t, (*deBoorRef[i]), (*deBoorRef[i+1]), 
						(*deBoorRef[i+2]), (*deBoorRef[i+3]), points[j]);
					++j;
				}
			}
		}
	}
}


BSpline3D::BSpline3D(void * scene):accuracy(15),BezierMode(false), world(scene)
{

}

BSpline3D::~BSpline3D()
{
	for ( int i=0; i < deBoorRef.size(); ++i ) {
		for ( int j = 0 ; j  < ((Point3D*)deBoorRef[i])->user.size(); ++j ) {
			if ( ((Point3D*)deBoorRef[i])->user[j] == this ) {
				((Point3D*)deBoorRef[i])->user[j]=0;
			}
		}
	}
	deBoorRef.clear();	
}

}