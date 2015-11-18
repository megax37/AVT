#include "Entity.h"
#include "flare.h"

Flare::Flare() : Entity() {
	glGenTextures(4, TextureArray);
	TGA_Texture(TextureArray, "Flare1.tga", 0);
	TGA_Texture(TextureArray, "Flare2.tga", 1);
	TGA_Texture(TextureArray, "Flare3.tga", 2);
	TGA_Texture(TextureArray, "Flare4.tga", 3);
	FLARE_randomize(4, 8, FLARE_MAXSIZE, FLARE_MINCOLOUR, FLARE_MAXCOLOUR);
}

Flare::Flare(int SCREENwidth, int SCREENheight) : Entity(8) {
	screenWidth = SCREENwidth;
	screenHeight = SCREENheight;
	glGenTextures(4, TextureArray);
	TGA_Texture(TextureArray, "Flare1.tga", 0);
	TGA_Texture(TextureArray, "Flare2.tga", 1);
	TGA_Texture(TextureArray, "Flare3.tga", 2);
	TGA_Texture(TextureArray, "Flare4.tga", 3);
	FLARE_randomize(4, 8, FLARE_MAXSIZE, FLARE_MINCOLOUR, FLARE_MAXCOLOUR);
}

void Flare::createMesh() {}

void Flare::render(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &texMode_uniformId)
{	
	int		cx = screenWidth / 2;
	int		cy = screenHeight / 2;
	int		lx = xFlare;
	int		ly = yFlare;
	int     dx, dy;          // Screen coordinates of "destination"
	int     px, py;          // Screen coordinates of flare element
	int     maxflaredist, flaredist, flaremaxsize, flarescale;
	int     width, height;    // Piece parameters;
	float alpha;
	int     i;
	FLARE_ELEMENT_DEF    *element;
	GLuint loc;

	// Compute how far off-center the flare source is.
	maxflaredist = isqrt(cx*cx + cy*cy);
	flaredist = isqrt((lx - cx)*(lx - cx) +	(ly - cy)*(ly - cy));
	flaredist = (maxflaredist - flaredist);
	flaremaxsize = (int)(screenWidth * flare.fMaxSize);
	flarescale = (int)(screenWidth * flare.fScale);

	// Destination is opposite side of centre from source
	dx = cx + (cx - lx);
	dy = cy + (cy - ly);

	// Render each element.
	for (i = 0; i < flare.nPieces; ++i)
	{
		element = &flare.element[i];

		// Position is interpolated along line between start and destination.
		px = (int)((1.0f - element->fDistance)*lx + element->fDistance*dx);
		py = (int)((1.0f - element->fDistance)*ly + element->fDistance*dy);

		// Piece size are 0 to 1; flare size is proportion of
		// screen width; scale by flaredist/maxflaredist.
		width = (int)((flaredist*flarescale*element->fSize) / maxflaredist);
		//printf(" width %d \n", width);

		// Width gets clamped, to allows the off-axis flares
		// to keep a good size without letting the elements get
		// too big when centered.
		if (width > flaremaxsize)
		{
			width = flaremaxsize;
		}

		// Flare elements are square (round) so height is just
		// width scaled by aspect ratio.
		height = heightFromWidth(width);
		alpha = (1.0f * flaredist) / maxflaredist;

		if (width > 1) {
			float element_color[4];

			element_color[0] = 0.9f;
			element_color[1] = 0.9f;
			element_color[2] = 0.5f;
			element_color[3] = alpha;
			memcpy(mesh[i].mat.diffuse, element_color, 4 * sizeof(float));

			float shininess = 120.0f;
			int texcount = 1;
			mesh[i].mat.shininess = shininess;
			mesh[i].mat.texCount = texcount;
			mesh[i].texUnits[0] = element->texture[0];
			createFlareQuad(px - width / 2, py - height / 2, width, height, mesh, i);
		}
	}

	//Draw Quads
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	for (int i = 0; i < meshLength; ++i) {
		if (mesh[i].mat.texCount != 0) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mesh[i].texUnits[0]);
			glUniform1i(texMode_uniformId, 3);
		}

		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
		glUniform4fv(loc, 1, mesh[i].mat.diffuse);
		loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
		glUniform1f(loc, mesh[i].mat.shininess);

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

		glUniform1i(texMode_uniformId, 0);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

void Flare::FLARE_randomize(int nTextures,
	int nPieces,
	float fMaxSize,
	unsigned int minColour,
	unsigned int maxColour)
{
	std::cout << "vim aqui" << std::endl;
	int             i;
	float           fFracDist;
	float           fEnvelopeSize;
	FLARE_ELEMENT_DEF *element;

	// Initialize flare with requested number of elements, with
	// random placement, colour, and size of elements.
	flare.nPieces = nPieces;
	flare.fScale = fMaxSize;
	flare.fMaxSize = fMaxSize;
	fFracDist = 1.0f / (float)(flare.nPieces - 1);
	for (i = 0; i < flare.nPieces; ++i)
	{
		element = &flare.element[i];
		float k = FLARE_FRANGE(0, fFracDist);
		element->fDistance = (fFracDist*i) + k;

		// Envelope size is maximum at ends of line, minimum in the middle (i.e. two
		// cones, touching at the tips).
		float q = (float)fabs(1.0f - 2 * element->fDistance);
		fEnvelopeSize = q;

		element->fSize = FLARE_FRANGE(0.6f, 1.0f) * fEnvelopeSize;
		element->argb = FLARE_RANGE(minColour & 0xff000000, maxColour & 0xff000000) |
			FLARE_RANGE(minColour & 0x00ff0000, maxColour & 0x00ff0000) |
			FLARE_RANGE(minColour & 0x0000ff00, maxColour & 0x0000ff00) |
			FLARE_RANGE(minColour & 0x000000ff, maxColour & 0x000000ff);
		element->texture[0] = TextureArray[FLARE_RANGE(0, nTextures - 1)];
	}
}
