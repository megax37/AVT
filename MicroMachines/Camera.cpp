#include <stdio.h>
#include <stdlib.h>

#include "Camera.h"

#define PI 3.14159265

Camera::Camera() {}

void Camera::calculate(float r, float alpha, float beta) {
	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r *   						     sin(beta * 3.14f / 180.0f);
}

void Camera::setValues(float camx, float camy, float camz) {
	camX = camx;
	camZ = camy;
	camY = camz;
}

void Camera::move(float currentRotation, float xposition, float yposition, float zposition) {
	//float horizontalDistance = calculateHorizontaDistance();
	//float verticalDistance = calculateVerticalDistance();
	calculateCameraPos(currentRotation, xposition, yposition, zposition);
	
}

/*float Camera::calculateHorizontaDistance() {
	return r * cos(pitch * (PI / 180.0f));
}*/

/*float Camera::calculateVerticalDistance() {
	return r * sin(pitch * (PI / 180.0f));
}*/

void Camera::calculateCameraPos(float currentRotation, float xposition, float yposition, float zposition) {
	/*float theta = currentRotation + angleAroundPlayer;
	float offsetX = horizDistance * sin(theta * (PI / 180.0f));
	float offsetZ = horizDistance * cos(theta * (PI / 180.0f));
	camX = xposition - offsetX;
	camY = yposition + verticDistance;
	camZ = zposition - offsetZ;*/
	float theta = currentRotation + angleAroundPlayer;
	float offsetX = r * cos(pitch * 3.14f / 180.0f) * sin(theta * 3.14f / 180.0f);
	float offsetZ = r * cos(pitch * 3.14f / 180.0f) * cos(theta * (PI / 180.0f));
	camX = xposition - offsetX;
	camY = yposition + (r *   						     sin(pitch * 3.14f / 180.0f));
	camZ = zposition - offsetZ;

}