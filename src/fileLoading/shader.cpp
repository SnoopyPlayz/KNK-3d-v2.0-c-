#include "model.h"
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>

unsigned int currentShaderProgram;

void shaderError(unsigned int vertexShader){
           int success;
           char infoLog[512];
           glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
           if(!success){
                   glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
                   printf("vertex compilation failed %s \n",infoLog);
		   exit(0);
           }
}

unsigned int compileShader(const unsigned int shaderType,const char ** shaderSource){
  	  unsigned int shader;
          shader = glCreateShader(shaderType);
  
          glShaderSource(shader, 1, shaderSource, NULL);
          glCompileShader(shader);
          shaderError(shader);
          return shader;
}

unsigned int createShader(const unsigned int shaderType,const char * filePath){
	FILE * shaderfile;

	shaderfile = fopen(filePath,"r");
	
	char readline[100];
	char allLines[1000] = "";
	if (!shaderfile){
		printf("no shader file");
		return 0;
	}

	
	while (fgets(readline, 100, shaderfile)){
		strcat(allLines,readline);	
	};
	fclose(shaderfile);

	const char * shaderSource = allLines;
	return compileShader(shaderType,&shaderSource);
}

unsigned int createShaderProgram(const char * vertexFilePath, const char * fragmentFilePath){
	char resVertFilePath[100];
	char resFragFilePath[100];

	addResPath(resVertFilePath, vertexFilePath);
	addResPath(resFragFilePath, fragmentFilePath);

	unsigned int vertexShader = createShader(GL_VERTEX_SHADER, resVertFilePath);
	unsigned int fragmentShader = createShader(GL_FRAGMENT_SHADER, resFragFilePath);


	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram,vertexShader);
	glAttachShader(shaderProgram,fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

void selectShader(unsigned int shaderProgram){
	currentShaderProgram = shaderProgram;
	glUseProgram(shaderProgram);
}
