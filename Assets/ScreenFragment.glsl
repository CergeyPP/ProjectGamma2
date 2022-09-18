#version 450 core

uniform sampler2D mainTexture;

in vec2 TexCoords;

out vec4 color;

void main(){

	float gamma = 2.2f;

	vec3 resultColor = vec3(texture(mainTexture, TexCoords));
	color = vec4(pow(resultColor, vec3(1.0/gamma)), 1.f);
}