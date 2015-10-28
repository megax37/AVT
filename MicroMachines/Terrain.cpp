#include <stdio.h>
#include <stdlib.h>

#include "Terrain.h"

/// The storage for matrices
extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];

Terrain::Terrain() : Entity(3) {
	glGenTextures(2, TextureArray);
	TGA_Texture(TextureArray, "lightwood.tga", 0);
	TGA_Texture(TextureArray, "stone.tga", 1);
}

void Terrain::createMesh() {

	// create geometry and VAO of the table
	float amb[] = { 0.4f, 0.270f, 0.075f, 1.0f };
	float diff[] = { 0.4f, 0.270f, 0.075f, 1.0f };
	float spec[] = { 0.4f, 0.270f, 0.075f, 1.0f };
	float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 100.0f;
	int texcount = 2;

	objId = 0;
	memcpy(mesh[objId].mat.ambient, amb, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	mesh[objId].texUnits[0] = TextureArray[0];
	mesh[objId].texUnits[1] = TextureArray[1];
	mesh[objId].position[0] = -100.0f;
	mesh[objId].position[1] = -5.0f;
	mesh[objId].position[2] = -100.0f;
	mesh[objId].vaoElements = 1;
	createCube(mesh, objId);

	objId = 1;
	memcpy(mesh[objId].mat.ambient, amb, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	mesh[objId].texUnits[0] = TextureArray[0];
	mesh[objId].texUnits[1] = TextureArray[1];
	mesh[objId].position[0] = 0.0f;
	mesh[objId].position[1] = -55.0f;
	mesh[objId].position[2] = 0.0f;
	mesh[objId].vaoElements = 4;
	createCube(mesh, objId);

	float amb1[] = { 0.4f, 0.35f, 0.075f, 1.0f };
	float diff1[] = { 0.5f, 0.4f, 0.075f, 1.0f };
	float spec1[] = { 0.4f, 0.35f, 0.075f, 1.0f };
	int texcount1 = 1;

	objId = 2;
	memcpy(mesh[objId].mat.ambient, amb1, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff1, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec1, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount1;
	mesh[objId].texUnits[0] = TextureArray[1];
	mesh[objId].position[0] = -150.0f;
	mesh[objId].position[1] = -56.0f;
	mesh[objId].position[2] = -150.0f;
	mesh[objId].vaoElements = 1;
	createCube(mesh, objId);
}

void Terrain::render(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &lPos_uniformId) {

	GLuint loc;

	for (int i = 0; i < meshLength; ++i) {

		if (mesh[i].mat.texCount != 0) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mesh[i].texUnits[0]);
			loc = glGetUniformLocation(shader.getProgramIndex(), "texMode");
			if (mesh[i].mat.texCount == 2) {
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, mesh[i].texUnits[1]);
				glUniform1i(loc, 3);
			}
			else {
				glUniform1i(loc, 2);
			}
		}
		for (int j = 0; j < mesh[i].vaoElements; j++) {
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

			if (i == 0) {
				scale(MODEL, 200.0f, 5.0f, 200.0f);
			}
			else if (i == 1) {	

				if (j == 0) {
					translate(MODEL, 80.0f, 0, -90.0f);
				}
				else if (j == 1) {
					translate(MODEL, -90.0f, 0, -90.0f);
				}
				else if (j == 2) {
					translate(MODEL, 80.0f, 0, 80.0f);
				}
				else if (j == 3) {
					translate(MODEL, -90.0f, 0, 80.0f);
				}
				scale(MODEL, 10.0f, 50.0f, 10.0f);
			}
			else if (i == 2) {
				scale(MODEL, 300.0f, 1.0f, 300.0f);
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
		loc = glGetUniformLocation(shader.getProgramIndex(), "texMode");
		glUniform1i(loc, 0);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Terrain::increasePosition(float dx, float dy, float dz) {}

void Terrain::increaseRotation(float dx, float dy, float dz) {}

void Terrain::move() {}