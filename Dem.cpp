/*
 *  Dem.cpp
 *  Branagan G3
 *
 */

#include "Dem.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <cassert>

#define SCALE_FACTOR 4
#define SCALEY_FACTOR .5

vector <Point> currentDEM;

float avgSlope = 0;

float minSlope, maxSlope;
using namespace std;
Dem::Dem(ifstream& myfile)
//..........................................
// SUMMARY: CTOR that uses input stream to create object
// the first number in file is the number of columns, second
// is the number of rows, max is the product of these two numbers
{
    int arrayStats[5];
    for ( int i = 0; i < 5; i++){
        myfile >> arrayStats[i];
        myfile >> arrayStats[i];
    }
    this->ncols = arrayStats[0];
    this->nrows = arrayStats[1];
    this->xllcorner = arrayStats[2];
    this->yllcorner = arrayStats[3];
    this->cellsize = arrayStats[4];
    
	long max = ncols * nrows;
	float variable; // temporarily represents each number in file
	
	// create new list with correct size, input values from file 
	// into list
	elevationValues = new float[sizeof(float)*max];
	maxValue = minValue = elevationValues[0];
	for (long i = 0; i < max; i++)
	{
		if (elevationValues[i] > maxValue) {
			maxValue = elevationValues[i];
		} else if (elevationValues[i] < minValue)
		{
			minValue = elevationValues[i];	
		}
		myfile >> variable;
		elevationValues[i] = variable;
	}

	scalex = scaley = scalez = 1;//ncols * 99999999;//((double)ncols / SCALE_FACTOR);

}

Dem:: Dem(){
	this->ncols = 0;
}

void Dem::randomMidpointDisplacement(){
	int width = 9;
	int height = 9;
	float elevationValues2[9][9] = {0};
	float a, b, c, d;

	ncols = width;
	nrows = height;
	long max = ncols * nrows;

	cellsize = 10;
	a = elevationValues2[0][0] = 100; // a
	b = elevationValues2[width][height] = 500; // d
	c = elevationValues2[0][height] = 100; // c
	d = elevationValues2[width][0] = 400; // b

	cout << a << " " << b << " " << c << " " << d << endl;
	float roughness;
	//cout << "Enter a roughness factor: ";
	//cin >> roughness;

	int tempw = (width-1)/2;
	int temph = (height-1)/2;

	for (int i = 0; i < 3; i++){
		//float g = sqrt( - 2 log(x1) ) cos( 2 pi x2 )
		float R = 0;//roughness*cellsize*
		elevationValues2[tempw][temph] = ((a+b+c+d)/4) + R;
		cout << elevationValues2[tempw][temph] << endl;
		tempw = (tempw)/2;
		temph = (tempw)/2;
		b = b+50;
		c = c+25;
	}
	
	elevationValues = new float[sizeof(float)*max];
	for (int j = 0; j < max; j++){
		for (int k = 0; k < max; k++){
			elevationValues[(j*max)+k] = elevationValues2[j][k];
		}
	}

}
void Dem:: inputFile(ifstream& myfile){


	string arrayStats[5];
    for ( int i = 0; i < 5; i++){
        myfile >> arrayStats[i];
        myfile >> arrayStats[i];
    }
    this->ncols = atoi(arrayStats[0].c_str());
    this->nrows = atoi(arrayStats[1].c_str());
    this->xllcorner = atoi(arrayStats[2].c_str());
    this->yllcorner = atoi(arrayStats[3].c_str());
    this->cellsize = atoi(arrayStats[4].c_str());

    //if (nrows > ncols){
    //	this->cellsize = 5000/nrows;
    //} /*else {
    	//this->cellsize = 5000/ncols;
    //}
	long max = ncols * nrows;
	float variable; // temporarily represents each number in file
	
	// create new list with correct size, input values from file 
	// into list
	elevationValues = new float[sizeof(float)*max];
	maxValue = 0;
	minValue = 99999;
	for (long i = 0; i < max; i++)
	{
		elevationValues[i] = variable;

		if (elevationValues[i] > maxValue) {
			maxValue = elevationValues[i];
		} else if (elevationValues[i] < minValue)
		{
			minValue = elevationValues[i];	
		}
		myfile >> variable;
		
	}
	if (nrows > ncols){
		float xspace = nrows*cellsize;

		scalex = scalez = SCALE_FACTOR/xspace; //(nrows * 1);//285555.5556);
	}else{
		float xspace = ncols*cellsize;
		scalex = scalez = SCALE_FACTOR/xspace;//(nrows * 1);// 285555.5556);
	}
	scaley = SCALEY_FACTOR/maxValue;
	//scalex = scaley = scalez = ((double)ncols / 285555.5556);
}

