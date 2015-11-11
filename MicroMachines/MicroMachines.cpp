#include <math.h>
#include <iostream>
#include <sstream>
#include <time.h>
#include <string>
#include <stdlib.h>

// include GLEW to access OpenGL 3.3 functions
#include <GL/glew.h>

// GLUT is the toolkit to interface with the OS
#include <GL/freeglut.h>

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
#include "Road.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "HudMessage.h"

#define MAX_LIVES 5
#define CAPTION "MicroMachines AVT"
int WindowHandle = 0;
int WinX = 640, WinY = 480;
unsigned int FrameCount = 0;

VSShaderLib shader;
Camera *camera;
Car *car;
Car *lives[MAX_LIVES];
Terrain *terrain;
Butter *butter;
Orange *orange[5];
Road *road;
DirectionalLight *dirLight0;
PointLight *pointLight1;
PointLight *pointLight2;
PointLight *pointLight3;
PointLight *pointLight4;
PointLight *pointLight5;
PointLight *pointLight6;
SpotLight *spotLight7;
SpotLight *spotLight8;
HudMessage *hudMessage;

int numberOfLives = MAX_LIVES;
float globalOrangesAccelaration = 0.0f;

bool paused = false;

std::vector<Entity*> entities;
std::vector<LightSource*> lights;

GLint pvm_uniformId;
GLint vm_uniformId;
GLint normal_uniformId;
GLint lPos_uniformId;
GLint tex_loc0, tex_loc1;
GLint texMode_uniformId;

// Mouse Tracking Variables
int startX, startY, tracking = 0;
bool keystates[256];
bool front = true;

bool firstView = false;
bool secondView = false;
bool thirdView = true;

// Frame counting and FPS computation
long t_actual, t_ant, timebase = 0, frame = 0;

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
	bool newFlag;

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
	case '4':
		camera->chooseView('4');
		break;
	case 'c':
		printf("Camera Spherical Coordinates (%f, %f, %f)\n", camera->getAngleAroundPlayer(), camera->getPitch(), camera->getR());
		break;
	case 'm':
		glEnable(GL_MULTISAMPLE);
		break;
	case 'n':
		glDisable(GL_MULTISAMPLE);
		break;
	case 'p':
		if (numberOfLives) { paused = !paused; }
		break;
	case 'r':
		paused = false;
		numberOfLives = MAX_LIVES;
		t_actual = 0;
		t_ant = 0;
		timebase = glutGet(GLUT_ELAPSED_TIME);
		car->current_position[0] = 0.0f;
		car->current_position[1] = 0.0f;
		car->current_position[2] = 0.0f;
		car->current_rotation[1] = 0.0f;
		car->setCurrent_Speed(0.0f);
		break;
	case 'M':
		dirLight0->setActive(!dirLight0->getActive());
		break;
	case 'C':
		newFlag = !pointLight1->getActive();
		pointLight1->setActive(newFlag);
		pointLight2->setActive(newFlag);
		pointLight3->setActive(newFlag);
		pointLight4->setActive(newFlag);
		pointLight5->setActive(newFlag);
		pointLight6->setActive(newFlag);
		break;
	case 'H':
		newFlag = !spotLight7->getActive();
		spotLight7->setActive(newFlag);
		spotLight8->setActive(newFlag);
		break;
	}

	keystates[key] = true;
}

void processUpKeys(unsigned char key, int xx, int yy) {
	keystates[key] = false;
}

void activeKeys() {

	if (keystates['w']) {
		if (car->getCurrent_Speed() < 0.0f) {
			car->setCurrent_Speed(car->getCurrent_Speed() + 0.5f);
		}
		car->setCurrent_Aceleration(car->getAceleration());
	}

	if (keystates['s']) {

		if (car->getCurrent_Speed() > 0.0f) {
			car->setCurrent_Speed(car->getCurrent_Speed() - 0.5f);
		}
		car->setCurrent_Aceleration(-car->getAceleration());
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

	printf("InfoLog for Hello World Shader\n%s\n\n", shader.getAllInfoLogs().c_str());

	return(shader.isProgramValid());
}

void changeSize(int w, int h) {
	// set the viewport to be the entire window
	WinX = w;
	WinY = h;
	glViewport(0, 0, w, h);
	camera->view(w, h);
}

void detectCollisions() {
	if (Box::intersectCircularBox(car->getBox(), butter->getBox())) {
		memcpy(car->current_position, car->previousPosition, 3 * sizeof(float));
	}

	if (Box::interserctTerrainBox(car->getBox(), terrain->getBox())) {
		if (car->current_position[1] > -55.0f)
			car->increasePosition(0, -1.0f, 0);
	}

	for (size_t i = 0; i < 5; i++)
	{
		if (Box::intersectCircularBox(car->getBox(), orange[i]->getBox())) {
			if (--numberOfLives == 0) {
				paused = true;
				return;
			}
			car->current_position[0] = 0.0f;
			car->current_position[1] = 0.0f;
			car->current_position[2] = 0.0f;
			orange[i]->current_position[0] = -100.0f + (rand() % 200);
			orange[i]->current_position[2] = -100.0f;
			return;
		}
	}
}

void update(int delta_t) {
	if (!paused) {
		activeKeys();

		for each(Entity* entity in entities) {
			entity->move(delta_t);
		}

		for (int i = 0; i < 5; i++){
			if (orange[i]->current_position[0]>100 || orange[i]->current_position[0] < -100 ||
				orange[i]->current_position[2]>100 || orange[i]->current_position[2] < -100){
				orange[i]->current_position[0] = -100.0f + (rand() % 200);
				orange[i]->current_position[2] = -100.0f;
			}
			//orange[i]->setAceleration(globalOrangesAccelaration);
			orange[i]->move(delta_t);
		}

		detectCollisions();
	}

	//globalOrangesAccelaration += 0.0002f;
}

void timeCount(int value)
{
	t_actual = glutGet(GLUT_ELAPSED_TIME) - timebase;
	update(t_actual - t_ant);
	t_ant = t_actual;
	glutTimerFunc(20, timeCount, 0);
}

void renderHUD() {
	pushMatrix(PROJECTION);
	loadIdentity(PROJECTION);
	pushMatrix(VIEW);
	loadIdentity(VIEW);
	ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f);
	if (paused) {
		if (!numberOfLives) {
			hudMessage->setMessageType(GAMEOVER);
		}
		else {
			hudMessage->setMessageType(GAMEPAUSE);
		}
		hudMessage->render(shader, pvm_uniformId, vm_uniformId, normal_uniformId, texMode_uniformId);
	}
	for (int i = 0; i < numberOfLives; i++) {
		pushMatrix(MODEL);
		translate(MODEL, 9.0f - i * 1, 9.0f, 0.0f);
		rotate(MODEL, 45.0f, 0, 1, 1);
		rotate(MODEL, 90.0f, 1, 0, 0);
		scale(MODEL, 0.2f, 0.2f, 0.2f);
		lives[i]->render(shader, pvm_uniformId, vm_uniformId, normal_uniformId, texMode_uniformId);
		popMatrix(MODEL);
	}
	popMatrix(VIEW);
	popMatrix(PROJECTION);
}

