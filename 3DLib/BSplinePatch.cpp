// Bezier3D.cpp: implementation of the Bezier3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BSplinePatch.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define dim 4

namespace ThreeDimLib {

void BSplinePatch::DrawObject3D(HDC hdc, Matrix3D & stereoLeft, Matrix3D & stereoRight, Matrix3D & CameraModification, double cutSurfaceInterval)
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

	/*
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
	}*/

	if ( selected )
		oldPen = (HPEN) SelectObject(hdc, rightSelectedObjectPen);
	else
		oldPen = (HPEN) SelectObject(hdc, rightObjectPen);

	/*
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
	*/
	if ( trimmed ) {
		SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));
		_TParam uv = trimcurve.uv[0];
		Vector3D v1=value(uv.u, uv.v);
		temp = v1*(CameraModification);

		temp[0] += temp[2]*c;
		temp[3] = temp[2]*d+1;
		pt[0].x = (long)(temp[0]/temp[3]);
		pt[0].y = (long)(temp[1]/temp[3]);

		SetPixelV(hdc, pt[0].x,pt[0].y, RGB(255,255,255));
		//MoveToEx(hdc, pt[0].x, pt[0].y, &p);  			
		for (int f=0; f<trimcurve.uv.size(); ++f) {
		uv = trimcurve.uv[f];
		v1=value(uv.u, uv.v);
		temp = v1*(CameraModification);

		temp[0] += temp[2]*c;
		temp[3] = temp[2]*d+1;
		pt[0].x = (long)(temp[0]/temp[3]);
		pt[0].y = (long)(temp[1]/temp[3]);

		SetPixelV(hdc, pt[0].x,pt[0].y, RGB(255,255,255));
		//LineTo(hdc, pt[0].x, pt[0].y);  			
		}
	}
	SetROP2(hdc, old);
	SelectObject(hdc, oldPen);


}


 
void BSplinePatch::ReCalculate()
{
	if ( !changed )
		return;
	changed=false;
	static int d;
	static Vector3D t1;
	d=0;

	//int di,dj;
// u

	double minu=u+1.0;
	double maxu=-1.0;
	double minv=(double)-1.0;
	double maxv=(double)v;

	if ( trimmed ) {
		for ( int r=0; r < trimcurve.uv.size(); ++r ) {
			if ( trimcurve.uv[r].u < minu )
				minu=trimcurve.uv[r].u ;
		}

		for ( r=0; r < trimcurve.uv.size(); ++r ) {
			if ( trimcurve.uv[r].u > maxu )
				maxu=trimcurve.uv[r].u ;
		}



		for ( r=0; r < trimcurve.uv.size(); ++r ) {
			if ( trimcurve.uv[r].v > minv && trimcurve.uv[r].v < (double)v/2.0 )
				minv=trimcurve.uv[r].v ;
		}

		for ( r=0; r < trimcurve.uv.size(); ++r ) {
			if ( trimcurve.uv[r].v < maxv && trimcurve.uv[r].v > (double)v/2.0 )
				maxv=trimcurve.uv[r].v ;
			
		}
	}

	bool lastpod=false;
	double rv=0.0;


	for ( int i=0; i < u; ++i ) 
		for ( int j=0;j < v; ++j ) {
			


			for ( int ff=0; ff < dim+1; ++ff ) {
				double _u = i+(double)ff/dim;
				double min=100.0;
				int mine=0;
				double max=100.0;
				int maxe=0;

				double maximU=0.0;
				double minimU=0.0;

				if ( !podstawa ) {
					if ( _u < maxu && _u > minu) {
						for ( int e=0; e < trimcurve.uv.size(); ++e )  {
							if ( fabs(trimcurve.uv[e].u-_u) < min && trimcurve.uv[e].v < (double)v/2.0 ) {
								min = fabs(trimcurve.uv[e].u-_u);
								mine=e;
								minimU = trimcurve.uv[e].v;
							}
							if ( fabs(trimcurve.uv[e].u-_u) < max && trimcurve.uv[e].v > (double)v/2.0 ) {
								max = fabs(trimcurve.uv[e].u-_u);
								maxe=e;
								maximU = trimcurve.uv[e].v;
							}
						}
					}
				}
				for ( int h=0; h < accuracy+1; ++h ) {
					double _v = j+(double)h/(accuracy);
					bool skip = true;
				
					if ( podstawa ) {
						if ( _u-(double)1.0/dim < minu )
							skip = false;
					} else {

						if ( _u < maxu && _u > minu) {

							if ( _v-minimU < 0.0 ) {
								points[d] = value(trimcurve.uv[mine].u, trimcurve.uv[mine].v);
								++d;
								skip=false;
							}
							if ( _v-maximU > 0.0 ) {
								points[d] = value(trimcurve.uv[maxe].u, trimcurve.uv[maxe].v);
								++d;
								skip=false;
							}

						} 
					}
					
					if ( skip ) {
						points[d] = value(_u, _v);
						++d;
					}

				}
			}




// v

			for ( int h=0; h < accuracy+1; ++h ) {
				double _v = j+(double)h/(accuracy);

				if ( !podstawa && (_v < minv || _v > maxv) ) {
					// skip=false;

					double min=100.0;
					int mine=0;
					double max=100.0;
					int maxe=0;
					double halfu = minu + (maxu-minu)/2.0;

					double maximU=0.0;
					double minimU=0.0;

					for ( int e=0; e < trimcurve.uv.size(); ++e )  {
						if ( fabs(trimcurve.uv[e].v-_v) < min && trimcurve.uv[e].u < halfu ) {
							min = fabs(trimcurve.uv[e].v-_v);
							mine=e;
							minimU = trimcurve.uv[e].u;
						}
						if ( fabs(trimcurve.uv[e].v-_v) < max && trimcurve.uv[e].u > halfu ) {
							max = fabs(trimcurve.uv[e].v-_v);
							maxe=e;
							maximU = trimcurve.uv[e].u;
						}
					}



					for ( int ff=0; ff < dim+1; ++ff ) {
						double _u = i+(double)ff/dim;
						bool skip=true;

						if ( minimU-_u < 0.0 ) {
							points[d] = value(trimcurve.uv[mine].u, trimcurve.uv[mine].v);
							++d;
							skip=false;
						}
						if ( skip ) {
							points[d] = value(_u, _v);
							++d;
						}

					}
					for ( ff=0; ff < dim+1; ++ff ) {
						double _u = i+(double)ff/dim;
						bool skip=true;

						if ( maximU-_u > 0.0 ) {
							points[d] = value(trimcurve.uv[maxe].u, trimcurve.uv[maxe].v);
							++d;
							skip=false;
						}
						if ( skip ) {
							points[d] = value(_u, _v);
							++d;
						}

					}

				} else {

					for ( int ff=0; ff < dim+1; ++ff ) {
						double _u = i+(double)ff/dim;
						bool skip = true;
						if ( podstawa ) {
							for ( int r=0; r < trimcurve.uv.size(); ++r ) {
								if ( fabs(trimcurve.uv[r].u - _u) < 0.5 ) {
									skip = false;
									for ( int e=0; e < trimcurve.uv.size(); ++e ) 
										if ( fabs(trimcurve.uv[e].v-_v) < 0.05 ) {
											points[d] = value(trimcurve.uv[e].u, trimcurve.uv[e].v);
										}
									++d;
									break;
								}
							}
						} 
						if ( skip ) {
							points[d] = value(_u, _v);
							++d;
						} 
					}
		
				}
			}
		}
		size = d;
}


