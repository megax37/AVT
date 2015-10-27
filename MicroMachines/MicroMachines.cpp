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
//#include "TGA.h"
#include "Camera.h"
#include "Car.h"
#include "Terrain.h"
#include "Orange.h"
#include "Butter.h"
#include "Road.h"
#include "DirectionalLight.h"
#include "PointLight.h"

#define CAPTION "MicroMachines AVT"
int WindowHandle = 0;
int WinX = 640, WinY = 480;

unsigned int FrameCount = 0;

VSShaderLib shader;
Camera *camera;
Car *car;
Terrain *terrain;
Butter *butter;
Orange *orange[5];
Road *road;
DirectionalLight *dirLight;
PointLight *pointLight;
float globalOrangesAccelaration = 0.0f;

std::vector<Entity*> entities;
std::vector<LightSource*> lights;

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
GLint tex_loc0, tex_loc1, tex_loc2;
GLint texMode_uniformId;

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
		camera->chooseView('1');
		break;
	case '2':
		camera->chooseView('2');
		break;
	case '3':
		camera->chooseView('3');
		break;
	case 'c':
		printf("Camera Spherical Coordinates (%f, %f, %f)\n", camera->getAngleAroundPlayer(), camera->getPitch(), camera->getR());
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
		car->setFront(true);
		if (car->getCurrent_Speed() < 0.0f) {
			car->setCurrent_Speed(car->getCurrent_Speed() + 0.5f);
		}
		car->setCurrent_Aceleration(car->getAceleration());
		//car->setCurrent_Speed(car->getCurrent_Speed() - 0.1);
		//return;
	}

	if (keystates['s']) {
		//car->setFront(false);

		if (car->getCurrent_Speed() > 0.0f) {
			car->setCurrent_Speed(car->getCurrent_Speed() - 0.5f);
		}
		car->setCurrent_Aceleration(-car->getAceleration());
		//car->setCurrent_Speed(car->getCurrent_Speed() + 0.1);
		//return;
	}

	if ((keystates['d'] && keystates['w']) || (keystates['a'] && keystates['s'])) {
		car->setCurrentTurn_speed(-car->getTurn_speed());
	}
	else if ((keystates['a'] && keystates['w']) || (keystates['d'] && keystates['s'])) {
		car->setCurrentTurn_speed(car->getTurn_speed());
	}

	if (!keystates['a'] && !keystates['d']) {
		car->setCurrentTurn_speed(0);
	}

	if (!keystates['w'] && !keystates['s']) {
		car->setCurrent_Aceleration(0);
		if (car->getCurrent_Speed() > 0) {
			if (car->getCurrent_Speed() < 2.0) {
				car->setCurrentTurn_speed(0);
			}
			else {
				if (keystates['a'])
					car->setCurrentTurn_speed(car->getTurn_speed());
				else if (keystates['d'])
					car->setCurrentTurn_speed(-car->getTurn_speed());
			}
			car->setCurrent_Speed(car->getCurrent_Speed() - 0.5f);
		}
		else if (car->getCurrent_Speed() < 0) {
			if (car->getCurrent_Speed() > -2.0) {
				car->setCurrentTurn_speed(0);
			}
			else {
				if (keystates['a'])
					car->setCurrentTurn_speed(-car->getTurn_speed());
				else if (keystates['d'])
					car->setCurrentTurn_speed(car->getTurn_speed());
			}
			car->setCurrent_Speed(car->getCurrent_Speed() + 0.5f);
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
		camera->setFreeView(true);
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
			camera->setAngleAroundPlayer(camera->getAngleAroundPlayer() - (xx - startX));
			camera->setPitch(camera->getPitch() + (yy - startY));
		}
		else if (tracking == 2) {
			camera->setAngleAroundPlayer(camera->getAngleAroundPlayer() - (xx - startX));
		}
		tracking = 0;
		camera->setFreeView(false);
	}
}

// Track mouse motion while buttons are pressed

