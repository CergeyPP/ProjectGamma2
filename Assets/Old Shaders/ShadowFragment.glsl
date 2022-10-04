#version 450 core
#define DIRECTIONAL 0
#define POINT 1
#define SPOT 2

uniform float farPlane;

in vec4 FragPos;

uniform vec4 lightPos;

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
	float lightDistance = length(FragPos.xyz - vec3(lightPos));
	lightDistance = lightDistance / farPlane;

	vec2 moments = ComputeMoments(lightDistance);
			//vec2(lightDistance, lightDistance * lightDistance);
	gl_FragDepth = moments.x;

	disperseDepth = vec4(moments,0,0);
}