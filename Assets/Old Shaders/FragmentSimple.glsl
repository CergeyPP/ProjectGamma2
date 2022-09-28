#version 450 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec4 gSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

void main() {
	gPosition = FragPos;
	gNormal = Normal;
	gAlbedo = vec4(0,1,0,1);
	gSpec = vec4(0,0,1,1);
}