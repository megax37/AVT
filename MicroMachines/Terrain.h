#ifndef __Terrain__
#define __Terrain__

#include "Entity.h"

class Terrain : public Entity {
private:
	GLuint TextureArray[2];

public:
	Terrain();

	void createMesh();
	void render(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &texMode_uniformId);
	void move(int delta_t){}
	void increasePosition(float dx, float dy, float dz){}
	void increaseRotation(float dx, float dy, float dz){}
};
#endif