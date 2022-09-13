#version 450 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

out vec3 Normal;
out vec2 TexCoords;

out vec3 fragPos;

void main(){
	Normal = mat3(transpose(inverse(Model))) * normal;
	TexCoords = texCoords;

	fragPos = vec3(Model * vec4(position, 1));

	gl_Position = Projection * View * Model * vec4(position, 1);
}