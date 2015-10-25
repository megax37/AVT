#include <stdio.h>
#include <stdlib.h>

#include "DirectionalLight.h"
#define PI 3.14159265
/// The storage for matrices
extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];

DirectionalLight::DirectionalLight(float x, float y, float z, float alpha) : LightSource(1) {
	position[0] = x;
	position[1] = y;
	position[2] = z;
	position[3] = alpha;
}

void DirectionalLight::createMesh() {
	float amb[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // luz ambiente do sol
	float diff[] = { 0.9f, 0.9f, 0.5f, 1.0f }; //luz amarela 
	float spec[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float emissive[] = { 0.9f, 0.9f, 0.5f, 1.0f };
	float shininess = 120.0f;
	int texcount = 0;

	objId = 0;
	memcpy(light->ambient, amb, 4 * sizeof(float));
	memcpy(light->diffuse, diff, 4 * sizeof(float));
	memcpy(light->specular, spec, 4 * sizeof(float));
	memcpy(light->emissive, emissive, 4 * sizeof(float));
	light->shininess = shininess;
	light->texCount = texcount;
	light->position[0] = position[0];
	light->position[1] = position[1] - 7;
	light->position[2] = position[2];
	light->position[3] = position[3];
	mesh[objId].position[0] = position[0];
	mesh[objId].position[1] = position[1];
	mesh[objId].position[2] = position[2];
	createSphere(3.0f, 20, mesh, objId);
}
void DirectionalLight::draw(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &lPos_uniformId) {

	GLuint loc;
	float res[4];
	multMatrixPoint(VIEW, light->position, res);   //lightPos definido em World Coord so is converted to eye space
	glUniform4fv(lPos_uniformId, 1, res);
	for (int i = 0; i < meshLength; ++i) {
		loc = glGetUniformLocation(shader.getProgramIndex(), "light.ambient");
		glUniform4fv(loc, 1, light->ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "light.diffuse");
		glUniform4fv(loc, 1, light->diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "light.specular");
		glUniform4fv(loc, 1, light->specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "light.shininess");
		glUniform1f(loc, light->shininess);

		// send the material
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
		glUniform4fv(loc, 1, light->ambient);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, light->diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
		glUniform4fv(loc, 1, light->specular);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, light->shininess);
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
