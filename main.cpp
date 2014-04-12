/* perspect1.cpp
 *
 * Shows how to use frustum 
 * (taken from page 98 in OpenGL (1.1) text or page 100 in OpenGL (1.2) text) 
 * User can change the value of the eye position by pressing 8 (moving
 * farther away) and 2 (moving closer) on keyboard.
 *
 * Experiment with this code be creating different polygons and changing
 * the center of projection position until you are sure you understand how
 * the frustum works.
 */

#include <iostream>
#ifdef __APPLE__ 
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "Dem.h"


#define VIEWING_DISTANCE_MIN  3.0
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800


using namespace std;

float xval = 0.0;
float yval = 0.0;
float z = 5.0;  // distance of eye (center of projection) from frustum
static float rotateY = 0.0;
static float rotateX = 1.0;
static float rotateZ = 0.0;
float rotX = 15;
float rotY = 0;
float ROT_INCRX = 15;
float ROT_INCRY = 0.0;
float ROT_INCRZ = 0.0;
static Dem CurrentValues;
bool input = false;
bool linearSpline = false;
bool quadraticSpline = false;
int increseGrid = 2;
int decreaseGrid = 0;
int gridCounter = 0;

void displayInstructions(){
  cout << "All points are being displayed." << endl;
  cout << "Press 8 to rotate the grid in a clockwise direction." << endl;
  cout << "Press 6 to rotate the grid about the x axis in a counter clockwise direction." << endl;
  cout << "Press 4 to rotate the grid about the y axis in a clockwise direction." << endl;
  cout << "Press 2 to rotate the grid about the y axis in a counter clockwise direction." << endl;
  cout << "Press 1 to zoom in or Press 7 to zoom out." << endl;
  cout << "Press 'q' to quit." << endl;
  cout << "Right click to view a pop-up menu that displays your spline options." << endl;  
}

void inputFile(){
  ifstream myfile;   // input stream 
  string fileName;
  
  //input file names and sorting method
  //cout << "Please enter the file name with extension of the Esri grid you would like displayed: " << endl;  
  //getline(cin, fileName);
  
  fileName = "testfile.txt";
  myfile.open (fileName.c_str());

  assert( myfile.is_open() );
  displayInstructions();
  //CurrentValues.inputFile(myfile);
  CurrentValues.randomMidpointDisplacement();
  CurrentValues.drawDEM(WINDOW_WIDTH, WINDOW_HEIGHT, rotateX, rotateY, rotateZ, increseGrid, decreaseGrid, ROT_INCRX, ROT_INCRY, ROT_INCRZ);

}

