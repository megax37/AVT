#include "ParticleSystem.h"
//#include "glbmp.h"

ParticleSystem::ParticleSystem() : Entity(1) {
	glGenTextures(1, TextureArray);
	TGA_Texture(TextureArray, "particle.tga", 0);
	particles = (Particle *) malloc(max_particles * sizeof(Particle));
}

ParticleSystem::ParticleSystem(int numberOfParticles) : Entity(1) {
	glGenTextures(1, TextureArray);
	TGA_Texture(TextureArray, "particle.tga", 0);
	//BMP_Texture(TextureArray, "particula.bmp", 0);
	max_particles = numberOfParticles;
	particles = (Particle *) malloc(max_particles * sizeof(Particle));
}

void ParticleSystem::createMesh() {

	float shininess = 100.0f;
	int texcount = 1;

	objId = 0;
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	mesh[objId].texUnits[0] = TextureArray[0];
	createQuad(1, 1, mesh, objId);
}

void ParticleSystem::render(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &texMode_uniformId) {

	GLuint loc;
	float particle_color[4];

	for (int i = 0; i < meshLength; ++i) {

		if (mesh[i].mat.texCount != 0) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mesh[i].texUnits[0]);
			glUniform1i(texMode_uniformId, 3);
		}
		for (int j = 0; j < max_particles; j++) {

			if (particles[j].life > 0.0f) {
				// send the material
				particle_color[0] = particles[j].r;
				particle_color[1] = particles[j].g;
				particle_color[2] = particles[j].b;
				particle_color[3] = particles[j].life;

				loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
				glUniform4fv(loc, 1, particle_color);
				loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
				glUniform1f(loc, mesh[i].mat.shininess);

				pushMatrix(MODEL);
				translate(MODEL, particles[j].x, particles[j].y, particles[j].z);

				// send matrices to OGL
				computeDerivedMatrix(PROJ_VIEW_MODEL);
				glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
				glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
				computeNormalMatrix3x3();
				glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

				// Render mesh
				glBindVertexArray(mesh[i].vao);
				glDrawElements(mesh[i].type, mesh[i].numIndexes, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);

				popMatrix(MODEL);
			}
		}
		glUniform1i(texMode_uniformId, 0);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ParticleSystem::move(int delta_t) {
	/* Método de Euler de integração de eq. diferenciais ordinárias
	h representa o step de tempo; dv/dt = a; dx/dt = v; e conhecem-se os valores iniciais de x e v */

	float h = 0.125f;
	for (int i = 0; i < max_particles; i++)
	{
		particles[i].x += (h*particles[i].vx);
		particles[i].y += (h*particles[i].vy);
		particles[i].z += (h*particles[i].vz);
		particles[i].vx += (h*particles[i].ax);
		particles[i].vy += (h*particles[i].ay);
		particles[i].vz += (h*particles[i].az);
		particles[i].life -= particles[i].fade;
	}
}