Dem :: Dem(int cols, int rows, float xcorn, float ycorn, float size){
	this->ncols = cols;
	this->nrows = rows;
	this->xllcorner = xcorn;
    this->yllcorner = ycorn;
	this->cellsize = size;
	
	this->arraySize = this->ncols * this->nrows;
	
	float elevations[arraySize];
}

void Dem::drawTriangulation(float windowWidth, float windowHeight, float rotateX, float rotateY, float rotateZ, float anglex, float angley, float anglez){
	glRotatef(anglex, rotateX, 0, 0);
	glRotatef(angley, 0, rotateY, 0);
	glRotatef(anglez, 0, 0, rotateZ);
	glScalef(scalex, scaley, scalez);

	glEnable(GL_NORMALIZE);
	float treeLineCutOff;
	float highSlope;
	//cout << "maxValue " << maxValue << " minValue " << minValue << endl;
	float x = maxValue - minValue;
	//cout << "maxValue - minValue " << x << endl;
	treeLineCutOff =(x*75)/100;
	x = maxSlope - minSlope;
	highSlope = (x*75)/100;
	//treeLineCutOff = maxValue-x;
	avgSlope = avgSlope / triangulation.size();
	//cout << "avgSlope: " << avgSlope << endl;
	for (int i = 0; i < triangulation.size(); i++){
		glBegin(GL_TRIANGLE_STRIP);
			float x1 = triangulation.at(i).get1X();
			float x2 = triangulation.at(i).get2X();
			float x3 = triangulation.at(i).get3X();
			float y1 = triangulation.at(i).get1Y();
			float y2 = triangulation.at(i).get2Y();
			float y3 = triangulation.at(i).get3Y();
			float z1 = triangulation.at(i).get1Z();
			float z2 = triangulation.at(i).get2Z();
			float z3 = triangulation.at(i).get3Z();
			// calculate slope
			float highestPoint = y1;
			float lowestPoint = y1;
			if (y2 > highestPoint)
				highestPoint = y2;
			else
				lowestPoint = y2;
			if (y3 > highestPoint)
				highestPoint = y3;
			else if(y3 < lowestPoint)
				lowestPoint = y3;
			float slope = highestPoint - lowestPoint;
			bool slopeColor = false;
			
			//cout << "slope " << slope << endl;
			if (slope == 0) {
				glColor3f(0.0,0.8,1.0); // no slope is blue
				slopeColor = true;
			//	cout << "blue" << endl;
			} else if(slope > highSlope){
				glColor3f(0.2,0.2,0.2); // steep slope gray
				slopeColor = true;
			///	cout << "gray" << endl;
			}
			glNormal3f(normals.at(i).getX(), normals.at(i).getY(), normals.at(i).getZ());
			if (!slopeColor && y1 >=treeLineCutOff){
				glColor3f(1.0,1.0,1.0); // above tree line is white
				//cout << "white" << endl;
			}else if (!slopeColor){
				glColor3f(0.2,0.6,0.2); // below tree line is green
				//cout << "green" << endl;
			}
			glVertex3f(x1, y1, z1);
			if (!slopeColor && y1 >=treeLineCutOff){
				glColor3f(1.0,1.0,1.0); // above tree line is white
			}else if (!slopeColor){
				glColor3f(0.2,0.6,0.2); // below tree line is green
			}
			glVertex3f(x2, y2, z2);
			if (!slopeColor && y1 >=treeLineCutOff){
				glColor3f(1.0,1.0,1.0); // above tree line is white
			}else if (!slopeColor){
				glColor3f(0.2,0.6,0.2); // below tree line is green
			}
			glVertex3f(x3, y3, z3);
		glEnd();
	}
}

