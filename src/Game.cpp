/*
#include <iostream>
#include <string.h>
#include <cstdlib>
#include <windef.h>
#include <GL/freeglut.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <list>
#include <iterator>
#include <algorithm>

#define PI 3.14159265

using namespace std;

// --- CONSTANTS --- //

const int	WINDOW_WIDTH = 640,
			WINDOW_HEIGHT = 480,
			WINDOW_START_POS_X = 50,
			WINDOW_START_POS_Y = 50;

// --- DELTA VARS --- //

int 		delta = 0,
			ts= 0,
			_ts = 0;

void update_delta() {
	ts = glutGet(GLUT_ELAPSED_TIME);
	delta = ts - _ts;
	_ts = ts;
}

void display() {
	update_delta();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Push back buffer & Update Display
	glutSwapBuffers();
	glutPostRedisplay();

}

void reshape(GLsizei width, GLsizei height) {
	if (height == 0) height++;
	GLfloat aspect = (GLfloat) width / (GLfloat) height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Bouncing Square in a Tractor Trailer Truck");

	glClearColor(0.0, 0.0, 0.0, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_FLAT);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//glOrtho( 0.f, WINDOW_X, WINDOW_Y, 0.f, 0.f, 1.f );

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);


	glutMainLoop();
	return 0;
}
*/

