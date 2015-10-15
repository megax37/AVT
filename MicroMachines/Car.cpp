#include <stdio.h>
#include <stdlib.h>

#include "Car.h"
#define PI 3.14159265
/// The storage for matrices
extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];

Car::Car() : Entity(2) {}

void Car::createMesh() {

	float amb[] = { 0.2f, 0.15f, 0.1f, 1.0f };
	float diff[] = { 1.0f, 0.1f, 0.1f, 1.0f };
	float spec[] = { 1.0f, 0.5f, 0.5f, 1.0f };
	float emissive[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 120.0f;
	int texcount = 0;

	// create geometry and VAO of the pawn
	objId = 0;
	memcpy(mesh[objId].mat.ambient, amb, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	mesh[objId].position[0] = 0.0f;
	mesh[objId].position[1] = 0.8f;
	mesh[objId].position[2] = 0.0f;
	mesh[objId].isRotate = false;
	mesh[objId].vaoElements = 1;
	createCube(mesh, objId);

	// create geometry and VAO of the cylinder
	objId = 1;
	memcpy(mesh[objId].mat.ambient, amb, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	mesh[objId].position[0] = 0.0f;
	mesh[objId].position[1] = 0.8f;
	mesh[objId].position[2] = 0.0f;
	mesh[objId].vaoElements = 2;
	mesh[objId].isRotate = true;
	mesh[objId].rotation[0] = 90.0f;
	mesh[objId].rotation[1] = 0.0f;
	mesh[objId].rotation[2] = 0.0f;
	mesh[objId].rotation[3] = 1.0f;
	createTorus(0.20f, 0.80f, 20, 20, mesh, objId);
}

void Car::render(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &lPos_uniformId) {

	GLuint loc;

	for (int i = 0; i < meshLength; ++i) {
		for (size_t j = 0; j < mesh[i].vaoElements; j++)
		{
			for (size_t k = 0; k < mesh[i].vaoElements; k++) {
				loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
				glUniform4fv(loc, 1, mesh[i].mat.ambient);
				loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
				glUniform4fv(loc, 1, mesh[i].mat.diffuse);
				loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
				glUniform4fv(loc, 1, mesh[i].mat.specular);
				loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
				glUniform1f(loc, mesh[i].mat.shininess);
				pushMatrix(MODEL);
				if (!mesh[i].isRotate) {
					translate(MODEL, (mesh[i].position[0] + current_position[0]), (mesh[i].position[1] + current_position[1]), (mesh[i].position[2] + current_position[2]));
					rotate(MODEL, current_rotation[1], 0.0f, 1.0f, 0.0f);
					scale(MODEL, 2.0, 1.0f, 3.0f);
					//translate(MODEL, -1.0, -1.3, -1.5); 
					//rotate(MODEL, current_rotation[1], 0.0f, 1.0f, 0.0f);
				}
				else {
					if (i == 1 && j == 0 && k == 0) { // primeira roda
						translate(MODEL, (mesh[i].position[0] + current_position[0]) + k*2.0f, mesh[i].position[1] + current_position[1], (mesh[i].position[2] + current_position[2] + j*3.0f));
						rotate(MODEL, mesh[i].rotation[0], mesh[i].rotation[1], mesh[i].rotation[2], mesh[i].rotation[3]);
						rotate(MODEL, current_rotation[1], 1.0f, 0.0f, 0.0f);
					}

					if (i == 1 && j == 0 && k == 1) { //segunda roda
						float dz = (float)(-2.0f * sin(current_rotation[1] * (PI / 180.f)));
						float dy = (float)(2.0f * cos(current_rotation[1] * (PI / 180.f)));
						translate(MODEL, (mesh[i].position[0] + current_position[0]) + k*2.0f, mesh[i].position[1] + current_position[1], (mesh[i].position[2] + current_position[2] + j*3.0f));
						rotate(MODEL, -mesh[i].rotation[0], mesh[i].rotation[1], mesh[i].rotation[2], mesh[i].rotation[3]);
						translate(MODEL, 0, dy - 2.0f, dz);
						rotate(MODEL, -current_rotation[1], 1.0f, 0.0f, 0.0f);
					}

					if (i == 1 && j == 1 && k == 0) { //terceira roda
						float dz = (float)(3.0f * cos(current_rotation[1] * (PI / 180.f)));
						float dy = (float)(-3.0f * sin(current_rotation[1] * (PI / 180.f)));
						translate(MODEL, (mesh[i].position[0] + current_position[0]) + k*2.0f, mesh[i].position[1] + current_position[1], (mesh[i].position[2] + current_position[2] + j*3.0f));
						rotate(MODEL, mesh[i].rotation[0], mesh[i].rotation[1], mesh[i].rotation[2], mesh[i].rotation[3]);
						translate(MODEL, 0, dy, dz - 3.0f);
						rotate(MODEL, current_rotation[1], 1.0f, 0.0f, 0.0f);
					}


					if (i == 1 && j == 1 && k == 1) { //quarta roda
						float dz = (float)(((3.0f * cos(current_rotation[1] * (PI / 180.f))) - (2.0f * sin(current_rotation[1] * (PI / 180.f)))));
						float dy = (float)(((3.0f * sin(current_rotation[1] * (PI / 180.f))) + (2.0f * cos(current_rotation[1] * (PI / 180.f)))));
						translate(MODEL, (mesh[i].position[0] + current_position[0]) + k*2.0f, mesh[i].position[1] + current_position[1], (mesh[i].position[2] + current_position[2] + j*3.0f));
						rotate(MODEL, -mesh[i].rotation[0], mesh[i].rotation[1], mesh[i].rotation[2], mesh[i].rotation[3]);
						translate(MODEL, 0, dy - 2.0f, dz - 3.0f);
						rotate(MODEL, -current_rotation[1], 1.0f, 0.0f, 0.0f);
					}
				}

				//rotate(MODEL, current_rotation[1], 0.0f, 1.0f, 0.0f);
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
	}
}

void Car::increasePosition(float dx, float dy, float dz) {
	current_position[0] += dx;
	current_position[1] += dy;
	current_position[2] += dz;
}

void Car::increaseRotation(float dx, float dy, float dz) {
	current_rotation[0] += dx;
	current_rotation[1] += dy;
	current_rotation[2] += dz;
}

void Car::move(){
	increaseRotation(0, currentTurn_speed *((1000.0f / 60.0f) / 1000.0f), 0);
	current_speed += current_Aceleration * ((1000.0f / 60.0f) / 1000.0f);
	if (current_speed > max_velocity) {
		current_speed = max_velocity;
	}
	else if (current_speed < -max_velocity) {
		current_speed = -max_velocity;
	}
	float distance = current_speed * ((1000.0f / 60.0f) / 1000.0f);
	float dx = (float)(distance * sin(Car::current_rotation[1] * (PI / 180.f)));
	float dz = (float)(distance * cos(Car::current_rotation[1] * (PI / 180.f)));
	increasePosition(dx, 0, dz);

}