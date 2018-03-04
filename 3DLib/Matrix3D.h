// Matrix3D.h: interface for the Matrix3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATRIX3D_H__0224DBE1_CDC0_4240_9456_8F874BC5430A__INCLUDED_)
#define AFX_MATRIX3D_H__0224DBE1_CDC0_4240_9456_8F874BC5430A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include <memory.h>
#include "Vector3D.h"
#include <algorithm> // std::swap
#include <vector>
#pragma warning( disable: 4127 ) // ignore conditional expression is constant
#pragma inline_depth( 1028 )
#pragma inline_recursion( on )

#define MSIZE 4

using namespace std;

namespace ThreeDimLib {
// definition of Matrix3D class
class Matrix3D;
class Mesh3D;
class Vector3D;


template< class Mtx, unsigned N > struct TransMtx
{
   static inline void eval( Mtx& mtx )
   {
      TransMtxImpl< Mtx, N, 0, 1, 0 >::eval( mtx );
   }
};

template< class Mtx, unsigned N, unsigned C, unsigned R, unsigned I >
struct TransMtxImpl
{
   enum
   {
      NxtI = I + 1,
      NxtC = NxtI / N % N,
      NxtR = ( NxtI % N ) + NxtC + 1
   };
   static inline void eval( Mtx& mtx )
   {
      if( R < N )
         std::swap( mtx[ C ][ R ], mtx[ R ][ C ] );
      TransMtxImpl< Mtx, N, NxtC, NxtR, NxtI >::eval( mtx );
   }
};

template<> struct TransMtxImpl< Matrix3D, 4, 0, 1, 4*4 >
{
   static inline void eval( Matrix3D& ) {}
};

#define TransMtxT( MtxType, Mtx, N ) TransMtx< MtxType, N >::eval( Mtx )


template< class Mtx, unsigned N > struct MultMtx
{
   static inline void eval( Mtx r, const Mtx a, const Mtx b )
   {
      MultMtxImpl< Mtx, N, 0, 0, 0, 0 >::eval( r, a, b );
   }
};

template< class Mtx, unsigned N, unsigned C, unsigned R, unsigned K, unsigned I >
struct MultMtxImpl
{
   enum
   {
      NxtI = I + 1,           
      NxtK = NxtI % N,        
      NxtC = NxtI / N % N,    
      NxtR = NxtI / N / N % N 
   };
   static inline void eval( Mtx r, const Mtx a, const Mtx b )
   {
      r[ C ][ R ] += a[ K ][ R ] * b[ C ][ K ];
      MultMtxImpl< Mtx, N, NxtC, NxtR, NxtK, NxtI >::eval( r,a,b );
   }
};


template<> struct MultMtxImpl< double [MSIZE][MSIZE], 4, 0, 0, 0, 4*4*4 >
{
   static inline void eval( double [MSIZE][MSIZE], const double [MSIZE][MSIZE], const double [MSIZE][MSIZE] ) {}
};

/*
 * Multiply Matrix Macro
 */
#define MultMtxT( MtxType, r, a, b, N ) MultMtx< MtxType, N >::eval( r, a, b )


	
class Matrix3D  
{
	/*
		This class will be used only for 3D application, because it is only performanced for it.
		It has only this methods which I used in 3D applications. Many methods are not so good that 
		thay can be, not so fast. if I have time I will make them faster.
		*/

	friend class Vector3D;				// Vector3D shlould be a friend of Matrix3D class
	friend class Object3D;
	friend class Mesh3D;
	double matrix[MSIZE][MSIZE];		// we operata on real values so it has double array.
public:

	/*
	 * Standard constructor
	 */
	inline Matrix3D()					
	{ 
		ZeroMatrix(); // clear matrix
	}

	/*
	 * Destructor, doing nothing, because memory is static
	 */
	virtual ~Matrix3D(); 

	/*
	 * Put zeros in matrix body
	 */
	inline void ZeroMatrix() {
		// clear all arrays
		memset(matrix[0], 0, sizeof(double)*MSIZE); 
		memset(matrix[1], 0, sizeof(double)*MSIZE); 
		memset(matrix[2], 0, sizeof(double)*MSIZE); 
		memset(matrix[3], 0, sizeof(double)*MSIZE); 
	}

	/*
	 * Load Identity Matrix
	 */
	inline void LoadIdentity() { 
		ZeroMatrix(); 
		// put on diagonal 1.0 value
		matrix[0][0]=1.0; 
		matrix[1][1]=1.0; 
		matrix[2][2]=1.0; 
		matrix[3][3]=1.0; 
	}

