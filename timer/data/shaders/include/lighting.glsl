#ifndef LIGHTING_GLSL
#define LIGHTING_GLSL


//CODE=========================================================================
float rand(vec2 co){
    return fract(sin(dot(vec2(floor(co.x),floor(co.y)) ,vec2(12.9898,78.233))) * 43758.5453);
}

vec3 correctGamma(vec3 color, float gamma){
    return pow(color, vec3(1.0 / gamma));
}

vec3 calculateColor(Light light, vec3 textureColor, vec3 normal, vec3 vertexPosition, vec3 surfaceToView, float specularIntensity = 1.0)
{ 
	vec3 surfaceToLight = vec3(0.0);
	float attenuation = 0.0;
	float intensity = 1.0;
	
	if(light.location.w == 0.0) //directional light
	{
		surfaceToLight = normalize(light.location.xyz);
		attenuation = 1.0;
		intensity = 1.0; //light.intensity / 100.0;
	}
	else // point light
	{
		surfaceToLight = normalize(light.location.xyz - vertexPosition);
		float distanceToLight = length(light.location.xyz - vertexPosition);
		attenuation = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 2.0));
		intensity = light.intensity;

		//cone
		if(light.coneAngle > 0.0){
			float lightToSurfaceAngle = degrees(acos(dot(-surfaceToLight, normalize(light.direction))));
			if(lightToSurfaceAngle > light.coneAngle)	
				attenuation = 0.0;
		}
	}

	float diffuseContribution = max(0.0,dot(normal, surfaceToLight));
	vec3 diffuse = textureColor.rgb * diffuseContribution  * light.color.rgb * intensity;
	
	
	//PHONG
	/*vec3 reflectDirection = reflect(-surfaceToLight, normal);
	float specularContribution = pow(max(0.0, dot(surfaceToView, reflectDirection)), light.shininess);*/
	
	//BLINN-PHONG
	vec3 halfVec = normalize(surfaceToLight + surfaceToView);
	float specularContribution = pow(max(0.0, dot(halfVec, normal)), light.shininess);
	
	vec3 specular = textureColor.rgb * specularContribution * light.color.rgb * intensity;
	specular *= specularIntensity;

	return diffuse * attenuation + specular * attenuation;
}

vec3 calcFinalColor(vec3 color, vec3 normal, vec3 vertex, vec3 surfaceToView, float specularIntensity = 1.0)
{
	vec3 finalColor = vec3(0.0);
	vec3 ambient = color.rgb * ambientColor;
	
	//return ambient;

	for(int i = 0; i < numLights; i++){
		finalColor += calculateColor(lights[i], color, normal, vertex, surfaceToView, specularIntensity);
	}
	
	return finalColor + ambient;
}


#endif //LIGHTING_GLSL