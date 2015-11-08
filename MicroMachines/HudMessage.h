#ifndef __HudMessage__
#define __HudMessage__

#include "Entity.h"

enum MessageType {
	GAMEPAUSE,
	GAMEOVER
};

class HudMessage : public Entity {
private:
	GLuint TextureArray[2];
	int messageType = 0;
public:

	HudMessage();
	HudMessage(float x, float y, float z);

	void createMesh();
	void move(int delta_t){}
	void increasePosition(float dx, float dy, float dz){}
	void increaseRotation(float dx, float dy, float dz){}
	void render(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &texMode_uniformId);

	void setMessageType(int type) { messageType = type; }
};

#endif