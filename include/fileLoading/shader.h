#pragma once
unsigned int createShaderProgram(const char * vertexFilePath, const char * fragmentFilePath);
void selectShader(unsigned int shaderProgram);

extern unsigned int currentShaderProgram;
