#include <math.h>
#include <iostream>
#include <sstream>

#include <string>

// include GLEW to access OpenGL 3.3 functions
#include <GL/glew.h>

// GLUT is the toolkit to interface with the OS
#include <GL/freeglut.h>

// Use Very Simple Libs
// Use Very Simple Libs
#include "VSShaderlib.h"
#include "AVTmathLib.h"
#include "VertexAttrDef.h"
#include "basic_geometry.h"
#include "Camera.h"
#include "Car.h"
#include "Terrain.h"
#include "Orange.h"

#define CAPTION "MicroMachines AVT"
int WindowHandle = 0;
int WinX = 640, WinY = 480;

unsigned int FrameCount = 0;

VSShaderLib shader;
Camera *camera;
Car *car;
Terrain *terrain;
Orange *orange;

//struct MyMesh mesh[4];
//int objId = 0; //id of the object mesh - to be used as index of mesh: mesh[objID] means the current mesh

/// The storage for matrices
extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];

GLint pvm_uniformId;
GLint vm_uniformId;
GLint normal_uniformId;
GLint lPos_uniformId;

// Mouse Tracking Variables
int startX, startY, tracking = 0;
bool keystates[256];

bool firstView = false;
bool secondView = false;
bool thirdView = true;

// Frame counting and FPS computation
long myTime, timebase = 0, frame = 0;
char s[32];
float lightPos[4] = { 4.0f, 6.0f, 2.0f, 1.0f };

void timer(int value)
{
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
	FrameCount = 0;
	glutTimerFunc(1000, timer, 0);
}

void refresh(int value)
{
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, refresh, 0);
}


// ------------------------------------------------------------
//
// Events from the Keyboard
//

void processKeys(unsigned char key, int xx, int yy)
{
	switch (key) {
	case 27:
		glutLeaveMainLoop();
		break;

	case 'c':
		printf("Camera Spherical Coordinates (%f, %f, %f)\n", camera->angleAroundPlayer, camera->pitch, camera->r);
		break;
	case 'm': glEnable(GL_MULTISAMPLE); break;
	case 'n': glDisable(GL_MULTISAMPLE); break;
	}

	keystates[key] = true;
}

void processUpKeys(unsigned char key, int xx, int yy) {

	if (key == 'w') {
		//car->setCurrent_Speed(0);
		car->setCurrentTurn_speed(0);
	}
	else if (key == 's') {
		//car->setCurrent_Speed(0);
		car->setCurrentTurn_speed(0);
	}
	else if (key == 'a') {
		car->setCurrentTurn_speed(0);
	}
	else if (key == 'd') {
		car->setCurrentTurn_speed(0);
	}


	keystates[key] = false;
}

void activeKeys() {

	if (keystates['w']) {
		if (keystates['d']) {
			car->setCurrentTurn_speed(-car->getTurn_speed());
		}
		else if (keystates['a']) {
			car->setCurrentTurn_speed(car->getTurn_speed());
		}
		car->setCurrent_Speed(car->getCurrent_Speed() - 0.1);
		return;
	}

	if (keystates['s']) {
		if (keystates['d']) {
			car->setCurrentTurn_speed(car->getTurn_speed());
		}
		else if (keystates['a']) {
			car->setCurrentTurn_speed(-car->getTurn_speed());
		}
		car->setCurrent_Speed(car->getCurrent_Speed() + 0.1);
		return;
	}

	if (car->getCurrent_Speed() > 0) {
		car->setCurrent_Speed(car->getCurrent_Speed() - 0.1);
	}
	else if (car->getCurrent_Speed() < 0) {
		car->setCurrent_Speed(car->getCurrent_Speed() + 0.1);
	}
}

// ------------------------------------------------------------
//
// Mouse Events
//

void processMouseButtons(int button, int state, int xx, int yy)
{
	// start tracking the mouse
	if (state == GLUT_DOWN)  {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
	}

	//stop tracking the mouse
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			camera->angleAroundPlayer -= (xx - startX);
			camera->pitch += (yy - startY);
		}
		else if (tracking == 2) {
			camera->r += (yy - startY) * 0.01f;
			if (camera->r < 0.1f)
				camera->r = 0.1f;
		}
		tracking = 0;
	}
}

// Track mouse motion while buttons are pressed

