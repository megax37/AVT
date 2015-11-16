#include "Firework.h"

Firework::Firework() : ParticleSystem() {}

Firework::Firework(float x, float y, float z, int numberOfParticles) : ParticleSystem(numberOfParticles) {
	startX = x;
	startY = y;
	startZ = z;
	initParticles();
}

void Firework::initParticles() {
	GLfloat v, theta, phi;
	int i;

	for (i = 0; i < max_particles; i++)
	{
		v = 0.3f * frand() + 0.1f;
		phi = frand() * (float) PI;
		theta = 2.0f * frand() * (float) PI;

		particles[i].x = startX;
		particles[i].y = startY;
		particles[i].z = startZ;
		particles[i].vx = v * cos(theta) * sin(phi);
		particles[i].vy = v * cos(phi);
		particles[i].vz = v * sin(theta) * sin(phi);
		particles[i].ax = 0.0f; /* simular um pouco de vento */
		particles[i].ay = -0.15f; /* simular a aceleração da gravidade */
		particles[i].az = 0.01f;

		/* tom amarelado que vai ser multiplicado pela textura que varia entre branco e preto */
		particles[i].r = red; //0.882f;
		particles[i].g = green; // 0.552f;
		particles[i].b = blue; // 0.211f;

		particles[i].life = 1.0f;		/* vida inicial */
		particles[i].fade = 0.005f;	    /* step de decréscimo da vida para cada iteração */
	}
}

void Firework::setRGB(float r, float g, float b) {
	red = r;
	green = g;
	blue = b;
}
