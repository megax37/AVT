#include "Road.h"

Road::Road() : Entity(2) {
	glGenTextures(1, TextureArray);
	TGA_Texture(TextureArray, "road.tga", 0);
}

void Road::createMesh() {

	float amb[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float diff[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float spec[] = { 0.0f, 0.0f, 0.0f, 1.0f };
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
	mesh[objId].vaoElements = 15;
	createCube(mesh, objId);

	float amb1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float diff1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float spec1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float emissive1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float shininess1 = 100.0f;
	int texcount1 = 1;

	objId = 1;
	memcpy(mesh[objId].mat.ambient, amb1, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff1, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec1, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive1, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess1;
	mesh[objId].mat.texCount = 0;
	//mesh[objId].texUnits[0] = TextureArray[1];
	mesh[objId].position[0] = 0.0f;
	mesh[objId].position[1] = 0.0f;
	mesh[objId].position[2] = 0.0f;
	mesh[objId].vaoElements = 1;
	createCube(mesh, objId);
}

void Road::render(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &texMode_uniformId) {

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

			translate(MODEL, mesh[i].position[0], mesh[i].position[1], mesh[i].position[2]);
			if (i == 0){
				if (j == 0){
					translate(MODEL, 0.0f, 0.0f, 0.0f);
					rotate(MODEL, 90.0f, 0.0f, 1.0f, 0.0f);
				}
				if (j == 1){
					translate(MODEL, 8.0f, 0.01f, 46.0f);
					rotate(MODEL, 110.0f, 0.0f, 1.0f, 0.0f);
				}
				if (j == 2){
					translate(MODEL, 36.0f, 0.0f, 73.0f);
					rotate(MODEL, -20.0f, 0.0f, 1.0f, 0.0f);

				}if (j == 3){
					translate(MODEL, 70.0f, 0.01f, 65.0f);
					rotate(MODEL, 45.0f, 0.0f, 1.0f, 0.0f);

				}if (j == 4){
					translate(MODEL, 85.0f, 0.0f, 26.0f);
					rotate(MODEL, 90.0f, 0.0f, 1.0f, 0.0f);
				}
				if (j == 5){
					translate(MODEL, 85.0f, 0.0f, -24.0f);
					rotate(MODEL, 90.0f, 0.0f, 1.0f, 0.0f);
				}
				if (j == 6){
					translate(MODEL, 70.0f, 0.01f, -60.0f);
					rotate(MODEL, -45.0f, 0.0f, 1.0f, 0.0f);
				}
				if (j == 7){
					translate(MODEL, 32.0f, 0.0f, -76.0f);
				}
				if (j == 8){
					translate(MODEL, -18.0f, 0.01f, -76.0f);
				}
				if (j == 9){
					translate(MODEL, -55.0f, 0.0f, -60.0f);
					rotate(MODEL, 45.0f, 0.0f, 1.0f, 0.0f);
				}
				if (j == 10){
					translate(MODEL, -70.0f, 0.01f, -23.0f);
					rotate(MODEL, 90.0f, 0.0f, 1.0f, 0.0f);
				}
				if (j == 11){
					translate(MODEL, -70.0f, 0.0f, 27.0f);
					rotate(MODEL, 90.0f, 0.0f, 1.0f, 0.0f);
				}
				if (j == 12){
					translate(MODEL, -53.0f, 0.01f, 45.0f);
					rotate(MODEL, 35.0f, 0.0f, 1.0f, 0.0f);
				}
				if (j == 13){
					translate(MODEL, -35.0f, 0.0f, 12.0f);
					rotate(MODEL, 90.0, 0.0f, 1.0f, 0.0f);
				}
				if (j == 14){
						translate(MODEL, -17.0f, 0.01f, -18.0f);
				}

				scale(MODEL, 50.0f, 0.2f, 15.0f);
			}

			if (i == 1){
				if (j == 0){
					translate(MODEL, 0.0f, 0.01f, 10.0f);
					rotate(MODEL, 90.0f, 0.0f, 1.0f, 0.0f);
					scale(MODEL, 2.0f, 0.2f, 15.0f);
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
		glUniform1i(texMode_uniformId, 0);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Road::increasePosition(float dx, float dy, float dz) {}

void Road::increaseRotation(float dx, float dy, float dz) {}

void Road::move(int delta_t) {}
