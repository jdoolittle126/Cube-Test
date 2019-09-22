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

const int
WINDOW_WIDTH = 640, WINDOW_HEIGHT = 480, WINDOW_START_POS_X = 50,
		WINDOW_START_POS_Y = 50;

const float DENSITY = 1.0f, ACCEL_GRAVITY = 981.0f;

const float colors[12][3] = { { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, {
		0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 1.0f }, { 0.0f,
		1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.9f, 0.9f, 0.9f }, { 0.8f, 0.8f, 0.8f }, { 0.7f, 0.7f, 0.7f }, { 0.6f, 0.6f, 0.6f }, {0.0f, 0.0f, 0.0f} };

// --- DELTA VARS --- //

int delta = 0,
ts= 0,
_ts = 0;

float _size = 0.5f;
float size = _size / 2.0f,

bound_x = 1.0f, bound_y = 1.0f, bound_z = 0.0f,

_bound_x = -1.0f, _bound_y = -1.0f, _bound_z = -10.0f,

pos_x = 0.0f, pos_y = 0.0f, pos_z = -8.0f,

vel_x = 0.5f, vel_y = 1.0f, vel_z = -1.0f,

pitch = 60.0f, yaw = 12.0f, roll = 15.0f,

vel_yaw = 22.0f, vel_pitch = 36.0f, vel_roll = 3.0f;


float
_pos_x,
_pos_y,
_pos_z,
_pitch,
_yaw,
_roll;

glm::vec4 	starting_position = glm::vec4(pos_x, pos_y, pos_z, 1.0f),
			starting_rotation = glm::vec4(pitch, yaw, roll, 1.0f),
			velocity = glm::vec4(vel_x, vel_y, vel_z, 1.0f),
			rotation = glm::vec4(vel_pitch, vel_yaw,vel_roll, 1.0f),
			vert0 = glm::vec4(-size, size, size, 1.0f),
			vert1 = glm::vec4(-size, size, -size, 1.0f),
			vert2 = glm::vec4(size, size, -size, 1.0f),
			vert3 = glm::vec4(size, size, size, 1.0f),
			vert4 = glm::vec4(-size, -size, size, 1.0f),
			vert5 = glm::vec4(-size, -size, -size, 1.0f),
			vert6 = glm::vec4(size, -size, -size, 1.0f),
			vert7 = glm::vec4(size, -size, size, 1.0f),
			center = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);


glm::vec4
_vert0,
_vert1,
_vert2,
_vert3,
_vert4,
_vert5,
_vert6,
_vert7;


list<glm::vec4*> verts;

glm::mat4x4 mat_transform, mat_translate, mat_scale, mat_rot;

void build_translate(float x, float y, float z) {
	float m[16] = { 1, 0, 0, x, 0, 1, 0, y, 0, 0, 1, z, 0, 0, 0, 1 };
	mat_translate = glm::make_mat4x4(m);

}

void build_scale(float sx, float sy, float sz) {
	float m[16] = { sx, 0, 0, 0, 0, sy, 0, 0, 0, 0, sz, 0, 0, 0, 0, 1 };
	mat_scale = glm::make_mat4x4(m);
}

void build_rot(float _rx, float _ry, float _rz) {
	float rx = _rx * (PI / 180.0f);
	float ry = _ry * (PI / 180.0f);
	float rz = _rz * (PI / 180.0f);

	float m[16] = { cos(ry) * cos(rz), -cos(ry) * sin(rz), sin(ry), 0, (cos(rx)
			* sin(rz)) + (sin(rx) * sin(ry) * cos(rz)), (cos(rx) * cos(rz))
			- (sin(rx) * sin(ry) * sin(rz)), -sin(rx) * cos(ry), 0, (sin(rx)
			* sin(rz)) - (cos(rx) * sin(ry) * cos(rz)), (sin(rx) * cos(rz))
			+ (cos(rx) * sin(ry) * sin(rz)), cos(rx) * cos(ry), 0, 0, 0, 0, 1 };

	mat_rot = glm::make_mat4x4(m);
}

void update_delta() {
	ts = glutGet(GLUT_ELAPSED_TIME);
	delta = ts - _ts;
	_ts = ts;
}

void update_color(int a) {
	glColor3f(colors[a][0], colors[a][1], colors[a][2]);
}

void draw_face(glm::vec4 a, glm::vec4 b, glm::vec4 c, glm::vec4 d) {
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);
	glVertex3f(c.x, c.y, c.z);
	glVertex3f(d.x, d.y, d.z);
}

void pos_update() {
	float d = static_cast<float> (delta) / 1000;

	_pos_x = pos_x,
	_pos_y = pos_y,
	_pos_z = pos_z,
	_pitch = pitch,
	_yaw = yaw,
	_roll = roll;

	pos_x += vel_x * d;
	pos_y += vel_y * d;
	pos_z += vel_z * d;
	pitch += vel_pitch * d;
	yaw += vel_yaw * d;
	roll += vel_roll * d;

	if (pitch > 360.0f)
		pitch -= 360.0f;
	else if (pitch < 0.0f)
		pitch += 360.0f;
	if (yaw > 360.0f)
		yaw -= 360.0f;
	else if (yaw < 0.0f)
		yaw += 360.0f;
	if (roll > 360.0f)
		roll -= 360.0f;
	else if (roll < 0.0f)
		roll += 360.0f;

}

void pos_step_back(){
	pos_x = _pos_x,
	pos_y = _pos_y,
	pos_z = _pos_z,
	pitch = _pitch,
	yaw = _yaw,
	roll = _roll;
}

