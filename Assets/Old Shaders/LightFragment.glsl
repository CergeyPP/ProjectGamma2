#version 450 core

in vec4 screenPos;
vec2 TexCoords;

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
	int isShadowCast;
};

uniform Light light;

uniform vec3 viewPos;

out vec4 color;

 float ChebyshevUpperBound(vec2 Moments, float t) {   
	float p = step(t, Moments.x);       
	float Variance = Moments.y - (Moments.x * Moments.x);  
	   
	Variance = max(Variance, 0.002);
	float d = t - Moments.x;
	float p_max = smoothstep(0, 1, Variance / (Variance + d*d));
	return min(max(p, p_max), 1.0);
}


float ShadowContribution(vec3 texCoords, float Distance){
	 vec2 Moments = texture(depthMap, texCoords, 0).rg;

	return ChebyshevUpperBound(Moments, Distance);
	//return Moments.x;
}

float ShadowCalculation(vec3 fragPos){
	
	vec3 fragToLight = fragPos - vec3(light.position);
	float currentDepth = length(fragToLight);

	float closestDepth = ShadowContribution(fragToLight, currentDepth);

	return closestDepth;
}

float ShadowCalculationPCF(vec3 fragPos)
{
    // расчет вектора между положением фрагмента и положением источника света
    vec3 fragToLight = fragPos - vec3(light.position);
    // полученный вектор направления от источника к фрагменту 
    // используется для выборки из кубической карты глубин
    float closestDepth = texture(depthMap, fragToLight).r;
    // получено линейное значение глубины в диапазоне [0,1]
    // проведем обратную трансформацию в исходный диапазон
    
    // получим линейное значение глубины для текущего фрагмента 
    // как расстояние от фрагмента до источника света
    float currentDepth = length(fragToLight);
    // тест затенения
    float bias = 0.05; 
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;

    return 1-shadow;
} 

vec3 calculatePointLight(Light pointLight, vec3 norm, vec3 FragPos, vec3 viewDir){

	//float linear = LINEAR / pointLight.radius;
	//float quadratic = QUADRATIC / (pointLight.radius * pointLight.radius);

	vec3 lightDir = vec3(pointLight.position) - FragPos;
	float distance = length(lightDir);
	float attenuation = 1.f / (distance * distance);
	//float attenuation = 0.8;
	lightDir = normalize(lightDir);

	vec3 diffuseLight = max(dot(norm, lightDir), 0) * vec3(pointLight.diffuse) * pointLight.diffuse.a;
	vec3 diffuse = diffuseLight * vec3(texture(albedo, TexCoords));

	vec4 spec = texture(specular, TexCoords);
	float shininess = spec.a;

	vec3 halfways = reflect(-lightDir, norm);
	float specularPower = pow(max(dot(viewDir, halfways), 0), shininess);
	vec3 specular = vec3(pointLight.specular) * specularPower * vec3(texture(specular, TexCoords)) * pointLight.specular.a;

	return (diffuse + specular) * attenuation * clamp(pointLight.radius - distance, 0, 1);

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
	//TexCoords = vec2(screenPos);

	vec3 norm = vec3(texture(normal, TexCoords));

	vec3 fragPos = vec3(texture(position, TexCoords));

	vec3 viewDir = normalize(viewPos - fragPos);

	float shadow = max(1 - light.isShadowCast, ShadowCalculation(fragPos));

	vec3 resultColor = calculatePointLight(light, norm, fragPos, viewDir) * shadow;
	color = texture(framebuffer, TexCoords) + vec4(resultColor, 1.0);
	//color = vec4(shadow, 0, 0, 1.f);
	//color = vec4(fragPos - vec3(light.position), 1.f);
	//color = vec4(norm, 1.f);
}