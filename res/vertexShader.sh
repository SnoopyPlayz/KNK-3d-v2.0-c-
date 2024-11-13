#version 330 core
layout (location = 1) in vec3 aPos;
layout (location = 2) in vec2 fragTexPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texPos;

void main(){
	gl_Position = projection * view * model * vec4(aPos,1);
	texPos = fragTexPos;
};
