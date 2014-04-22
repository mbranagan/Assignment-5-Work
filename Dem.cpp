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
vector <Point> uiPoints;

float avgSlope;
bool wA2 = false;
bool wB2 = false;
bool wC2 = false;
bool wD2 = false;
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

void Dem::generateRandomDEM(int size, float rough, string output, float a, float b, float c, float d, bool wA, bool wB, bool wC, bool wD){
	ncols = size;
	nrows = size;
	xllcorner = 0;
	yllcorner = 0;
	cellsize = 10;
	long max = ncols * nrows;

	maxValue = 0;
	minValue = 99999;
	elevationValues = new float[sizeof(float)*max];
	for (long i = 0; i < max; i++)
	{
		elevationValues[i] = -1;		
	}
	if (nrows > ncols){
		float xspace = nrows*cellsize;

		scalex = scalez = SCALE_FACTOR/xspace; //(nrows * 1);//285555.5556);
	}else{
		float xspace = ncols*cellsize;
		scalex = scalez = SCALE_FACTOR/xspace;//(nrows * 1);// 285555.5556);
	}
	scaley = SCALEY_FACTOR/maxValue;

	//cout << "a " << a << " b " << b << " c " << c << " d " << d << endl;
	//cout << "loc1 " << 0 << " loc2 " << ncols-1 << " loc3 " << max-ncols << " loc4 " << max-1 << endl;
	elevationValues[0] = a;
	elevationValues[ncols-1] = b;
	elevationValues[max-ncols] = c;
	elevationValues[max-1] = d;

	randomMidpointDisplacement(a, b, c, d, 0, ncols-1, max-ncols, max-1, rough);

	if (wC){
		cout << "wC" << endl;
		int a1 = (max-ncols)+1;
		int b1 = (max-ncols)+2;
		int c1 = (max-ncols)-(ncols-1);
		elevationValues[a1] = elevationValues[b1] = elevationValues[c1];
		for (int i = 0; i < ceil((ncols)/8); i++){
			elevationValues[a1+1] = elevationValues[b1+1] = elevationValues[c1+1];
			elevationValues[a1-ncols] = elevationValues[b1-ncols] = elevationValues[c1-ncols];
			a1 = a1+1;
			b1 = b1+1;
			c1 = c1+1;
			//cout << "i " << i << endl;
		}
		cout << "CEILING " << ceil((ncols)/8) << endl;
		//cout << "ncols " << ncols;
		wC2 = true;
	}
	if (wD){
		//cout << "wD" << endl;
		wD2 = true;
		//elevationValues[(max-1)-1] = elevationValues[(max-1)-(ncols+1)] = elevationValues[(max-1)-(ncols+2)];
	}
	if (wA){
		//wA2 = true;
		//cout << "wA" << endl;
		elevationValues[1] = elevationValues[2] = elevationValues[(ncols+2)];
	}
	if (wB){
		wB2 = true;
		//cout << "wB" << endl;
		elevationValues[ncols-2] = elevationValues[ncols-3] = elevationValues[(ncols-1)+(ncols-1)];
	}
 	ofstream myfile;
  	myfile.open (output);
  	maxValue = 0;
  	minValue = 99999;
  	
	myfile << "ncols " << ncols << endl;
	myfile << "nrows " << nrows << endl;
	myfile << "xllcorner " << 0 << endl;
	myfile << "yllcorner " << 0 << endl;
	myfile << "cellsize " << 10 << endl;
	for (int j = 0; j < max; j++){
		//cout << j << " " << elevationValues[j] << endl;
		assert(elevationValues[j] != -1);
		if (elevationValues[j] == -1){
			elevationValues[j] = 30;
		}
		if (elevationValues[j] < minValue)
			minValue = elevationValues[j];
		if (elevationValues[j] > maxValue)
			maxValue = elevationValues[j];
	    myfile << elevationValues[j] << " ";
	}
	myfile.close();
}
void Dem::randomMidpointDisplacement(float a, float b, float c, float d, int loc1, int loc2, int loc3, int loc4, float rough){
	// calculate gaussion random value
	float x, y,s;
	float tempcell;
	tempcell = (this->cellsize*(loc2-loc1))+10;
	x = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/2)))-1;
	y = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/2)))-1;
	s = (x*x) + (y*y);
	while (s >= 1){
		x = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/2)))-1;
		y = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/2)))-1;
		s = (x*x) + (y*y);
	}
	assert(s < 1);
	s = x*(sqrt((-2*log(s))/s));
	if (s<0){
		s = sqrt(s*s);
	}
	float R = rough * tempcell * s;
	//cout << "tempcell " << tempcell << endl;
	float e = ((a + b + c + d)/4) + R;
	assert(a == elevationValues[loc1]);
	assert(b == elevationValues[loc2]);
	assert(c == elevationValues[loc3]);
	assert(d == elevationValues[loc4]);
	//assert(elevationValues[loc1] >= 0);
	//assert(elevationValues[loc2] >= 0);
	//assert(elevationValues[loc3] >= 0);
	//assert(elevationValues[loc4] >= 0);
	

	
	int newLoc = (loc1+loc2+loc3+loc4)/4;
	assert(newLoc != 0);
	//cout << " newLoc " << newLoc << endl;
	if (elevationValues[newLoc] < 0)
		elevationValues[newLoc] = e;
	if ((newLoc-loc1) == (ncols+1)){
		if (elevationValues[loc2-1] < 0){
			elevationValues[loc2-1] = (a+b)/2;
		}
		if (elevationValues[loc4-1] < 0){
			elevationValues[loc4-1] = (d+c)/2;
			//cout << "loc4-1 " << loc4-1 << endl;
		}
		if (elevationValues[loc3-ncols] < 0){
			elevationValues[loc3-ncols] = (a+c)/2;
			//cout << "loc3-ncols " << loc3-ncols << endl;
		}
		if (elevationValues[(loc3-ncols)+2] < 0){
			elevationValues[(loc3-ncols)+2] = (b+d)/2;
		}
		//cout << "finished" << endl;
	}else{
		assert(loc1 >= 0);
		assert(loc2 >= 0);
		assert(loc3 >= 0);
		assert(loc4 >= 0);
		int loc2a = loc1 + ((loc2 - loc1) / 2);
		int loc3a = loc1 + ((loc3 - loc1) / 2);
		if (elevationValues[loc2a]  < 0)
			elevationValues[loc2a] = (a+b)/2;
		if (elevationValues[loc3a] < 0)
			elevationValues[loc3a] = (a+c)/2;
		// top left square
		randomMidpointDisplacement(a, elevationValues[loc2a], elevationValues[loc3a], e, loc1, loc2a, loc3a, newLoc, rough);

		// top right square
		int loc4a = loc2 + ((loc4 - loc2)/2);	
		if (elevationValues[loc4a] < 0)
			elevationValues[loc4a] = (d+b)/2;
		randomMidpointDisplacement(elevationValues[loc2a], b, e, elevationValues[loc4a], loc2a, loc2, newLoc, loc4a, rough);

		// bottom left square
		int loc4b = loc3 +((loc4 - loc3)/2);
		if (elevationValues[loc4b] < 0)
			elevationValues[loc4b] = (c+d)/2;
		randomMidpointDisplacement(elevationValues[loc3a], e, c, elevationValues[loc4b], loc3a, newLoc, loc3, loc4b, rough);

		// bottom right square
		randomMidpointDisplacement(e, elevationValues[loc4a], elevationValues[loc4b], d, newLoc, loc4a, loc4b, loc4, rough);
	}
	//cout << "loc1 " << loc1 << " loc2 " << loc2 << " loc3 " << loc3 << " loc4 " << loc4 << endl;
 
}
void Dem:: inputFile(float rows, float cols, float xllcorner, float yllcorner, float cellsize){


    this->ncols = rows;
    this->nrows = cols;
    this->xllcorner = xllcorner;
    this->yllcorner = yllcorner;
    this->cellsize = cellsize;

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
		elevationValues[i] = 10;
				if (elevationValues[i] > maxValue) {
			maxValue = elevationValues[i];
		} else if (elevationValues[i] < minValue)
		{
			minValue = elevationValues[i];	
		}
		
	}
	if (nrows > ncols){
		float xspace = nrows*cellsize;

		scalex = scalez = SCALE_FACTOR/xspace; //(nrows * 1);//285555.5556);
	}else{
		float xspace = ncols*cellsize;
		scalex = scalez = SCALE_FACTOR/xspace;//(nrows * 1);// 285555.5556);
	}
	scaley = SCALEY_FACTOR/maxValue;
	cout << "NORWS " << nrows << " NCOLS  " << ncols << "MAX " << maxValue << endl;
	//scalex = scaley = scalez = ((double)ncols / 285555.5556);
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
	//cout << "maxSlope - minSlope " << x << endl;
	highSlope = (x*75)/100;
	//treeLineCutOff = maxValue-x;
	//cout << avgSlope << " " << triangulation.size() << endl;
	avgSlope = avgSlope / triangulation.size();
	//cout << "highSlope: " << highSlope << endl;
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
			float slope = triangulation.at(i).getSlope();
			bool slopeColor = false;

			//cout << "slope " << slope << endl;
			//cout << "slope " << slope << endl;
			if (/*slope == 0*/ slope <1) {
				glColor3f(0.0,0.8,1.0); // no slope is blue
				slopeColor = true;
				//cout << "BLUE" << endl;
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
void Dem::calculateVertexNormals(){
	vertexNormals.clear();

	// initialize vector data structure
	for (int i = 0; i < (triangulation.size()*3); i++){
		Point p1(-1,-1,-1);
		vertexNormals.push_back(p1);
	}

	// bottom right & top left corner vertices are based only on one surface normal
	int singleCorners[2] = {(ncols-1)*3, (triangulation.size()-(ncols-2))};
	for (int j = 0; j < sizeof(singleCorners); j++){
		vertexNormals.at(singleCorners[j]) = normals.at(singleCorners[j]);
	}

	// bottom left & top right corner vertices are based only on two surface normals
	int doubleCorners[4] = {0, ((triangulation.size()/2)*3)+1, (((triangulation.size()-1)/2)*3), (triangulation.size()*3)-1};	
	vertexNormals.at(doubleCorners[0]) = vertexNormals.at(doubleCorners[1]) = normals.at(doubleCorners[0]).average(normals.at(doubleCorners[1]));
	vertexNormals.at(doubleCorners[2]) = vertexNormals.at(doubleCorners[3]) = (normals.at(doubleCorners[2]).average(normals.at(doubleCorners[3])));

	// fill in edges
	//for (int m = 0; m )
	for (int k = 0; k < triangulation.size()*3; k++){
		if (vertexNormals.at(k).isNull()){
			// check if an edge
		}
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
		Point v1(x2-x1, y2-y1, z2-z1);
		Point v2(x3-x1, y3-y1, z3-z1);
		Point surfaceNorm((y1*z2)-(z1*y2), (z1*x2)-(x1*z2), (x1*y2)-(y1*x2));
		normals.push_back(surfaceNorm);
	}
}
void Dem::triangulateDEM(vector<Point> allPoints){
	triangulation.clear();
	//cout << "Begin triangulation" << endl;
	avgSlope = 0;
	maxSlope = 0;
	minSlope = 99999;
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
			avgSlope = avgSlope + newTri.getSlope();
			
			//newTri.printTriangle();
			triangulation.push_back(newTri);
			if (newTri.getSlope() > maxSlope){
				maxSlope = newTri.getSlope();
			} else if (newTri.getSlope() < minSlope){
				minSlope = newTri.getSlope();

			}
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
	uiPoints.clear();

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
	glPointSize(5);
	glBegin(GL_POINTS);
	for (int i = 0; i < nrows; i++) {
		for (int j = 0; j < ncols; j++){
			float yvalue = (*tempPointer)*increase-(decrease*(*tempPointer));
			RB = 1-(*tempPointer)/maxValue;
			glColor3f(RB, 1.0f, RB);
			glVertex3f(startx, yvalue, startz);
			Point p(startx/scalex, yvalue/scaley,startz/scalez);
			cout << "point " << startx/scalex << " " << yvalue/scaley << " " << startz/scalez << endl;
			uiPoints.push_back(p);
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
