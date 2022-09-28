#version 450 core

layout(location = 0) in vec3 position;

uniform mat4 Model;

void main(){
	gl_Position = Model * vec4(position, 1.0);
	
}