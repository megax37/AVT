#include <stdio.h>
#include <stdlib.h>

#include "LightSource.h"
#define PI 3.14159265
/// The storage for matrices
extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];

LightSource::LightSource() {
	light = (Light*)malloc(sizeof(*light));
}

LightSource::LightSource(int meshCapacity, int lID) {
	light = (Light*)malloc(sizeof(*light));
	mesh = (MyMesh*)malloc(meshCapacity * sizeof(*mesh));
	meshLength = meshCapacity;
	lightID = lID;
}