Vector3D BSplinePatch::value(double _u, double _v) 
{
	static int d;
	Vector3D t1;

	
	int i = (int) _u;

	if ( _u == u ) {
		--i;
		_u = 1.0;
	} else
		_u= _u-(int)(_u);

	int j = (int) _v;
	if ( _v == v ) {
		--j;
		_v = 1.0;
	} else
		_v= _v-(int)(_v);
	/*if ( _u > u )
		_u-=u;
	
	if ( _v > v )
		_v-=v;*/

	t1.ZeroVector();
	for ( int x=0; x < 4; ++x )
		for ( int y=0; y < 4; ++y ) {
			t1 += ((Point3D*)(PointsRef[(i+x)*(v+3)+(j+y)]))->getVector() * B(x, _u) * B(y, _v);
		}
	return t1;
}

void BSplinePatch::Generate(vector<Object3D *> & selectedPoints,int _u, int _v)
{
	podstawa=false;
	v = _v;
	u = _u;
	changed = true;
	if ( points )
		delete [] points;

	accuracy = 4;



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


	size = (u+(u-1))*(v+(v-1))*((dim+1)*(accuracy+1))+ ((dim+1)*(accuracy+1));
	size*=2;
	points = new Vector3D[size];


	PointsRef=selectedPoints;

	for ( int g=0; g < selectedPoints.size(); ++g )
	{
		((Point3D*)selectedPoints[g])->user.push_back(this);
	}


	ReCalculate();	
}

