/*
 * 	Dem.h
 *  Branagan G3
 *
 */
 
#include <iostream>
using namespace std;
#include <vector>
#include "Triangle.h"
#include <stdio.h>
#ifdef __APPLE__ 
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

class Dem
{
private: 
	int ncols;
	int nrows;
	float xllcorner;
	float yllcorner;
	float cellsize;
	int arraySize;
	float maxValue;
	float minValue;
	float* elevationValues; 
	float scalex;
	float scalez;
	float scaley;
	vector<Triangle> triangulation;
	
public:
    Dem(ifstream& myfile);
    Dem();
    void inputFile(ifstream& myfile);
    void randomMidpointDisplacement();
    void triangulateDEM(vector<Point> allPoints);
	Dem(int cols, int rows, float xcorn, float ycorn, float size);
	// getters
	int getCols();
	int getRows();
	float getXcorn();
	float getYcorn();
	float getCellSize();
	void drawDEM(float windowWidth, float windowHeight, float rotateX, float rotateY, float rotateZ, int increase, int decrease, float anglex, float angley, float anglez );
	void drawLinearSpline(float windowWidth, float windowHeight, bool rotateX, bool rotateY, bool rotateZ, int increase, int decrease, float anglex, float angley, float anglez);
	void drawQuadraticSpline(float windowWidth, float windowHeight, bool rotateX, bool rotateY, bool rotateZ, int increase, int decrease, float anglex, float angley, float anglez);
	
};