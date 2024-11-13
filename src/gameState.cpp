#include <GL/glew.h>
#include <cstdio>
#include "GLFW/glfw3.h"
#include "opengl.h"
#include "shader.h"
#include "camera.h"
#include "input.h"
#include "physics.h"
#include "model.h"
#include "entity.h"
#include "networking.h"
#include "texture.h"

float rot = 0;
void update(){
	physicsUpdate();
	keyInput();
	rot += 1;
}

unsigned int texture1 = 0;
unsigned int texture = 0;
Model car = (Model) {};
Model cube = (Model) {};
//JPH::BodyID sphereID;

void render(){
	//updateNetworking();
	glfwPollEvents();
	mouseInput();

	glClearColor(0.1f,0.1f,0.1f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cameraDraw();

	Model sphereM = loadModelWithTexture("model/shapes/sphere.ply", "texture/redd.png");

	if (glfwGetKey(window, GLFW_KEY_F)) {
		Entity *e = addEntity(sphereM, addSphere(2, camera.pos, EMotionType::Dynamic), {2,2,2});
		bodyInterface->AddLinearVelocity(e->body, (JPH::Vec3Arg){camera.front.x, camera.front.y, camera.front.z} * 100);
	}

	entitiesDraw();
	//physicsDraw();

	//drawModel(car, (glm::vec3){2,1,2}, (glm::vec3){0, 180, 90});
	//drawModel(cube, (glm::vec3){2,5,2}, (glm::vec3){rot, rot, rot});

	glfwSwapBuffers(window);
}

void init(){
	openglInit();
	physicsInit();
	//initNetworking();

	unsigned int shaderProgram = createShaderProgram("vertexShader.sh","fragmentShader.sh");
	selectShader(shaderProgram);

	Model boxM = loadModelWithTexture("model/shapes/box.ply", "texture/redd.png");
	addEntity(boxM, addBox({1000,1,1000},{0,-1,0}, EMotionType::Static),{1000,1,1000});
	addEntity(boxM, addBox({1,100,1000},   {1000,100,0},   EMotionType::Static),{1,100,1000});
	//addEntity(boxM, addBox({1000,100,1000},{1000,-1,1000},EMotionType::Static),{1000,100,1000});
	//addEntity(boxM, addBox({1000,100,1},   {1000,-1,1},   EMotionType::Static),{1000,100,1});

   	//cube = loadModel("model/sphere2BIN.ply");
   	//loadModel("model/sphere2BIN.ply");
	
	Model sphereM = loadModelWithTexture("model/shapes/sphere.ply", "texture/redd.png");
	for (int i = 1; i < 10; i++) {
		Entity *e = addEntity(sphereM, addSphere(1, (glm::vec3){1,i * 15,1 * i}, EMotionType::Dynamic), {1,1,1});
		bodyInterface->SetRestitution(e->body, 0.3);
	}

	for (int i = 1; i < 10; i++) {
		Entity *box = addEntity(boxM, addBox((glm::vec3){2,2,2}, (glm::vec3){1, i * 15,1}, EMotionType::Dynamic));
		box->size = (glm::vec3){2,2,2};
		//bodyInterface->SetRestitution(e->body, 0.3);
	}

	//loadTexture("texture/redd.png");
	//loadTexture("texture/the.png"); 
	//Texture t = loadTexture("texture/ram2.png"); 
	//glDeleteTextures(1, &t);

	//loadTexture("texture/awesomeface.png");

		//entityInit("model/monkey.ply", "texture/redd.png", btVector3(0,10 * i,3), btVector3(0,90,0), 10);
}