void check_collide(){
	bool d_x = true, d_y = true, d_z = true;
	for(list<glm::vec4*>::iterator iterator = verts.begin(); iterator != verts.end(); iterator++){
		glm::vec4* t = *iterator;
		if((t->x >= bound_x || t->x <= _bound_x) && d_x){
			vel_x *=-1;
			d_x = false;
		}

		if((t->y >= bound_y || t->y <= _bound_y) && d_y){
			vel_y *=-1;
			d_y = false;
		}

		if((t->z >= bound_z || t->z <= _bound_z) && d_z){
			vel_z *=-1;
			d_z = false;
		}

		if(!d_x || !d_y || !d_z) pos_step_back();
	}
}

/*
 * Yaw = Y
 * Pitch = X
 * Roll = Z
 */

void display() {
	update_delta();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	pos_update();

	build_translate(pos_x, pos_y, pos_z);
	build_rot(pitch, yaw, roll);
	build_scale(1.0f, 1.0f, 1.0f);

	mat_transform = mat_rot * mat_translate * mat_scale;

	_vert0 = vert0 * mat_transform,
	_vert1 = vert1 * mat_transform,
	_vert2 = vert2 * mat_transform,
	_vert3 = vert3 * mat_transform,
	_vert4 = vert4 * mat_transform,
	_vert5 = vert5 * mat_transform,
	_vert6 = vert6 * mat_transform,
	_vert7 = vert7 * mat_transform;

	check_collide();

	glBegin(GL_QUADS);

	/*
	 * a = 0
	 * b = 1
	 * c = 2
	 * d = 3
	 * e = 4
	 * f = 5
	 * g = 6
	 * h = 7
	 */

	//TOP (a, b, c, d)
	update_color(0);
	draw_face(_vert0, _vert1, _vert2, _vert3);

	//BOTTOM (e, f, g, h)
	update_color(1);
	draw_face(_vert4, _vert5, _vert6, _vert7);

	//FRONT (a, d, h, e)
	update_color(2);
	draw_face(_vert0, _vert3, _vert7, _vert4);

	//BACK (b, c, f, g)
	update_color(3);
	draw_face(_vert1, _vert2, _vert6, _vert5);

	//LEFT (a, b, e, f)
	update_color(4);
	draw_face(_vert0, _vert1, _vert5, _vert4);

	//RIGHT (d, c, h, g)
	update_color(5);
	draw_face(_vert3, _vert2, _vert6, _vert7);

	//LEFT
	update_color(6);
	draw_face(glm::vec4(_bound_x, _bound_y, 0.0f, 1.0f), glm::vec4(_bound_x, _bound_y, _bound_z, 1.0f),
			glm::vec4(_bound_x, bound_y, _bound_z, 1.0f), glm::vec4(_bound_x, bound_y, 0.0f, 1.0f));

	//RIGHT
	update_color(8);
	draw_face(glm::vec4(bound_x, _bound_y, 0.0f, 1.0f), glm::vec4(bound_x, _bound_y, _bound_z, 1.0f),
			glm::vec4(bound_x, bound_y, _bound_z, 1.0f), glm::vec4(bound_x, bound_y, 0.0f, 1.0f));

	//CEIL
	update_color(9);
	draw_face(glm::vec4(_bound_x, bound_y, 0.0f, 1.0f), glm::vec4(_bound_x, bound_y, _bound_z, 1.0f),
			glm::vec4(bound_x, bound_y, _bound_z, 1.0f), glm::vec4(bound_x, bound_y, 0.0f, 1.0f));

	//FLOOR
	update_color(7);
	draw_face(glm::vec4(_bound_x, _bound_y, 0.0f, 1.0f), glm::vec4(_bound_x, _bound_y, _bound_z, 1.0f),
			glm::vec4(bound_x, _bound_y, _bound_z, 1.0f), glm::vec4(bound_x, _bound_y, 0.0f, 1.0f));

	//BACK
	update_color(10);
	draw_face(glm::vec4(_bound_x, _bound_y, _bound_z, 1.0f), glm::vec4(_bound_x, bound_y, _bound_z, 1.0f),
			glm::vec4(bound_x, bound_y, _bound_z, 1.0f), glm::vec4(bound_x, _bound_y, _bound_z, 1.0f));

	glEnd();

	glLineWidth(5.0f);

	glBegin(GL_LINES);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, _bound_y, _bound_z);
		glVertex3f(0.0f, bound_y, _bound_z);


	glEnd();


	glRasterPos3f(-0.9f, 0.5f, -9.99f);
	const unsigned char* t = reinterpret_cast<const unsigned char *>("Bouncing\nCube\nMoving Co.");
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, t);


	//Push back buffer
	glutSwapBuffers();
	glutPostRedisplay();

}

void reshape(GLsizei width, GLsizei height) {
	if (height == 0)
		height++;
	GLfloat aspect = (GLfloat) width / (GLfloat) height;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

int main(int argc, char** argv) {

	verts.push_back(&_vert0);
	verts.push_back(&_vert1);
	verts.push_back(&_vert2);
	verts.push_back(&_vert3);
	verts.push_back(&_vert4);
	verts.push_back(&_vert5);
	verts.push_back(&_vert6);
	verts.push_back(&_vert7);

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
	//glOrtho( 0.f, WINDOW_X, WINDOW_Y, 0.f, 0.f, 1.f );

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glutMainLoop();
	return 0;
}
