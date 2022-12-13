#version 450 core

layout(location = 0) in vec3 pos;
layout(location = 2) in vec2 texCoord;

uniform vec3 centerPos;
uniform mat4 ProjectionView;
uniform mat4 viewM;
uniform vec2 size;

out vec2 TexCoords;

void main(){

    vec3 up = vec3(viewM[0][1], viewM[1][1], viewM[2][1]);
    vec3 right = vec3(viewM[0][0], viewM[1][0], viewM[2][0]);

    vec3 position = centerPos + (right * pos.x * size.x + up * pos.y * size.y);

	gl_Position = ProjectionView * vec4(position, 1);
	TexCoords = texCoord;
}