// Vector3D.h: interface for the Vector3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECTOR3D_H__3C8439F0_9151_4568_9935_F0A9DADF26EF__INCLUDED_)
#define AFX_VECTOR3D_H__3C8439F0_9151_4568_9935_F0A9DADF26EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <memory.h>
#include <math.h>
#ifndef MSIZE
#define MSIZE 4
#endif
//#include "Matrix3D.h"

namespace ThreeDimLib {

class Vector3D  
{
	friend class Matrix3D;
	double vector[MSIZE];
public:


	// Implemetation of some methods
	inline void set4()
	{
		vector[3] = 1;
	}
	inline Vector3D() 
	{ 
		ZeroVector(); 
	}

	Vector3D(const double x, const double y, const double z) { 
		vector[0] = x; 
		vector[1] = y; 
		vector[2] = z; 
		vector[3] = 1; 
	}

	void setVector(const double x, const double y, const double z) { 
		vector[0] = x; 
		vector[1] = y; 
		vector[2] = z; 
		vector[3] = 1; 
	}

	Vector3D(const Vector3D & rhs) { 
		memcpy(vector, rhs.vector, sizeof(double)*MSIZE); 
	}

	inline void Normalize()
	{
		const double norma = L2Norm();
		for ( register int i=0; i < MSIZE; ++i ) 
			vector[i] /= norma; 
		//vector[3] = 1; 
	}

	inline const Vector3D operator * (const double s) const { 
		Vector3D temp(*this);
		for ( register int i=0; i < MSIZE; ++i ) 
			temp.vector[i] = vector[i]*s; 
		return temp;
	}

	inline const Vector3D operator / (const double s) const { 
		Vector3D temp(*this);
		for ( register int i=0; i < MSIZE; ++i ) 
			temp.vector[i] /= s; 
		return temp;
	}


	inline const Vector3D operator + (const Vector3D & rhs) const { 
		Vector3D temp(*this);
		for ( register int i=0; i < MSIZE; ++i ) temp.vector[i] = vector[i] + rhs.vector[i]; 	
		return temp;
	}

	inline const Vector3D operator - (const Vector3D & rhs) const{ 
		Vector3D temp(*this);
		for ( register int i=0; i < MSIZE; ++i ) temp.vector[i] = vector[i] - rhs.vector[i]; 	
		return temp;
	}

	inline Vector3D & operator += (const Vector3D & rhs) { 
		for ( register int i=0; i < MSIZE; ++i ) 
			vector[i] += rhs.vector[i]; 
		return *this; 
	} 

	inline Vector3D & operator -= (const Vector3D & rhs) { 
		for ( register int i=0; i < MSIZE; ++i ) 
			vector[i] -= rhs.vector[i]; 
		return *this; 
	} 

	inline Vector3D & operator = (const Vector3D & rhs)
	{
		memcpy(vector, rhs.vector, sizeof(double)*MSIZE); 
		return *this;
	}

	inline double & operator [] ( int index ) { 
		return vector[index]; 
	}

	inline void ZeroVector() { 
		memset(vector, 0, sizeof(double)*MSIZE); 
	}

	/* 
	 * Compute vector L1 norm
	 */
	inline const double L1Norm() const
	{
		double a=0.0;
		a+=fabs(vector[0]);
		a+=fabs(vector[1]);
		a+=fabs(vector[2]);
		a+=fabs(vector[3]);
		return a;
	}

	/* 
	 * Compute vector L2 norm
	 */
	inline const double L2Norm() const
	{
		double a=0.0;
		a+=vector[0]*vector[0];
		a+=vector[1]*vector[1];
		a+=vector[2]*vector[2];
		//a+=vector[3]*vector[3];
		return sqrt(a);
	}

	/* 
	 * Compute vector L3 norm
	 */
	inline const double L3Norm() const
	{
		double a=0.0;
		a+=fabs(vector[0]*vector[0]*vector[0]);
		a+=fabs(vector[1]*vector[1]*vector[1]);
		a+=fabs(vector[2]*vector[2]*vector[2]);
		a+=fabs(vector[3]*vector[3]*vector[3]);
		return pow(a, 1.0/3.0);
	}

	/* 
	 * Compute vector LN norm
	 * int N - Norm Dimension
	 */
	inline const double LNNorm(int N) const
	{
		double a=1.0, b=1.0, c=1.0, d=1.0;
		for ( register int i=0; i < N; ++i ) {
			a*=vector[0];
			b*=vector[1];
			c*=vector[2];
			d*=vector[3];
		}
		a = fabs(a);
		a += fabs(b)+fabs(c)+fabs(d);
		return pow(a, 1.0/double(N));
	}

	inline const double distPoint(Vector3D & rhs)
	{
		Vector3D t = (*this)-rhs;
		
/*		double a=0.0;
		a+=(vector[0]-rhs.vector[0])*(vector[0]-rhs.vector[0]);
		a+=(vector[1]-rhs.vector[1])*(vector[1]-rhs.vector[1]);
		a+=(vector[2]-rhs.vector[2])*(vector[2]-rhs.vector[2]);
		//a+=fabs(vector[3]*rhs.vector[3]);
		a=sqrt(a);*/
		return t.L2Norm();;
	}


	// definition
	virtual ~Vector3D();
	Vector3D operator * (const Matrix3D & rhs);
	Vector3D& operator *= (const Matrix3D & rhs);
	friend inline Vector3D cross(const Vector3D & u, const Vector3D & v);
	friend inline double dot(const Vector3D & u, const Vector3D & v);
};

inline double dot(const Vector3D & u, const Vector3D & v) {
		double a=0.0;
		a+=u.vector[0]*v.vector[0];
		a+=u.vector[1]*v.vector[1];
		a+=u.vector[2]*v.vector[2];
		return a;
}


inline Vector3D cross(const Vector3D & u, const Vector3D & v) {
	return Vector3D(u.vector[1]*v.vector[2]-u.vector[2]*v.vector[1], u.vector[2]*v.vector[0]-u.vector[0]*v.vector[2], u.vector[0]*v.vector[1]-u.vector[1]*v.vector[0]);
}

}

#endif // !defined(AFX_VECTOR3D_H__3C8439F0_9151_4568_9935_F0A9DADF26EF__INCLUDED_)
