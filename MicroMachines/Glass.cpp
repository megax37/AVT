#include "Glass.h"

Glass::Glass() : Entity(1) {
	//glGenTextures(1, TextureArray);
	//TGA_Texture(TextureArray, "butter.tga", 0);
	box = new Box(current_position[0], current_position[2], 2.8f);
}

Glass::Glass(float x, float y, float z) : Entity(1) {
	current_position[0] = x;
	current_position[1] = y;
	current_position[2] = z;
	//glGenTextures(1, TextureArray);
	//TGA_Texture(TextureArray, "butter.tga", 0);
	box = new Box(x, z, 2.8f);
}

void Glass::createMesh() {

	float amb[] = { 0.1f, 0.2f, 0.25f, 0.2f };
	float diff[] = { 0.5f, 0.7f, 0.7f, 0.3f };
	float spec[] = { 0.4f, 0.7f, 0.7f, 0.25f };
	float shininess = 100.0f;
	int texcount = 0;

	// create geometry and VAO of the glass
	objId = 0;
	memcpy(mesh[objId].mat.ambient, amb, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	//mesh[objId].texUnits[0] = TextureArray[0];
	mesh[objId].vaoElements = 1;
	createCylinder(10.0f, 2.8f, 20, mesh, objId);
}

void Glass::render(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &texMode_uniformId) {

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

void Glass::increaseRotation(float dx, float dy, float dz) {}

void Glass::increasePosition(float dx, float dy, float dz) {}

void Glass::move(int delta_t){}
