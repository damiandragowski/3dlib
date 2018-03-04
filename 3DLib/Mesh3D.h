// Mesh3D.h: interface for the Mesh3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESH3D_H__321462FD_2759_4969_B4DF_17BACD24F381__INCLUDED_)
#define AFX_MESH3D_H__321462FD_2759_4969_B4DF_17BACD24F381__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <vector>
#include <list>
#include <fstream>
#include "Matrix3d.h"
#include "Object3d.h"
#include "Torus3D.h"
#include "Sphere3D.h"
#include "Point3D.h"
#include "Vector3D.h"	// Added by ClassView
#include "Cursor3D.h"	
#include "Bezier3D.h"
#include "BSpline.h"
#include "Interpol3D.h"
#include "BezierPatchC0.h"
#include "BSplinePatch.h"

namespace ThreeDimLib {

using namespace std;

class Mesh3D  
{
	friend class BSpline3D;
	Matrix3D	CameraModification;
	Matrix3D	CameraInvertModification;
	Matrix3D	Projection;
	Matrix3D	stereoLeft;
	Matrix3D	stereoRight;
	double		depthInterval;
	double		stereoInterval;
	double		cutSurfaceInterval;
	vector<Object3D *> Objects;
	vector<Object3D *> selectedPoints;
	BSplinePatch	* hand;
	BSplinePatch	* tube;

	Bezier3D		* tempBezier3D;
	BSpline3D		* tempBSpline3D;
	Interpol3D		* tempInterpol3D;
public:
	Cursor3D		cursor;

	inline int getSelectedCount() const { return selectedPoints.size(); }

	inline void GenerateSurfaceC0(int u, int v)
	{
		Vector3D t1=(*((Point3D*)selectedPoints[0])).getVector();
		Vector3D t2=(*((Point3D*)selectedPoints[1])).getVector();
		Vector3D t3=(*((Point3D*)selectedPoints[2])).getVector();
		deleteSelected();
		//deselectAll();

		Vector3D t4 = t2 - t1;
		Vector3D t5 = t3 - t1;

		int rows = 3*u+1;
		int cols = 3*v+1;

		for (int i = 0; i < rows; ++i) 
			for (int j = 0; j < cols; ++j) {
				Vector3D t6 = t4 * (double)(i)/(rows-1);
				Vector3D t7 = t5 * (double)(j)/(cols-1);
				Point3D * point = new Point3D((t1+t6+t7));
				if ( point == 0 )
					MessageBox(0, "dsdS","dsds",0);
				Objects.push_back((Object3D*)point);
				point->selected=true;
				selectedPoints.push_back(point);
			}
		// MessageBox(0, "dsdS","dsDS",0); 
		Point3D * temp = (Point3D*)selectedPoints[selectedPoints.size()-1];
		BezierPatchC0 * object = new BezierPatchC0();
		object->Generate(selectedPoints, u, v);
		Objects.push_back((Object3D*)object);
		selectedPoints.clear();
	}

	inline void GeneratePipeX(double r, double dx, double dy, double dz, int u, int v, vector<Object3D*> & arr)
	{
		double x;
		int s=u;
		int div = 1;
		int f=0;

		for (int i=0; i < v+3; ++i) {
			x = (double)60.0*(i/div-1.0) + dx;
			{
				int j=0;
				for (j =-1 ; j < s-1; ++j) {
					double y = r*cos((double)j/s * 2.0*PI) + dy;
					double z = r*sin((double)j/s * 2.0*PI) + dz;
					Vector3D pos(x,y,z);
					Point3D * point = new Point3D(pos);
					Objects.push_back((Object3D*)point);
					arr.push_back((Object3D*)point);
					++f;
				}
				for ( int d=0;d < 3; ++d)
					arr.push_back((Object3D*)arr[f-u+d]);
				f+=3;
			}
		}
	}

	inline void GeneratePipeY(double r, double dx, double dy, double dz, int u, int v, vector<Object3D*> & arr)
	{
		double y;
		int s=u;
		int div = 1;
		int f=0;

		for (int i=0; i < v+3; ++i) {
			y = (double)60.0*(i/div-1.0) + dy;
			{
				int j=0;
				for (j =-1 ; j < s-1; ++j) {
					double x = r*cos((double)j/s * 2.0*PI) + dx;
					double z = r*sin((double)j/s * 2.0*PI) + dz;
					Vector3D pos(x,y,z);
					Point3D * point = new Point3D(pos);
					Objects.push_back((Object3D*)point);
					arr.push_back((Object3D*)point);
					++f;
				}
				for ( int d=0;d < 3; ++d)
					arr.push_back((Object3D*)arr[f-u+d]);
				f+=3;
			}
		}
	}

