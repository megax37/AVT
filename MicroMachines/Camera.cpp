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

void Camera::move(bool front, float currentRotation, float xposition, float yposition, float zposition) {
	calculateCameraPos(front, currentRotation, xposition, yposition, zposition);

}

void Camera::calculateCameraPos(bool front, float currentRotation, float xposition, float yposition, float zposition) {
	float theta = currentRotation + angleAroundPlayer;
	float offsetX = r * cos(pitch * 3.14f / 180.0f) * sin(theta * 3.14f / 180.0f);
	float offsetZ = r * cos(pitch * 3.14f / 180.0f) * cos(theta * (PI / 180.0f));
	if (front) {
		camX = xposition + offsetX;
		camZ = zposition + offsetZ;
	}
	else {
		camX = xposition - offsetX;
		camZ = zposition - offsetZ;
	}
	camY = yposition + (r *   						     sin(pitch * 3.14f / 180.0f));
}