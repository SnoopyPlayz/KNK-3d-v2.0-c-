#pragma once
#include <glm/glm.hpp>

struct CameraStruct {
	glm::vec3 pos;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 speed;
};
extern struct CameraStruct camera; 

void cameraDraw();

void cameraMoveForward();

void cameraMoveBack();

void cameraMoveDown();

void cameraMoveUp();

void cameraMoveRight();

void cameraMoveLeft();
