#ifndef __Orange__
#define __Orange__

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
#include "Entity.h"


class Orange : public Entity {
private:
	float position[3];
	float aceleration = 0.0f;
public:

	Orange();
	Orange(float x, float y, float z);

	void createMesh();
	void move();
	void increasePosition(float dx, float dy, float dz);
	void increaseRotation(float dx, float dy, float dz);
	void render(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &lPos_uniformId);

	void setAceleration(float newAceleration) {
		aceleration = newAceleration;
	}

	float getAceleration() {
		return aceleration;
	}
};

#endif