void Dem::calculateNormals(){
	normals.clear();
	float currentSlope;

	for (int i = 0; i < triangulation.size(); i++){
		float x1 = triangulation.at(i).get1X();
		float x2 = triangulation.at(i).get2X();
		float x3 = triangulation.at(i).get3X();
		float y1 = triangulation.at(i).get1Y();
		float y2 = triangulation.at(i).get2Y();
		float y3 = triangulation.at(i).get3Y();
		float z1 = triangulation.at(i).get1Z();
		float z2 = triangulation.at(i).get2Z();
		float z3 = triangulation.at(i).get3Z();
		// calculate slope
		float highestPoint = y1;
		float lowestPoint = y1;
		if (y2 > highestPoint)
			highestPoint = y2;
		else
			lowestPoint = y2;
		if (y3 > highestPoint)
			highestPoint = y3;
		else if(y3 < lowestPoint)
			lowestPoint = y3;
		currentSlope = highestPoint - lowestPoint;
		if (i == 0)
			maxSlope = minSlope = currentSlope;
		else if (currentSlope > maxSlope)
			maxSlope = currentSlope;
		else if (currentSlope < minSlope)
			minSlope = currentSlope;
		avgSlope += currentSlope;
		Point v1(x2-x1, y2-y1, z2-z1);
		Point v2(x3-x1, y3-y1, z3-z1);
		Point surfaceNorm((y1*z2)-(z1*y2), (z1*x2)-(x1*z2), (x1*y2)-(y1*x2));
		normals.push_back(surfaceNorm);
	}
}
void Dem::triangulateDEM(vector<Point> allPoints){
	triangulation.clear();
	//cout << "Begin triangulation" << endl;
	for (int i = 0; i < nrows-1; i++){
		for (int j = 0; j<ncols-1; j++){
			//cout << "POINT 1: " << (i*ncols)+j << endl;
			//cout << currentDEM.at((i*ncols)+j).getX() << " " << currentDEM.at((i*ncols)+j).getY() << " " << currentDEM.at((i*ncols)+j).getZ() << endl;
			//cout << "POINT 2: " << (i*ncols)+j+1 << endl;
			//cout << currentDEM.at((i*ncols)+j+1).getX() << " " << currentDEM.at((i*ncols)+j+1).getY() << " " << currentDEM.at((i*ncols)+j+1).getZ() << endl;
			//cout << "POINT 3: " << ((i+1)*ncols)+j+1 << endl;
			//cout << currentDEM.at(((i+1)*ncols)+j+1).getX() << " " << currentDEM.at(((i+1)*ncols)+j+1).getY() << " " << currentDEM.at(((i+1)*ncols)+j+1).getZ() << endl;
			Point p1 = currentDEM.at((i*ncols)+j);
			Point p2 = currentDEM.at((i*ncols)+j+1);
			Point p3 = currentDEM.at(((i+1)*ncols)+j+1);
			Triangle newTri(p1, p2, p3);
			//newTri.printTriangle();
			triangulation.push_back(newTri);
		}
	}

	for (int i = 0; i < nrows-1; i++){
		for (int j = 0; j<ncols-1; j++){
			/*cout << "POINT 1: " << (i*ncols)+j << endl;
			cout << "POINT 2: " << ((i+1)*ncols)+j+1 << endl;
			cout << "POINT 3: " << ((i+1)*ncols)+j << endl;*/
			Point p1 = currentDEM.at((i*ncols)+j);
			Point p2 = currentDEM.at(((i+1)*ncols)+j+1);
			Point p3 = currentDEM.at(((i+1)*ncols)+j);
			Triangle newTri(p1, p2, p3);
			triangulation.push_back(newTri);
		}
	}
	calculateNormals();
	
}

