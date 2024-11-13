#pragma once
#include <vector>
#include "texture.h"

typedef struct Model {
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	int numIndices;
	Texture texture;
} Model;

Model loadModel(const char * fileLocation);
Model loadModelWithTexture(const char * modelLocation, const char * textureLocation);

void loadPlyModel(const char * filePath, std::vector<float> &vertecies, std::vector<int> &indicies, int &vertAmount, int &indsAmount);
Model bufferModel(std::vector<float> vert, std::vector<int> inds);

void addResPath(char *dest, const char * filePath);
