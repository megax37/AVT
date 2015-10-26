#include <stdio.h>
#include <stdlib.h>

#include "DirectionalLight.h"
#define PI 3.14159265
/// The storage for matrices
extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];

DirectionalLight::DirectionalLight(float x, float y, float z, float w) : LightSource(1) {
	light->position[0] = x;
	light->position[1] = y;
	light->position[2] = z;
	light->position[3] = w;
}

void DirectionalLight::createMesh() {
	float l_diff[] = { 1.0f, 1.0f, 0.9f, 1.0f }; //luz amarela 
	float l_spec[] = { 1.0f, 1.0f, 0.8f, 1.0f };
	memcpy(light->diffuse, l_diff, 4 * sizeof(float));
	memcpy(light->specular, l_spec, 4 * sizeof(float));

	float amb[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // luz ambiente do sol
	float diff[] = { 0.9f, 0.9f, 0.5f, 1.0f }; //luz amarela 
	float spec[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float emissive[] = { 0.9f, 0.9f, 0.5f, 1.0f };
	float shininess = 120.0f;
	int texcount = 0;

	objId = 0;
	memcpy(mesh[objId].mat.ambient, amb, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	mesh[objId].position[0] = -100.0f;
	mesh[objId].position[1] = 100.0f;
	mesh[objId].position[2] = 0.0f;
	createSphere(3.0f, 20, mesh, objId);
}

void DirectionalLight::draw(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &lPos_uniformId) {

	GLuint loc;
	float res[4];
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

	for (int i = 0; i < meshLength; ++i) {
		// send the material
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, mesh[i].mat.ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[i].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, mesh[i].mat.specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[i].mat.shininess);
		pushMatrix(MODEL);
		translate(MODEL, mesh[i].position[0], mesh[i].position[1], mesh[i].position[2]);

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
	// create geometry and VAO of the pawn
}
