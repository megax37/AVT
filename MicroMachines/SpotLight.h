#ifndef __SpotLight__
#define __SpotLight__
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

class SpotLight : public LightSource {
public:
	SpotLight();
	SpotLight(float x, float y, float z, float w, int lightID);
	~SpotLight();
	void createMesh();
	void draw(VSShaderLib &shader);
	void setPos(float x, float y, float z, float dxx, float dzz);
	void setDir(float dirX, float dirY, float dirZ);

};
#endif