void Dem::calculatePoints(float windowWidth, float windowHeight, float rotateX, float rotateY, float rotateZ, float anglex, float angley, float anglez){
	float* tempPointer = elevationValues;
	currentDEM.clear();
	//glRotatef(angle, rotateX, rotateY, rotateZ);
	glRotatef(anglex, rotateX, 0, 0);
	glRotatef(angley, 0, rotateY, 0);
	glRotatef(anglez, 0, 0, rotateZ);
	glScalef(scalex, scaley, scalez);

	float xspace = ncols * cellsize;
	float zspace = nrows * cellsize;

	float startx = -(xspace / 2);
	float startz = -(zspace / 2);
	
	float tempcell;
	tempcell = this->cellsize;

	tempPointer = elevationValues;
	for (int i = 0; i < nrows; i++) {
		for (int j = 0; j < ncols; j++){
			float yvalue = (*tempPointer);
			Point newPoint(startx, yvalue,startz);
			//cout << "Adding Point " << startx << " " << yvalue << " " << startz << endl;
			currentDEM.push_back(newPoint);
			startx = startx +tempcell;//-windowWidth/2;// tempcell;
			tempPointer++;
		}
		startz = startz + tempcell;
		startx = -(xspace /2);// -windowWidth/2-500;  //this->xllcorner;
	}
/*
	cout << "CALCULATED POINTS " << endl;
	cout << "SIZE: " << currentDEM.size() << endl;
	for (int p = 0; p <currentDEM.size(); p++){
		cout << "NUMBER: " << p << endl;
		cout << currentDEM.at(p).getX() << " " << currentDEM.at(p).getY() << " " << currentDEM.at(p).getZ() << endl;
	}*/
	triangulateDEM(currentDEM);
}
void Dem:: drawDEM(float windowWidth, float windowHeight, float rotateX, float rotateY, float rotateZ, int increase, int decrease, float anglex, float angley, float anglez ) {
	float* tempPointer = elevationValues;
	//glRotatef(angle, rotateX, rotateY, rotateZ);
	glRotatef(anglex, rotateX, 0, 0);
	glRotatef(angley, 0, rotateY, 0);
	glRotatef(anglez, 0, 0, rotateZ);
	glScalef(scalex, scaley, scalez);

	float xspace = ncols * cellsize;
	float zspace = nrows * cellsize;

	float startx = -(xspace / 2);
	float startz = -(zspace / 2);
	
	float tempcell;
	tempcell = this->cellsize;

	if (decrease == 0) {
		decrease = 1;
	}
	glPointSize( 1.0 );
	float RB = 1.0;
	glColor3f(1.0f, 1.0f, 1.0f);
	tempPointer = elevationValues;
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < nrows; i++) {
		for (int j = 0; j < ncols; j++){
			float yvalue = (*tempPointer)*increase-(decrease*(*tempPointer));
			RB = 1-(*tempPointer)/maxValue;
			glColor3f(RB, 1.0f, RB);
			glVertex3f(startx, yvalue, startz);

			//Point newPoint(startx, yvalue,startz);
			//currentDEM.push_back(newPoint);
			startx = startx +tempcell;//-windowWidth/2;// tempcell;
			tempPointer++;
		}
		startz = startz + tempcell;
		startx = -(xspace /2);// -windowWidth/2-500;  //this->xllcorner;
	}
	glEnd();
}

void Dem::drawLinearSpline(float windowWidth, float windowHeight, bool rotateX, bool rotateY, bool rotateZ, int increase, int decrease, float anglex, float angley, float anglez ){
	float *tempPointer = elevationValues;
	glRotatef(anglex, rotateX, 0, 0);
	glRotatef(angley, 0, rotateY, 0);
	glRotatef(anglez, 0, 0, rotateZ);
	glScalef(scalex, scaley, scalez);


	float xspace = ncols * cellsize;
	float zspace = nrows * cellsize;

	float startx = -(xspace / 2);
	float startz = -(zspace / 2);
	float tempcell;

	if (decrease == 0) {
		decrease = 1;
	}
	tempcell = this->cellsize;
	glPointSize( 1.0 );
	float RG = 1.0;
	glColor3f(1.0f, 1.0f, 1.0f);
	// draw horizontal lines
	for (int i = 0; i < nrows; i++) {
		glBegin(GL_LINE_STRIP);
		//glBegin(GL_PO);
		for (int j = 0; j < ncols; j++){
			float yvalue = (*tempPointer)*increase-(decrease*(*tempPointer));
			RG = 1-((*tempPointer)/maxValue);
			glColor3f(RG, RG, 1.0f);
			glVertex3f(startx, yvalue, startz);
			startx = startx + tempcell;
			tempPointer++;
		}
		glEnd();
		startz = startz + tempcell;
		startx = -(xspace / 2);
	}
	// reset
	float *tempPointer2 = elevationValues;
	startx = -(xspace / 2);
	startz = -(zspace / 2);
	RG = 1.0;
	// draw veritcal lines
	int counter = 1;
	for (int i = 0; i < nrows; i++) {
		glBegin(GL_LINE_STRIP);
		//glBegin(GL_POLYGON);
		for (int j = 0; j < ncols; j++){
			float yvalue = (*tempPointer2)*increase-(decrease*(*tempPointer2));
			RG = 1-((*tempPointer)/maxValue);
			glColor3f(RG, RG, 1.0f);
			glVertex3f(startx, yvalue, startz);
			startz = startz + tempcell;
			tempPointer2+=ncols;
		}
		tempPointer2 = elevationValues + counter;
		counter ++;
		glEnd();
		startx = startx + tempcell;
		startz = -(zspace / 2);
	}
}

