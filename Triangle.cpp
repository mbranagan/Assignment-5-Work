#include "Triangle.h"
#include <iostream>
#ifdef __APPLE__ 
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif
using namespace std; 

Triangle::Traingle(Point uno, Point dos, Point tres){
	float x, y, z;

	x = uno.getX();
	y = uno.getY();
	z = uno.getZ();
	one.setVs(x,y,z);

	x = two.getX();
	y = two.getY();
	z = two.getZ();
	two.setVs(x,y,z);

	x = three.getX();
	y = three.getY();
	z = three.getZ();
	three.setVs(x,y,z);
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