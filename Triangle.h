/*
 * 	Triangle.h
 *  Branagan G5
 *
 */
 
#include <iostream>
using namespace std;
#include <vector>

#include "point.h"
#include <stdio.h>
#ifdef __APPLE__ 
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

class Triangle
{
private: 
	// all points should be stored in counter clockwise order starting from bottom left
	Point one;
	Point two;
	Point three;
	
public:
	Triangle(Point x, Point y, Point z);
	void getVertices(Point& x, Point& y , Point& z);
	float get1X();
	float get2X();
	float get3X();
	float get1Y();
	float get2Y();
	float get3Y();
	float get1Z();
	float get2Z();
	float get3Z();
};