void processMouseMotion(int xx, int yy)
{

	int deltaX, deltaY;
	float alphaAux, betaAux;
	float rAux;

	deltaX = xx - startX;
	deltaY = yy - startY;

	// left mouse button: move camera
	if (tracking == 1) {

		alphaAux = camera->angleAroundPlayer + deltaX;
		betaAux = camera->pitch + deltaY;

		if (betaAux > 85.0f)
			betaAux = 85.0f;
		else if (betaAux < -85.0f)
			betaAux = -85.0f;
		rAux = camera->r;
	}
	// right mouse button: zoom
	else if (tracking == 2) {
		alphaAux = camera->angleAroundPlayer;
		betaAux = camera->angleAroundPlayer;
		rAux = camera->r + (deltaY * 0.01f);
		if (rAux < 0.1f)
			rAux = 0.1f;
	}

	camera->calculate(rAux, alphaAux, betaAux);

	//  uncomment this if not using an idle func
	//	glutPostRedisplay();
}


void mouseWheel(int wheel, int direction, int x, int y) {

	camera->r += direction * 0.1f;
	if (camera->r < 0.1f)
		camera->r = 0.1f;

	camera->calculate(camera->r, camera->angleAroundPlayer, camera->pitch);

	//  uncomment this if not using an idle func
	//	glutPostRedisplay();
}

GLuint setupShaders() {

	// Shader for models
	shader.init();
	shader.loadShader(VSShaderLib::VERTEX_SHADER, "shaders/pointlight.vert");
	shader.loadShader(VSShaderLib::FRAGMENT_SHADER, "shaders/pointlight.frag");

	// set semantics for the shader variables
	glBindFragDataLocation(shader.getProgramIndex(), 0, "colorOut");
	glBindAttribLocation(shader.getProgramIndex(), VERTEX_COORD_ATTRIB, "position");
	glBindAttribLocation(shader.getProgramIndex(), NORMAL_ATTRIB, "normal");
	glBindAttribLocation(shader.getProgramIndex(), TEXTURE_COORD_ATTRIB, "texCoord");

	glLinkProgram(shader.getProgramIndex());

	pvm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_pvm");
	vm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_viewModel");
	normal_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_normal");
	lPos_uniformId = glGetUniformLocation(shader.getProgramIndex(), "l_pos");

	printf("InfoLog for Hello World Shader\n%s\n\n", shader.getAllInfoLogs().c_str());

	return(shader.isProgramValid());
}

void changeSize(int w, int h) {

	float ratio;
	// Prevent a divide by zero, when window is too short
	if (h == 0)
		h = 1;
	// set the viewport to be the entire window
	glViewport(0, 0, w, h);
	// set the projection matrix
	ratio = (1.0f * w) / h;
	loadIdentity(PROJECTION);
	perspective(60.0f, ratio, 0.1f, 1000.0f);
}

void renderScene(void) {

	GLint loc;

	FrameCount++;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1, 1, 1, 1);
	// load identity matrices
	loadIdentity(VIEW);
	loadIdentity(MODEL);
	// set the camera using a function similar to gluLookAt
	//camera->move(car->current_rotation[1], car->current_position[0], car->current_position[1], car->current_position[2]);
	lookAt(camera->getCamX(), camera->getCamY(), camera->getCamZ(), 0, 0, 0, 0, 1, 0);
	// use our shader
	terrain->renderTerrain(shader, pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
	orange->~Orange();
	orange->move();
	orange->renderOrange(shader, pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);

	glUseProgram(shader.getProgramIndex());

	//send the light position in eye coordinates

	glUniform4fv(lPos_uniformId, 1, lightPos); //efeito capacete do mineiro, ou seja lighPos foi definido em eye coord

	//float res[4];
	//multMatrixPoint(VIEW, lightPos, res);   //lightPos definido em World Coord so is converted to eye space
	//glUniform4fv(lPos_uniformId, 1, res); 
	activeKeys();
	car->move();
	car->renderCar(shader, pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);

	glutSwapBuffers();
}

void init()
{

	car->createCarMesh();
	terrain->createTerrainMesh();
	orange->createOrangeMesh();

	camera->calculate(camera->r, camera->angleAroundPlayer, camera->pitch);

	// some GL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

}

int main(int argc, char **argv) {

	//  GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);

	glutInitContextVersion(3, 1);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WinX, WinY);
	WindowHandle = glutCreateWindow(CAPTION);

	if (camera == NULL)
		camera = new Camera();

	if (car == NULL)
		car = new Car();

	if (terrain == NULL)
		terrain = new Terrain();

	if (orange == NULL)
		orange = new Orange(7.0f, 3.0f, 0.0f);

	//  Callback Registration
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	//glutIdleFunc(renderScene);

	//	Mouse and Keyboard Callbacks
	glutKeyboardFunc(processKeys);
	glutKeyboardUpFunc(processUpKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
	glutMouseWheelFunc(mouseWheel);
	glutTimerFunc(0, timer, 0);
	glutTimerFunc(0, refresh, 0);


	//	return from main loop
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	//	Init GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	if (!setupShaders())
		return(1);

	init();

	//  GLUT main loop
	glutMainLoop();

	return(0);

}