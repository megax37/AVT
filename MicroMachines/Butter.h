#ifndef __Butter__
#define __Butter__

#include "Entity.h"

class Butter : public Entity {
private:
	float position[3];
	GLuint TextureArray[1];
public:

	Butter();
	Butter(float x, float y, float z);

	void createMesh();
	void move();
	void increasePosition(float dx, float dy, float dz);
	void increaseRotation(float dx, float dy, float dz);
	void render(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &texMode_uniformId);
};

#endif