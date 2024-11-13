#include <GL/glew.h>
#include <cstdio>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "model.h"
#include "camera.h"
#include "shader.h"

// OPENGL INIT
GLFWwindow* window = 0;

void sendUniform4f(const char * name, glm::mat4 data){
        int uniformLoc = glGetUniformLocation(currentShaderProgram, name);
        glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(data));
}

void drawModel(Model m, glm::vec3 position, glm::quat rotation, glm::vec3 scale){
	glm::mat4 model(1.0);

	// move model around camera fixes floating point inprecision error
	position -= camera.pos; 

	model = glm::translate(model, position); // move
	model = model * glm::mat4_cast(rotation); //rotate
	model = glm::scale(model, scale); //scale

	sendUniform4f("model", model);

	glBindVertexArray(m.VAO);
	glBindTexture(GL_TEXTURE_2D, m.texture);
	glDrawElements(GL_TRIANGLES, m.numIndices, GL_UNSIGNED_INT, 0);
}
// DrawModel Without Scale
void drawModel(Model m, glm::vec3 position, glm::quat rotation){
	drawModel(m, position, rotation, (glm::vec3){1.,1.,1.});
}
// DrawModel Without Scale and vec3 rotation
void drawModel(Model m, glm::vec3 position, glm::vec3 rotation){
	glm::quat quat= glm::quat(glm::radians(rotation));
	drawModel(m, position, quat, (glm::vec3){1.,1.,1.});
}

int screenX, screenY;
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	(void)window;
        screenX = width;
        screenY = height;
        glViewport(0, 0, width, height);
}

void openglError(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	(void)source, (void)type, (void)id, (void)severity, (void)length, (void)userParam;
        printf("Opengl:  %s \n", message);
}

int openglInit(){
	if (!glfwInit())
		fprintf(stderr, "Failed to init GLFW\n");
	

        window = glfwCreateWindow(800, 600, "KNK", NULL, NULL);

	if (!window)
		fprintf(stderr, "Failed to create GLFW window\n");

        glfwMakeContextCurrent(window);

        if (glewInit() != GLEW_OK)
		fprintf(stderr, "Failed to init GLEW\n");

        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(openglError, 0);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	return 0;
}