void BSplinePatch::MakeMesh(double minu, double maxu, double minv, double maxv, PreciseMesh & mesh)
{
	double dSizeU = (maxu-minu);
	double dSizeV = (maxv-minv);
	double u;
	double v;
	for(int i=0;i<20;++i) {
		u = minu + (double)i/20 * dSizeU;
		for(int j=0;j<20;++j) {
			v = minv + (double)j/20 * dSizeV;
			mesh.coordinates[i][j]=value(u,v);
			mesh.uv[i][j] = _TParam(u,v);
		}
	}
}

bool BSplinePatch::startingPoint(BSplinePatch & bs)
{
	_TParam uv;
	const double eps = 1e-5;
	int distWithoutChange = 0;
	double dist = 10000.0, distPrev=1000000.0;
	double minU = 0, maxU = this->u;
	double minV = 0, maxV = this->v;
	double minS = 0, maxS = bs.u;
	double minT = 0, maxT = bs.v;
	unsigned i1,i2, j1, j2;
	int i1min=-1,j1min=-1,i2min=-1,j2min=-1;
	double u,v,s,t;
	
	while( dist > eps && distWithoutChange < 10)
	{		
		PreciseMesh mesh1, mesh2;
		MakeMesh(minU, maxU, minV, maxV, mesh1);
		bs.MakeMesh(minS, maxS, minT, maxT, mesh2);

		for(i1 = 0; i1<20; ++i1)
			for(j1 = 0; j1< 20; ++j1)
				for(i2 = 0; i2< 20; ++i2)
					for(j2 = 0; j2< 20; ++j2) {
						double pom = mesh1.coordinates[i1][j1].distPoint(mesh2.coordinates[i2][j2]);
						if(pom <= dist) {	
							i1min = i1;
							j1min = j1;
							i2min = i2;
							j2min = j2;
							dist = pom;
						}
					}


		double du = (double)(maxU-minU)/ 40.0,
			   dv = (double)(maxV-minV)/ 40.0,
			   ds = (double)(maxS-minS)/ 40.0,
			   dt = (double)(maxT-minT)/ 40.0;

		_TParam uv1 = mesh1.uv[i1min][j1min];
		_TParam uv2 = mesh2.uv[i2min][j2min];
		u = uv1.u; v = uv1.v;
		s = uv2.u; t = uv2.v;
	
		minU = (u<=0)?		minU:(u-du);
		maxU = (u>=this->u)?maxU:(u+du);
		minV = (v<=0)?		minV:(v-dv);
		maxV = (v>=this->v)?maxV:(v+dv);
		
		minS = (s<=0 )?			  minS:(s-ds);
		maxS = (s>=bs.u)?maxS:(s+ds);
		minT = (t<=0)?			  minT:(t-dt);
		maxT = (t>=bs.v)?maxT:(t+dt);
		
		if (dist < distPrev) {
			distWithoutChange = 0;
			distPrev = dist;
		}
		else
			++distWithoutChange;

		
	}

	if (distWithoutChange == 10) 
		return false;
	
	trimcurve.uv.push_back(_TParam(u,v));
	bs.trimcurve.uv.push_back(_TParam(s,t));

	return true;
}

