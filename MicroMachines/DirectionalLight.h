#ifndef __DirectionLight__
#define __DirectionLight__

#include "LightSource.h"

class DirectionalLight : public LightSource {

public:
	DirectionalLight(float x, float y, float z, float w, int lightID);
	~DirectionalLight();
	void createMesh();
	void draw(VSShaderLib &shader);

};
#endif