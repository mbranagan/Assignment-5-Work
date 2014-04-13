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
int smooth = 1; // smooth shading is off = flat shading
int lighting = 1; // lighting default is on
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

bool inputFile(){
  ifstream myfile;   // input stream 
  string fileName;
  
  //input file names and sorting method
  cout << "Please type the name of the file holding the DEM you would like to use. If you would like to generate a DEM press 'g'" << endl;
 // getline(cin, fileName);
 // fileName = "testfile.txt";
  fileName = "g";
  //fileName = "testOutput.grd";
  if (fileName != "g"){
  
    myfile.open (fileName.c_str());

    assert( myfile.is_open() );
    //displayInstructions();
    CurrentValues.inputFile(myfile);
    // CurrentValues.randomMidpointDisplacement();
   // CurrentValues.drawDEM(WINDOW_WIDTH, WINDOW_HEIGHT, rotateX, rotateY, rotateZ, increseGrid, decreaseGrid, ROT_INCRX, ROT_INCRY, ROT_INCRZ);
    return true;
  } else {
    int size;
    float rough;
    float a,b,c,d;
    string output = ""; 
   /* cout << "Enter the number of rows (=the number of columns): ";
    cin >> size;
    cout << "Enter the roughness level: ";
    cin >> rough;
    cout << "Where would you like to put this file? ";
    cin >> output;
    cout << "Please enter the four starting elevations: ";
    cin >> a >> b >> c >> d;*/
    //defaults for testing
    size = 5;
    rough = 0;
    output = "testOutput.grd";
    a = 20;
    b = 10;
    c = 50;
    d = 15;
    CurrentValues.generateRandomDEM(size, rough, output, a, b, c, d);
    myfile.open (output.c_str());
    assert( myfile.is_open() );
    CurrentValues.inputFile(myfile);
    return false;
  }
}

void display ()
{
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity ();
  //eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ
  
  gluLookAt (xval, yval, z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 
  glColor3f (1.0, 0.0, 0.0);
  if (input == false){
    bool isThereInput;
    isThereInput =inputFile();

    //if (isThereInput){
      CurrentValues.calculatePoints(WINDOW_WIDTH, WINDOW_HEIGHT, rotateX, rotateY, rotateZ, ROT_INCRX, ROT_INCRY, ROT_INCRZ);
      CurrentValues.drawTriangulation(WINDOW_WIDTH, WINDOW_HEIGHT, rotateX, rotateY, rotateZ, ROT_INCRX, ROT_INCRY, ROT_INCRZ);
      glutPostRedisplay();
    //} else {

    //}
    input = true;
  }
  else {
    CurrentValues.drawTriangulation(WINDOW_WIDTH, WINDOW_HEIGHT, rotateX, rotateY, rotateZ, ROT_INCRX, ROT_INCRY, ROT_INCRZ);
    glutSwapBuffers();
  }

  
}

void mymenu (int value)
// Define actions for the menu based on values given in initmenu()
{
  GLfloat light0_pos[] = {0,0,0,0}; 
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
  // 1.  set up lighting parameters 
   //   a. set up light position or direction
   GLfloat light0_pos[] = {-1,0,0,0};  // directional light along -x axis (west)
   //GLfloat light0_pos[] = {10,0,0,1}; // positional;radiates in all directions 
   //   b. define kind of light/material
   //   Each defines how much of each color the light is or what color is
   //   reflected by the material.
   //   In other words, you could have a brilliant, white light, but a material
   //   that only reflects blue.
   GLfloat ambientlight[] = {0.1, 0.1, 0.1, 1};   // color of "overall" light
   GLfloat diffuselight[] = {0.7, 0.7, 0.7, 1};   // color of diffuse light 
   GLfloat specularlight[] = {1.0, 1.0, 1.0, 1};  // color of specular light
   // For example, if you make this blue and keep the specular reflection of
   // the material white in the red object, then the "shiny spot" will be
   // a mixture of the red object and the blue light = purple.  Try it!

   GLfloat ambientmaterial[] = {0.1, 0.1, 0.1, 1}; // color of reflected ambient
   GLfloat diffusematerial[] = {0.7, 0.7, 0.7, 1}; 
   GLfloat specularmaterial[] = {1.0, 1.0, 1.0, 1}; 
   GLfloat shininessmaterial[] = {100.0};  // 0 means entire object;
                             // >0 means smaller point reflection
      
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glEnable (GL_DEPTH_TEST);

   // 2.  set flat or smooth shading; can be changed via menu
   glShadeModel (GL_FLAT);
   //glShadeModel (GL_SMOOTH);

   // 3.  set up light properties using definitions above
   glLightfv (GL_LIGHT0, GL_POSITION, light0_pos);  // light0 has position light0_pos
   
   // 4.  override default LIGHT0 parameters, using lighting definitions above
   glLightfv (GL_LIGHT0, GL_AMBIENT, ambientlight);
   glLightfv (GL_LIGHT0, GL_DIFFUSE, diffuselight);
   glLightfv (GL_LIGHT0, GL_SPECULAR, specularlight);

   // 5.  can also set the material reflective properties
   // (front refers to visible polygons)
   glMaterialfv (GL_FRONT, GL_AMBIENT, ambientmaterial);
   glMaterialfv (GL_FRONT, GL_DIFFUSE, diffusematerial);
   glMaterialfv (GL_FRONT, GL_SPECULAR, specularmaterial);
   glMaterialfv (GL_FRONT, GL_SHININESS, shininessmaterial);

   // 6.  turn lighting on!
   glEnable (GL_LIGHTING);

   // 7.  tell OpenGL to use your lighting and material definitions
   glEnable (GL_LIGHT0);
   glEnable (GL_COLOR_MATERIAL);
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
        break;
      case '2': // rotate grid about x axis in a counter clockwise direction
        rotateX = 1;
        ROT_INCRX = ROT_INCRX + 5;
        break;
      case '4': // rotate grid about y axis in a clockwise direction
        rotateY = 1;
        ROT_INCRY = ROT_INCRY - 5;
        break;
      case '6': // rotate grid about y axis in a counter clockwise direction
        rotateY = 1;
        ROT_INCRY = ROT_INCRY + 5;
        break;
      case '1': // zoom in
        cout << "Zooming In" << endl;
        z = z - .5;
        break;
      case '7': // zoom out
        cout << "Zooming Out" << endl;
        z = z + .5;
        break;
      case 'q': cout << "Exit Program" << endl;
      exit (1);
   }
   glutPostRedisplay();
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