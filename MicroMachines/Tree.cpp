#include "Tree.h"

Tree::Tree() : Entity(1) {
	glGenTextures(1, TextureArray);
	TGA_Texture(TextureArray, "tree.tga", 0);
}

Tree::Tree(float x, float y, float z) : Entity(1) {
	glGenTextures(1, TextureArray);
	TGA_Texture(TextureArray, "tree.tga", 0);
	current_position[0] = x;
	current_position[1] = y;
	current_position[2] = z;
}

void Tree::createMesh() {

	float spec[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	float shininess = 100.0f;
	int texcount = 1;

	// create geometry and VAO of the Tree
	objId = 0;
	memcpy(mesh[objId].mat.specular, spec, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	mesh[objId].texUnits[0] = TextureArray[0];
	mesh[objId].vaoElements = 1;

	createQuad(6, 6, mesh, objId);
}

void Tree::render(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &texMode_uniformId){}

void Tree::render2(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &texMode_uniformId, float camPosX, float camPosY, float camPosZ,int type) {
	
	GLuint loc;

	float modelview[16];  //To be used in "Cheating" Matrix reset Billboard technique

	for (int i = 0; i < meshLength; ++i) {

		if (mesh[i].mat.texCount != 0) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mesh[i].texUnits[0]);
			glUniform1i(texMode_uniformId, 2);
		}
		for (int j = 0; j < mesh[i].vaoElements; j++) {

			pushMatrix(MODEL);
			translate(MODEL, current_position[0], current_position[1], current_position[2]);

			float pos[3] = { current_position[0], current_position[1], current_position[2] };
			float cam[3] = { camPosX, camPosY, camPosZ};

			if (type == 2)
				l3dBillboardSphericalBegin(cam, pos);
			else if (type == 3)
				l3dBillboardCylindricalBegin(cam, pos);

			// send the material
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
			glUniform4fv(loc, 1, mesh[i].mat.specular);
			loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
			glUniform1f(loc, mesh[i].mat.shininess);

			pushMatrix(MODEL);

			// send matrices to OGL
			if (type == 0 || type == 1)	{     //Cheating matrix reset billboard techniques
				computeDerivedMatrix(VIEW_MODEL);
				memcpy(modelview, mCompMatrix[VIEW_MODEL], sizeof(float) * 16);  //save VIEW_MODEL in modelview matrix

				//reset VIEW_MODEL
				if (type == 0) BillboardCheatSphericalBegin();
				else BillboardCheatCylindricalBegin();

				computeDerivedMatrix_PVM(); // calculate PROJ_VIEW_MODEL
			}
			else computeDerivedMatrix(PROJ_VIEW_MODEL);

			// send matrices to OGL
			glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
			glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
			computeNormalMatrix3x3();
			glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

			// Render mesh
			glBindVertexArray(mesh[i].vao);
			glDrawElements(mesh[i].type, mesh[i].numIndexes, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			popMatrix(MODEL);

			if (type==0 || type==1) 
				BillboardEnd(modelview);  

			popMatrix(MODEL);
		}
		glUniform1i(texMode_uniformId, 0);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Tree::increaseRotation(float dx, float dy, float dz) {}

void Tree::increasePosition(float dx, float dy, float dz) {}

void Tree::move(int delta_t){}
