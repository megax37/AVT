#ifndef __ObjLoader__
#define __ObjLoader__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <array>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include "Vector4.h"

struct ObjMesh {
	float *verticesArray;
	long int verticesLength;
	float *textVerticesArray;
	long int textVerticesLength;
	float *normalArray;
	long int normalLength;
	unsigned int *indicesArray;
	long int indicesLength;
};

class ObjLoader {
protected:
	ObjMesh* objMesh;

public:

	ObjLoader();

	void loadObjModel(std::string fileName);
	void printArrays(float* array, long int length);
	void printArrays(int* array, long int length);
	ObjMesh* getObjMesh() { return objMesh; }
};
#endif