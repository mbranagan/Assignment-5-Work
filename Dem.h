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
	vector<Point> normals;
	vector<Point> vertexNormals;
	
public:
    Dem(ifstream& myfile);
    Dem();
    void generateRandomDEM(int size, float rough, string output, float a, float b, float c, float d);
    void inputFile(ifstream& myfile);
    void randomMidpointDisplacement(float a, float b, float c, float d, int loc1, int loc2, int loc3, int loc4, float rough);
    void drawTriangulation(float windowWidth, float windowHeight, float rotateX, float rotateY, float rotateZ, float anglex, float angley, float anglez);
    void calculateVertexNormals();
    void calculateNormals();
    void triangulateDEM(vector<Point> allPoints);
	Dem(int cols, int rows, float xcorn, float ycorn, float size);
	// getters
	int getCols();
	int getRows();
	float getXcorn();
	float getYcorn();
	float getCellSize();
	void calculatePoints(float windowWidth, float windowHeight, float rotateX, float rotateY, float rotateZ, float anglex, float angley, float anglez);
	void drawDEM(float windowWidth, float windowHeight, float rotateX, float rotateY, float rotateZ, int increase, int decrease, float anglex, float angley, float anglez );
	void drawLinearSpline(float windowWidth, float windowHeight, bool rotateX, bool rotateY, bool rotateZ, int increase, int decrease, float anglex, float angley, float anglez);
	void drawQuadraticSpline(float windowWidth, float windowHeight, bool rotateX, bool rotateY, bool rotateZ, int increase, int decrease, float anglex, float angley, float anglez);
	
};