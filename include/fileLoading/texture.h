#pragma once
typedef unsigned int Texture;

Texture loadTexture(const char * fileLocation);
void addResPath(char *dest, const char *filePath);
