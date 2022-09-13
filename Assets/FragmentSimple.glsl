#version 450 core

//uniform sampler2D Albedo;
uniform vec3 Albedo;

in vec2 TexCoords;

out vec4 color;

void main(){

	//color = texture(Albedo, TexCoords);
	color = vec4(Albedo, 1);
}