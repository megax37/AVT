#include <stdio.h>
#include <stdlib.h>

#include "Car.h"

Car::Car() : Entity(2) {
	box = new Box(current_position[0], current_position[2], 2.0);
}

Car::Car(SpotLight* spot, SpotLight* spot1) : Entity(2) {
	spotLight = spot;
	spotLight1 = spot1;
	box = new Box(current_position[0], current_position[2], 2.0);
}

void Car::createMesh() {

	float amb[] = { 0.4f, 0.1f, 0.1f, 1.0f };
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
	mesh[objId].vaoElements = 1;
	createCube(mesh, objId);

	float amb1[] = { 0.05f, 0.05f, 0.1f, 1.0f };
	float diff1[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float spec1[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float emissive1[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess1 = 100.0f;
	int texcount1 = 0;

	// create geometry and VAO of the cylinder
	objId = 1;
	memcpy(mesh[objId].mat.ambient, amb1, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff1, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec1, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive1, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess1;
	mesh[objId].mat.texCount = texcount1;
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

void Car::render(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &texMode_uniformId) {

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
				if (i == 0) {
					translate(MODEL, current_position[0], mesh[i].position[1] + current_position[1], current_position[2]);
					rotate(MODEL, current_rotation[1], 0.0f, 1.0f, 0.0f);
					scale(MODEL, 2.0f, 1.0f, 3.0f);
				}
				else if(i == 1) 
				{
					translate(MODEL, current_position[0], current_position[1], current_position[2]);
					rotate(MODEL, current_rotation[1], 0.0f, 1.0f, 0.0f);
					translate(MODEL, -1.2f + j*2.4f, mesh[i].position[1], -1.35f + k*2.7f);
					rotate(MODEL, 90.0f, 0.0f, 0.0f, 1.0f);
					if (k == 0) {
						scale(MODEL, 1.0f, 1.2f, 1.0f);
					}
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
	}
}

void Car::increasePosition(float dx, float dy, float dz) {
	previousPosition[0] = current_position[0];
	previousPosition[1] = current_position[1];
	previousPosition[2] = current_position[2];
	current_position[0] += dx;
	current_position[1] += dy;
	current_position[2] += dz;
	box->setX(current_position[0]);
	box->setZ(current_position[2]);

	// spotLight7
	float dxx = (float)(sin((current_rotation[1] - 15) * (PI / 180.f)));
	float dzz = (float)(cos((current_rotation[1] - 15) * (PI / 180.f)));
	spotLight->setPos(current_position[0], current_position[1] + 1.0f, current_position[2], -dxx, -dzz);
	spotLight->setDir(dxx, 0.0f, dzz);

	// spotlight8
	float dxx1 = (float)(sin((current_rotation[1] + 15) * (PI / 180.f)));
	float dzz1 = (float)(cos((current_rotation[1] + 15) * (PI / 180.f)));
	spotLight1->setPos(current_position[0], current_position[1] + 1.0f, current_position[2], -dxx1, -dzz1);
	spotLight1->setDir(dxx1, 0.0f, dzz1);
}

void Car::increaseRotation(float dx, float dy, float dz) {
	current_rotation[0] += dx;
	current_rotation[1] += dy;
	current_rotation[2] += dz;
}

void Car::move(int delta_t){
	increaseRotation(0, currentTurn_speed *(delta_t / 1000.0f), 0);
	current_speed += current_Aceleration * (delta_t / 1000.0f);
	if (current_speed > max_velocity) {
		current_speed = max_velocity;
	}
	else if (current_speed < -max_velocity) {
		current_speed = -max_velocity;
	}
	float distance = current_speed * (delta_t / 1000.0f);
	float dx = (float)(distance * sin(Car::current_rotation[1] * (PI / 180.f)));
	float dz = (float)(distance * cos(Car::current_rotation[1] * (PI / 180.f)));
	increasePosition(dx, 0, dz);

}