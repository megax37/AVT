#ifndef __ParticleSystem__
#define __ParticleSystem__

#include "Entity.h"

typedef struct {
	float	life;		// lifespan
	float	fade;		// fade
	float	r, g, b;    // color
	GLfloat x, y, z;    // position
	GLfloat vx, vy, vz; // velocity 
	GLfloat ax, ay, az; // aceleration
} Particle;

class ParticleSystem : public Entity {

protected:
	int max_particles = 500;
	Particle *particles;
	GLuint TextureArray[1];
public:

	ParticleSystem();
	ParticleSystem(int numberOfParticles);

	virtual void initParticles() = 0;
	void createMesh();
	void move(int delta_t);
	void increasePosition(float dx, float dy, float dz){};
	void increaseRotation(float dx, float dy, float dz){};
	void render(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &texMode_uniformId);
};

#endif