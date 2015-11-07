#ifndef __Car__
#define __Car__

#include "Entity.h"
#include "SpotLight.h"

class Car : public Entity {
private:
	float max_velocity = 30;
	float turn_speed = 160;
	float current_speed = 0;
	float currentTurn_speed = 0;
	float aceleration = 5.0;
	float current_Aceleration = 0.0f;
	bool front = true;
	SpotLight* spotLight;
	SpotLight* spotLight1;

public:
	Car();
	Car(SpotLight* spot, SpotLight* spot1);

	void createMesh();
	void render(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &texMode_uniformId);
	void move(int delta_t);
	void increasePosition(float dx, float dy, float dz);
	void increaseRotation(float dx, float dy, float dz);
	bool getFront() { return front; }
	void setFront(bool newFront) { front = newFront; }
	float getTurn_speed() { return turn_speed; }
	float getAceleration() { return aceleration; }
	float getCurrent_Aceleration() { return current_Aceleration; }
	void setCurrent_Aceleration(float newAceleration) { current_Aceleration = newAceleration; }
	float getCurrent_Speed() { return current_speed; }
	void setCurrent_Speed(float newSpeed) { current_speed = newSpeed; }
	float getCurrentTurn_speed() { return currentTurn_speed; }
	void setCurrentTurn_speed(float newSpeed) { currentTurn_speed = newSpeed; }
};

#endif