#include <stdio.h>
#include <stdlib.h>

#include "PointLight.h"
#define PI 3.14159265
/// The storage for matrices
extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];

PointLight::PointLight(float x, float y, float z, float w) : LightSource(1) {
	light->position[0] = x;
	light->position[1] = y;
	light->position[2] = z;
	light->position[3] = w;
	light->constantAttenuation = 0.5f;
	light->linearAttenuation = 0.01f;
	light->quadraticAttenuation = 0.0f;
	//light->spotDirection[0] = 0.0;
	//light->spotDirection[1] = -y;
	//light->spotDirection[2] = 0.0;
	light->spotCutoff = 180.0f; //180 graus
	//light->spotExponent = 0.0f;
}

void PointLight::createMesh() {
	//float amb[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // luz ambiente do sol
	//float diff[] = { 0.9f, 0.9f, 0.5f, 1.0f }; //luz amarela 
	float l_diff[] = { 1.0f, 1.0f, 0.9f, 1.0f }; //luz amarela
	float l_spec[] = { 0.8f, 0.8f, 0.4f, 1.0f };
	//float emissive[] = { 0.9f, 0.9f, 0.5f, 1.0f };
	//float shininess = 120.0f;
	//int texcount = 0;

	//objId = lightID;
	//memcpy(light->lightColor, amb, 4 * sizeof(float));
	memcpy(light->diffuse, l_diff, 4 * sizeof(float));
	memcpy(light->specular, l_spec, 4 * sizeof(float));
		//memcpy(light->emissive, emissive, 4 * sizeof(float));
		//light->shininess = shininess;
		//light->texCount = texcount;
	/*light->position[0] = position[0];
	light->position[1] = position[1];
	light->position[2] = position[2];
	light->position[3] = position[3];*/
		//mesh[objId].position[0] = position[0];
		//mesh[objId].position[1] = position[1];
		//mesh[objId].position[2] = position[2];
		//createSphere(3.0f, 20, mesh, objId);

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
		//createSphere(3.0f, 20, mesh, objId);

		objId = 2;
		//memcpy(light->lightColor, amb, 4 * sizeof(float));
		memcpy(light[2].lightColor, diff, 4 * sizeof(float));
		//memcpy(light->specular, spec, 4 * sizeof(float));
		//memcpy(light->emissive, emissive, 4 * sizeof(float));
		//light->shininess = shininess;
		//light->texCount = texcount;
		light[2].position[0] = position[0] - 200;
		light[2].position[1] = position[1];
		light[2].position[2] = position[2] - 200;
		light[2].position[3] = position[3];
		//mesh[objId].position[0] = position[0];
		//mesh[objId].position[1] = position[1];
		//mesh[objId].position[2] = position[2];
		//createSphere(3.0f, 20, mesh, objId);

		objId = 3;
		//memcpy(light->lightColor, amb, 4 * sizeof(float));
		memcpy(light[3].lightColor, diff, 4 * sizeof(float));
		//memcpy(light->specular, spec, 4 * sizeof(float));
		//memcpy(light->emissive, emissive, 4 * sizeof(float));
		//light->shininess = shininess;
		//light->texCount = texcount;
		light[3].position[0] = position[0];
		light[3].position[1] = position[1];
		light[3].position[2] = position[2] - 200;
		light[3].position[3] = position[3];
		//mesh[objId].position[0] = position[0];
		//mesh[objId].position[1] = position[1];
		//mesh[objId].position[2] = position[2];
		//createSphere(3.0f, 20, mesh, objId);*/
}
void PointLight::draw(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &lPos_uniformId) {

	GLuint loc;
	float res[4];		
	float res1[4];
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
	//multMatrixPoint(VIEW, light->spotDirection, res1);
	//loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].spotDirection");
	//glUniform4fv(loc, 1, light->spotDirection);
	//loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].spotExponent");
	//glUniform1f(loc, light->spotExponent);
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].spotCutoff");
	glUniform1f(loc, light->spotCutoff);
	loc = glGetUniformLocation(shader.getProgramIndex(), "lights[0].isActive");
	glUniform1i(loc, isActive);

	/*ss << "lights[" << lightID << "].l_pos";
	uniformName = ss.str();
	shader.setUniform(uniformName.c_str(), res);
	ss << "lights[" << lightID << "].diffuse";
	uniformName = ss.str();
	shader.setUniform(uniformName.c_str(), light->diffuse);
	ss << "lights[" << lightID << "].specular";
	uniformName = ss.str();
	shader.setUniform(uniformName.c_str(), light->specular);
	ss << "lights[" << lightID << "].constantAttenuation";
	uniformName = ss.str();
	shader.setUniform(uniformName.c_str(), light->constantAttenuation);
	ss << "lights[" << lightID << "].linearAttenuation";
	uniformName = ss.str();
	shader.setUniform(uniformName.c_str(), light->linearAttenuation);
	ss << "lights[" << lightID << "].quadraticAttenuation";
	uniformName = ss.str();
	shader.setUniform(uniformName.c_str(), light->quadraticAttenuation);
	ss << "lights[" << lightID << "].spotDirection";
	uniformName = ss.str();
	shader.setUniform(uniformName.c_str(), res1);
	ss << "lights[" << lightID << "].spotCutoff";
	uniformName = ss.str();
	shader.setUniform(uniformName.c_str(), light->spotCutoff);
	ss << "lights[" << lightID << "].spotExponent";
	uniformName = ss.str();
	shader.setUniform(uniformName.c_str(), light->spotExponent);
	ss << "lights[" << lightID << "].isActive";
	uniformName = ss.str();
	shader.setUniform(uniformName.c_str(), isActive);*/

}

