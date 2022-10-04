#version 450 core
#define LINEAR 6
#define QUADRATIC 200

in vec4 screenPos;
vec2 TexCoords;

in vec4 globalPos;

uniform sampler2D position;
uniform sampler2D normal;
uniform sampler2D albedo;
uniform sampler2D specular;

uniform samplerCube depthMap;

uniform sampler2D framebuffer;

struct Light{
	vec4 diffuse;
	vec4 specular;	
	vec4 position;
	float radius;
};

uniform Light light;

in vec3 ViewPos;

out vec4 color;

float linstep(float min, float max, float value){
	return clamp((value - min)/(max - min), 0, 1);
}

 float ChebyshevUpperBound(vec2 Moments, float t) {   
	// One-tailed inequality valid if t > Moments.x    
	float p = step(t, Moments.x);   // Compute variance.    
	float Variance = Moments.y - (Moments.x * Moments.x);  
	// Compute probabilistic upper bound.    
	Variance = max(Variance, 0.00002);
	float d = t - Moments.x;
	float p_max = linstep(0.2, 1, Variance / (Variance + d*d));
	return min(max(p, p_max), 1.0);
}


float ShadowContribution(vec3 texCoords, float Distance){
	 vec2 Moments = vec2(0,0);

	 vec2 texelSize = 1/textureSize(depthMap, 0);

	 for (int x = -1; x <= 1; x++){
	 	for (int y = -1; y <= 1; y++){
	 			Moments += texture(depthMap, texCoords + texelSize.r * vec3(x, y, 0)).rg;
	 	}
	 }

	 Moments /= 9;
	//vec2 Moments = texture(depthMap, texCoords).rg;

	return ChebyshevUpperBound(Moments, Distance);
}

float ShadowCalculation(vec3 fragPos){
	
	vec3 fragToLight = fragPos - vec3(light.position);
	float currentDepth = length(fragToLight) / light.radius;

	float closestDepth = ShadowContribution(fragToLight, currentDepth);
	//closestDepth *= farPlane;

	//float bias = //max(0.5 * (1 - dot(vec3(texture(normal, TexCoords)), normalize(-fragToLight))), 0.1);
				//0;
	//float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;


	return closestDepth;
}

vec3 calculatePointLight(Light pointLight, vec3 norm, vec3 FragPos, vec3 viewDir){

	//float linear = LINEAR / pointLight.radius;
	//float quadratic = QUADRATIC / (pointLight.radius * pointLight.radius);

	vec3 lightDir = vec3(pointLight.position) - FragPos;
	float distance = length(lightDir);
	float attenuation = 1 / distance / distance;
	//float attenuation = 0.8;
	lightDir = normalize(lightDir);

	vec3 diffuseLight = max(dot(norm, lightDir),0) * vec3(pointLight.diffuse) * pointLight.diffuse.a;
	vec3 diffuse = diffuseLight * vec3(texture(albedo, TexCoords));

	vec4 spec = texture(specular, TexCoords);
	float shininess = spec.a * 256;

	vec3 halfways = normalize(lightDir + viewDir);
	float specularPower = pow(max(dot(norm, halfways), 0), shininess);
	vec3 specular = vec3(pointLight.specular) * specularPower * vec3(texture(specular, TexCoords)) * pointLight.specular.a;

	float depth = length(ViewPos - globalPos.xyz) - length(ViewPos - FragPos);

	return (diffuse + specular)*attenuation*clamp(depth, 0.0, 1.0);

}

// vec3 calculateSpotLight(Light light, vec3 norm, vec3 FragPos, vec3 viewDir){

// 	vec3 lightDir = normalize(vec3(-light.direction));
    
//     // check if lighting is inside the spotlight cone
//     float theta = dot(lightDir, vec3(normalize(viewDir))); 
    
//     if(theta > light.cutoff) // remember that we're working with angles as cosines instead of degrees so a '>' is used.
//     {            
//         // diffuse 
//         float diff = max(dot(norm, lightDir), 0.0);
//         vec3 diffuse = vec3(light.diffuse) * diff * texture(albedo, TexCoords).rgb;  
        
//         // specular 
// 		vec4 spec = texture(specular, TexCoords);
// 		float shininess = spec.a * 256;

//         vec3 halfways = normalize(lightDir + viewDir);
// 		float specularPower = pow(max(dot(norm, halfways), 0), shininess);
// 		vec3 specular = vec3(light.specular) * specularPower * vec3(texture(albedo, TexCoords)) * light.specular.a;  
        
//         // attenuation
//         float distance    = length(vec3(light.position) - FragPos);
//         float attenuation = 1.0 / (1 + light.linear * distance + light.quadratic * (distance * distance));    

//         // ambient  *= attenuation; // remove attenuation from ambient, as otherwise at large distances the light would be darker inside than outside the spotlight due the ambient term in the else branche
//         diffuse   *= attenuation;
//         specular *= attenuation;   

//         return diffuse + specular;
//     }

//     return vec3(0,0,0);
// }

void main(){

	vec3 screenCoords = screenPos.xyz / screenPos.w;
	TexCoords = vec2(screenCoords * 0.5 + 0.5);

	vec3 norm = normalize(vec3(texture(normal, TexCoords)));

	vec3 fragPos = vec3(texture(position, TexCoords));

	vec3 viewDir = normalize(ViewPos - fragPos);

	vec3 resultColor = calculatePointLight(light, norm, fragPos, viewDir) //* ShadowCalculation(fragPos);
			;
	color = texture(framebuffer, TexCoords) + vec4(resultColor, 1.0);

	//color = vec4(fragPos, 1);
	//color = vec4(fragPos, 1);
}