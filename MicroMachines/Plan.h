int verticeCountPlan = 4;
int faceCountPlan = 2;

float verticesPlan[] = {
	-100.0f, 0.0f, -100.0f, 1.0f,
	-100.0f, 0.0f, 100.0f, 1.0f,
	100.0f, 0.0f, 100.0f, 1.0f,
	100.0f, 0.0f, -100.0f, 1.0f
};

float normalsPlan[] = {
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f
};

float texCoordsPlan[] = {
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f
};

unsigned int faceIndexPlan[] = {
	0, 1, 2, 0, 2, 3
};