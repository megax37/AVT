#include "Entity.h"
#include "flare.h"

Flare::Flare() : Entity() {
	glGenTextures(4, TextureArray);
	TGA_Texture(TextureArray, "crcl.tga", 0);
	TGA_Texture(TextureArray, "hxgn.tga", 1);
	TGA_Texture(TextureArray, "ring.tga", 2);
	TGA_Texture(TextureArray, "flar.tga", 3);
	newFlare(0);
}

Flare::Flare(int SCREENwidth, int SCREENheight) : Entity(9) {
	screenWidth = SCREENwidth;
	screenHeight = SCREENheight;
	glGenTextures(4, TextureArray);
	TGA_Texture(TextureArray, "crcl.tga", 0);
	TGA_Texture(TextureArray, "hxgn.tga", 1);
	TGA_Texture(TextureArray, "ring.tga", 2);
	TGA_Texture(TextureArray, "flar.tga", 3);
	newFlare(0);
}

void Flare::createMesh() {

	
}
void Flare::render(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &texMode_uniformId)
{	
	int		cx = screenWidth / 2;
	int		cy = screenHeight / 2;
	int		lx = xFlare;
	int		ly = yFlare;
	int     dx, dy;          // Screen coordinates of "destination"
	int     px, py;          // Screen coordinates of flare element
	int     maxflaredist, flaredist, flaremaxsize, flarescale;
	int     width, height, alpha;    // Piece parameters;
	int     i;
	FLARE_ELEMENT_DEF    *element;
	GLuint loc;

	// Compute how far off-center the flare source is.
	maxflaredist = isqrt(cx*cx + cy*cy);
	//printf(" maxFlaredist %d \n", maxflaredist);
	flaredist = isqrt((lx - cx)*(lx - cx) +
		(ly - cy)*(ly - cy));
	//printf(" flaredist %d \n", flaredist);
	flaredist = (maxflaredist - flaredist);
	//printf(" flaredist1 %d \n", flaredist);
	flaremaxsize = (int)(screenWidth * flare.fMaxSize);
	//printf(" flaremaxsize %d \n", flaremaxsize);
	//printf(" SCREENwidth %d \n", screenWidth);
	flarescale = (int)(screenWidth * flare.fScale);
	//printf(" flarescale %d \n", flarescale);
	//printf(" SCREENwidth %d \n", screenWidth);

	// Destination is opposite side of centre from source
	dx = cx + (cx - lx);
	dy = cy + (cy - ly);

	// Render each element.
	for (i = 0; i < flare.nPieces; ++i)
	{
		element = &flare.element[i];

		// Position is interpolated along line between start and destination.
		//printf(" element->fDistance %f \n", element->fDistance);
		//printf(" dx %d \n", dx);
		px = (int)((1.0f - element->fDistance)*lx + element->fDistance*dx);
		//printf(" px %d \n", px);
		py = (int)((1.0f - element->fDistance)*ly + element->fDistance*dy);
		//printf(" py %d \n", py);

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
		alpha = (flaredist*(element->argb >> 24)) / maxflaredist;

		if (width > 1)
		{
			unsigned int    argb = (alpha << 24) | (element->argb & 0x00ffffff);

			float element_color[4];
			element_color[0] = (GLbyte)(argb >> 16 & 0xff);
			element_color[1] = (GLbyte)(argb >> 8 & 0xff);
			element_color[2] = (GLbyte)(argb >> 0 & 0xff);
			element_color[3] = (GLbyte)(argb >> 24 & 0xff);
			memcpy(mesh[i].mat.diffuse, element_color, 4 * sizeof(float));

			std::cout << "valores das cores a: " << element_color[0] << " da cor b " << element_color[1] << " da cor c " << element_color[2] << " e da cor d " << element_color
				[3] << std::endl;

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
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_ONE, GL_ONE);
	for (int i = 0; i < meshLength; ++i) {
		if (mesh[i].mat.texCount != 0) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, mesh[i].texUnits[0]);
			glUniform1i(texMode_uniformId, 2);
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
	glEnable(GL_LIGHTING);
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
	//printf("valor do fFracDist %f ", fFracDist);
	for (i = 0; i < flare.nPieces; ++i)
	{
		element = &flare.element[i];
		float k = FLARE_FRANGE(0, fFracDist);
		//printf("valor do float %f ", k);
		element->fDistance = (fFracDist*i) + k;

		// Envelope size is maximum at ends of line, minimum in the middle (i.e. two
		// cones, touching at the tips).
		float q = (float)fabs(1.0f - 2 * element->fDistance);
		//printf("valor do fEnvelopeSize %f \n", q);
		fEnvelopeSize = q;

		element->fSize = FLARE_FRANGE(0.6f, 1.0f) * fEnvelopeSize;
		element->argb = FLARE_RANGE(minColour & 0xff000000, maxColour & 0xff000000) |
			FLARE_RANGE(minColour & 0x00ff0000, maxColour & 0x00ff0000) |
			FLARE_RANGE(minColour & 0x0000ff00, maxColour & 0x0000ff00) |
			FLARE_RANGE(minColour & 0x000000ff, maxColour & 0x000000ff);
		//element->texture = TM_getNthTexture(FLARE_RANGE(0, nTextures - 1));
		element->texture[0] = TextureArray[FLARE_RANGE(0, nTextures - 1)];
		printf(" formato %d", FLARE_RANGE(0, nTextures - 1));
	}
}

void Flare::newFlare(int bFromFile)
{
	int randomIntRange = FLARE_RANGE(FLARE_MINELEMENTSPERFLARE, FLARE_MAXELEMENTSPERFLARE);
	printf(" randomIntRange %d \n", randomIntRange);
	FLARE_randomize(4, 9, FLARE_MAXSIZE, FLARE_MINCOLOUR, FLARE_MAXCOLOUR);
}