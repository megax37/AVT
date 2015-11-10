#ifndef __basic_geometry__
#define __basic_geometry__

#define MAX_TEXTURES 4

struct Material{
	float diffuse[4];
	float ambient[4];
	float specular[4];
	float emissive[4];
	float shininess;
	int texCount;
};
// A model can be made of many meshes. Each is stored  in the following structure
struct MyMesh {
	GLuint vao;
	GLuint texUnits[MAX_TEXTURES];
	GLuint texTypes[MAX_TEXTURES];
	float transform[16];
	float position[3];
	float rotation[4];
	float scale[3];
	int vaoElements = 0;
	bool isScale = false;
	bool isRotate = false;
	int numIndexes;
	unsigned int type;
	struct Material mat;
};

void createCar(MyMesh *mesh, int objId);
void createCube(MyMesh *mesh, int objId);
void createPlan(MyMesh *mesh, int objId);
void createSphere(float radius, int divisions, MyMesh *mesh, int objId);
void createTorus(float innerRadius, float outerRadius, int rings, int sides, MyMesh *mesh, int objId);
void createCylinder(float height, float radius, int sides, MyMesh *mesh, int objId);
void createCone(float height, float baseRadius, int sides, MyMesh *mesh, int objId);
void createPawn(MyMesh *mesh, int objId);
void computeVAO(int numP, float *p, float *pfloatoints, int sides, float smoothCos, MyMesh *mesh, int objId);
void create(float *p, int numP, int sides, int closed, float smoothCos);
int revSmoothNormal2(float *p, float *nx, float *ny, float smoothCos, int beginEnd);
float *circularProfile(float minAngle, float maxAngle, float radius, int divisions, float transX = 0.0f, float transY = 0.0f);

#endif
