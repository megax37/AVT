#ifndef __Camera__
#define __Camera__

#include <string>
#include <vector>
#include <map>
#include <GL/glew.h>


class Camera {
private:
	// Camera Position
	float camX, camY, camZ;
	//float distanceFromPlayer = 50;

	//float pitch, yaw, roll;

public:
	// Camera Spherical Coordinates
	//float alpha = 39.0f, beta = 51.0f;
	//float alpha = 0.0f, beta = 20.0f;
	float r = 100.0f;
	float angleAroundPlayer = 0;
	float pitch = 90;

	Camera();

	float getCamX() { return camX; }
	float getCamY() { return camY; }
	float getCamZ() { return camZ; }
	//float getAlpha() { return alpha; }
	//float getbeta() { return beta; }

	/*void setAlpha(float newAlpha) {
	alpha = newAlpha;
	}

	void setBeta(float newBeta) {
	beta = newBeta;
	}*/

	//float getDistanceFromPlayer() { return distanceFromPlayer; }
	float getAngleAroundPlayer() { return angleAroundPlayer; }
	float getPitch() { return pitch; }

	/*void setDistanceFromPlayer(float newdistanceFromPlayer) {
	distanceFromPlayer = newdistanceFromPlayer;
	}*/
	void setAngleAroundPlayer(float newangleAroundPlayer) {
		angleAroundPlayer = newangleAroundPlayer;
	}

	void setCamX(float camx) {
		camX = camx;
	}

	void setCamY(float camy) {
		camY = camy;
	}

	void setCamZ(float camz) {
		camZ = camz;
	}

	void setPitch(float newpitch) {
		pitch = newpitch;
	}

	void calculate(float r, float alpha, float beta);
	float calculateHorizontaDistance();
	float calculateVerticalDistance();
	void calculateCameraPos(bool front, float currentRotation, float xposition, float yposition, float zposition);
	void setValues(float camx, float camy, float camz);
	void move(bool front, float currentRotation, float xposition, float yposition, float zposition);

};

#endif