void renderScene(void) {

	FrameCount++;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
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
		light->draw(shader);
	}

	if (camera->getCurrentView() == "pilotView") {
		glStencilFunc(GL_ALWAYS, 0x1, 0x1);
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
		car->drawWindshield(shader, pvm_uniformId, vm_uniformId, normal_uniformId, texMode_uniformId);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilFunc(GL_EQUAL, 0x1, 0x1);
	}
	else {
		glStencilFunc(GL_ALWAYS, 0x1, 0x1);
	}

	for each(Entity* entity in entities) {
		entity->render(shader, pvm_uniformId, vm_uniformId, normal_uniformId, texMode_uniformId);
	}

	for (int i = 0; i < 5; i++) {
		orange[i]->render(shader, pvm_uniformId, vm_uniformId, normal_uniformId, texMode_uniformId);
	}

	renderHUD();

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
	for (i = 0; i < 5; i++){
		lives[i]->createMesh();
	}

	hudMessage->createMesh();

	// some GL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

}

int main(int argc, char **argv) {

	//  GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_STENCIL | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);

	glutInitContextVersion(3, 1);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WinX, WinY);
	WindowHandle = glutCreateWindow(CAPTION);

	if (terrain == NULL)
		terrain = new Terrain();

	if (road == NULL)
		road = new Road();

	srand((unsigned int)time(NULL));
	//Orange respawn
	for (int i = 0; i < 5; i++){
		if (orange[i] == NULL){
			//Spawns an orange between -100 and 100
			orange[i] = new Orange(-100.0f + (rand() % 200), 3.0f, -100.0f + (rand() % 200));
		}
	}

	if (butter == NULL){
		butter = new Butter(-80.0f + (rand() % 160), 1.0f, -80.0f + (rand() % 160));
	}

	if (dirLight0 == NULL) {
		dirLight0 = new DirectionalLight(-1.0f, 1.0f, 0.0f, 0.0f, 0);
		dirLight0->setActive(false);
	}
	if (pointLight1 == NULL) {
		pointLight1 = new PointLight(80.0f, 5.0f, -80.0f, 1.0f, 1);
		pointLight2 = new PointLight(-80.0f, 5.0f, -80.0f, 1.0f, 2);
		pointLight3 = new PointLight(40.0f, 5.0f, 0.0f, 1.0f, 3);
		pointLight4 = new PointLight(-40.0f, 5.0f, 0.0f, 1.0f, 4);
		pointLight5 = new PointLight(80.0f, 5.0f, 80.0f, 1.0f, 5);
		pointLight6 = new PointLight(-80.0f, 5.0f, 80.0f, 1.0f, 6);
	}
	if (spotLight7 == NULL) {
		spotLight7 = new SpotLight(0.0f, 0.0f, 0.0f, 1.0f, 7);
		spotLight8 = new SpotLight(0.0f, 0.0f, 0.0f, 1.0f, 8);
	}

	if (car == NULL)
		car = new Car(spotLight7, spotLight8);

	for (int i = 0; i < MAX_LIVES; i++) {
		if (lives[i] == NULL){
			lives[i] = new Car();
		}
	}

	if (camera == NULL)
		camera = new Camera(car);

	if (hudMessage == NULL)
		hudMessage = new HudMessage(0.0f, -3.0f, 0.0f);

	entities.push_back(car);
	entities.push_back(terrain);
	entities.push_back(butter);
	entities.push_back(road);
	lights.push_back(dirLight0);
	lights.push_back(pointLight1);
	lights.push_back(pointLight2);
	lights.push_back(pointLight3);
	lights.push_back(pointLight4);
	lights.push_back(pointLight5);
	lights.push_back(pointLight6);
	lights.push_back(spotLight7);
	lights.push_back(spotLight8);
	//  Callback Registration
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutTimerFunc(0, timer, 0);
	glutTimerFunc(0, refresh, 0);
	glutTimerFunc(0, timeCount, 0);

	//	Mouse and Keyboard Callbacks
	glutKeyboardFunc(processKeys);
	glutKeyboardUpFunc(processUpKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
	glutMouseWheelFunc(mouseWheel);
	


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