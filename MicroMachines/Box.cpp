#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include "Box.h"

Box::Box() {}

Box::Box(float x, float z, float radiouss) {
	xpos = x;
	zpos = z;
	radious = radiouss;
}

bool Box::intersectCircularBox(Box* carBox, Box* otherBox) {
	float xLength = otherBox->getXpos() - carBox->getXpos();
	float yLength = otherBox->getZpos() - carBox->getZpos();
	float sumRadious = otherBox->getRadious() + carBox->getRadious();
	float sumPow = pow(xLength, 2) + pow(yLength, 2);
	float sumPowR = pow(sumRadious, 2);

	return (sumPow <= sumPowR);
}

bool Box::interserctTerrainBox(Box* carBox, Box* terrainBox) {
	return (carBox->getZpos() > terrainBox->getHeight()) || (carBox->getZpos() < -terrainBox->getHeight()) || (carBox->getXpos() > terrainBox->getWidth()) || (carBox->getXpos() < -terrainBox->getWidth());
}