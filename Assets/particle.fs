#version 450 core

in vec2 TexCoords;
in float Fade;

uniform sampler2D Texture;

out vec4 FragColor;

void main(){
	vec4 color = texture(Texture, TexCoords);
	color.a *= Fade;
	FragColor =  color;
}