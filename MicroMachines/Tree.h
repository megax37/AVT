#ifndef __Tree__
#define __Tree__

#include "Entity.h"
#include "Billboard.h"

class Tree : public Entity {
private:
	GLuint TextureArray[1];
public:

	Tree();
	Tree(float x, float y, float z);
	float currentposion[3];
	void createMesh();
	void move(int delta_t);
	void increasePosition(float dx, float dy, float dz);
	void increaseRotation(float dx, float dy, float dz);
	void render(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &texMode_uniformId);
	void render2(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &texMode_uniformId, float camPosX, float camPosY, float camPosZ, int type);
	
};

#endif