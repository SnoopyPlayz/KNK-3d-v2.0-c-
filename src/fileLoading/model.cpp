#include <GL/glew.h>
#include <stdint.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
//#include <meshoptimizer.h>
#include "texture.h"

// PLY MODEL LOADING			PLY MODEL LOADING

typedef struct Model {
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	int numIndices;
	Texture texture;
} Model;

typedef struct vertex{
	float x;
	float y;
	float z;
	float nx;
	float ny;
	float nz;
	float s;
	float t;
} vertex;

Model bufferModel(std::vector<vertex> vert, std::vector<int> inds){
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
 
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(vertex), vert.data(), GL_STATIC_DRAW);
 
        unsigned int EBO;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, inds.size() * sizeof(int), inds.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *) (sizeof(float) * 6));
	glEnableVertexAttribArray(2);


	return (Model) {VAO, VBO, EBO, (int)inds.size(), 0};
}

typedef struct Indecie{
	uint8_t num_indices;
	uint32_t* indices;
} Indecie;

void saveInds(std::vector<int> &inds, int indsPos, char readline[100]){
	int x,y,z,a = 0;

	sscanf(readline, "%*d %d %d %d %d", &x, &y, &z, &a);

	inds[indsPos] = x; //Triangle 1
	inds[indsPos+1] = y;
	inds[indsPos+2] = z;

	if (!a) // is there another triangle
		return;
	
	inds[indsPos+3] = x; //Triangle 2
	inds[indsPos+4] = z;
	inds[indsPos+5] = a;
}

void saveVerts(std::vector<vertex> &V, int vertPos, char readline[100]){
		sscanf(readline, "%f %f %f %f %f %f %f %f", 
				&V[vertPos].x, &V[vertPos].y, &V[vertPos].z,
				&V[vertPos].nx, &V[vertPos].ny, &V[vertPos].nz,
				&V[vertPos].s, &V[vertPos].t);
}

void loadPlyModelASCII(std::vector<vertex> &vertecies, std::vector<int> &indicies, int numVert, int numInds, FILE *modelFile){
	char readline[100];

	for (int i = 0; i < numVert; i++){
		fgets(readline, 100, modelFile);
		saveVerts(vertecies, i, readline);
	}

	for (int i = 0; i < numInds * 6; i+=6){
		fgets(readline, 100, modelFile);
		saveInds(indicies, i, readline);
	}
}

void loadPlyModelBin(std::vector<vertex> &vertecies, std::vector<int> &indicies, int numVert, int numInds, FILE *modelFile){
	// Load Verts
	for (int i = 0; i < numVert; i++)
		fread(&vertecies[i], sizeof(vertex), 1, modelFile);

	// Load Inds
	Indecie inds;
	inds.indices = (unsigned int *)malloc(sizeof(unsigned int) * 4);

	int indsIndex = 0;

	for (int i = 0; i < numInds; i++) {
		fread(&inds, sizeof(unsigned char), 1, modelFile);

		if (inds.num_indices == 0)
			continue;

		fread(inds.indices, sizeof(unsigned int), inds.num_indices, modelFile);

		indicies[indsIndex++] = inds.indices[0];
		indicies[indsIndex++] = inds.indices[1];
		indicies[indsIndex++] = inds.indices[2];

		if (inds.num_indices < 4) // second triangle
			continue;

		indicies[indsIndex++] = inds.indices[0];
		indicies[indsIndex++] = inds.indices[2];
		indicies[indsIndex++] = inds.indices[3];
	}

	free(inds.indices);
}

void loadPlyModel(const char * filePath, std::vector<vertex> &vertecies, std::vector<int> &indicies, int &vertAmount, int &indsAmount){
	char resFilePath[100];// model/model.ply -> ../res/model/model.ply (example)
	addResPath(resFilePath, filePath);

        FILE * modelFile = fopen(resFilePath,"r");
	
        if (!modelFile){
		printf("NO MODEL FILE %s \n", resFilePath);
		return;
	}

        char readline[100];
	int numVert = 1, numInds = 1;
	char format[50];

	while (fgets(readline, 100, modelFile)){ // Model metadata

		if(!strncmp(readline, "format", 6))
			sscanf(readline,"%*s %s", format);

		if(!strncmp(readline, "element vertex", 14))
			sscanf(readline,"%*s %*s %d", &numVert);

		if(!strncmp(readline, "element face", 12))
			sscanf(readline,"%*s %*s %d", &numInds);

		if(!strncmp(readline, "end_header", 10))
			break;
	}

	vertecies.resize(numVert);
	indicies.resize(numInds * 6);

	if(strcmp(format, "ascii") == 0){ // load ply ascii file
		loadPlyModelASCII(vertecies, indicies, numVert, numInds, modelFile);

	}else{	// load ply bin file
		loadPlyModelBin(vertecies, indicies, numVert, numInds, modelFile);
	}

	vertAmount = numVert;
	indsAmount = numInds;

	fclose(modelFile);
}

/*
void optimizeMesh(std::vector<vertex> &vertecies, std::vector<int> &indecies){
	size_t numInds = indecies.size();
	size_t numVert = vertecies.size();
	
	std::vector<unsigned int> remap(numInds); // allocate temporary memory for the remap table
	size_t optNumVert = meshopt_generateVertexRemap(remap.data(), indecies.data(), numInds, vertecies.data(), numVert, sizeof(vertex));

	std::vector<int> optIndecies;
	std::vector<vertex> optVertecies;
	optIndecies.resize(numInds);
	optVertecies.resize(numVert);

	meshopt_remapIndexBuffer(optIndecies.data(), indecies.data(), numInds, remap.data());
	meshopt_remapVertexBuffer(optVertecies.data(), vertecies.data(), numVert, sizeof(vertex), remap.data());

	meshopt_optimizeVertexCache(optIndecies.data(), optIndecies.data(), numInds, optNumVert);

	meshopt_optimizeOverdraw(optIndecies.data(), optIndecies.data(), numInds, &(optVertecies[0].x), optNumVert, sizeof(vertex), 1.05f);

	meshopt_optimizeVertexFetch(optVertecies.data(), optIndecies.data(), numInds, optVertecies.data(), optNumVert, sizeof(vertex));
	
	vertecies.clear();
	vertecies = optVertecies;

	indecies.clear();
	indecies = optIndecies;
}
*/
typedef struct loadedModels{
	Model m;
	const char * filePath;
} loadedModels;
std::vector<loadedModels> allLoadedModels;

Model loadModel(const char * filePath){
	// check if model cached
	for (unsigned long i = 0; i < allLoadedModels.size(); i++) {
		if (strcmp(allLoadedModels[i].filePath, filePath) == 0) {
			return allLoadedModels[i].m;
		}
	}

	std::vector<vertex> vertecies;
	std::vector<int> indecies;
	int vertAmount;
	int indsAmount;
	loadPlyModel(filePath, vertecies, indecies, vertAmount, indsAmount);

	//optimizeMesh(vertecies, indecies);

	// cache model
	Model m = bufferModel(vertecies, indecies);
	allLoadedModels.push_back((loadedModels){m, filePath});

	return m;
}

Model loadModelWithTexture(const char * filePath, const char * textureFilePath){
	Model m;
	m = loadModel(filePath);
	m.texture = loadTexture(textureFilePath);
	return m;
}