	inline void GenerateDryer(double r, double dx, double dy, double dz, int u, int v, vector<Object3D*> & arr)
	{
		double y;
		int s=u;
		int div = 1;
		int f=0;
		dx+=20;

		for (int i=0; i < v+3; ++i) {
			if ( i == v+2 ) {
				r-=10;
			}
			y = (double)60.0*(i/div-1.0) + dy;
			{
				int j=0;
				for (j =-1 ; j < s-1; ++j) {
					double x = r*cos((double)j/s * 2.0*PI) + dx;
					double z = r*sin((double)j/s * 2.0*PI) + dz;
					Vector3D pos(x,y,z);
					Point3D * point = new Point3D(pos);
					Objects.push_back((Object3D*)point);
					arr.push_back((Object3D*)point);
					++f;
				}
				for ( int d=0;d < 3; ++d)
					arr.push_back((Object3D*)arr[f-u+d]);
				f+=3;
				dx-=20;
			}
			dy+=10;
		}
		dy+=200;
		r-=10;

			y = (double)60.0*(1/div-1.0) + dy;
			{
				int j=0;
				for (j =-1 ; j < s-1; ++j) {
					double x = r*cos((double)j/s * 2.0*PI) + dx;
					double z = r*sin((double)j/s * 2.0*PI) + dz;
					Vector3D pos(x,y,z);
					Point3D * point = new Point3D(pos);
					Objects.push_back((Object3D*)point);
					arr.push_back((Object3D*)point);
					++f;
				}
				for ( int d=0;d < 3; ++d)
					arr.push_back((Object3D*)arr[f-u+d]);
				f+=3;
			}


	}

	inline void GenerateDryerTube(double r, double dx, double dy, double dz, int u, int v, vector<Object3D*> & arr, vector<Point3D *> & arr2)
	{
		double x;
		int s=u;
		int div = 1;
		int f=0;
		double r1[]={r-40, r-20, r+10, r-10, r-15, r-15};

		for (int i=0; i < v+3; ++i) {
			x = (double)60.0*(i/div-1.0) + dx;
			{
				int j=0;
				for (j =-1 ; j < s-1; ++j) {
					double y = r1[i]*cos((double)j/s * 2.0*PI) + dy;
					double z = r1[i]*sin((double)j/s * 2.0*PI) + dz;
					Vector3D pos(x,y,z);
					Point3D * point = new Point3D(pos);
					Objects.push_back((Object3D*)point);
					arr2.push_back(point);
					arr.push_back((Object3D*)point);
					++f;
				}
				for ( int d=0;d < 3; ++d)
					arr.push_back((Object3D*)arr[f-u+d]);
				f+=3;
			}
		}
		dx+=150;
		r-=20;

			x = (double)60.0*(1/div-1.0) + dx;
			{
				int j=0;
				for (j =-1 ; j < s-1; ++j) {
					double y = r*cos((double)j/s * 2.0*PI) + dy;
					double z = r*sin((double)j/s * 2.0*PI) + dz;
					Vector3D pos(x,y,z);
					Point3D * point = new Point3D(pos);
					Objects.push_back((Object3D*)point);
					arr2.push_back(point);
					arr.push_back((Object3D*)point);
					++f;
				}
				for ( int d=0;d < 3; ++d)
					arr.push_back((Object3D*)arr[f-u+d]);
				f+=3;
			}
			dx+=25;
			x = (double)60.0*(1/div-1.0) + dx;
			{
				int j=0;
				Point3D * t1;
				int ds = s/4;
				for (j =0 ; j < s; ++j) {
					double y;
					if ( j == 1 )
						y = r*cos((double)j/s * 2.0*PI) + dy -10;
					else if ( j == 7 )
						y = r*cos((double)j/s * 2.0*PI) + dy +10;
					else
						y = r*cos((double)j/s * 2.0*PI) + dy;

					double z = r*sin((double)j/s * 2.0*PI) + dz;
					Vector3D pos(x,y,z);
					if ( j%ds==0 ) {
						Point3D * point = new Point3D(pos);
						t1=point;
						Objects.push_back((Object3D*)point);
						arr2.push_back(point);
						arr.push_back((Object3D*)point);
					} else {
						arr.push_back((Object3D*)t1);
						arr2.push_back(t1);
					}
					++f;
				}
				for ( int d=0;d < 3; ++d)
					arr.push_back((Object3D*)arr[f-u+d]);
				f+=3;
			}

			dx+=25;

			x = (double)60.0*(1/div-1.0) + dx;
			{
				int j=0;
				Point3D * t1;
				int ds = s/6;
				for (j =0 ; j < s; ++j) {
					double y;
					if ( j == 1 )
						y = r*cos((double)j/s * 2.0*PI) + dy - 15; // 
					else if ( j == 5) // tu zmiana
						y = r*cos((double)j/s * 2.0*PI) + dy + 15;
					else
						y = r*cos((double)j/s * 2.0*PI) + dy;
					 
					double z = r*sin((double)j/s * 2.0*PI) + dz;
					Vector3D pos(x,y,z);
					if ( j%ds==0 ) {
						Point3D * point = new Point3D(pos);
						t1=point;
						Objects.push_back((Object3D*)point);
						arr2.push_back(point);
						arr.push_back((Object3D*)point);
					} else {
						arr.push_back((Object3D*)t1);
						arr2.push_back(t1);
					}
					++f;
				}
				for ( int d=0;d < 3; ++d)
					arr.push_back((Object3D*)arr[f-u+d]);
				f+=3;
			}

			
			dx+=10;
			x = (double)60.0*(1/div-1.0) + dx;
			{
				int j=0;
				Point3D * t1;
				int ds = s/6;
				for (j =0 ; j < s; ++j) {
					double y;
					if ( j == 1 )
						y = r*cos((double)j/s * 2.0*PI) + dy - 15; // 
					else if ( j == 5) // tu zmiana
						y = r*cos((double)j/s * 2.0*PI) + dy + 15;
					else
						y = r*cos((double)j/s * 2.0*PI) + dy;
					 
					double z = r*sin((double)j/s * 2.0*PI) + dz;
					Vector3D pos(x,y,z);
					if ( j%ds==0 ) {
						Point3D * point = new Point3D(pos);
						t1=point;
						Objects.push_back((Object3D*)point);
						arr2.push_back(point);
						arr.push_back((Object3D*)point);
					} else {
						arr.push_back((Object3D*)t1);
						arr2.push_back(t1);
					}
					++f;
				}
				for ( int d=0;d < 3; ++d)
					arr.push_back((Object3D*)arr[f-u+d]);
				f+=3;
			}


			dx+=50;
			r+=20;
			x = (double)60.0*(1/div-1.0) + dx;
			{
				int j=0;
				Point3D * t1;
				int ds = s/6;
				for (j =0 ; j < s; ++j) {
					double y = r*cos((double)j/s * 2.0*PI) + dy;
					double z = r*sin((double)j/s * 2.0*PI) + dz;
					Vector3D pos(x,y,z);
					if ( j%ds==0 ) {
						Point3D * point = new Point3D(pos);
						t1=point;
						Objects.push_back((Object3D*)point);
						arr2.push_back(point);
						arr.push_back((Object3D*)point);
					} else {
						arr.push_back((Object3D*)t1);
						arr2.push_back(t1);
					}
					++f;
				}
				for ( int d=0;d < 3; ++d)
					arr.push_back((Object3D*)arr[f-u+d]);
				f+=3;
			}


	}


