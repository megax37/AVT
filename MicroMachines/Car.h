#ifndef __Car__
#define __Car__

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
#include "vsShaderLib.h"
#include "AVTmathLib.h"
#include "basic_geometry.h"

class Car {
private:
	float max_velocity = 25;
	float velocity = 20;
	float turn_speed = 160;
	float current_speed = 0;
	float currentTurn_speed = 0;
public:
	MyMesh mesh[5];
	int meshLength = 5;
	float current_position[3];
	float current_rotation[3];
	float lastRotationValue = 0.0f;
	int frame = 0;
	int objId = 0; //id of the object mesh - to be used as index of mesh: mesh[objID] means the current mesh


	Car();

	void createCarMesh();
	void renderCar(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &lPos_uniformId);
	void move();
	void increasePosition(float dx, float dy, float dz);
	void increaseRotation(float dx, float dy, float dz);
	MyMesh* getMesh() { return mesh; }
	int getMeshLength() { return meshLength; }
	float getVelocity() { return velocity; }
	float getTurn_speed() { return turn_speed; }
	float getCurrent_Speed() { return current_speed; }
	void setCurrent_Speed(float newSpeed) { current_speed = newSpeed; }
	float getCurrentTurn_speed() { return currentTurn_speed; }
	void setCurrentTurn_speed(float newSpeed) { currentTurn_speed = newSpeed; }
};

#endif