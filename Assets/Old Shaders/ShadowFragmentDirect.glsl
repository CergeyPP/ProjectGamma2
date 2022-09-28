#version 450 core

out vec4 depth;

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

	depth = vec4(ComputeMoments(gl_FragCoord.z), gl_FragCoord.z, 0);
}