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
#include "Butter.h"

#define CAPTION "MicroMachines AVT"
int WindowHandle = 0;
int WinX = 640, WinY = 480;

unsigned int FrameCount = 0;

VSShaderLib shader;
Camera *camera;
Car *car;
Terrain *terrain;
Orange *orange[5];
float globalOrangesAccelaration = 0;

Butter *butter;

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
bool front = true;

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
	case '1':
		firstView = true;
		secondView = false;
		thirdView = false;
		break;
	case '2':
		firstView = false;
		secondView = true;
		thirdView = false;
		break;
	case '3':
		firstView = false;
		secondView = false;
		thirdView = true;
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
	keystates[key] = false;
}

void activeKeys() {

	if (keystates['w']) {
		front = true;
		car->setCurrent_Speed(car->getCurrent_Speed() - 0.1);
		//return;
	}

	if (keystates['s']) {
		front = false;
		car->setCurrent_Speed(car->getCurrent_Speed() + 0.1);
		//return;
	}

	if ((keystates['a'] && keystates['w']) || (keystates['a'] && keystates['s'])) {
		car->setCurrentTurn_speed(car->getTurn_speed());
	}
	else if ((keystates['d'] && keystates['w']) || (keystates['d'] && keystates['s'])) {
		car->setCurrentTurn_speed(-car->getTurn_speed());
	}

	if (!keystates['a'] && !keystates['d']) {
		car->setCurrentTurn_speed(0);
	}

	if (!keystates['w'] && !keystates['s']) {
		if (car->getCurrent_Speed() > 0) {
			if (fabs(car->getCurrent_Speed()) < 4.0) {
				car->setCurrentTurn_speed(0);
			}
			else {
				if (keystates['a'])
					car->setCurrentTurn_speed(-car->getTurn_speed());
				else if (keystates['d'])
					car->setCurrentTurn_speed(car->getTurn_speed());
			}
			car->setCurrent_Speed(car->getCurrent_Speed() - 0.1);
		}
		else if (car->getCurrent_Speed() < 0) {
			if (fabs(car->getCurrent_Speed()) < 4.0) {
				car->setCurrentTurn_speed(0);
			}
			else {
				if (keystates['a'])
					car->setCurrentTurn_speed(car->getTurn_speed());
				else if (keystates['d'])
					car->setCurrentTurn_speed(-car->getTurn_speed());
			}
			car->setCurrent_Speed(car->getCurrent_Speed() + 0.1);
		}
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

	//camera->calculate(rAux, alphaAux, betaAux);

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
	if (secondView || thirdView) {
		perspective(60.0f, ratio, 0.1f, 1000.0f);
	}
	else if (firstView) {
		//ortho(0.0f, w, h, 0.0f, 0.0f, -1.0f);
		ortho(-101, 101, -101, 101, -1.0f, 200.0f);
	}
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
	if (firstView) {
		camera->pitch = 90.0f;
		camera->setCamX(0.0f);
		camera->setCamY(150.0f);
		camera->setCamZ(0.08f);
		lookAt(camera->getCamX(), camera->getCamY(), camera->getCamZ(), 0, 0, 0, 0, 1, 0);
	}
	else if (secondView) {
		//camera->r = 105.0f;
		camera->pitch = 90.0f;
		camera->setCamX(0.0f);
		camera->setCamY(150.0f);
		camera->setCamZ(0.08f);
		std::cout << camera->getCamX() << " valor de x " << camera->getCamY() << " valor de y " << camera->getCamZ() << " valor de z " << std::endl;
		lookAt(camera->getCamX(), camera->getCamY(), camera->getCamZ(), 0, 0, 0, 0, 1, 0);

	}
	else if (thirdView) {
		camera->r = 20.0f;
		camera->pitch = 15.0f;
		camera->move(front, car->current_rotation[1], car->current_position[0], car->current_position[1], car->current_position[2]);
		lookAt(camera->getCamX(), camera->getCamY(), camera->getCamZ(), car->current_position[0], 0, car->current_position[2], 0, 1, 0);
	}

	glUseProgram(shader.getProgramIndex());
	glUniform4fv(lPos_uniformId, 1, lightPos);
	// use our shader
	terrain->renderTerrain(shader, pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
	//Oranges movement
	int i;
	for (i = 0; i < 5; i++){
		if (orange[i]->current_position[0]>100 || orange[i]->current_position[0] < -100 ||
			orange[i]->current_position[2]>100 || orange[i]->current_position[2] < -100){
			orange[i] = new Orange(-100 + rand() % 200, 3.0f, -100 + rand() % 200);
			orange[i]->createOrangeMesh();
		}
		orange[i]->move(globalOrangesAccelaration);
		orange[i]->renderOrange(shader, pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
	}
	globalOrangesAccelaration += 0.0002;

	butter->renderButter(shader, pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);

	//float res[4];
	//multMatrixPoint(VIEW, lightPos, res);   //lightPos definido em World Coord so is converted to eye space
	//glUniform4fv(lPos_uniformId, 1, res); 
	activeKeys();
	car->move();
	car->renderCar(shader, pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
	//send the light position in eye coordinates
	 //efeito capacete do mineiro, ou seja lighPos foi definido em eye coord
	glutSwapBuffers();
}

void init()
{

	car->createCarMesh();
	terrain->createTerrainMesh();

	//Initialize oranges
	int i;
	for (i = 0; i < 5; i++){
		orange[i]->createOrangeMesh();
	}

	butter->createButterMesh();
	

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

	//Orange respawn
	for (int i = 0; i < 5; i++){
		if (orange[i] == NULL)
			//Spawns an orange between -100 and 100
			orange[i] = new Orange(-100 + rand() % 200, 3.0f, -100 + rand() % 200);
	}

	if (butter == NULL)
		butter = new Butter(-20 + (rand() % 40), 0.3f, -20 + (rand() % 40));


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