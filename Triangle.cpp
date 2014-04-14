#include "Triangle.h"
#include <iostream>
#include <cmath>
#ifdef __APPLE__ 
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif
using namespace std; 

Triangle::Triangle(Point uno, Point dos, Point tres){
	float x, y, z;

	x = uno.getX();
	y = uno.getY();
	z = uno.getZ();
	one.setVs(x,y,z);

	x = dos.getX();
	y = dos.getY();
	z = dos.getZ();
	two.setVs(x,y,z);

	x = tres.getX();
	y = tres.getY();
	z = tres.getZ();
	three.setVs(x,y,z);
	calculateSlope();
}

float Triangle::get1X(){
	return one.getX();
}

float Triangle::get2X(){
	return two.getX();
}

float Triangle::get3X(){
	return three.getX();
}

float Triangle::get1Y(){
	return one.getY();
}

float Triangle::get2Y(){
	return two.getY();
}

float Triangle::get3Y(){
	return three.getY();
}

float Triangle::get1Z(){
	return one.getZ();
}

float Triangle::get2Z(){
	return two.getZ();
}

float Triangle::get3Z(){
	return three.getZ();
}

float Triangle::getSlope(){
	return slope;
}
void Triangle::printTriangle(){
	cout << "VERTEX 1" << endl;
	cout << one.getX() << " " << one.getY() << " " << one.getZ() << endl;
	cout << "VERTEX 2" << endl;
	cout << two.getX() << " " << two.getY() << " " << two.getZ() << endl;
	cout << "VERTEX 3" << endl;
	cout << three.getX() << " " << three.getY() << " " << three.getZ() << endl;

}

void Triangle::calculateSlope(){
	float a1 = get2X() - get1X(); // delta X12
	float b1 = get2Y() - get1Y(); // delta Y12
	float c1 = get2Z() - get1Z(); // delta Z12

	float a2 = get3X() - get1X(); // delta X13
	float b2 = get3Y() - get1Y(); // delta Y13
	float c2 = get3Z() - get1Z(); // delta Z13

	float a3 = get3X() - get2X(); // delta X23
	float b3 = get3Y() - get2Y(); // delta Y23
	float c3 = get3Z() - get2Z(); // delta Z23

	float d1 = sqrt(a1*a1+b1*b1); // length of side 1-2
	float d2 = sqrt(a2*a2+b2*b2); // length of side 1-3
	float d3 = sqrt(a3*a3+b3*b3); // length of side 2-3

	float s = (d1+d2+d3)*.5; // triangle perimeter /2
	float r = sqrt((s-d1)*(s-d2)*(s-d3)/s); // radius of inscribed circle

	float a0 = b1*c2-b2*c1; // "i" element of vector cross product
	float b0 = a2*c1-a1*c2; // "j" element of vector cross product
	float c0 = a1*b1-a2*b1; // "k" element of vector cross product

	float d0 = sqrt(a0*a0+b0*b0); // magnitude of vector in X-Y plane
	slope = abs(d0/c0*100); // triangle slope in %

}