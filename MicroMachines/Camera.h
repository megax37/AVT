#ifndef __Camera__
#define __Camera__

#include <string>
#include <vector>
#include <map>
#include <GL/glew.h>
#include "Car.h"


class Camera {
private:
	// Camera Position
	float camX, camY, camZ;
	float r = 10.0f;
	float angleAroundPlayer = 0.0;
	float pitch = 90;
	Car* car;
	bool orthoView = false;
	bool perspectiveView = false;
	bool thirdPersonView = true;
	bool freeView = false;
	std::string currentView = "thirdPersonView";

public:
	Camera(Car* carr);

	float getCamX() { return camX; }
	float getCamY() { return camY; }
	float getCamZ() { return camZ; }

	float getAngleAroundPlayer() { return angleAroundPlayer; }

	void setAngleAroundPlayer(float newangleAroundPlayer) {
		angleAroundPlayer = newangleAroundPlayer;
	}

	float getPitch() { return pitch; }

	void setPitch(float newpitch) {
		pitch = newpitch;
	}

	float getR() { return r; }

	void setR(float newR) {
		r = newR;
	}

	float getFreeView() { return freeView; }

	void setFreeView(bool newFreeView) {
		freeView = newFreeView;
	}

	void view(int width, int height);
	void lookat();
	void chooseView(unsigned char key);
	void calculate(float newR, float newAngleAroundPlayer, float newPitch);
	void move();

};

#endif