#version 450 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec4 gSpec;

struct Material{
	sampler2D Albedo0;
	sampler2D Specular0;
	float shininess;
};

uniform Material material;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

void main() {
	gPosition = FragPos;
	gNormal = Normal;
	gAlbedo = texture(material.Albedo0, TexCoords);
	gSpec = texture(material.Specular0, TexCoords);
	gSpec.a = material.shininess;
}