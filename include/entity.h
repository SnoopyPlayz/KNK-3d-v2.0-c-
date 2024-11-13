#pragma once
#include "model.h"
#include "physics.h"
typedef struct Entity{
	Model model;
	BodyID body;
	glm::vec3 size;
}Entity;

Entity* addEntity(Model m, BodyID body);
Entity* addEntity(Model m, BodyID body, glm::vec3 size);
void entitiesDraw();
