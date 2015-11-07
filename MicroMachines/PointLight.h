#ifndef __PointLight__
#define __PointLight__

#include "LightSource.h"

class PointLight : public LightSource {

public:
	PointLight(float x, float y, float z, float w, int lightID);
	~PointLight();
	void createMesh();
	void draw(VSShaderLib &shader);

};
#endif