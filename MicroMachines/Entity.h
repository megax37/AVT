#ifndef __Entity__
#define __Entity__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <sstream>
#include <string>

// include GLEW to access OpenGL 3.3 functions
#include <GL/glew.h>

// GLUT is the toolkit to interface with the OS
#include <GL/freeglut.h>

// Use Very Simple Libs
#include "vsShaderLib.h"
#include "AVTmathLib.h"
#include "basic_geometry.h"
#include "TGA.h"
#include "Box.h"

#define PI 3.14159265
// The storage for matrices
extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

// The normal matrix
extern float mNormal3x3[9];

class Entity {
protected:
	MyMesh* mesh;
	Box* box;
	int meshLength;
	int frame = 0;
	int objId = 0; //id of the object mesh - to be used as index of mesh: mesh[objID] means the current mesh

public:
	float current_position[3];
	float previousPosition[3];
	float current_rotation[3];
	float initial_velocity[3];
	float current_scale[3];
	bool rotateY = false;
	bool rotateX = false;
	bool rotateZ = false;

	Entity();
	Entity(int meshCapacity);

	Box* getBox() {
		return box;
	}

	virtual void createMesh() = 0;
	virtual void render(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &texMode_uniformId) = 0;
	virtual void move() = 0;
	virtual void increasePosition(float dx, float dy, float dz) = 0;
	virtual void increaseRotation(float dx, float dy, float dz) = 0;
};

#endif