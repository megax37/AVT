#ifndef __PointLight__
#define __PointLight__
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
#include "LightSource.h"

class PointLight : public LightSource {

public:
	PointLight(float x, float y, float z, float w, int lightID);
	~PointLight();
	void createMesh();
	void draw(VSShaderLib &shader);

};
#endif