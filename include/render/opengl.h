#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/fwd.hpp"
#include "model.h"

extern GLFWwindow* window;
extern int screenX, screenY;
using glm::vec3;

void drawModel(Model, glm::vec3 position, glm::vec3 rotation);
void drawModel(Model, glm::vec3 position, glm::quat rotation);
void drawModel(Model, glm::vec3 position, glm::quat rotation, glm::vec3 scale);
void sendUniform4f(const char * name, glm::mat4 data);
void openglInit();
