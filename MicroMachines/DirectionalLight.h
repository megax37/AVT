#ifndef __DirectionLight__
#define __DirectionLight__
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
#include "LightSource.h"

class DirectionalLight : public LightSource {

public:
	DirectionalLight(float x, float y, float z, float w);
	~DirectionalLight();
	void createMesh();
	void draw(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &lPos_uniformId);

};
#endif