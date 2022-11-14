#version 450 core

#define SAMPLES 4

in vec2 TexCoords;

uniform sampler2DMS depthMap;

out vec2 AAdepth;

ivec2 offsets[9] = {
	ivec2(-1,-1),
	ivec2(-1,0),
	ivec2(-1,1),
	ivec2(0,-1),
	ivec2(0,0),
	ivec2(0,1),
	ivec2(1,-1),
	ivec2(1,0),
	ivec2(1,1),
};

float blur[9] = {
	1,2,1,
	2,4,2,
	1,2,1,
};

float blurDivide = 16;

vec2 blurColor(){
	const float constStep = 1.f / 600.f;
	vec2 offsetStep = vec2(textureSize(depthMap)) * constStep;
	ivec2 depthMapCoords = ivec2(vec2(textureSize(depthMap)) * TexCoords);

	vec2 blurDepth = vec2(0, 0);
	for (int level = 0; level < SAMPLES; level++)
	 	for (int i = 0; i < 9; i++){
	 		blurDepth += texelFetch(depthMap, depthMapCoords + ivec2(vec2(offsets[i]) * offsetStep), level).rg * blur[i];
	 	}

	blurDepth /= SAMPLES * blurDivide;

	return blurDepth;
}

void main(){
	ivec2 depthMapCoords = ivec2(vec2(textureSize(depthMap)) * TexCoords);
	//AAdepth = texelFetch(depthMap, depthMapCoords, 0).rg;
	AAdepth = blurColor();
} 