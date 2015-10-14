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


class Orange {
private:
	float position[3];
public:
	float current_position[3];
	float current_rotation[3];
	float initial_velocity[3];
	MyMesh mesh[1];
	int meshLength = 1;
	int objId = 0; //id of the object mesh - to be used as index of mesh: mesh[objID] means the current mesh


	Orange();
	Orange(float x, float y, float z);

	void createOrangeMesh();
	void move(float accelaration);
	void increasePosition(float dx, float dy, float dz);
	void increaseRotation(float dx, float dy, float dz);
	void renderOrange(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &lPos_uniformId);
	MyMesh* getMesh() { return mesh; }
	int getMeshLength() { return meshLength; }
};

#endif