void display ()
{
  glClear (GL_COLOR_BUFFER_BIT);
  glLoadIdentity ();
  //eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ
  //assert(rotateX = 0.0 && rotateY == 0.0 && rotateZ == 0.0);
  
  gluLookAt (xval, yval, z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 
  glColor3f (1.0, 0.0, 0.0);
  if (input == false){
    inputFile();
    glFlush ();
    input = true;
  }
  else if (linearSpline==true)
  {
    CurrentValues.drawLinearSpline(WINDOW_WIDTH, WINDOW_HEIGHT, rotateX, rotateY, rotateZ, increseGrid, decreaseGrid, ROT_INCRX, ROT_INCRY, ROT_INCRZ);
    glutSwapBuffers();
  }
  else if (quadraticSpline == true){
    CurrentValues.drawQuadraticSpline(WINDOW_WIDTH, WINDOW_HEIGHT, rotateX, rotateY, rotateZ, increseGrid, decreaseGrid, ROT_INCRX, ROT_INCRY, ROT_INCRZ);
    glutSwapBuffers();
  }
  else{
    CurrentValues.drawDEM(WINDOW_WIDTH, WINDOW_HEIGHT, rotateX, rotateY, rotateZ, increseGrid, decreaseGrid, ROT_INCRX, ROT_INCRY, ROT_INCRZ);
    glutSwapBuffers();
   
  }
  
}

void mymenu (int value)
// Define actions for the menu based on values given in initmenu()
{
  switch (value) {
  case 1: cout << "Displaying a linear piecewise spline..." << endl; 
    linearSpline = true;
    quadraticSpline = false;
    glutPostRedisplay();
    break;
  case 2: cout << "Displaying a quadratic piecewise spline..." << endl;
    quadraticSpline = true;
    linearSpline = false;
    glutPostRedisplay();
    break;
  case 3: cout << "Turning splines off..." << endl;
    quadraticSpline = false;
    linearSpline = false;
    glutPostRedisplay();
    break;
  case 4: cout << "Increasing grid elevations...";
      increseGrid ++;
      decreaseGrid = 0;
      glutPostRedisplay();
      gridCounter++;
      break;
  case 5: cout << "Decreasing grid elevations..." << endl;
      increseGrid--;
      glutPostRedisplay();
      gridCounter--;
      break;
  case 6: cout << "Reseting to original grid..." << endl;
        increseGrid = 1;
        decreaseGrid = 0;
        gridCounter = 0;
        rotateX = 1;
        ROT_INCRX = 15;
        rotateY = rotateZ = 0; 
        xval = 0.0;
        yval = 0.0;
        z = 20.0;
        glutPostRedisplay();
      break;
  case 7: cout << "Goodbye!" << endl;
    exit(1);
  }
}

void initmenu ()
// set up menu
{
  glutCreateMenu (mymenu);       // register callback function
  glutAddMenuEntry ("Display a Linear Piecewise Spline", 1);
  glutAddMenuEntry ("Display a Quadratic Piecewise Spline", 2);
  glutAddMenuEntry ("Turn Splines Off", 3);
  glutAddMenuEntry ("Increase Grid Elevations by a Factor of 2", 4);
  glutAddMenuEntry ("Decrease Grid Elevations by a Factor of 2", 5);
  glutAddMenuEntry ("Reset to Original Grid", 6);
  glutAddMenuEntry ("Quit", 7);
  glutAttachMenu (GLUT_RIGHT_BUTTON);
}

void myMouse (int button, int state, int x, int y)
/* Function to draw points and lines using the mouse 
 * Left button - draws point 
 * Right button - draws line from previous right button point
 * Center (both) button - stops line drawing 
 *
 * Note that mouse y position is 0 at TOP of window
 * Note also that if the window gets covered, OpenGL won't remember 
 * all the lines/points that were drawn previously 
 */
{
  if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
    initmenu();
    }
}

void init ()
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   /* set up matrices for projection coordinate system */
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   /* Frustum normally defined as -1 to 1 in x, y and a larger spread in z */
   glFrustum (-1, 1, -1, 1, 1.5, 20.0);
   glMatrixMode (GL_MODELVIEW);
}

void keyboard (unsigned char key, int x, int y)
{

   switch (key) {
      case '8': // rotate grid about the x axis in a clockwise direction
          rotateX = 1;
          ROT_INCRX = ROT_INCRX - 5;
          glutPostRedisplay ();
        break;
      case '2': // rotate grid about x axis in a counter clockwise direction
        rotateX = 1;
        ROT_INCRX = ROT_INCRX + 5;
        glutPostRedisplay ();
        break;
      case '4': // rotate grid about y axis in a clockwise direction
        rotateY = 1;
        ROT_INCRY = ROT_INCRY - 5;
        glutPostRedisplay ();
        break;
      case '6': // rotate grid about y axis in a counter clockwise direction
        rotateY = 1;
        ROT_INCRY = ROT_INCRY + 5;
        glutPostRedisplay ();
        break;
      case '1': // zoom in
        z = z - .5;
        glutPostRedisplay ();
        break;
      case '7': // zoom out
        z = z + .5;
        glutPostRedisplay ();
        break;
      case 'q': cout << "Exit Program" << endl;
      exit (1);
   }
}


int main (int argc, char** argv) {
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (WINDOW_WIDTH, WINDOW_HEIGHT); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Frustum Fun");
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc (reshape);
   glutKeyboardFunc (keyboard);
   glutMouseFunc (myMouse);
   glutMainLoop();
   return 0; 
}