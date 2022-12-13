#version 450 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 worldSpacePos;
layout(location = 4) in float time;

uniform mat4 ProjectionView;
uniform mat4 View;
uniform vec2 size;
uniform float maxLifetime;

out vec2 TexCoords;
out float Fade;

void main(){

	vec3 up = vec3(View[0][1], View[1][1], View[2][1]);
    vec3 right = vec3(View[0][0], View[1][0], View[2][0]);

    vec3 position = worldSpacePos + (right * pos.x * size.x + up * pos.y * size.y);

	gl_Position = ProjectionView * vec4(position, 1);
	TexCoords = texCoord;

	Fade = smoothstep(0, 1, time/maxLifetime);
}