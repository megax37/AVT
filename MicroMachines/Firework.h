#ifndef __Firework__
#define __Firework__

#include "ParticleSystem.h"

class Firework : public ParticleSystem {

public:

	Firework();
	Firework(int numberOfParticles);

	void initParticles();
};

#endif