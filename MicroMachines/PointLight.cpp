#include <stdio.h>
#include <stdlib.h>

#include "PointLight.h"

/// The storage for matrices
extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];

PointLight::PointLight(float x, float y, float z, float w, int lightID) : LightSource(1, lightID) {
	light->position[0] = x;
	light->position[1] = y;
	light->position[2] = z;
	light->position[3] = w;
	light->constantAttenuation = 0.5f;
	light->linearAttenuation = 0.01f;
	light->quadraticAttenuation = 0.0f;
	light->spotCutoff = 180.0f;
}

void PointLight::createMesh() {
	float l_diff[] = { 1.0f, 1.0f, 0.9f, 1.0f }; //luz amarela
	float l_spec[] = { 0.8f, 0.8f, 0.4f, 1.0f };

	memcpy(light->diffuse, l_diff, 4 * sizeof(float));
	memcpy(light->specular, l_spec, 4 * sizeof(float));

	/*objId = 1;
	//memcpy(light->lightColor, amb, 4 * sizeof(float));
	memcpy(light[1].lightColor, diff, 4 * sizeof(float));
	//memcpy(light->specular, spec, 4 * sizeof(float));
	//memcpy(light->emissive, emissive, 4 * sizeof(float));
	//light->shininess = shininess;
	//light->texCount = texcount;
	light[1].position[0] = position[0] - 200;
	light[1].position[1] = position[1];
	light[1].position[2] = position[2];
	light[1].position[3] = position[3];
	//mesh[objId].position[0] = position[0];
	//mesh[objId].position[1] = position[1];
	//mesh[objId].position[2] = position[2];
	//createSphere(3.0f, 20, mesh, objId);*/
}

void PointLight::draw(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &lPos_uniformId) {

	GLuint loc;
	float res[4];
	std::ostringstream ss;
	std::string uniformName;
	multMatrixPoint(VIEW, light->position, res);   //lightPos definido em World Coord so is converted to eye space

	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].l_pos");
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

	ss << "light" << lightID << ".l_pos";
	uniformName = ss.str();
	
	/*if (b) {
		printf("%s \n", uniformName.data());
		b = false;
	}
	
	loc = glGetUniformLocation(shader.getProgramIndex(), uniformName.c_str());
	glUniform4fv(loc, 1, res);
	
	ss << "light" << lightID << ".diffuse";
	uniformName = ss.str();
	loc = glGetUniformLocation(shader.getProgramIndex(), uniformName.c_str());
	glUniform4fv(loc, 1, light->diffuse);
	
	ss << "light" << lightID << ".specular";
	uniformName = ss.str();
	loc = glGetUniformLocation(shader.getProgramIndex(), uniformName.c_str());
	glUniform4fv(loc, 1, light->specular);
	
	ss << "light" << lightID << ".constantAttenuation";
	uniformName = ss.str();
	loc = glGetUniformLocation(shader.getProgramIndex(), uniformName.c_str());
	glUniform1f(loc, light->constantAttenuation);
	
	ss << "light" << lightID << ".linearAttenuation";
	uniformName = ss.str();
	loc = glGetUniformLocation(shader.getProgramIndex(), uniformName.c_str());
	glUniform1f(loc, light->linearAttenuation);
	
	ss << "light" << lightID << ".quadraticAttenuation";
	uniformName = ss.str();
	loc = glGetUniformLocation(shader.getProgramIndex(), uniformName.c_str());
	glUniform1f(loc, light->quadraticAttenuation);
	
	ss << "light" << lightID << ".spotCutoff";
	uniformName = ss.str();
	loc = glGetUniformLocation(shader.getProgramIndex(), uniformName.c_str());
	glUniform1f(loc, light->spotCutoff);
	
	ss << "light" << lightID << ".isActive";
	uniformName = ss.str();
	loc = glGetUniformLocation(shader.getProgramIndex(), uniformName.c_str());
	glUniform1i(loc, isActive);*/

}

