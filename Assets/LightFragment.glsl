#version 450 core
#define DIRECTIONAL 0
#define POINT 1
#define SPOT 2

in vec2 TexCoords;

uniform sampler2D position;
uniform sampler2D normal;
uniform sampler2D albedo;
uniform sampler2D specular;

uniform samplerCube depthMap;

uniform sampler2D framebuffer;

struct Light{
	int type;
	float cutoff;
	float linear;
	float quadratic;
	vec4 diffuse;
	vec4 specular;	
	vec4 position;
	vec4 direction;
};

uniform float farPlane;

uniform Light light;

uniform vec3 viewPos;

out vec4 color;

float ShadowCalculation(vec3 fragPos){
	
	vec3 fragToLight = fragPos - vec3(light.position);
	float currentDepth = length(fragToLight);

	float shadow = 0;
    vec2 texelSize = 8.0 / textureSize(depthMap, 0);
    for (int x = -1; x <= 1; x++){
    	for (int y = -1; y <= 1; y++){
			for (int z = -1; z <= 1; z++){
			float closestDepth = texture(depthMap, vec3(fragToLight + vec3(x, y, z) * texelSize.x)).r;
			closestDepth *= farPlane;

			float bias = max(0.25 * (1 - dot(vec3(texture(normal, TexCoords)), normalize(-fragToLight))), 0.05);
			shadow += currentDepth - bias > closestDepth ? 1.0 : 0.0;
		}
	}
	}
	shadow /= 27;

	return 1 - shadow;
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

vec3 calculatePointLight(Light pointLight, vec3 norm, vec3 FragPos, vec3 viewDir){
	vec3 lightDir = vec3(pointLight.position) - FragPos;
	float distance = length(lightDir);
	float attenuation = 1/(1 + pointLight.linear * distance + 
		pointLight.quadratic * pointLight.quadratic * distance);
	lightDir = normalize(lightDir);

	vec3 diffuseLight = max(dot(norm, lightDir),0) * vec3(pointLight.diffuse) * pointLight.diffuse.a;
	vec3 diffuse = diffuseLight * vec3(texture(albedo, TexCoords));

	vec4 spec = texture(specular, TexCoords);
	float shininess = spec.a * 256;

	vec3 halfways = normalize(lightDir + viewDir);
	float specularPower = pow(max(dot(norm, halfways), 0), shininess);
	vec3 specular = vec3(pointLight.specular) * specularPower * vec3(texture(specular, TexCoords)) * pointLight.specular.a;

	return (diffuse + specular)*attenuation;


}

vec3 calculateSpotLight(Light light, vec3 norm, vec3 FragPos, vec3 viewDir){

	vec3 lightDir = normalize(vec3(-light.direction));
    
    // check if lighting is inside the spotlight cone
    float theta = dot(lightDir, vec3(normalize(viewDir))); 
    
    if(theta > light.cutoff) // remember that we're working with angles as cosines instead of degrees so a '>' is used.
    {            
        // diffuse 
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = vec3(light.diffuse) * diff * texture(albedo, TexCoords).rgb;  
        
        // specular 
		vec4 spec = texture(specular, TexCoords);
		float shininess = spec.a * 256;

        vec3 halfways = normalize(lightDir + viewDir);
		float specularPower = pow(max(dot(norm, halfways), 0), shininess);
		vec3 specular = vec3(light.specular) * specularPower * vec3(texture(albedo, TexCoords)) * light.specular.a;  
        
        // attenuation
        float distance    = length(vec3(light.position) - FragPos);
        float attenuation = 1.0 / (1 + light.linear * distance + light.quadratic * (distance * distance));    

        // ambient  *= attenuation; // remove attenuation from ambient, as otherwise at large distances the light would be darker inside than outside the spotlight due the ambient term in the else branche
        diffuse   *= attenuation;
        specular *= attenuation;   

        return diffuse + specular;
    }

    return vec3(0,0,0);
}

void main(){
	vec3 norm = normalize(vec3(texture(normal, TexCoords)));

	vec3 fragPos = vec3(texture(position, TexCoords));

	vec3 viewDir = normalize(viewPos - fragPos);

	vec3 resultColor = vec3(0,0,0);
	switch(light.type){
			case DIRECTIONAL:
				resultColor += calculateDirectionLight(light, norm, fragPos, viewDir) * ShadowCalculation(fragPos);
			break;
			case POINT:
				resultColor += calculatePointLight(light, norm, fragPos, viewDir) * ShadowCalculation(fragPos);
			break;
			case SPOT:
				resultColor += calculateSpotLight(light, norm, fragPos, viewDir) * ShadowCalculation(fragPos);
			break;
		}

	color = texture(framebuffer, TexCoords) + vec4(resultColor, 1.0);
	//vec3 fragToLight = fragPos - viewPos;
	//float closestDepth = texture(depthMap, fragToLight).r;
	//color = texture(framebuffer, TexCoords) + vec4(vec3(closestDepth/farPlane), 1.0);
}