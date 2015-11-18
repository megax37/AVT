#ifndef __FLARE_H__
#define __FLARE_H__

#define isqrt(x)        (int)((double)(x))

#define FLARE_MAXELEMENTSPERFLARE         15
#define FLARE_MINCOLOUR        MAKEID(140, 100, 50, 100)
#define FLARE_MAXCOLOUR        MAKEID(255, 255, 200, 255)
#define FLARE_MINELEMENTSPERFLARE         8
#define FLARE_MAXSIZE                   0.3f

#define FLARE_RANGE(A,B)    ( (rand()%((B)-(A)+1)) + (A) )
#define FLARE_FRANGE(A,B)   ( ((float)(rand()&0xffffff)/(float)0xfffffe)*((B)-(A)) + (A) )

#define MAKEID(a,b,c,d)     (((a)<<24) | ((b)<<16) | ((c)<<8) | ((d)<<0))

	/* --- Types --- */

	typedef struct FLARE_ELEMENT_DEF
	{
		//struct TEXTURE_DEF  *texture;
		GLuint			texture[1];

		float           fDistance;        // Distance along ray from source (0.0-1.0)
		float           fSize;            // Size relative to flare envelope (0.0-1.0)
		unsigned int    argb;            // ARGB for intensity mapping
	}
	FLARE_ELEMENT_DEF;

	typedef struct FLARE_DEF
	{
		float           fScale;     // Scale factor for adjusting overall size of flare elements.
		float           fMaxSize;   // Max size of largest element, as proportion of screen width (0.0-1.0)
		int             nPieces;    // Number of elements in use


		FLARE_ELEMENT_DEF    element[FLARE_MAXELEMENTSPERFLARE];
	}
	FLARE_DEF;

	enum
	{
		TEX_CRCL,
		TEX_HXGN,
		TEX_RING,
		TEX_FLAR,

		NPIECETEXTURES,

		TEX_SUN = NPIECETEXTURES,
		TEX_BACK,

		NTEXTURES
	};

/* --- Prototypes --- */
class Flare : Entity {
private:
	int screenWidth;
	int screenHeight;
	GLuint TextureArray[4];
	FLARE_DEF flare;
	int     xFlare = 10;
	int     yFlare = 10;
public:
	Flare();
	Flare(int SCREENwidth, int SCREENheight);
	void    FLARE_randomize(int nTextures,
		int nPieces,
		float fMaxSize,
		unsigned int minColour,
		unsigned int maxColour);

	int heightFromWidth(int width) {
		return ((320 * width * screenHeight) / (240 * screenHeight));
	}

	int getXFlare() { return xFlare; }
	int getYFlare() { return yFlare; }

	void setXFlare(int newXFlare) {
		xFlare = newXFlare;
	}
	void setYFlare(int newYFlare) {
		yFlare = newYFlare;
	}
	int getScreenWidth() { return screenWidth; }
	int getScreenHeight() { return screenHeight; }

	void createMesh();
	void move(int delta_t) {};
	void increasePosition(float dx, float dy, float dz){};
	void increaseRotation(float dx, float dy, float dz){};
	void render(VSShaderLib &shader, GLint &pvm_uniformId, GLint &vm_uniformId, GLint &normal_uniformId, GLint &texMode_uniformId);
};
#endif
