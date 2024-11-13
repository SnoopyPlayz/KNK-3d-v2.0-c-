#include <stdio.h>
#include "opengl.h"
#include "gameState.h"

int main(){
	init();
	//glfwSwapInterval(0); // Vsync disable

	double timePerFrame = 1./60.; 

	double currentTime = glfwGetTime();
	double accumulator = 0.0;

	int FPSTimes = 0;
	int FPSOldtime = 0;

	while (!glfwWindowShouldClose(window)) {
		const double newTime = glfwGetTime();
		const double frameTime = newTime - currentTime;
		//printf("%g \n", frameTime * 1000);

		currentTime = newTime;

		accumulator += frameTime;

		while (accumulator >= timePerFrame) {
			update();
			accumulator -= timePerFrame;
		}
		render();

		FPSTimes += 1;
		if (newTime - FPSOldtime > 1.0){
			char FPSstring[100];
			sprintf(FPSstring, "KNK   FPS: %d  TPF: %g", FPSTimes, frameTime * 1000);
			glfwSetWindowTitle(window, FPSstring);

			FPSTimes = 0;
			FPSOldtime = newTime;
		}
	}
	glfwTerminate();
}

