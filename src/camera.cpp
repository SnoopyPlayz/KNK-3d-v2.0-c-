#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/trigonometric.hpp"
#include "opengl.h"

struct CameraStruct {
	glm::vec3 pos;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 speed;
};

struct CameraStruct camera = {
	{0.0f,1.0f,0.0f},
	{0.0f,0.0f,-1.0f},
	{0.0f,1.0f,0.0f},
	{0.5f,0.5f,0.5f}
};

void cameraDraw(){
	glm::mat4 projection;
	
	projection = glm::perspective(glm::radians(90.0f), (float)screenX / (float)screenY, 0.01f, 1000.f);
	sendUniform4f("projection", projection);
	
	// move world around camera fix floating point error
	glm::mat4 glmView = glm::lookAt((glm::vec3){0,0,0}, camera.front, (glm::vec3){0,1,0});

	sendUniform4f("view", glmView);
}

void cameraMoveForward(){
	camera.pos += camera.speed * camera.front;
}

void cameraMoveBack(){
	camera.pos -= camera.speed * camera.front;
}

void cameraMoveUp(){
	camera.pos.y += camera.speed.y;
}

void cameraMoveDown(){
	camera.pos.y -= camera.speed.y;
}

void cameraMoveLeft(){
	glm::vec3 result;
	result = glm::cross(camera.front, camera.up);
	result = glm::normalize(result);

	camera.pos -= result * camera.speed;
}

void cameraMoveRight(){
	glm::vec3 result;
	result = glm::cross(camera.front, camera.up);
	result = glm::normalize(result);

	camera.pos += result * camera.speed;
}
