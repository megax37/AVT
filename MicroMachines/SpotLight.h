#ifndef __SpotLight__
#define __SpotLight__

#include "LightSource.h"

class SpotLight : public LightSource {
public:
	SpotLight();
	SpotLight(float x, float y, float z, float w, int lightID);
	~SpotLight();
	void createMesh();
	void draw(VSShaderLib &shader);
	void setPos(float x, float y, float z, float dxx, float dzz);
	void setDir(float dirX, float dirY, float dirZ);

};
#endif