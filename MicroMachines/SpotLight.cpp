#include <stdio.h>
#include <stdlib.h>

#include "SpotLight.h"

SpotLight::SpotLight() {}

SpotLight::SpotLight(float x, float y, float z, float w, int lightID) : LightSource(lightID) {
	light->position[0] = x;
	light->position[1] = y;
	light->position[2] = z;
	light->position[3] = w;
	light->constantAttenuation = 0.1f;
	light->linearAttenuation = 0.003f;
	light->quadraticAttenuation = 0.00001f;
	light->spotDirection[0] = 0.0f;
	light->spotDirection[1] = 0.0f;
	light->spotDirection[2] = 0.0f;
	light->spotDirection[3] = 0.0f;
	light->spotCutoff = 40.0f;
	light->spotExponent = 6.0f;
}

void SpotLight::createMesh() {
	float l_diff[] = { 0.9f, 0.9f, 0.7f, 1.0f };
	float l_spec[] = { 0.7f, 0.7f, 0.4f, 1.0f };

	memcpy(light->diffuse, l_diff, 4 * sizeof(float));
	memcpy(light->specular, l_spec, 4 * sizeof(float));
}

void SpotLight::setPos(float x, float y, float z, float dxx, float dzz) {
	light->position[0] = x + dxx;
	light->position[1] = y;
	light->position[2] = z + dzz;
	light->position[3] = 1.0f;
}

void SpotLight::setDir(float dirX, float dirY, float dirZ) {
	light->spotDirection[0] = dirX;
	light->spotDirection[1] = dirY;
	light->spotDirection[2] = dirZ;
}

void SpotLight::draw(VSShaderLib &shader) {

	GLuint loc;
	float res[4];
	float res1[4];
	multMatrixPoint(VIEW, light->position, res);   //lightPos definido em World Coord so is converted to eye space
	multMatrixPoint(VIEW, light->spotDirection, res1);

	if (lightID == 7) {
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[7].l_pos");
		glUniform4fv(loc, 1, res);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[7].diffuse");
		glUniform4fv(loc, 1, light->diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[7].specular");
		glUniform4fv(loc, 1, light->specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[7].constantAttenuation");
		glUniform1f(loc, light->constantAttenuation);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[7].linearAttenuation");
		glUniform1f(loc, light->linearAttenuation);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[7].quadraticAttenuation");
		glUniform1f(loc, light->quadraticAttenuation);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[7].spotDirection");
		glUniform4fv(loc, 1, res1);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[7].spotExponent");
		glUniform1f(loc, light->spotExponent);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[7].spotCutoff");
		glUniform1f(loc, light->spotCutoff);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[7].isActive");
		glUniform1i(loc, isActive);
	}
	else if (lightID == 8) {
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[8].l_pos");
		glUniform4fv(loc, 1, res);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[8].diffuse");
		glUniform4fv(loc, 1, light->diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[8].specular");
		glUniform4fv(loc, 1, light->specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[8].constantAttenuation");
		glUniform1f(loc, light->constantAttenuation);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[8].linearAttenuation");
		glUniform1f(loc, light->linearAttenuation);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[8].quadraticAttenuation");
		glUniform1f(loc, light->quadraticAttenuation);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[8].spotDirection");
		glUniform4fv(loc, 1, res1);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[8].spotExponent");
		glUniform1f(loc, light->spotExponent);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[8].spotCutoff");
		glUniform1f(loc, light->spotCutoff);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[8].isActive");
		glUniform1i(loc, isActive);
	}
}