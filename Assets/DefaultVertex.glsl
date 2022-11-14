#version 450 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

uniform mat4 ProjectionView;
uniform mat4 Model;

out vec3 Normal;
out vec2 TexCoords;
out vec3 FragPos;

void main(){

	Normal = normalize(mat3(transpose(inverse(Model))) * normal);
	TexCoords = texCoords;

	FragPos = vec3(Model * vec4(position, 1));

	gl_Position = ProjectionView * Model * vec4(position, 1);
}