	inline void GenerateDryerObject()
	{
		deleteSelected();

		int u=12;
		int v=1;
		double r  = 30.0;
		double dx = 0;
		double dy = 0;
		double dz = 0;

		vector<Object3D *> arr;
		arr.clear();
		// Handle
		GenerateDryer(r, dx, dy, dz, u, v, arr);
		hand = new BSplinePatch();


		{
			int _s = Objects.size();
			fstream file_op("c:\\Hand.txt",ios::out);

		    file_op.setf (ios::fixed, ios::floatfield); 
			file_op.precision (4); 


			for ( int i=0; i < u; ++i )  {
				for ( int j = 0; j < v+4; ++j )	{
					Point3D * temp = (Point3D *)Objects[(j*u)+i];
					Vector3D vect = temp->getVector();
					file_op << vect[0] << " " << vect[1] << " " << vect[2] << ";";
				}
				file_op << endl;
			}	

			file_op.close();		
		}

		hand->Generate(arr, v+1, u);
		Objects.push_back((Object3D*)hand);

		// Body
		r = 50;
		dx-=50;
		
		vector<Point3D *> kse;

		arr.clear();
		GenerateDryerTube(r, dx, dy, dz, u, v, arr, kse);
		tube = new BSplinePatch();

		{
			int _s = kse.size();
			fstream file_op("c:\\Tube.txt",ios::out);

		    file_op.setf (ios::fixed, ios::floatfield); 
			file_op.precision (4); 

			for ( int i=0; i < u; ++i )  {
				for ( int j = 0; j < v+7; ++j )	{
					Point3D * temp = (Point3D *)kse[(j*u)+i];
					Vector3D vect = temp->getVector();
					file_op << vect[0] << " " << vect[1] << " " << vect[2] << ";";
				}
				file_op << endl;
			}

			file_op.close();		
		}


		kse.clear();
		tube->Generate(arr, v+5, u);
		Objects.push_back((Object3D*)tube);
		arr.clear();

		hand->Trim(*tube);
		
	}

	inline void RefreshDryerObject()
	{
		hand->Trim(*tube);
	}


	inline void GenerateSurfaceBSplinePipe(int u, int v, bool btrue)
	{
		if ( u < 3 || v < 1 )
			return;
		deleteSelected();

		double r  = 50.0;
		double dx = 0;
		double dy = 0;
		double dz = 0;

		vector<Object3D *> arr;
		if ( btrue )
			GeneratePipeY(r, dx, dy, dz, u, v, arr);
		else
			GeneratePipeX(r, dx, dy, dz, u, v, arr);

		BSplinePatch * object = new BSplinePatch();
		object->Generate(arr, v, u);
		Objects.push_back((Object3D*)object);
	}

	inline void GenerateSurfaceBSplinePoints(int u, int v)
	{
		Vector3D t1=(*((Point3D*)selectedPoints[0])).getVector();
		Vector3D t2=(*((Point3D*)selectedPoints[1])).getVector();
		Vector3D t3=(*((Point3D*)selectedPoints[2])).getVector();
		deleteSelected();
		//deselectAll();

		Vector3D t4 = t2 - t1;
		Vector3D t5 = t3 - t1;

		int rows = 3+u;
		int cols = 3+v;

		for (int i = 0; i < rows; ++i) 
			for (int j = 0; j < cols; ++j) {
				Vector3D t6 = t4 * (double)(i)/(rows-1);
				Vector3D t7 = t5 * (double)(j)/(cols-1);
				Point3D * point = new Point3D((t1+t6+t7));
				if ( point == 0 )
					MessageBox(0, "dsdS","dsds",0);
				Objects.push_back((Object3D*)point);
				point->selected=true;
				selectedPoints.push_back(point);
			}
		// MessageBox(0, "dsdS","dsDS",0); 
		Point3D * temp = (Point3D*)selectedPoints[selectedPoints.size()-1];
		BSplinePatch * object = new BSplinePatch();
		object->Generate(selectedPoints, u, v);
		Objects.push_back((Object3D*)object);
		selectedPoints.clear();
	}