void processMouseMotion(int xx, int yy)
{
	int deltaX, deltaY;
	float alphaAux, betaAux;
	float rAux;

	deltaX = -xx + startX;
	deltaY = yy - startY;

	// left mouse button: move camera
	if (tracking == 1) {

		alphaAux = camera->getAngleAroundPlayer() + deltaX;
		betaAux = camera->getPitch() + deltaY;

		if (betaAux > 85.0f)
			betaAux = 85.0f;
		else if (betaAux < -85.0f)
			betaAux  = - 85.0f;
		rAux = camera->getR();
	}
	// right mouse button: zoom
	else if (tracking == 2) {
		alphaAux = camera->getAngleAroundPlayer() + deltaX;
		camera->setAngleAroundPlayer(alphaAux);
		betaAux = camera->getPitch();
		rAux = camera->getR();

	}

	camera->calculate(rAux, alphaAux, betaAux);

	//  uncomment this if not using an idle func
	//	glutPostRedisplay();
}


void mouseWheel(int wheel, int direction, int x, int y) {

	camera->setR(camera->getR() + (direction * 0.1f));
	if (camera->getR() < 0.1f)
		camera->setR(0.1f);

	camera->move();

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

	texMode_uniformId = glGetUniformLocation(shader.getProgramIndex(), "texMode");
	pvm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_pvm");
	vm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_viewModel");
	normal_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_normal");
	lPos_uniformId = glGetUniformLocation(shader.getProgramIndex(), "l_pos");
	tex_loc0 = glGetUniformLocation(shader.getProgramIndex(), "texmap0");
	tex_loc1 = glGetUniformLocation(shader.getProgramIndex(), "texmap1");
	tex_loc2 = glGetUniformLocation(shader.getProgramIndex(), "texmap2");

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
	camera->view(ratio);
}

void renderScene(void) {

	//GLint loc;
	FrameCount++;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1, 1, 1, 1);
	// load identity matrices
	loadIdentity(VIEW);
	loadIdentity(MODEL);
	// set the camera using a function similar to gluLookAt
	camera->lookat();

	glUseProgram(shader.getProgramIndex());

	glUniform1i(texMode_uniformId, 0);
	glUniform1i(tex_loc0, 0);
	glUniform1i(tex_loc1, 1);

	for each(LightSource* light in lights) {
		light->draw(shader, pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
	}

	activeKeys();
	for each(Entity* entity in entities) {
		entity->move();
		entity->render(shader, pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
	}

	int i;
	for (i = 0; i < 5; i++){
		if (orange[i]->current_position[0]>100 || orange[i]->current_position[0] < -100 ||
			orange[i]->current_position[2]>100 || orange[i]->current_position[2] < -100){
			orange[i] = new Orange(-100.0f + rand() % 200, 3.0f, -100.0f + rand() % 200);
			orange[i]->createMesh();
		}
		orange[i]->setAceleration(globalOrangesAccelaration);
		orange[i]->move();
		orange[i]->render(shader, pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
	}
	globalOrangesAccelaration += 0.0002f;

	glutSwapBuffers();
}

void init()
{

	for each(LightSource* light in lights) {
		light->createMesh();
	}

	for each(Entity* entity in entities) {
		entity->createMesh();
	}

	//Initialize oranges
	int i;
	for (i = 0; i < 5; i++){
		orange[i]->createMesh();
	}

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

	if (car == NULL)
		car = new Car();

	if (camera == NULL)
		camera = new Camera(car);

	if (terrain == NULL)
		terrain = new Terrain();

	if (road == NULL)
		road = new Road();


	//Orange respawn
	for (int i = 0; i < 5; i++){
		if (orange[i] == NULL)
			//Spawns an orange between -100 and 100
			orange[i] = new Orange(-100.0f + rand() % 200, 3.0f, -100.0f + rand() % 200);
		//entities.push_back(orange[i]);
	}

	if (butter == NULL)
		butter = new Butter((10 * rand()) % 20, 0.3f, (10 * rand()) % 20);

	if (dirLight == NULL)
		dirLight = new DirectionalLight(-1.0f, 1.0f, 0.0f, 0.0f);

	if (pointLight == NULL)
		pointLight = new PointLight(0.0f, 5.0f, 0.0f, 1.0f);

	entities.push_back(car);
	entities.push_back(terrain);
	entities.push_back(butter);
	entities.push_back(road);
	lights.push_back(pointLight);
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