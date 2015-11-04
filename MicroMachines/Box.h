#ifndef __Box__
#define __Box__

#include <math.h>

class Box {
protected:
	float radious;
	float xpos;
	float zpos;
public:

	Box();
	Box(float x, float z, float radiouss);

	void setX(float newX){
		xpos = newX;
	}

	void setZ(float newZ) {
		zpos = newZ;
	}

	float getXpos() { return xpos; }

	float getZpos() { return zpos; }

	float getRadious() { return radious; }

	//For StaticCubeBoxes
	float getHeight() { return zpos + radious; }

	float getWidth() { return xpos + radious; }

	static bool intersectCircularBox(Box* carBox, Box* otherBox);
	static bool interserctTerrainBox(Box* carBox, Box* terrainBox);
};

#endif