	Vector3D CountPointer3D(double x, double y, double z)
	{
		Vector3D temp(x,y,z);

		temp[0] = (temp[0]*(z+depthInterval))/depthInterval;
		temp[1] = (temp[1]*(z+depthInterval))/depthInterval;

		temp = temp*(CameraInvertModification);

		cursor.setX(temp[0]);
		cursor.setY(temp[1]);
		cursor.setZ(temp[2]);

		return temp;
	}

	inline void PointsUnion(Point3D * point1, Point3D * point2)
	{
			point1->ReplaceRef(point1, point2);
			// delete point from the list
			vector<Object3D *>::iterator i;
			i = find(Objects.begin(), Objects.end(), point1);
			if (i != Objects.end()) 
				Objects.erase(i);
			// delete point object
			delete point1;
			point2->changed();
	}

	inline void PointsUnion()
	{
		if ( selectedPoints.size() == 2 ) {
			Point3D * point1 = (Point3D *)selectedPoints[0];
			Point3D * point2 = (Point3D *)selectedPoints[1];
			deselectAll();
			//point1 union with point2
			point1->ReplaceRef(point1, point2);
			// delete point from the list
			vector<Object3D *>::iterator i;
			i = find(Objects.begin(), Objects.end(), point1);
			if (i != Objects.end()) 
				Objects.erase(i);
			// delete point object
			delete point1;
			point2->changed();
		} else {
			MessageBox(0, TEXT("NaleŸy zaznaczyæ dwa punkty"), TEXT("Informacja"), 0 );
		}
	}

	inline void AddInterpolToObject()
	{
		if ( tempInterpol3D ) {
			if ( tempInterpol3D->isValidate() ) {
				Objects.push_back((Object3D*)tempInterpol3D);
			} else 
				delete tempInterpol3D;
			tempInterpol3D=0;
		}
	}

	inline void CreateInterpol()
	{
		tempInterpol3D = new Interpol3D(this);
	}

	inline void AddPointToInterpol(double x=0, double y=0, double z=0)
	{
		Object3D * object = new Point3D(x,y,z);
		Objects.push_back(object);
		tempInterpol3D->AddRef(object);
	}

	inline void AddBezierPatch(int u, int v)
	{
		BezierPatchC0 * object = new BezierPatchC0();
		object->Generate(selectedPoints, u, v);
		Objects.push_back((Object3D*)object);
	}

	inline void AddBezierToObject()
	{
		if ( tempBezier3D ) {
			if ( tempBezier3D->isValidate() ) {
				Objects.push_back((Object3D*)tempBezier3D);
			} else 
				delete tempBezier3D;
			tempBezier3D=0;
		}
	}

	inline void CreateBezier()
	{
		tempBezier3D = new Bezier3D();
	}

	inline void AddPointToBezier(double x=0, double y=0, double z=0)
	{
		Object3D * object = new Point3D(x,y,z);
		Objects.push_back(object);
		tempBezier3D->AddRef(object);
	}


	inline void AddBSplineToObject()
	{
		if ( tempBSpline3D ) {
			if ( tempBSpline3D->isValidate() ) {
				Objects.push_back((Object3D*)tempBSpline3D);
			} else 
				delete tempBSpline3D;
			tempBSpline3D=0;
		}
	}

	inline void CreateBSpline()
	{
		tempBSpline3D = new BSpline3D(this);
	}

	inline void ChangeBSplineMode()
	{
		vector<Object3D *>::iterator i = Objects.begin();
		while ( i != Objects.end()) {
			(*i)->ChangeMode();
			++i;
		}
		if ( tempBSpline3D )
			tempBSpline3D->ChangeMode();
	}

	inline void AddPointToBSpline(double x=0, double y=0, double z=0)
	{
		if ( tempBSpline3D->BezierMode ) {
			Object3D * object = new Point3D(x,y,z);
			tempBSpline3D->AddBezierRef(object);
			Objects.push_back(object);
		} else {
			Object3D * object = new Point3D(x,y,z);
			Objects.push_back(object);
			tempBSpline3D->AdddeBoorRef(object);
		}
	}




	void Refresh() 
	{
		vector<Object3D *>::iterator i = Objects.begin();
		while ( i != Objects.end()) {
			(*i)->ReCalculate();
			++i;
		}
	}

	void DrawCursor3D(HDC hdc)
	{
		cursor.DrawObject3D(hdc, stereoLeft, stereoRight, CameraModification, cutSurfaceInterval);
	}

	inline void RotYSelected(double alfa)
	{
		vector<Object3D *>::iterator i = Objects.begin();
		while ( i != Objects.end()) {
			if ( (*i)->selected ) {
				(*i)->RotY(alfa);
			}
			++i;
		}
	}

	inline void RotZSelected(double alfa)
	{
		vector<Object3D *>::iterator i = Objects.begin();
		while ( i != Objects.end()) {
			if ( (*i)->selected ) {
				(*i)->RotZ(alfa);
			}
			++i;
		}
	}

	inline void RotXSelected(double alfa)
	{
		vector<Object3D *>::iterator i = Objects.begin();
		while ( i != Objects.end()) {
			if ( (*i)->selected ) {
				(*i)->RotX(alfa);
			}
			++i;
		}
	}

