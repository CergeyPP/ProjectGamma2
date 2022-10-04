#version 450 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

uniform mat4 ProjectionView;
uniform mat4 Model;

out vec3 Normal;
//out vec2 TexCoords;

void main(){

	//Normal = mat3(transpose(inverse(Model))) * normal;
	//TexCoords = texCoords;
	Normal = normal;
	gl_Position = ProjectionView * Model * vec4(position, 1);
}