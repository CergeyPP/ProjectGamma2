#version 450 core

in vec2 TexCoords;

uniform sampler2D Image;

out vec4 color;

void main(){
	color = texture(Image, TexCoords) / 2;
}