	/*
	 * Copy constructor of Matrix3D
	 */ 
	inline Matrix3D(Matrix3D & rhs) { 
		// copy arrays
		memcpy(matrix[0], rhs.matrix[0], sizeof(double) * MSIZE); 
		memcpy(matrix[1], rhs.matrix[1], sizeof(double) * MSIZE); 
		memcpy(matrix[2], rhs.matrix[2], sizeof(double) * MSIZE); 
		memcpy(matrix[3], rhs.matrix[3], sizeof(double) * MSIZE); 
	}

	/*
	 * Multiplaying matrix by another matrix
	 */
	inline Matrix3D operator * (Matrix3D & rhs) {
		Matrix3D temp;
		// using expandable macro to multiply matrix by matrix
		MultMtxT(double [MSIZE][MSIZE], temp.matrix, matrix, rhs.matrix, 4);
		return temp;
	}

	/*
	 * Add To matrix one value
	 */
	inline Matrix3D operator + (double d) {
		Matrix3D temp;
		for(register int i=0;i<MSIZE;i++)
			for(register int j=0;j<MSIZE;j++)
				temp.matrix[i][j] = matrix[i][j] + d;
		return temp;
	}

	/*
	 * Add One Matrix to another
	 */
	inline Matrix3D operator + (Matrix3D & rhs) {
		Matrix3D temp;
		for(register int i=0;i<MSIZE;i++)
			for(register int j=0;j<MSIZE;j++)
				temp.matrix[i][j] = matrix[i][j] + rhs.matrix[i][j];
		return temp;
	}

	/*
	 * Subract One Matrix from another
	 */
	inline Matrix3D operator - (Matrix3D & rhs) {
		Matrix3D temp;
		for(register int i=0;i<MSIZE;i++)
			for(register int j=0;j<MSIZE;j++)
				temp.matrix[i][j] = matrix[i][j] - rhs.matrix[i][j];
		return temp;
	}

	/*
	 * Subract value from matrix
	 */
	inline Matrix3D operator - (double d) {
		Matrix3D temp;
		for(register int i=0;i<MSIZE;i++)
			for(register int j=0;j<MSIZE;j++)
				temp.matrix[i][j] = matrix[i][j] - d;
		return temp;
	}


	inline Matrix3D & operator += (Matrix3D & rhs) {
		for(register int i=0;i<MSIZE;i++)
			for(register int j=0;j<MSIZE;j++)
				matrix[i][j] += rhs.matrix[i][j];
		return *this;
	}

	inline Matrix3D & operator += (double d) {
		for(register int i=0;i<MSIZE;i++)
			for(register int j=0;j<MSIZE;j++)
				matrix[i][j] += d;
		return *this;
	}

	/*
	 * Clear Matrix and
	 * put on diagonal some values
	 */
	inline void Diagonal(double x, double y, double z, double w) {
		ZeroMatrix();
		matrix[0][0] = x;
		matrix[1][1] = y;
		matrix[2][2] = z;
		matrix[3][3] = w;
	}

	inline Matrix3D & operator -= (double d) {
		for(register int i=0;i<MSIZE;i++)
			for(register int j=0;j<MSIZE;j++)
				matrix[i][j] -= d;
		return *this;
	}

	inline Matrix3D & operator -= (Matrix3D & rhs) {
		for(register int i=0;i<MSIZE;i++)
			for(register int j=0;j<MSIZE;j++)
				matrix[i][j] -= rhs.matrix[i][j];
		return *this;
	}

	/*
	 * Get apropiate array 
	 * Method didn;t check if it is good index, must be good.
	 */
	inline double * operator[](int index) { 
		return (matrix[index]); 
	}

	Vector3D operator * (Vector3D & rhs);
	Vector3D& operator *= (Vector3D & rhs);

	inline Vector3D SolveSE(Matrix3D& A,Vector3D &B) {

		Vector3D X;
		Matrix3D L;
		int i,j,k;


		for(k = 0; k<4; ++k)
			for(i = k+1;i<4; ++i)
			{


				int jmax = k;

				double MAX = fabs(A[k][k]);



				for(j = k+1; j<4; ++j)

					if(fabs(A[j][k])>MAX)

					{

						MAX = fabs(A[j][k]);

						jmax = j;

					}



				if(jmax != k)

				{

					for(j = k; j<4; ++j)

						std::swap(A[k][j],A[jmax][j]);

					

					std::swap(B[k],B[jmax]);

				}



				L[i][k] = A[i][k]/A[k][k];



				for(j = k+1; j<4; ++j)

					A[i][j] -= L[i][k]*A[k][j];



				B[i] -= L[i][k]*B[k];

			}



		X[3] = B[3]/A[3][3];



		for(i=2;i>=0; --i)

		{

			X[i] = B[i];

			for(j = i+1; j<4; ++j)

				X[i] -= A[i][j]*X[j];

			X[i]/=A[i][i];

		}

		return X;

	}


};

}

#endif // !defined(AFX_MATRIX3D_H__0224DBE1_CDC0_4240_9456_8F874BC5430A__INCLUDED_)
