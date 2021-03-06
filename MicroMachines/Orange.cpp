#include "Orange.h"

Orange::Orange() : Entity(1) {
	initial_velocity[0] = 0.2f + (rand() % 100) / 100;
	initial_velocity[1] = 0;
	initial_velocity[2] = 0.2f + (rand() % 100) / 100;
	glGenTextures(1, TextureArray);
	TGA_Texture(TextureArray, "orange.tga", 0);
	box = new Box(current_position[0], current_position[2], 3.0);
}

Orange::Orange(float x, float y, float z) : Entity(1) {
	current_position[0] = x;
	current_position[1] = y;
	current_position[2] = z;
	initial_velocity[0] = 0.2f + (rand() % 101) / 100;
	initial_velocity[1] = 0;
	initial_velocity[2] = 0.2f + (rand() % 101) / 100;
	glGenTextures(1, TextureArray);
	TGA_Texture(TextureArray, "orange.tga", 0);
	box = new Box(current_position[0], current_position[2], 3.0);
}

void Orange::createMesh() {

	float amb[] = { 0.2f, 0.15f, 0.1f, 1.0f };
	float diff[] = { 1.0f, 0.5f, 0.0f, 1.0f };
	float spec[] = { 1.0f, 0.5f, 0.0f, 1.0f };
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
	mesh[objId].position[0] = current_position[0];
	mesh[objId].position[1] = current_position[1];
	mesh[objId].position[2] = current_position[2];
	mesh[objId].vaoElements = 1;
	createSphere(3.0f, 20, mesh, objId);

}

void Orange::render(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &texMode_uniformId) {

	GLuint loc;

	for (int i = 0; i < meshLength; ++i) {

		if (mesh[i].mat.texCount != 0) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mesh[i].texUnits[0]);
			glUniform1i(texMode_uniformId, 2);
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

			translate(MODEL, current_position[0], current_position[1], current_position[2]);
			rotate(MODEL, current_rotation[0], 1.0f, 0.0f, 0.0f);

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
		glUniform1i(texMode_uniformId, 0);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Orange::increasePosition(float dx, float dy, float dz) {
	previousPosition[0] = current_position[0];
	previousPosition[1] = current_position[1];
	previousPosition[2] = current_position[2];
	current_position[0] += dx;
	current_position[1] += dy;
	current_position[2] += dz;
	box->setX(current_position[0]);
	box->setZ(current_position[2]);
}

void Orange::increaseRotation(float dx, float dy, float dz) {
	current_rotation[0] += dx;
	current_rotation[1] += dy;
	current_rotation[2] += dz;
}

void Orange::setCurrentSpeed(float speed){
	current_speed = speed;
}

void Orange::move(int delta_t) {
	increaseRotation(160.0f *(delta_t / 1000.0f), 0, 0);
	current_speed += aceleration * delta_t;
	float distance = current_speed * delta_t;
	increasePosition(0, 0, distance);
	//increasePosition(0, 0, initial_velocity[2] + aceleration);
}
