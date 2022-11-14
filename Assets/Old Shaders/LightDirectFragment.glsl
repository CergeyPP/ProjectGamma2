#version 450 core

#define SAMPLES 1

in vec2 TexCoords;

uniform sampler2D position;
uniform sampler2D normal;
uniform sampler2D albedo;
uniform sampler2D specular;

uniform sampler2D depthMap;

uniform sampler2D framebuffer;

struct Light{
	vec4 diffuse;
	vec4 specular;	
	vec4 position;
	vec4 direction;
};

uniform float farPlane;
uniform mat4 LightPV;

uniform Light light;

uniform vec3 ViewPos;

out vec4 color;

 float ChebyshevUpperBound(vec2 Moments, float t) {   
	// One-tailed inequality valid if t > Moments.x    
	float p = step(t, Moments.x);   // Compute variance.    
	float Variance = Moments.y - (Moments.x * Moments.x);  
	// Compute probabilistic upper bound.    
	Variance = max(Variance, 0.0000002);
	float d = t - Moments.x;
	float p_max = smoothstep(0.2, 1, Variance / (Variance + d*d));
	return min(max(p, p_max), 1.0);
}

float shadowPCF(vec3 fragPos){
	vec4 fragPosLightSpace = LightPV * vec4(fragPos, 1);
	// perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    //closestDepth *= farPlane;
    // get depth of current fragment from light's perspective
	float currentDepth = projCoords.z;

	float bias = max(0.002 * (1 - dot(texture(normal, TexCoords), light.direction)), 0.0001);

	return (currentDepth - bias > texture(depthMap, projCoords.xy).r ? 0.0 : 1.0); 
}

float ShadowContribution(vec2 texCoords, float Distance){

	vec2 Moments = texture(depthMap, texCoords).rg;

	return ChebyshevUpperBound(Moments, Distance);
}

float ShadowCalculation(vec3 fragPos){
	
	vec4 fragPosLightSpace = LightPV * vec4(fragPos, 1);
	// perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    //closestDepth *= farPlane;
    // get depth of current fragment from light's perspective
	float currentDepth = projCoords.z;
    //currentDepth *= farPlane;

    //float shadow = currentDepth < closestDepth ? 1 : 0;

    return ShadowContribution(projCoords.xy, currentDepth);
    //return shadow;
    //return 1;
}

vec3 calculateDirectionLight(Light directLight, vec3 norm, vec3 FragPos, vec3 viewDir){
	vec3 lightDir = normalize(vec3(-directLight.direction));

	vec3 diffuseLight = max(dot(norm, lightDir),0) * vec3(directLight.diffuse) * directLight.diffuse.a;
	vec3 diffuse = diffuseLight * vec3(texture(albedo, TexCoords));

	vec4 spec = texture(specular, TexCoords);
	float shininess = spec.a * 256;

	vec3 halfways = normalize(lightDir + viewDir);
	float specularPower = pow(max(dot(norm,halfways), 0), shininess);
	vec3 specular = vec3(directLight.specular) * specularPower * vec3(texture(specular, TexCoords)) * directLight.specular.a;

	return diffuse + specular;
}


void main(){
	vec3 norm = normalize(vec3(texture(normal, TexCoords)));

	vec3 fragPos = vec3(texture(position, TexCoords));

	vec3 viewDir = normalize(ViewPos - fragPos);

	float shadow = ShadowCalculation(fragPos);
	//float shadow = shadowPCF(fragPos);

	vec3 resultColor = calculateDirectionLight(light, norm, fragPos, viewDir) * shadow;

	color = texture(framebuffer, TexCoords) + vec4(resultColor, 1.0);
	//vec3 fragToLight = fragPos - ViewPos;	
	//float closestDepth = texture(depthMap, fragToLight).r;
	//color = texture(framebuffer, TexCoords) + vec4(vec3(closestDepth/farPlane), 1.0);


}