#version 450 core
#define DIRECTIONAL 0
#define POINT 1
#define SPOT 2

struct Light{
	int type;
	vec4 position;
};

uniform float farPlane;

in vec4 FragPos;

uniform Light light;

void main(){
	if (light.type != DIRECTIONAL){
		float lightDistance = length(FragPos.xyz - vec3(light.position));
		lightDistance = lightDistance / farPlane;

		gl_FragDepth = lightDistance;
	} else {
		gl_FragDepth = gl_FragCoord.z / farPlane;
	}
}