#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(float x, float y, float z, float w, int lightID) : LightSource(lightID) {
	light->position[0] = x;
	light->position[1] = y;
	light->position[2] = z;
	light->position[3] = w;
}

void DirectionalLight::createMesh() {
	float l_diff[] = { 0.8f, 0.8f, 0.7f, 1.0f }; //luz amarela 
	float l_spec[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	memcpy(light->diffuse, l_diff, 4 * sizeof(float));
	memcpy(light->specular, l_spec, 4 * sizeof(float));
}

void DirectionalLight::draw(VSShaderLib &shader) {

	GLuint loc;
	float res[4];
	multMatrixPoint(VIEW, light->position, res);   //lightPos definido em World Coord so is converted to eye space

	char * uniformName = (char*) malloc(35 * sizeof(char));

	sprintf_s(uniformName, 35, "lights[%d].l_pos", lightID);

	loc = glGetUniformLocation(shader.getProgramIndex(), uniformName);
	glUniform4fv(loc, 1, res);
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].diffuse");
	glUniform4fv(loc, 1, light->diffuse);
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].specular");
	glUniform4fv(loc, 1, light->specular);
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].constantAttenuation");
	glUniform1f(loc, light->constantAttenuation);
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].linearAttenuation");
	glUniform1f(loc, light->linearAttenuation);
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].quadraticAttenuation");
	glUniform1f(loc, light->quadraticAttenuation);
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].spotCutoff");
	glUniform1f(loc, light->spotCutoff);
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].isActive");
	glUniform1i(loc, isActive);

	free(uniformName);
}