	inline void TransSelected(double x,double y, double z)
	{
		vector<Object3D *>::iterator i = Objects.begin();
		while ( i != Objects.end()) {
			if ( (*i)->selected ) {
				(*i)->TransX(x);
				(*i)->TransY(y);
				(*i)->TransZ(z);
			}
			++i;
		}
	}

	
	inline void selectObject(Vector3D & temp)
	{
		for( register int i=0; i < Objects.size(); ++i ) {
			if ( Objects[i]->isNear(temp) ) {
				if ( Objects[i]->selected ) {
					if ( dynamic_cast<Point3D *>(Objects[i]) != 0 ) {
						vector<Object3D *>::iterator j = selectedPoints.begin();
						while ( j != selectedPoints.end()) {
							if ( Objects[i] == (*j) ) {
								selectedPoints.erase(j);
								j=selectedPoints.begin();
								break;
							}
							++j;
						}
					}
				}
				Objects[i]->selected=!Objects[i]->selected;
				if ( Objects[i]->selected ) {
					if ( dynamic_cast<Point3D *>(Objects[i]) != 0 ) 
						selectedPoints.push_back(Objects[i]);
				}
				break;
			}
		}
	}

	inline void deselectAll()
	{
		vector<Object3D *>::iterator i = Objects.begin();
		while ( i != Objects.end()) {
			(*i)->selected = false;
			if ( dynamic_cast<Point3D *>(*i) != 0 ) {
				vector<Object3D *>::iterator j = selectedPoints.begin();
				while ( j != selectedPoints.end()) {
					if ( (*i) == (*j) ) {
						selectedPoints.erase(j);
						j=selectedPoints.begin();
						break;
					}
					++j;
				}
			}
			++i;
		}
		selectedPoints.clear();
	}
	

	inline void selectAll()
	{
		vector<Object3D *>::iterator i = Objects.begin();
		while ( i != Objects.end()) {
			(*i)->selected = true;
			if ( dynamic_cast<Point3D *>(*i) != 0 ) {
				selectedPoints.push_back((*i));
			}
			++i;
		}

	}

	inline void deleteSelected()
	{
		vector<Object3D *>::iterator i = Objects.begin();
		while ( i != Objects.end()) {
			if ( (*i)->selected ) {
				if ( dynamic_cast<Point3D *>(*i) != 0 ) {
					vector<Object3D *>::iterator j = selectedPoints.begin();
					while ( j != selectedPoints.end()) {
						if ( (*i) == (*j) ) {
							selectedPoints.erase(j);
							j=selectedPoints.begin();
							break;
						}
						++j;
					}
				}
				delete *i;
				Objects.erase(i);
				i=Objects.begin();
				continue;
			}
			++i;
		}
		i = Objects.begin();
		while ( i != Objects.end()) {
			if ( (*i)->deleteTHIS ) {
				delete *i;
				Objects.erase(i);
				i=Objects.begin();
				continue;
			}
			++i;
		}
	}


	inline Mesh3D(double _depthInterval=100.0, double _cutSurfaceInterval=0, double _stereoInterval=10)
		   :depthInterval(_depthInterval), stereoInterval(_stereoInterval), cutSurfaceInterval(_cutSurfaceInterval),
		   tempBezier3D(0), tempBSpline3D(0),tempInterpol3D(0)
	{
		// set projection matrix
		//Objects.resize(100);
		//selectedPoints.resize(100);
		Objects.reserve(500);
		selectedPoints.reserve(500);

		CameraModification.LoadIdentity();
		CameraInvertModification.LoadIdentity();

		Projection.Diagonal(1,1,0,1);
		Projection[3][2] = 1.0 / depthInterval;

		// set Steroscopia matricies
		stereoLeft.Diagonal(1,1,0,1);
		stereoRight.Diagonal(1,1,0,1);
		stereoLeft[3][2] = 1.0 / depthInterval;
		stereoRight[3][2] = 1.0 / depthInterval;
		stereoLeft[0][2] = stereoInterval / (2*depthInterval);
		stereoRight[0][2] = -stereoInterval / (2*depthInterval);
	}
	inline virtual ~Mesh3D() {
		vector<Object3D *>::iterator i = Objects.begin();
		while ( i != Objects.end()) {
			delete *i;
			Objects.erase(i);
			i=Objects.begin();
			continue;
			++i;
		}
	}

	inline void setDepthInterval(double _depthInterval)
	{
		depthInterval = _depthInterval;
		Projection[3][2] = 1.0/depthInterval;
		stereoLeft[3][2] = 1.0/depthInterval;
		stereoRight[3][2] = 1.0/depthInterval;
		stereoLeft[0][2] = stereoInterval/(2*depthInterval);
		stereoRight[0][2] = -stereoInterval/(2*depthInterval);
	}

	inline void setCutSurface(double _cutSurfaceInterval)
	{
		cutSurfaceInterval = _cutSurfaceInterval;
	}
	inline double getEyeWide(void) const { return stereoInterval; }
	inline void setEyeWide(double _stereoInterval)
	{
		stereoInterval = _stereoInterval;
		stereoLeft[0][2] = stereoInterval/(2*depthInterval);
		stereoRight[0][2] = -stereoInterval/(2*depthInterval);
	}

	/*
	inline void Mesh3D::DrawCursor3D(HDC hdc)
	{
		cursor.DrawObject3D(hdc, stereoLeft, stereoRight, CameraModification, cutSurfaceInterval);
	}*/


