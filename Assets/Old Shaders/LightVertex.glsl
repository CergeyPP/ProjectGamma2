#version 450 core
layout(location = 0) in vec3 position;
layout(location = 2) in vec2 texCoord;

uniform mat4 Model;
uniform mat4 ProjectionView;

out vec4 screenPos;

void main(){
	vec4 pos = ProjectionView * Model * vec4(position, 1);
	gl_Position = pos;
	
	screenPos = pos;
}
