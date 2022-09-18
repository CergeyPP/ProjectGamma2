#version 450 core

layout(location = 0) in vec3 position;

uniform mat4 LightPV;
uniform mat4 Model;

void main(){
	gl_Position = LightPV * Model * vec4(position, 1.0);
	
}