#include "Firework.h"

Firework::Firework() : ParticleSystem() {}

Firework::Firework(int numberOfParticles) : ParticleSystem(numberOfParticles) {
	initParticles();
}

void Firework::initParticles() {
	GLfloat v, theta, phi;
	int i;

	for (i = 0; i < max_particles; i++)
	{
		v = 0.8f * frand() + 0.2f;
		phi = frand() * (float) PI;
		theta = 2.0f * frand() * (float) PI;

		particles[i].x = 0.0f;
		particles[i].y = 8.0f;
		particles[i].z = 0.0f;
		particles[i].vx = v * cos(theta) * sin(phi);
		particles[i].vy = v * cos(phi);
		particles[i].vz = v * sin(theta) * sin(phi);
		particles[i].ax = 0.0f; /* simular um pouco de vento */
		particles[i].ay = -0.05f; /* simular a aceleração da gravidade */
		particles[i].az = 0.01f;

		/* tom amarelado que vai ser multiplicado pela textura que varia entre branco e preto */
		particles[i].r = 0.882f;
		particles[i].g = 0.552f;
		particles[i].b = 0.211f;

		particles[i].life = 1.0f;		/* vida inicial */
		particles[i].fade = 0.005f;	    /* step de decréscimo da vida para cada iteração */
	}
}
