#include <stdio.h>
#include <stdlib.h>

#include "Camera.h"

#define PI 3.14159265

Camera::Camera(Car* carr) {
	car = carr;
}

void Camera::chooseView(unsigned char key) {
	switch (key) {
	case '1':
		orthoView = true;
		perspectiveView = false;
		thirdPersonView = false;
		break;
	case '2':
		orthoView = false;
		perspectiveView = true;
		thirdPersonView = false;
		break;
	case '3':
		orthoView = false;
		perspectiveView = false;
		thirdPersonView = true;
		break;
	}
}

void Camera::view(float ratio) {
	if (orthoView) {
		ortho(-200, 200, -200, 200, -200.0f, 200.0f);
	}
	else if (perspectiveView || thirdPersonView) {
		perspective(90.0f, ratio, 0.1f, 1000.0f);
	}
}

void Camera::lookat() {
	if (freeView) {
		lookAt(camX, camY, camZ, 0, 0, 0, 0, 1, 0);
		return;
	}
	else if (orthoView) {
		pitch = 90.0f;
		camX = 0.0f;
		camY = 105.0f;
		camZ = 0.0f;
		lookAt(camX, camY, camZ, 0, 0, 0, 0, 0, 1);
	}
	else if (perspectiveView) {
		pitch = 90.0f;
		camX = 0.0f;
		camY = 100.0f;
		camZ = -40.0f;
		lookAt(camX, camY, camZ, 0, 0, -20.0f, 0, 0, 1);

	}
	else if (thirdPersonView) {
		pitch = 25.0f;
		//angleAroundPlayer = -5.0;
		move();
		lookAt(camX, camY, camZ, car->current_position[0], 0, car->current_position[2], 0, 1, 0);
	}
}

void Camera::calculate(float newR, float newAngleAroundPlayer, float newPitch) {
	camX = newR * sin(newAngleAroundPlayer * 3.14f / 180.0f) * cos(newPitch * 3.14f / 180.0f);
	camZ = newR * cos(newAngleAroundPlayer * 3.14f / 180.0f) * cos(newPitch * 3.14f / 180.0f);
	camY = newR *   						     sin(newPitch * 3.14f / 180.0f);
}

void Camera::move() {
	float theta = car->current_rotation[1] + angleAroundPlayer;
	float offsetX = r * cos(pitch * 3.14f / 180.0f) * sin(theta * 3.14f / 180.0f);
	float offsetZ = r * cos(pitch * 3.14f / 180.0f) * cos(theta * ((float) PI / 180.0f));
	if (car->getFront()) {
		camX = car->current_position[0] - offsetX;
		camZ = car->current_position[2] - offsetZ;
	}
	camY = car->current_position[1] + (r *   						     sin(pitch * 3.14f / 180.0f));

}