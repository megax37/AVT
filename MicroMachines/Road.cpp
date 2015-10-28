#include <stdio.h>
#include <stdlib.h>

#include "Road.h"
#include "TGA.h"

/// The storage for matrices
extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];


Road::Road() : Entity(1) {
	glGenTextures(1, TextureArray);
	TGA_Texture(TextureArray, "road.tga", 0);
}

void Road::createMesh() {

	float amb[] = { 0.2f, 0.15f, 0.1f, 1.0f };
	float diff[] = { 0.8f, 0.6f, 0.4f, 1.0f };
	float spec[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 100.0f;
	int texcount = 1;

	objId = 0;
	memcpy(mesh[objId].mat.ambient, amb, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	mesh[objId].texUnits[0] = TextureArray[0];
	mesh[objId].position[0] = 0.0f;
	mesh[objId].position[1] = 0.0f;
	mesh[objId].position[2] = 0.0f;
	mesh[objId].vaoElements = 14;
	createCube(mesh, objId);

}

void Road::render(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &lPos_uniformId) {

	GLuint loc;

	for (int i = 0; i < meshLength; ++i) {

		if (mesh[i].mat.texCount != 0) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mesh[i].texUnits[0]);
			loc = glGetUniformLocation(shader.getProgramIndex(), "texMode");
			glUniform1i(loc, 2);
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
			if (i == 0){
				if (j == 0){
					translate(MODEL, 0.0f, 0.0f, -80.0f);
				}
				else if (j == 1) {
					translate(MODEL, 50.0f, 0.01f, -80.0f);
					rotate(MODEL, -45.0f, 0.0f, 1.0f,0.0f);
					
				}
				else if (j == 2) {
					translate(MODEL, 85.0f, 0.0f, -45.0f);
					rotate(MODEL, -100.0f, 0.0f, 1.0f, 0.0f);
				}
				else if (j == 3) {
					translate(MODEL, 76.0f, 0.01f, 0.0f);
					rotate(MODEL, -80.0f, 0.0f, 1.0f, 0.0f);
				}
				else if (j == 4) {
					translate(MODEL, 85.0f, 0.0f, 45.0f);
					rotate(MODEL, -100.0f, 0.0f, 1.0f, 0.0f);
				}
				else if (j == 5) {
					translate(MODEL, 28.0f, 0.01f, 85.0f);
				}
				else if (j == 6) {
					translate(MODEL, -20.0f, 0.0f, 85.0f);
				}
				else if (j == 7) {
					translate(MODEL, -31.0f, 0.01f, 49.0f);
					rotate(MODEL, -60.0f, 0.0f, 1.0f, 0.0f);
				}
				else if (j == 8) {
					translate(MODEL, -43.0f, 0.0f, 56.0f);
					rotate(MODEL, 45.0f, 0.0f, 1.0f, 0.0f);
				}
				else if (j == 9) {
					translate(MODEL, -12.0f, 0.01f, 32.0f);
					rotate(MODEL, 90.0f, 0.0f, 1.0f, 0.0f);
				}
				else if (j == 10) {
					translate(MODEL, -49.0f, 0.0f, -15.0f);
					rotate(MODEL, 20.0f, 0.0f, 1.0f, 0.0f);
				}
				else if (j == 11) {
					translate(MODEL, -45.0f, 0.01f, -2.0f);
					rotate(MODEL, 134.0f, 0.0f, 1.0f, 0.0f);
				}
				else if (j == 12) {
					translate(MODEL, -86.0f, 0.01f, -44.0f);
					rotate(MODEL, 45.0f, 0.0f, 1.0f, 0.0f);
				}
				else if (j == 13) {
					translate(MODEL, -50.0f, 0.0f, -80.0f);
				}
				scale(MODEL, 50.0f, 0.2f, 15.0f);
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

void Road::increasePosition(float dx, float dy, float dz) {}

void Road::increaseRotation(float dx, float dy, float dz) {}

void Road::move() {}
