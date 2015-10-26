#ifndef __LightSource__
#define __LightSource__

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

struct Light{
	float position[4];
	float diffuse[4];
	float specular[4];
	float constantAttenuation = 1.0f, linearAttenuation = 0.1f, quadraticAttenuation = 0.0f;
	float spotCutoff = 180;
	float spotExponent = 0;
	float spotDirection[3];
};

class LightSource {
protected:
	Light* light;
	MyMesh* mesh;
	int meshLength;
	int frame = 0;
	int objId = 0;
	bool isActive = true;

public:
	LightSource();
	LightSource(int meshCapacity);
	~LightSource();

	virtual void createMesh() = 0;
	virtual void draw(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &lPos_uniformId) = 0;

};
#endif