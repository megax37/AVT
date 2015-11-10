#ifndef __Vector4__
#define __Vector4__

#include <stdio.h>
#include <stdlib.h>

class Vector4 {

public:
	float x;
	float y;
	float z;
	float w;

	Vector4(float coorx, float coory, float coorz, float coorw);

	/*float getX() { return x; }
	float getY() { return y; }
	float getZ() { return z; }
	float getW() { return w; }*/
};
#endif