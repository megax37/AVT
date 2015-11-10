#include <stdio.h>
#include <stdlib.h>

#include "ObjLoader.h"

ObjLoader::ObjLoader() {
	objMesh = (ObjMesh*)malloc(sizeof(*objMesh));
}

void ObjLoader::loadObjModel(std::string fileName) {
	std::ifstream myfile(fileName);
	std::vector<Vector4*> vertices;
	std::vector<int> tmp_vertices;
	long int verticesIndice = 0;
	std::vector<Vector4*> textVertices;
	std::vector<int> tmp_textVertices;
	long int textVerticesIndice = 0;
	std::vector<Vector4*> normals;
	std::vector<int> tmp_normals;
	long int normalsIndice = 0;
	std::vector<int> indices;
	std::vector<int> temp_indices;
	long int indicesIndice = 0;
	float *verticesArray = NULL;
	float *textVerticesArray = NULL;
	float *normalArray = NULL;
	unsigned int *indicesArray = NULL;
	int currentNumber = 0;
	int firstf = 0;
	std::map<int, int> verticeNumber;
	std::map<std::string, int> facesProcessed;

	std::string line;
	std::string verticePrefix = "v";
	std::string textVerticePrefix = "vt";
	std::string normalPrefix = "vn";
	std::string facePrefix = "f";

	if (myfile.is_open())
	{
		while (std::getline(myfile, line))
		{
			std::istringstream buf(line);
			std::istream_iterator<std::string> beg(buf), end;
			std::vector<std::string> tokens(beg, end); // done
			//std::cout << " qual é o token " << tokens[0] << std::endl;
			if (tokens[0].compare(0, normalPrefix.size(), normalPrefix) == 0) {
				Vector4* normal = new Vector4(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]), 0.0f);
				normals.push_back(normal);
				normalsIndice += 1;
			}
			else if (tokens[0].compare(0, textVerticePrefix.size(), textVerticePrefix) == 0) {
				Vector4* textures = new Vector4(std::stof(tokens[1]), std::stof(tokens[2]), 0.0f, 1.0f);
				textVertices.push_back(textures);
				textVerticesIndice += 1;
			}
			else if (tokens[0].compare(0, verticePrefix.size(), verticePrefix) == 0) {
				//std::cout << " vamos ao debug " << std::endl;
				Vector4* vertice = new Vector4(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]), 1.0f);
				vertices.push_back(vertice);
				verticesIndice += 1;
			}
			else if (tokens[0].compare(0, facePrefix.size(), facePrefix) == 0) {
				for (size_t i = 1; i < 4; i++) {
					std::stringstream ss(tokens[i]);
					std::string faceTokens;
					std::string completeToken = ss.str();
					int k = 1; //indica em que array colocar
					int currentVertex = 0;
					std::map<std::string, int>::iterator it2 = facesProcessed.find(completeToken);
					if (it2 != facesProcessed.end()) {
						indices.push_back(it2->second);
					}
					else {
						while (std::getline(ss, faceTokens, '/'))
						{
							int currentProcess = std::stoi(faceTokens) - 1;
							if (k == 1) {
								currentVertex = currentProcess;
								facesProcessed.insert(std::pair<std::string, int>(completeToken, currentNumber));
								verticeNumber.insert(std::pair<int, int>(currentNumber, currentProcess));
								indices.push_back(currentNumber);
								tmp_vertices.push_back(currentNumber);
								indicesIndice += 1;
								++currentNumber;
							}
							else if (k == 2) {
								tmp_textVertices.push_back(currentProcess);
							}
							else if (k == 3) {
								tmp_normals.push_back(currentProcess);
							}
							k += 1;
						}
					}
				}
			}
			else {
				continue;
			}
		}
		myfile.close();
	}
	else std::cout << "Unable to open file";

	int vertexPoint = 0;
	verticesArray = new float[indicesIndice * 4];
	textVerticesArray = new float[indicesIndice * 4];
	normalArray = new float[indicesIndice * 4];
	indicesArray = new unsigned int[indices.size()];
	for (int i = 0; i < indicesIndice; i++) {
		vertexPoint = tmp_vertices[i];
		std::map<int, int>::iterator it = verticeNumber.find(vertexPoint);
		if (it != verticeNumber.end()) {
			verticesArray[vertexPoint * 4] = vertices[it->second]->x;
			verticesArray[vertexPoint * 4 + 1] = vertices[it->second]->y;
			verticesArray[vertexPoint * 4 + 2] = vertices[it->second]->z;
			verticesArray[vertexPoint * 4 + 3] = vertices[it->second]->w;
			textVerticesArray[vertexPoint * 4] = textVertices[tmp_textVertices[i]]->x;
			textVerticesArray[vertexPoint * 4 + 1] = 1 - textVertices[tmp_textVertices[i]]->y;
			textVerticesArray[vertexPoint * 4 + 2] = textVertices[tmp_textVertices[i]]->z;
			textVerticesArray[vertexPoint * 4 + 3] = textVertices[tmp_textVertices[i]]->w;
			normalArray[vertexPoint * 4] = normals[tmp_normals[i]]->x;
			normalArray[vertexPoint * 4 + 1] = normals[tmp_normals[i]]->y;
			normalArray[vertexPoint * 4 + 2] = normals[tmp_normals[i]]->z;
			normalArray[vertexPoint * 4 + 3] = normals[tmp_normals[i]]->w;
		}
	}

	for (int i = 0; i < indices.size(); i++) {
		indicesArray[i] = indices[i];
	}

	objMesh->verticesArray = verticesArray;
	objMesh->textVerticesArray = textVerticesArray;
	objMesh->normalArray = normalArray;
	objMesh->indicesArray = indicesArray;
	objMesh->verticesLength = indicesIndice * 4;
	objMesh->textVerticesLength = indicesIndice * 4;
	objMesh->normalLength = indicesIndice * 4;
	objMesh->indicesLength = (long int) indices.size();
}

void ObjLoader::printArrays(float* array, long int length) {
	for (int i = 0; i < length; i++) {
		std::cout << " nao apanhei nenhum valor e ja tenho o que preciso " << i << " valor " << array[i] << std::endl;
	}
}

void ObjLoader::printArrays(int* array, long int length) {
	for (int i = 0; i < length; i++) {
		std::cout << " nao apanhei nenhum valor e ja tenho o que preciso " << i << " valor " << array[i] << std::endl;
	}
}