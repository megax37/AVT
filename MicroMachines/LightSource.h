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
#include "vsShaderLib.h"
#include "AVTmathLib.h"
#include "basic_geometry.h"

struct Light{
	float position[4];
	float diffuse[4];
	float specular[4];
	float constantAttenuation, linearAttenuation, quadraticAttenuation;
	float spotCutoff;
	float spotExponent;
	float spotDirection[4];
};

class LightSource {
protected:
	Light* light;
	int lightID;
	bool isActive = true;

public:
	LightSource();
	LightSource(int lID);
	~LightSource();

	virtual void createMesh() = 0;
	virtual void draw(VSShaderLib &shader) = 0;

	bool getActive() {
		return isActive;
	}

	void setActive(bool active) {
		isActive = active;
	}
};
#endif