	inline void DrawObjects3D(HDC hdc)
	{
		for( register int i=0; i < Objects.size(); ++i ) {
			Objects[i]->DrawObject3D(hdc, stereoLeft, stereoRight, CameraModification, cutSurfaceInterval);
		}
		if ( tempBezier3D )
			tempBezier3D->DrawObject3D(hdc, stereoLeft, stereoRight, CameraModification, cutSurfaceInterval);
		if ( tempBSpline3D )
			tempBSpline3D->DrawObject3D(hdc, stereoLeft, stereoRight, CameraModification, cutSurfaceInterval);
		if ( tempInterpol3D )
			tempInterpol3D->DrawObject3D(hdc, stereoLeft, stereoRight, CameraModification, cutSurfaceInterval);

	}


	inline void DrawObjects(HDC hdc)
	{
		for( register int i=0; i < Objects.size(); ++i ) {
			Objects[i]->DrawObject(hdc, Projection, CameraModification, cutSurfaceInterval);
		}
		if ( tempBezier3D )
			tempBezier3D->DrawObject(hdc, Projection, CameraModification, cutSurfaceInterval);
		if ( tempBSpline3D )
			tempBSpline3D->DrawObject(hdc, Projection, CameraModification, cutSurfaceInterval);
		if ( tempInterpol3D )
			tempInterpol3D->DrawObject(hdc, Projection, CameraModification, cutSurfaceInterval);
	}

	inline void AddTorus(double ray0=100.0, double ray1=20.0, int du=20, int dv=20)
	{
		Object3D * object = new Torus3D(ray0, ray1, du, dv);
		object->Generate();
		Objects.push_back(object);
	}

	inline void AddSphere(double ray0=100.0, int du=20, int dv=20)
	{
		Object3D * object = new Sphere3D(ray0, du, dv);
		object->Generate();
		Objects.push_back(object);
	}

	inline void AddBezier()
	{
		if ( selectedPoints.size() >= 4 ) {
			Bezier3D * object = new Bezier3D();
			object->Generate(15, selectedPoints);
			Objects.push_back((Object3D*)object);
		}
	}

	inline void AddBSpline()
	{
		if ( selectedPoints.size() >= 4 ) {
			BSpline3D * object = new BSpline3D((void *)this);
			object->Generate(15, selectedPoints);
			Objects.push_back((Object3D*)object);
		}
	}

	inline void AddInterpol()
	{
		if ( selectedPoints.size() >= 2 ) {
			Interpol3D * object = new Interpol3D(this);
			object->Generate(15, selectedPoints);
			Objects.push_back((Object3D*)object);
		}
	}

	inline void AddPoint(Object3D * that)
	{
		Objects.push_back(that);
	}


	inline void AddPoint(double x=0, double y=0, double z=0)
	{
		Object3D * object = new Point3D(x,y,z);
		Objects.push_back(object);
	}

	inline void RotZ(double alfa)
	{
		static double x;
		static double y;
		static double a11,a21,a31,a41;

		x = cos(alfa);
		y = sin(alfa);
		a11 = CameraModification.matrix[0][0] * x - CameraModification.matrix[0][1] * y;
		a21 = CameraModification.matrix[1][0] * x - CameraModification.matrix[1][1] * y;
		a31 = CameraModification.matrix[2][0] * x - CameraModification.matrix[2][1] * y;
		a41 = CameraModification.matrix[3][0] * x - CameraModification.matrix[3][1] * y;

		CameraModification.matrix[0][1] = CameraModification.matrix[0][0] * y + CameraModification.matrix[0][1] * x;
		CameraModification.matrix[1][1] = CameraModification.matrix[1][0] * y + CameraModification.matrix[1][1] * x; 
		CameraModification.matrix[2][1] = CameraModification.matrix[2][0] * y + CameraModification.matrix[2][1] * x;
		CameraModification.matrix[3][1] = CameraModification.matrix[3][0] * y + CameraModification.matrix[3][1] * x; 

		CameraModification.matrix[0][0] = a11;
		CameraModification.matrix[1][0] = a21;
		CameraModification.matrix[2][0] = a31;
		CameraModification.matrix[3][0] = a41;

		x = cos(-alfa);
		y = sin(-alfa);
		a11 = CameraInvertModification.matrix[0][0] * x + CameraInvertModification.matrix[1][0] * y;
		a21 = CameraInvertModification.matrix[0][1] * x + CameraInvertModification.matrix[1][1] * y;
		a31 = CameraInvertModification.matrix[0][2] * x + CameraInvertModification.matrix[1][2] * y;
		a41 = CameraInvertModification.matrix[0][3] * x + CameraInvertModification.matrix[1][3] * y;

		CameraInvertModification.matrix[1][0] = CameraInvertModification.matrix[1][0] * x - CameraInvertModification.matrix[0][0] * y;
		CameraInvertModification.matrix[1][1] = CameraInvertModification.matrix[1][1] * x - CameraInvertModification.matrix[0][1] * y; 
		CameraInvertModification.matrix[1][2] = CameraInvertModification.matrix[1][2] * x - CameraInvertModification.matrix[0][2] * y;
		CameraInvertModification.matrix[1][3] = CameraInvertModification.matrix[1][3] * x - CameraInvertModification.matrix[0][3] * y; 

		CameraInvertModification.matrix[0][0] = a11;
		CameraInvertModification.matrix[0][1] = a21;
		CameraInvertModification.matrix[0][2] = a31;
		CameraInvertModification.matrix[0][3] = a41;

		/*
		Matrix3D temp;
		temp.Diagonal(1.0, cos(alfa), cos(alfa), 1.0);

		temp[2][1] = -sin(alfa);
		temp[1][2] = sin(alfa);
		CameraModification = temp * CameraModification;

		temp[2][1] = sin(alfa);
		temp[1][2] = -sin(alfa);
		CameraInvertModification= CameraInvertModification*temp;


		Matrix3D t1 = CameraModification*CameraInvertModification;*/
	}

