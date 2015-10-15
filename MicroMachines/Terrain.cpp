#include <stdio.h>
#include <stdlib.h>

#include "Terrain.h"
#define PI 3.14159265
/// The storage for matrices
extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];

Terrain::Terrain() : Entity(5) {}

void Terrain::createMesh() {

	// create geometry and VAO of the table
	float amb[] = { 0.4f, 0.270f, 0.075f, 1.0f };
	float diff[] = { 0.4f, 0.270f, 0.075f, 1.0f };
	float spec[] = { 0.4f, 0.270f, 0.075f, 1.0f };
	float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 100.0f;
	int texcount = 0;

	objId = 0;
	memcpy(mesh[objId].mat.ambient, amb, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	mesh[objId].position[0] = 0.0f;
	mesh[objId].position[1] = 0.0f;
	mesh[objId].position[2] = 0.0f;
	createPlan(mesh, objId);

	// create geometry and VAO of the road
	float amb1[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float diff1[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float spec1[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float emissive1[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess1 = 100.0f;
	int texcount1 = 0;

	objId = 1;
	memcpy(mesh[objId].mat.ambient, amb1, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff1, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec1, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive1, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess1;
	mesh[objId].mat.texCount = texcount1;
	mesh[objId].position[0] = -100.0f;
	mesh[objId].position[1] = 0.02f;
	mesh[objId].position[2] = 0.0f;
	createCube(mesh, objId);

	// create geometry and VAO of the road line

	float amb2[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float diff2[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float spec2[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float emissive2[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess2 = 0.0f;
	int texcount2 = 0;

	objId = 2;
	memcpy(mesh[objId].mat.ambient, amb2, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff2, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec2, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive2, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess2;
	mesh[objId].mat.texCount = texcount2;
	mesh[objId].position[0] = -100.0f;
	mesh[objId].position[1] = 0.07f;
	mesh[objId].position[2] = 15.0f;
	createCube(mesh, objId);

	// create geometry and VAO of the road borders
	float amb3[] = { 0.377f, 0.377f, 0.377f, 1.0f };
	float diff3[] = { 0.377f, 0.377f, 0.377f, 1.0f };
	float spec3[] = { 0.377f, 0.377f, 0.377f, 1.0f };
	float emissive3[] = { 0.377f, 0.377f, 0.377f, 1.0f };
	float shininess3 = 0.0f;
	int texcount3 = 0;

	objId = 3;
	memcpy(mesh[objId].mat.ambient, amb3, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff3, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec3, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive3, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess3;
	mesh[objId].mat.texCount = texcount3;
	mesh[objId].position[0] = -100.0f;
	mesh[objId].position[1] = 0.03f;
	mesh[objId].position[2] = -12.0f;
	createCube(mesh, objId);

	// create geometry and VAO of the road borders 2

	objId = 4;
	memcpy(mesh[objId].mat.ambient, amb3, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff3, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec3, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive3, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess3;
	mesh[objId].mat.texCount = texcount3;
	mesh[objId].position[0] = -100.0f;
	mesh[objId].position[1] = 0.03f;
	mesh[objId].position[2] = 30.0f;
	createCube(mesh, objId);


}

void Terrain::render(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &lPos_uniformId) {

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

		translate(MODEL, mesh[i].position[0], mesh[i].position[1], mesh[i].position[2]);

		if (i == 1) {
			scale(MODEL, 200.0f, 0.0f, 30.0f);
		}

		if (i == 2) {
			scale(MODEL, 200.0f, 0.0f, 0.9f);
		}

		if (i == 3) {
			scale(MODEL, 200.0f, 0.0f, 12.0f);
		}
		if (i == 4) {
			scale(MODEL, 200.0f, 0.0f, 12.0f);
		}

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

void Terrain::increasePosition(float dx, float dy, float dz) {}

void Terrain::increaseRotation(float dx, float dy, float dz) {}

void Terrain::move() {}