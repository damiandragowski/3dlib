// Matrix3D.cpp: implementation of the Matrix3D class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Matrix3D.h"
#include "Vector3D.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace ThreeDimLib {

Vector3D Matrix3D::operator * (Vector3D & rhs)
{
	Vector3D temp;
	temp.vector[0] = matrix[0][0] * rhs.vector[0] + matrix[0][1] * rhs.vector[1] + matrix[0][2] * rhs.vector[2] + matrix[0][3] * rhs.vector[3];
	temp.vector[1] = matrix[1][0] * rhs.vector[0] + matrix[1][1] * rhs.vector[1] + matrix[1][2] * rhs.vector[2] + matrix[1][3] * rhs.vector[3];
	temp.vector[2] = matrix[2][0] * rhs.vector[0] + matrix[2][1] * rhs.vector[1] + matrix[2][2] * rhs.vector[2] + matrix[2][3] * rhs.vector[3];
	temp.vector[3] = matrix[3][0] * rhs.vector[0] + matrix[3][1] * rhs.vector[1] + matrix[3][2] * rhs.vector[2] + matrix[3][3] * rhs.vector[3];
	return temp;
}

Vector3D& Matrix3D::operator *= (Vector3D & rhs)
{
	double a(rhs.vector[0]), b(rhs.vector[0]), c(rhs.vector[0]), d(rhs.vector[0]);
	rhs.vector[0] = matrix[0][0] * a + matrix[0][1] * b + matrix[0][2] * c + matrix[0][3] * d;
	rhs.vector[1] = matrix[1][0] * a + matrix[1][1] * b + matrix[1][2] * c + matrix[1][3] * d;
	rhs.vector[2] = matrix[2][0] * a + matrix[2][1] * b + matrix[2][2] * c + matrix[2][3] * d;
	rhs.vector[3] = matrix[3][0] * a + matrix[3][1] * b + matrix[3][2] * c + matrix[3][3] * d;
	return rhs;
}


Matrix3D::~Matrix3D()
{

}

}