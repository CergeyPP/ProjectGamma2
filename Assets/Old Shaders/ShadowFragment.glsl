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

out vec4 disperseDepth;

vec2 ComputeMoments(float Depth) {   
	vec2 Moments;   
	// First moment is the depth itself.   
	Moments.x = Depth;   
	// Compute partial derivatives of depth.    
	float dx = dFdx(Depth);   
	float dy = dFdy(Depth);   
	// Compute second moment over the pixel extents.   
	Moments.y = Depth*Depth + 0.25*(dx*dx + dy*dy);   
	return Moments; 
} 

void main(){
	if (light.type != DIRECTIONAL){
		float lightDistance = length(FragPos.xyz - vec3(light.position));
		lightDistance = lightDistance / farPlane;

		vec2 moments = ComputeMoments(lightDistance);
			//vec2(lightDistance, lightDistance * lightDistance);
		gl_FragDepth = moments.x;

		disperseDepth = vec4(moments,0,0);
	} else {		
		gl_FragDepth = gl_FragCoord.z / farPlane;
	}
}