	inline void RotX(double alfa)
	{
		static double x;
		static double y;
		static double a11,a21,a31,a41;

		x = cos(alfa);
		y = sin(alfa);
		a11 = CameraModification.matrix[0][1] * x - CameraModification.matrix[0][2] * y;
		a21 = CameraModification.matrix[1][1] * x - CameraModification.matrix[1][2] * y;
		a31 = CameraModification.matrix[2][1] * x - CameraModification.matrix[2][2] * y;
		a41 = CameraModification.matrix[3][1] * x - CameraModification.matrix[3][2] * y;

		CameraModification.matrix[0][2] = CameraModification.matrix[0][1] * y + CameraModification.matrix[0][2] * x;
		CameraModification.matrix[1][2] = CameraModification.matrix[1][1] * y + CameraModification.matrix[1][2] * x; 
		CameraModification.matrix[2][2] = CameraModification.matrix[2][1] * y + CameraModification.matrix[2][2] * x;
		CameraModification.matrix[3][2] = CameraModification.matrix[3][1] * y + CameraModification.matrix[3][2] * x; 

		CameraModification.matrix[0][1] = a11;
		CameraModification.matrix[1][1] = a21;
		CameraModification.matrix[2][1] = a31;
		CameraModification.matrix[3][1] = a41;

		x = cos(-alfa);
		y = sin(-alfa);
		a11 = CameraInvertModification.matrix[1][0] * x + CameraInvertModification.matrix[2][0] * y;
		a21 = CameraInvertModification.matrix[1][1] * x + CameraInvertModification.matrix[2][1] * y;
		a31 = CameraInvertModification.matrix[1][2] * x + CameraInvertModification.matrix[2][2] * y;
		a41 = CameraInvertModification.matrix[1][3] * x + CameraInvertModification.matrix[2][3] * y;

		CameraInvertModification.matrix[2][0] = CameraInvertModification.matrix[2][0] * x - CameraInvertModification.matrix[1][0] * y;
		CameraInvertModification.matrix[2][1] = CameraInvertModification.matrix[2][1] * x - CameraInvertModification.matrix[1][1] * y;
		CameraInvertModification.matrix[2][2] = CameraInvertModification.matrix[2][2] * x - CameraInvertModification.matrix[1][2] * y;
		CameraInvertModification.matrix[2][3] = CameraInvertModification.matrix[2][3] * x - CameraInvertModification.matrix[1][3] * y; 

		CameraInvertModification.matrix[1][0] = a11;
		CameraInvertModification.matrix[1][1] = a21;
		CameraInvertModification.matrix[1][2] = a31;
		CameraInvertModification.matrix[1][3] = a41;

//Matrix3D t1 = CameraModification*CameraInvertModification;
	}

