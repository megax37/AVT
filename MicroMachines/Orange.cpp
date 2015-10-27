#include <stdio.h>
#include <stdlib.h>

#include "Orange.h"
#define PI 3.14159265
/// The storage for matrices
extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];

Orange::Orange() : Entity() {
	initial_velocity[0] = 0.2f + (rand() % 100) / 100;
	initial_velocity[1] = 0;
	initial_velocity[2] = 0.2f + (rand() % 100) / 100;
}

Orange::Orange(float x, float y, float z) : Entity(1) {
	{
		current_position[0] = x;
		current_position[1] = y;
		current_position[2] = z;
		initial_velocity[0] = 0.2f + (rand() % 101) / 100;
		initial_velocity[1] = 0;
		initial_velocity[2] = 0.2f + (rand() % 101) / 100;
	}
}

void Orange::createMesh() {

	float amb[] = { 0.2f, 0.15f, 0.1f, 1.0f };
	float diff[] = { 1.0f, 0.5f, 0.0f, 1.0f };
	float spec[] = { 1.0f, 0.5f, 0.0f, 1.0f };
	float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 100.0f;
	int texcount = 0;

	// create geometry and VAO of the pawn
	objId = 0;
	memcpy(mesh[objId].mat.ambient, amb, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	mesh[objId].position[0] = current_position[0];
	mesh[objId].position[1] = current_position[1];
	mesh[objId].position[2] = current_position[2];
	createSphere(3.0f, 20, mesh, objId);

}

void Orange::render(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &lPos_uniformId) {

	GLuint loc;

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
		translate(MODEL, current_position[0], current_position[1], current_position[2]);

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

void Orange::increasePosition(float dx, float dy, float dz) {
	current_position[0] += dx;
	current_position[1] += dy;
	current_position[2] += dz;
}

void Orange::increaseRotation(float dx, float dy, float dz) {
	current_rotation[0] += dx;
	current_rotation[1] += dy;
	current_rotation[2] += dz;
}

void Orange::move(){
	increaseRotation(0, 0, 160.0f *((1000.0f / 60.0f) / 1000.0f));
	//float distance = current_speed * ((1000.0f / 60.0f) / 1000.0f);
	increasePosition(0, 0, initial_velocity[2] + aceleration);

}