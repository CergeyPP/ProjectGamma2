#version 450 core

in vec2 TexCoords;

uniform sampler2D position;
uniform sampler2D albedo;
uniform sampler2D normal;
uniform sampler2D specular;

out vec4 color;

void main(){
	color = /*texture(position, TexCoords) + */texture(normal, TexCoords) + texture(albedo, TexCoords) + texture(specular, TexCoords);
	//color = vec4(1,0,0,1);
}