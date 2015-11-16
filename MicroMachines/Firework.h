#ifndef __Firework__
#define __Firework__

#include "ParticleSystem.h"

class Firework : public ParticleSystem {
private:
	float red = 1.0;
	float green = 1.0;
	float blue = 1.0;
public:
	Firework();
	Firework(float x, float y, float z, int numberOfParticles);

	void initParticles();
	void setRGB(float r, float g, float b);
};

#endif