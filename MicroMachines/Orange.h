#ifndef __Orange__
#define __Orange__

#include "Entity.h"

class Orange : public Entity {
private:
	float position[3];
	float aceleration = 0.0f;
	GLuint TextureArray[1];

public:

	Orange();
	Orange(float x, float y, float z);

	void createMesh();
	void move();
	void increasePosition(float dx, float dy, float dz);
	void increaseRotation(float dx, float dy, float dz);
	void render(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &texMode_uniformId);

	void setAceleration(float newAceleration) {
		aceleration = newAceleration;
	}

	float getAceleration() {
		return aceleration;
	}
};

#endif