#version 450 core
layout(location = 0) in vec3 position;
layout(location = 2) in vec2 texCoords;

uniform mat4 ProjectionView;
uniform mat4 Model;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

void main(){
	 gl_Position = ProjectionView * Model * vec4(position, 1);
	FragPos = vec3(Model*vec4(position, 1));
	TexCoords = texCoords;
	Normal = vec3(1,0,0);
}