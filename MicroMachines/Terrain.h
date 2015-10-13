#ifndef __Terrain__
#define __Terrain__

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


class Terrain {
public:
	MyMesh mesh[1];
	int meshLength = 1;
	int objId = 0; //id of the object mesh - to be used as index of mesh: mesh[objID] means the current mesh


	Terrain();

	void createTerrainMesh();
	void renderTerrain(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &lPos_uniformId);
	MyMesh* getMesh() { return mesh; }
	int getMeshLength() { return meshLength; }
};

#endif