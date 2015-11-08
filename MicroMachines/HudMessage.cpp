#include "HudMessage.h"

HudMessage::HudMessage() : Entity(1) {
	glGenTextures(2, TextureArray);
	TGA_Texture(TextureArray, "pausegame.tga", 0);
	TGA_Texture(TextureArray, "gameover.tga", 1);
}

HudMessage::HudMessage(float x, float y, float z) : Entity(1) {
	current_position[0] = x;
	current_position[1] = y;
	current_position[2] = z;
	glGenTextures(2, TextureArray);
	TGA_Texture(TextureArray, "pausegame.tga", 0);
	TGA_Texture(TextureArray, "gameover.tga", 1);
}

void HudMessage::createMesh() {

	float amb[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	float diff[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float shininess = 80.0f;
	int texcount = 1;

	// create geometry and VAO of the pawn
	objId = 0;
	memcpy(mesh[objId].mat.ambient, amb, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	mesh[objId].texUnits[0] = TextureArray[0];
	mesh[objId].texUnits[1] = TextureArray[1];
	mesh[objId].position[0] = current_position[0];
	mesh[objId].position[1] = current_position[1];
	mesh[objId].position[2] = current_position[2];
	mesh[objId].vaoElements = 1;
	createCube(mesh, objId);

}

void HudMessage::render(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &texMode_uniformId) {

	GLuint loc;

	for (int i = 0; i < meshLength; ++i) {

		if (mesh[i].mat.texCount != 0) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mesh[i].texUnits[messageType]);
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
			if (i == 0) {
				scale(MODEL, 4.0f, 4.0f, 0.0f);
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
		glUniform1i(texMode_uniformId, 0);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}
