#ifndef __Road__
#define __Road__

#include "Entity.h"

class Road : public Entity {
private:
	GLuint TextureArray[2];

public:
	Road();

	void createMesh();
	void render(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &texMode_uniformId);
	void move();
	void increasePosition(float dx, float dy, float dz);
	void increaseRotation(float dx, float dy, float dz);
};
#endif