void BSplinePatch::Trim(BSplinePatch & bs)
{
	// szukanie punktu startowego
	podstawa=true;
	trimcurve.clear();
	bs.trimcurve.clear();
	if ( startingPoint(bs) )
	{
		_TParam uv, st;
		uv = trimcurve.uv[0];
		st = bs.trimcurve.uv[0];

		double u = uv.u,
			   v = uv.v,
			   s = st.u,
			   t = st.v;



		Vector3D f1 = value(u,v);
		Vector3D f2 = bs.value(s,t);
		double u0, v0;
		double d = 0.01;			//distance
		double d1 = 1.0;
		double d2 = 1.0;
		int i =0;
		do {
			++i;
		
			Vector3D diffU = derivativeU(u,v);
			Vector3D diffV = derivativeV(u,v);
			Vector3D diffS = bs.derivativeU(s,t);
			Vector3D diffT = bs.derivativeV(s,t);
			Vector3D norm1 = cross(diffU,diffV);
			Vector3D norm2 = cross(diffS,diffT);
			Vector3D tau  = cross(norm1,norm2);
			tau.Normalize();
			
			u0 = u;
			v0 = v;

			u += dot(diffU, tau * d);
			v += dot(diffV, tau * d);
			s += dot(diffS, tau * d);
			t += dot(diffT, tau * d);

			int newtonLoop = 0;
			do 
			{
				
				if (newtonLoop == 100)
					return;
				newtonLoop++;
				Vector3D F;
				Matrix3D J;
			
				F = countF(bs,u,v,u0,v0,s,t,tau,d);
				J = countJacobi(bs,u,v,s,t,tau,d);

				Vector3D stepX = J.SolveSE(J,F);
				
				u -= stepX[0];
				v -= stepX[1];
				s -= stepX[2];
				t -= stepX[3];

				while ( u > this->u )
					u -= this->u;
				while ( u < 0 )
					u += this->u;
				while ( v > this->v )
					v -= this->v;
				while ( v < 0)
					v += this->v;
				while ( s > bs.u )
					s -= bs.u;
				while ( s < 0 )
					s += bs.u;
				while ( t < 0  )
					t += bs.v;
				while ( t > bs.v )
					t -= bs.v;

			
			} while (value(u,v).distPoint(bs.value(s,t)) > 1e-10);
			
			//if ( i % 10 == 0 ) {
				trimcurve.uv.push_back(_TParam(u,v));
				bs.trimcurve.uv.push_back(_TParam(s,t));
			//}
			//if ( i == 140 ) {
///                int y=i;				
//			}

			d1 = value(u,v).distPoint(f1);
			d2 = bs.value(s,t).distPoint(f2);
		} while (i < 10 || (d1 > 1.0 && d2 > 1.0));

		
		//sort(trimcurve.uv.begin(), trimcurve.uv.end(), _TParam::lessDictX);
		//sort(bs.trimcurve.uv.begin(), bs.trimcurve.uv.end(), _TParam::lessDictX);
		changed=true;
		bs.changed=true;
		trimmed = true;
		bs.trimmed = true;
		ReCalculate();
		bs.ReCalculate();
	}
}

Matrix3D BSplinePatch::countJacobi(BSplinePatch& bs2, double u, double v, double s, double t,Vector3D tau,double d) {
	Matrix3D J;

	Vector3D P  = value(u , v );
	Vector3D Q  = bs2.value(s , t );

	Vector3D dPu = derivativeU(u,v);
	Vector3D dPv = derivativeV(u,v);
	Vector3D dQu = bs2.derivativeU(s,t);
	Vector3D dQv = bs2.derivativeV(s,t);
	
	J[0][0] = dPu[0]; J[0][1] = dPv[0]; J[0][2] = -dQu[0]; J[0][3] = -dQv[0];
	J[1][0] = dPu[1]; J[1][1] = dPv[1]; J[1][2] = -dQu[1]; J[1][3] = -dQv[1];
	J[2][0] = dPu[2]; J[2][1] = dPv[2]; J[2][2] = -dQu[2]; J[2][3] = -dQv[2];
	J[3][0] = dot(dPu , tau*d);
	J[3][1] = dot(dPv , tau*d);
	J[3][2] = 0; J[3][3] = 0;
	return J;
}


Vector3D BSplinePatch::countF(BSplinePatch& bs2,double u, double v,double u0, double v0, double s, double t,Vector3D tau, double d) {
	Vector3D F;	
	
	Vector3D P  = value(u , v);
	Vector3D Q  = bs2.value(s , t);
	Vector3D P0 = value(u0, v0);
		
	for(int i = 0 ; i < 3 ; ++i) {
		F[i] =  P[i] - Q[i];
	}
	F[3] =  dot((P - P0) , tau*d)-d;
	return F;
}

BSplinePatch::BSplinePatch():accuracy(15)
{
	trimmed=false;
	B1[0]=0;
	B1[1]=0;
	B1[2]=0;
	B1[3]=0;
	B2[0]=0;
	B2[1]=0;
	B2[2]=0;
	B2[3]=0;
}

BSplinePatch::~BSplinePatch()
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