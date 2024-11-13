#include <GL/glew.h>
#include <cstdio>
#include <cstdlib>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//some chatgbt in this func
void addResPath(char *dest, const char *filePath){
	const char *prefix = "../../../../res/"; // change res location here!!
	size_t prefix_len = strlen(prefix);
	size_t filepath_len = strlen(filePath);

	char *new_path = (char *)malloc(prefix_len + filepath_len + 1);//+1 for '\0'

	strcpy(new_path, prefix);
	strcat(new_path, filePath);
	strcpy(dest, new_path);
	free(new_path);	
}
typedef unsigned int Texture;

typedef struct loadedTextures{
	Texture tex;
	const char * filePath;
} loadedTextures;

std::vector<loadedTextures> allLoadedTextures;

unsigned int loadTexture(const char* fileLocation) {
	// check if texture cashed
	for (unsigned long i = 0; i < allLoadedTextures.size(); i++) {
		if (strcmp(allLoadedTextures[i].filePath, fileLocation) == 0) {
			return allLoadedTextures[i].tex;
		}
	}

	stbi_set_flip_vertically_on_load(1);

	char resFilePath[100]; // texture/tex.png -> ../res/texture/tex.png (example)
	addResPath(resFilePath, fileLocation);

 	int width, height, nrChannels;

 	unsigned char *data = stbi_load(resFilePath, &width, &height, &nrChannels, 0);
	 
 	if (!data){
 		printf("error loading image file: %s \n Reason: %s\n",resFilePath, stbi_failure_reason());
 		return 0;
 	}
	 
	unsigned int texture;
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	if (nrChannels == 4){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}else if (nrChannels == 3){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}else {
		printf("IMAGE FAIL TO LOAD INVALID nrChannels %s \n", resFilePath);
		stbi_image_free(data);
	};

	glGenerateMipmap(GL_TEXTURE_2D);
	
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

	// cache texture
	allLoadedTextures.push_back((loadedTextures){texture, fileLocation});

	return texture;
}
