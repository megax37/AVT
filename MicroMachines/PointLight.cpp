#include "PointLight.h"

PointLight::PointLight(float x, float y, float z, float w, int lightID) : LightSource(lightID) {
	light->position[0] = x;
	light->position[1] = y;
	light->position[2] = z;
	light->position[3] = w;
	light->constantAttenuation = 0.5f;
	light->linearAttenuation = 0.01f;
	light->quadraticAttenuation = 0.0001f;
	light->spotCutoff = 180.0f;
}

void PointLight::createMesh() {
	float l_diff[] = { 1.0f, 1.0f, 0.9f, 1.0f };
	float l_spec[] = { 0.8f, 0.8f, 0.4f, 1.0f };

	memcpy(light->diffuse, l_diff, 4 * sizeof(float));
	memcpy(light->specular, l_spec, 4 * sizeof(float));
}

void PointLight::draw(VSShaderLib &shader) {

	GLuint loc;
	float res[4];
	multMatrixPoint(VIEW, light->position, res);   //lightPos definido em World Coord so is converted to eye space

	if (lightID == 1) {
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[1].l_pos");
		glUniform4fv(loc, 1, res);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[1].diffuse");
		glUniform4fv(loc, 1, light->diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[1].specular");
		glUniform4fv(loc, 1, light->specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[1].constantAttenuation");
		glUniform1f(loc, light->constantAttenuation);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[1].linearAttenuation");
		glUniform1f(loc, light->linearAttenuation);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[1].quadraticAttenuation");
		glUniform1f(loc, light->quadraticAttenuation);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[1].spotCutoff");
		glUniform1f(loc, light->spotCutoff);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[1].isActive");
		glUniform1i(loc, isActive);
	}
	else if (lightID == 2) {
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[2].l_pos");
		glUniform4fv(loc, 1, res);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[2].diffuse");
		glUniform4fv(loc, 1, light->diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[2].specular");
		glUniform4fv(loc, 1, light->specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[2].constantAttenuation");
		glUniform1f(loc, light->constantAttenuation);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[2].linearAttenuation");
		glUniform1f(loc, light->linearAttenuation);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[2].quadraticAttenuation");
		glUniform1f(loc, light->quadraticAttenuation);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[2].spotCutoff");
		glUniform1f(loc, light->spotCutoff);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[2].isActive");
		glUniform1i(loc, isActive);
	}
	else if (lightID == 3) {
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[3].l_pos");
		glUniform4fv(loc, 1, res);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[3].diffuse");
		glUniform4fv(loc, 1, light->diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[3].specular");
		glUniform4fv(loc, 1, light->specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[3].constantAttenuation");
		glUniform1f(loc, light->constantAttenuation);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[3].linearAttenuation");
		glUniform1f(loc, light->linearAttenuation);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[3].quadraticAttenuation");
		glUniform1f(loc, light->quadraticAttenuation);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[3].spotCutoff");
		glUniform1f(loc, light->spotCutoff);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[3].isActive");
		glUniform1i(loc, isActive);
	}
	else if (lightID == 4) {
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[4].l_pos");
		glUniform4fv(loc, 1, res);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[4].diffuse");
		glUniform4fv(loc, 1, light->diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[4].specular");
		glUniform4fv(loc, 1, light->specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[4].constantAttenuation");
		glUniform1f(loc, light->constantAttenuation);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[4].linearAttenuation");
		glUniform1f(loc, light->linearAttenuation);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[4].quadraticAttenuation");
		glUniform1f(loc, light->quadraticAttenuation);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[4].spotCutoff");
		glUniform1f(loc, light->spotCutoff);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[4].isActive");
		glUniform1i(loc, isActive);
	}
	else if (lightID == 5) {
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[5].l_pos");
		glUniform4fv(loc, 1, res);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[5].diffuse");
		glUniform4fv(loc, 1, light->diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[5].specular");
		glUniform4fv(loc, 1, light->specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[5].constantAttenuation");
		glUniform1f(loc, light->constantAttenuation);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[5].linearAttenuation");
		glUniform1f(loc, light->linearAttenuation);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[5].quadraticAttenuation");
		glUniform1f(loc, light->quadraticAttenuation);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[5].spotCutoff");
		glUniform1f(loc, light->spotCutoff);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[5].isActive");
		glUniform1i(loc, isActive);
	}
	else if (lightID == 6) {
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[6].l_pos");
		glUniform4fv(loc, 1, res);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[6].diffuse");
		glUniform4fv(loc, 1, light->diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[6].specular");
		glUniform4fv(loc, 1, light->specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[6].constantAttenuation");
		glUniform1f(loc, light->constantAttenuation);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[6].linearAttenuation");
		glUniform1f(loc, light->linearAttenuation);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[6].quadraticAttenuation");
		glUniform1f(loc, light->quadraticAttenuation);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[6].spotCutoff");
		glUniform1f(loc, light->spotCutoff);
		loc = glGetUniformLocation(shader.getProgramIndex(), "lights[6].isActive");
		glUniform1i(loc, isActive);
	}
}
