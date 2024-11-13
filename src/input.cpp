#include <GLFW/glfw3.h>
#include "glm/common.hpp"
#include "opengl.h"
#include "camera.h"

void mouseInput(){
	static float lastX = 960, lastY = 540, camRotX = -90.0f, camRotY = 0;
	static bool mouseUsed = false;
	const float sensitivity = 0.1f;

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	if (!mouseUsed){
		lastX = xpos;
		lastY = ypos;
		mouseUsed = true;
		return;
	}

	camRotX += (xpos - lastX) * sensitivity;
        camRotY += (lastY - ypos) * sensitivity;

	lastX = xpos;
        lastY = ypos;

	// camera rot min: -89  max: 89
	camRotY = glm::clamp(camRotY, -89.f, 89.f);

        camera.front.x = cos(glm::radians(camRotX)) * cos(glm::radians(camRotY));
        camera.front.y = sin(glm::radians(camRotY));
        camera.front.z = sin(glm::radians(camRotX)) * cos(glm::radians(camRotY));
	camera.front = glm::normalize(camera.front);
}

void keyInput(){
        if (glfwGetKey(window, GLFW_KEY_W)){
		cameraMoveForward();
        };
        if (glfwGetKey(window, GLFW_KEY_S)){
		cameraMoveBack();
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE)){
		cameraMoveUp();
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)){
		cameraMoveDown();
        }
        if (glfwGetKey(window, GLFW_KEY_A)){
		cameraMoveLeft();
        }
        if (glfwGetKey(window, GLFW_KEY_D)){
		cameraMoveRight();
        }
}
