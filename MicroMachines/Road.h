#ifndef __Road__
#define __Road__

#include "Entity.h"

class Road : public Entity {
private:
	GLuint TextureArray[1];

public:
	Road();

	void createMesh();
	void render(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &texMode_uniformId);
	void move(int delta_t);
	void increasePosition(float dx, float dy, float dz);
	void increaseRotation(float dx, float dy, float dz);
};
#endif