	inline void RotY(double alfa)
	{
		static double x;
		static double y;
		static double a11,a21,a31,a41;

		x = cos(alfa);
		y = sin(alfa);
		a11 = CameraModification.matrix[0][0] * x + CameraModification.matrix[0][2] * y;
		a21 = CameraModification.matrix[1][0] * x + CameraModification.matrix[1][2] * y;
		a31 = CameraModification.matrix[2][0] * x + CameraModification.matrix[2][2] * y;
		a41 = CameraModification.matrix[3][0] * x + CameraModification.matrix[3][2] * y;

		CameraModification.matrix[0][2] = CameraModification.matrix[0][2] * x - CameraModification.matrix[0][0] * y;
		CameraModification.matrix[1][2] = CameraModification.matrix[1][2] * x - CameraModification.matrix[1][0] * y; 
		CameraModification.matrix[2][2] = CameraModification.matrix[2][2] * x - CameraModification.matrix[2][0] * y;
		CameraModification.matrix[3][2] = CameraModification.matrix[3][2] * x - CameraModification.matrix[3][0] * y; 

		CameraModification.matrix[0][0] = a11;
		CameraModification.matrix[1][0] = a21;
		CameraModification.matrix[2][0] = a31;
		CameraModification.matrix[3][0] = a41;

		x = cos(-alfa);
		y = sin(-alfa);
		a11 = CameraInvertModification.matrix[0][0] * x - CameraInvertModification.matrix[2][0] * y;
		a21 = CameraInvertModification.matrix[0][1] * x - CameraInvertModification.matrix[2][1] * y;
		a31 = CameraInvertModification.matrix[0][2] * x - CameraInvertModification.matrix[2][2] * y;
		a41 = CameraInvertModification.matrix[0][3] * x - CameraInvertModification.matrix[2][3] * y;

		CameraInvertModification.matrix[2][0] = CameraInvertModification.matrix[0][0] * y + CameraInvertModification.matrix[2][0] * x;
		CameraInvertModification.matrix[2][1] = CameraInvertModification.matrix[0][1] * y + CameraInvertModification.matrix[2][1] * x; 
		CameraInvertModification.matrix[2][2] = CameraInvertModification.matrix[0][2] * y + CameraInvertModification.matrix[2][2] * x;
		CameraInvertModification.matrix[2][3] = CameraInvertModification.matrix[0][3] * y + CameraInvertModification.matrix[2][3] * x; 

		CameraInvertModification.matrix[0][0] = a11;
		CameraInvertModification.matrix[0][1] = a21;
		CameraInvertModification.matrix[0][2] = a31;
		CameraInvertModification.matrix[0][3] = a41;

//   Matrix3D Identity = CameraInvertModification*CameraModification;
	}
	inline void TransX(double pos)
	{
		CameraModification.matrix[0][0] += CameraModification.matrix[0][3] * pos;
		CameraModification.matrix[1][0] += CameraModification.matrix[1][3] * pos;
		CameraModification.matrix[2][0] += CameraModification.matrix[2][3] * pos;
		CameraModification.matrix[3][0] += CameraModification.matrix[3][3] * pos;

		pos = -pos;
		CameraInvertModification.matrix[3][0] += CameraInvertModification.matrix[0][0] * pos;
		CameraInvertModification.matrix[3][1] += CameraInvertModification.matrix[0][1] * pos;
		CameraInvertModification.matrix[3][2] += CameraInvertModification.matrix[0][2] * pos;
		CameraInvertModification.matrix[3][3] += CameraInvertModification.matrix[0][3] * pos;

		//Matrix3D temp = CameraInvertModification*CameraModification;
	}
	inline void TransY(double pos)
	{
		CameraModification.matrix[0][1] += CameraModification.matrix[0][3] * pos;
		CameraModification.matrix[1][1] += CameraModification.matrix[1][3] * pos;
		CameraModification.matrix[2][1] += CameraModification.matrix[2][3] * pos;
		CameraModification.matrix[3][1] += CameraModification.matrix[3][3] * pos;

		pos = -pos;
		CameraInvertModification.matrix[3][0] += CameraInvertModification.matrix[1][0] * pos;
		CameraInvertModification.matrix[3][1] += CameraInvertModification.matrix[1][1] * pos;
		CameraInvertModification.matrix[3][2] += CameraInvertModification.matrix[1][2] * pos;
		CameraInvertModification.matrix[3][3] += CameraInvertModification.matrix[1][3] * pos;
		//Matrix3D temp = CameraInvertModification*CameraModification;
	}

	inline void TransZ(double pos)
	{
		CameraModification.matrix[0][2] += CameraModification.matrix[0][3] * pos;
		CameraModification.matrix[1][2] += CameraModification.matrix[1][3] * pos;
		CameraModification.matrix[2][2] += CameraModification.matrix[2][3] * pos;
		CameraModification.matrix[3][2] += CameraModification.matrix[3][3] * pos;

		pos = -pos;
		CameraInvertModification.matrix[3][0] += CameraInvertModification.matrix[2][0] * pos;
		CameraInvertModification.matrix[3][1] += CameraInvertModification.matrix[2][1] * pos;
		CameraInvertModification.matrix[3][2] += CameraInvertModification.matrix[2][2] * pos;
		CameraInvertModification.matrix[3][3] += CameraInvertModification.matrix[2][3] * pos;
		//Matrix3D temp = CameraInvertModification*CameraModification;
	}

	inline void ScaleX(double scale)
	{
		CameraModification.matrix[0][0] *= scale;
		CameraModification.matrix[1][0] *= scale;
		CameraModification.matrix[2][0] *= scale;
		CameraModification.matrix[3][0] *= scale;

		scale = 1/scale;
		CameraInvertModification.matrix[0][0] *= scale;
		CameraInvertModification.matrix[0][1] *= scale;
		CameraInvertModification.matrix[0][2] *= scale;
		CameraInvertModification.matrix[0][3] *= scale;
//		Matrix3D temp = CameraInvertModification*CameraModification;
	}

	inline void ScaleY(double scale)
	{
		CameraModification.matrix[0][1] *= scale;
		CameraModification.matrix[1][1] *= scale;
		CameraModification.matrix[2][1] *= scale;
		CameraModification.matrix[3][1] *= scale;

		scale = 1/scale;
		CameraInvertModification.matrix[1][0] *= scale;
		CameraInvertModification.matrix[1][1] *= scale;
		CameraInvertModification.matrix[1][2] *= scale;
		CameraInvertModification.matrix[1][3] *= scale;
	}
	inline void ScaleZ(double scale)
	{
		CameraModification.matrix[0][2] *= scale;
		CameraModification.matrix[1][2] *= scale;
		CameraModification.matrix[2][2] *= scale;
		CameraModification.matrix[3][2] *= scale;

		scale = 1/scale;
		CameraInvertModification.matrix[2][0] *= scale;
		CameraInvertModification.matrix[2][1] *= scale;
		CameraInvertModification.matrix[2][2] *= scale;
		CameraInvertModification.matrix[2][3] *= scale;
//		Matrix3D temp = CameraInvertModification*CameraModification;
	}


};

}

#endif // !defined(AFX_MESH3D_H__321462FD_2759_4969_B4DF_17BACD24F381__INCLUDED_)