void Dem::drawQuadraticSpline(float windowWidth, float windowHeight, bool rotateX, bool rotateY, bool rotateZ, int increase, int decrease, float anglex, float angley, float anglez ){
	float *tempPointer = elevationValues;
	glRotatef(anglex, rotateX, 0, 0);
	glRotatef(angley, 0, rotateY, 0);
	glRotatef(anglez, 0, 0, rotateZ);
	glScalef(scalex, scaley, scalez);
	float startx = -windowWidth/2-500;
	float startz = -windowWidth/2;
	float tempcell;
	float splineYPoints[3];
	float splineXPoints[3];
	bool firstRound = true;
	float GB = 1;
	float y1, y2, x1, x2, d1, d2, z1, z2, a, b, c, yvalue;

	if (decrease == 0) {
		decrease = 1;
	}
	tempcell = this->cellsize;
	glPointSize( 1.0 );
	float RG = 1.0;
	glColor3f(1.0f, 1.0f, 1.0f);
	// draw horizontal lines
	for (int i = 0; i < nrows; i++) {
		
		for (int j = 0; j < ncols; j++){
			if (firstRound){
				y1 = (*tempPointer);
				tempPointer++;
				y2 = (*tempPointer);
				tempPointer++;
				firstRound = false;
				x1 = startx;
				x2 = startx + tempcell;
				d1 = (y2 - y1)/(x2-x1);
				z1 = startz;
				GB = 1 - (*tempPointer/maxValue);
				glColor3f(1.0f, GB, GB);
				glBegin(GL_LINES);
					glVertex3f(x1, y1, z1);
					glVertex3f(x2, y2, z1);
				glEnd();
			} else {
				x1 = x2;
				x2 = startx + tempcell;
				y1 = y2;
				y2 = (*tempPointer);
				tempPointer++;
				c = y1;
				b = d1;
				a = x2 - b - c;
				b = b +( x1*x1*a);
				c = c+(a*x1*x1)+(b*x1);
				glBegin(GL_LINES);
				for ( int m = 0; m < tempcell; m++){
					float thisx = startx+m;
					yvalue = a*thisx*thisx + b*thisx + c;
						glVertex3f(thisx, yvalue, startz);
					thisx = startx+m+1;
					yvalue = a*thisx*thisx + b*thisx + c;
						glVertex3f(thisx, yvalue, startz);
					
				}
				glEnd();
				d1 = 2*a*x2+b;
			}	
		}
		glEnd();
		startz = startz + tempcell;
		startx = -windowWidth/2-500;
	}
	// reset
	startx = -windowWidth/2-500;
	startz = -windowWidth/2;
	// draw veritcal lines
	for (int i = 0; i < nrows; i++) {
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < ncols; j++){
			float yvalue = (*tempPointer)*increase*(-decrease);
			RG = 1-(*tempPointer)/maxValue;
			glColor3f(RG, RG, 1.0f);
			glVertex3f(startx, yvalue, startz);
			startz = startz + tempcell;
			tempPointer++;
		}
		glEnd();
		startx = startx + tempcell;
		startz = -windowWidth/2;
	}
}
// getters
int Dem :: getCols(){
	return this->ncols;
}

int Dem :: getRows(){
	return this->nrows;
}

float Dem :: getXcorn(){
	return this->xllcorner;
}

float Dem :: getYcorn(){
	return this->yllcorner;
}

float Dem :: getCellSize(){
	return this->cellsize;
}
