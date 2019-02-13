/*
MIT License

Copyright (c) 2019 MaticVrtacnik

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

//CONSTS===============================================================
#include "include/commons.glsl"
#include "include/PBR.glsl"
#include "include/shadows.glsl"

#define AMBIENT_COLOR_SUN
#define USE_SHADOWS
#define MAX_LIGHTS 10
#define AMBIENT_COLOR 0.15
#define GAMMA 2.2


//INPUTS===============================================================
in vec2 outUV;

//UNIFORMS=============================================================
layout (binding = 0) uniform samplerCube u_skyboxTexture;
layout (binding = 1) uniform sampler2D u_shadowTexture;
layout (binding = 2) uniform sampler2D u_locationTexture;
layout (binding = 3) uniform sampler2D u_normalTexture;
layout (binding = 4) uniform sampler2D u_albedoTexture;
layout (binding = 5) uniform sampler2D u_RMOETexture;
layout (binding = 6) uniform sampler2D u_SSAOTexture;

uniform mat4 u_shadowLightMatrix;
uniform mat4 u_inverseViewMatrix;
uniform mat3 u_inverseNormalMatrix;

uniform int u_numLights;
uniform struct Light{
	vec4 	location;
	vec3 	direction;
	float 	coneAngle;
	vec3 	color;
	float 	intensity;
	float 	quadricAttenuation;
	float 	linearAttenuation;
}u_lights[MAX_LIGHTS];

//OUTPUTS==============================================================
layout (location = 0) out vec4 o_fragColor;
layout (location = 1) out vec4 o_bloomTexture;


//CODE=================================================================
vec3 calculateSunColor(Light light, vec3 location, vec3 normal, vec3 albedo, vec3 envDiffuse, 
	vec3 envSpecular, float roughness, float metallic, float occlusion, float emissive);
vec3 calculateLighting(vec3 location, vec3 normal, vec3 albedo, vec3 envDiffuse, 
	vec3 envSpecular, float roughness, float metallic, float occlusion, float emissive);
	
	
vec3 getColorAt(vec2 uv){
	vec3 albedo = pow(texture(u_albedoTexture, uv, 0).rgb, vec3(GAMMA));
	float SSAO = texture(u_SSAOTexture, uv, 0).r;
	float occlusion = texture(u_RMOETexture, uv, 0).b;
	
	return vec3(pow(uv, vec2(214.0)), 0.0);
}

float getEdge(vec2 uv){
	const vec3 W = vec3(0.2125, 0.7154, 0.0721);

	const float imageHeightFactor = 1080.0;
	const float imageWidthFactor = 1920.0;
	
	const float bias = 1.0;
    vec2 stp0 = vec2(bias / imageWidthFactor, 0.0);
    vec2 st0p = vec2(0.0, bias / imageHeightFactor);
    vec2 stpp = vec2(bias / imageWidthFactor, bias / imageHeightFactor);
    vec2 stpm = vec2(bias / imageWidthFactor, -bias / imageHeightFactor);

    float i00   = dot(getColorAt(uv), W);
    float im1m1 = dot(getColorAt(uv - stpp).rgb, W);
    float ip1p1 = dot(getColorAt(uv + stpp).rgb, W);
    float im1p1 = dot(getColorAt(uv - stpm).rgb, W);
    float ip1m1 = dot(getColorAt(uv + stpm).rgb, W);
    float im10 = dot(getColorAt(uv - stp0).rgb, W);
    float ip10 = dot(getColorAt(uv + stp0).rgb, W);
    float i0m1 = dot(getColorAt(uv - st0p).rgb, W);
    float i0p1 = dot(getColorAt(uv + st0p).rgb, W);
    float h = -im1p1 - 2.0 * i0p1 - ip1p1 + im1m1 + 2.0 * i0m1 + ip1m1;
    float v = -im1m1 - 2.0 * im10 - im1p1 + ip1m1 + 2.0 * ip10 + ip1p1;

    float edge = 1.0 - length(vec2(h, v));
	
	return edge;
}


void main()
{
	

	vec3 location = textureLod(u_locationTexture, outUV, 0).xyz;
	vec3 normal = normalize(textureLod(u_normalTexture, outUV, 0).xyz);	
	vec3 albedo = pow(textureLod(u_albedoTexture, outUV, 0).rgb, vec3(GAMMA));
	float SSAO = textureLod(u_SSAOTexture, outUV, 0).r;
	
	
	vec4 roughnessMetallicOcclusionEmissive = textureLod(u_RMOETexture, outUV, 0).rgba;
	float roughness = roughnessMetallicOcclusionEmissive.r;
	float metallic = roughnessMetallicOcclusionEmissive.g;
	float occlusion = roughnessMetallicOcclusionEmissive.b * SSAO;
	float emissive = roughnessMetallicOcclusionEmissive.a * 5.0;
	
	//albedo = occlusion.rrr;

	//TODO actually calculate
	vec3 rawNormal = u_inverseNormalMatrix * normal;
	vec3 envDiffuse = pow(textureLod(u_skyboxTexture, rawNormal, 15).rgb, vec3(GAMMA));
	//vec3 envSpecular = envDiffuse;
	
	vec3 reflected = u_inverseNormalMatrix * reflect(normalize(-location), normal);
	float lodLevel = textureQueryLod(u_skyboxTexture, -reflected).y;
	lodLevel = max(roughness * 10.0, lodLevel);
	vec3 envSpecular = pow(textureLod(u_skyboxTexture, -reflected, lodLevel).rgb, vec3(GAMMA));
	
	
	float refractionFactor = 1.0 / 1.0;
	vec3 refracted = u_inverseNormalMatrix * refract(normalize(location), normal, refractionFactor);
	//albedo = pow(textureLod(u_skyboxTexture, refracted, 0).rgb, vec3(GAMMA));
	
	//roughness = 0.015;		
	//TODO get skybox emissive

	
	//COOL BUG
	//envDiffuse.rgb += reflection;


	o_fragColor.rgb = calculateSunColor(u_lights[0], location, normal, albedo, 
		envDiffuse, envSpecular, roughness, metallic, occlusion, emissive);
	o_fragColor.rgb += calculateLighting(location, normal, albedo, 
		envDiffuse, envSpecular, roughness, metallic, occlusion, emissive);
		
		
	//o_fragColor.rgb = albedo;
		
	//float edge = getEdge(outUV);	
	//o_fragColor.rgb = albedo * occlusion;
	
	//finalColor += reflection * (1.0 - roughness);
	o_fragColor.a = 1.0;
		
	//o_fragColor.rgb = albedo;
	
	//o_fragColor = vec4(occlusion.rrr, 1.0);
	//float shadowFactor = texture(shadowTexture, outUV).r; //get from sun
	
	
	
	//envDiffuse.rgb = vec3(0.0);
	//FOG
	const float fogIntensity = 0.005;
	float d = max(0.001, length(location.xyz) - 5.0);
	float fogFactor = smoothstep(0.0, 1.0, 1.0 / exp(d * fogIntensity));
	//fogFactor = 0.0;
	
	vec3 fogColor = pow(textureLod(u_skyboxTexture, -rawNormal, 15).rgb, vec3(GAMMA));
	fogColor = vec3(1.0);
	//o_fragColor.rgb = mix(pow(fogColor, vec3(1.0)), o_fragColor.rgb, pow(fogFactor, 5.0));
	
	//o_fragColor.rgb = envDiffuse.rgb;
	
	//o_fragColor.rgb = rawNormal;
	
	
	float brightness = pow(getBrightness(o_fragColor.rgb) * 5, 0.25);
	//o_bloomTexture = vec4(o_fragColor.rgb, shadowFactor);
	o_bloomTexture = vec4(o_fragColor.rgb * brightness, 1.0);
	//o_bloomTexture = vec4(brightness.rrr, 1.0);
	//o_bloomTexture = vec4(0, 0, 0, 1);
	
	o_bloomTexture.a += emissive;
	o_bloomTexture.rgb += albedo * emissive;
	
	//o_bloomTexture.rgb = vec3(0.0);
}


vec3 calculateSunColor(Light light, vec3 location, vec3 normal, vec3 albedo, vec3 envDiffuse, 
	vec3 envSpecular, float roughness, float metallic, float occlusion, float emissive)
{

#ifdef USE_SHADOWS
	vec4 lightVertex = u_shadowLightMatrix * vec4(location, 1.0);
	float shadowFactor = getShadowFactor(u_shadowTexture, lightVertex, 0.0);	
	//shadowFactor = max(0.0001, shadowFactor);

#else
	float shadowFactor = 1.0;
#endif
	
	vec3 specular = mix(vec3(0.04), albedo, metallic);

	vec3 diffLight = envDiffuse; //texture(indirectLightTexture, outUV).rgb;
	vec3 envLightDiffuse = diffLight; // * light.color;
	
	float environmentBrightness = getBrightness(envDiffuse);		
	
	//diffLight *= envShadowFactor;
	//envLightDiffuse *= envShadowFactor;
	
#ifdef AMBIENT_COLOR_SUN
	vec3 lightColor = light.intensity * envDiffuse.rgb + light.color * light.intensity * 0.5;
	//lightColor = light.intensity * envDiffuse.rgb;
	//vec3 lightColor = light.intensity * (envDiffuse.rgb + light.color);
	//vec3 lightColor = light.intensity * light.color + envDiffuse.rgb;

	//lightColor = light.intensity * light.color;
#else
	vec3 lightColor = light.intensity * light.color;
	//vec3 lightColor = light.intensity * envDiffuse.rgb + light.color * light.intensity * 0.5;
#endif //AMBIENT_COLOR_SUN

	
	vec3 surfaceToView = normalize(-location); 
	vec3 surfaceToLight = normalize(light.location.xyz - location);
	vec3 halfVector = normalize(surfaceToLight + surfaceToView);
	
	//Cell shading
	const float levels = 6;
	const float levelsScale = 1.0 / levels;

	float NdL = dot(normal, surfaceToLight);
	//NdL = max(NdL, dot(normal, -surfaceToLight));
	//NdL = floor(NdL * levels) * levelsScale;
	
	//NdL = 1.0;
	//shadowFactor = 1.0;
	float rawNdotL = max(emissive, NdL * shadowFactor);
	
	//albedo = vec3(1.0);
	
	float NdotL = max(0.0001, rawNdotL);
	float NdotV = max(0.0001, dot(normal, surfaceToView));	
	float NdotH = max(0.0001, dot(normal, halfVector));
	float HdotV = max(0.0001, dot(halfVector, surfaceToView));
	

	//vec3 fresnelF = fresnelFactor(specular, pow(HdotV, 0.1));
	//vec3 fresnelF = fresnelFactor(specular, HdotV);
	vec3 fresnelF = fresnelFactor(specular, pow(HdotV, 1.0 - roughness));
	//vec3 fresnelF = fresnelSchlickRoughness(NdotV, specular, roughness); //will blackscreen??? TODO fix??
	
	vec3 specularReflection = cooktorranceSpecular(
		NdotL, NdotV, NdotH, fresnelF, roughness);
	specularReflection = specularReflection * rawNdotL;
	
	
	environmentBrightness = max(NdotL, environmentBrightness);
	float envShadowFactor = max(shadowFactor, environmentBrightness);
	
	
	
	vec3 diffusedLight = vec3(0.07 * occlusion);
	vec3 reflectedLight = vec3(0.0);
	
	vec3 diffuseReflection = (vec3(1.0) - fresnelF) * INVERSE_PI * rawNdotL;
	reflectedLight += specularReflection * lightColor * occlusion;
	diffusedLight += diffuseReflection * lightColor * occlusion;
	
	float glossFactor = 1.0 - roughness;
	//vec3 fresnelReflection = fresneSlF;
	vec3 fresnelReflection = specular * glossFactor;

	//vec3 fresnelReflection = fresnelFactor(specular, pow(NdotV, glossFactor)) * glossFactor;
	//vec3 fresnelReflection = fresnelFactor(specular, pow(NdotV, 1.0 - roughness));
	//vec3 fresnelReflection = fresnelSchlickRoughness(specular, HdotV, roughness);
	
	reflectedLight += envSpecular.rgb * fresnelReflection;
	//reflectedLight += diffLight * fresnelReflection * envShadowFactor;
	//reflectedLight += diffLight * fresnelReflection * envShadowFactor;
	//diffusedLight += envDiffuse * INVERSE_PI * occlusion;
	diffusedLight += envDiffuse * INVERSE_PI;
		
	//vec3 diffusedFactor = mix(albedo, vec3(0.0), metallic);	
	vec3 diffusedFactor = mix(albedo, vec3(0.0), metallic);	
	vec3 combinedLight = diffusedLight * diffusedFactor + reflectedLight;
	
	//albedo = vec3(1.0);
	//albedo += vec3(NdotL, 0.0, 0);
	//albedo = vec3(4,63,140) / 255.0
	
	//return occlusion.rrr;
	//return rawNdotL.rrr;
	//return albedo * occlusion.r;
	//return albedo * occlusion * NdotL;
	//return shadowFactor.rrr; 
	return combinedLight;
}

vec3 calculateLighting(vec3 location, vec3 normal, vec3 albedo, vec3 envDiffuse, 
	vec3 envSpecular, float roughness, float metallic, float occlusion, float emissive)
{
	vec3 specular = mix(vec3(0.04), albedo, metallic);
	
	vec3 surfaceToView = normalize(-location);
	float NdotV = max(0.0001, dot(normal, surfaceToView));
		
	
	vec3 finalColor = vec3(0.0);
	for(uint i = 1; i < u_numLights; ++i){
		vec3 vertexToLight = u_lights[i].location.xyz - location;	
		float distanceToLight = length(vertexToLight);
		float distanceToLight2 = distanceToLight * distanceToLight;

		const float cAttenuation = 1.0;
		const float lAttenuation = u_lights[i].linearAttenuation * distanceToLight;
		const float qAttenuation = u_lights[i].quadricAttenuation * distanceToLight2;	
		const float combinedAttenuation = cAttenuation + lAttenuation + qAttenuation;
		float attenuation = u_lights[i].intensity / combinedAttenuation;
	
		if(u_lights[i].coneAngle > 0.0){
			float LdotD = dot(-normalize(vertexToLight), u_lights[i].direction);
			float edge = 1.0 - step(u_lights[i].coneAngle, degrees(acos(LdotD)));
			attenuation *= edge;
		}
		
		vec3 lightColor = u_lights[i].color * attenuation;

		vec3 surfaceToLight = normalize(vertexToLight);
		vec3 halfVector = normalize(surfaceToLight + surfaceToView);
		
		float NdotL = dot(normal, surfaceToLight);
		NdotL = max(0.0001, max(0.1, NdotL) * step(0.0, NdotL));
		 
		float NdotH = max(0.0001, dot(normal, halfVector));
		float HdotV = max(0.0001, dot(halfVector, surfaceToView));
		

		//vec3 fresnelF = fresnelFactor(specular, HdotV);
		vec3 fresnelF = fresnelFactor(specular, pow(HdotV, 1.0 - roughness));
		vec3 specularReflection = cooktorranceSpecular(
			NdotL, NdotV, NdotH, fresnelF, roughness);
		specularReflection *= vec3(NdotL);
		
		vec3 diffuseReflection = (vec3(1.0) - fresnelF) * INVERSE_PI * NdotL;		
		vec3 reflectedLight = specularReflection * lightColor;
		vec3 diffusedLight = diffuseReflection * lightColor;
		
		vec3 diffusedFactor = mix(albedo, vec3(0.0), metallic);
		finalColor += (diffusedLight * diffusedFactor + reflectedLight) * occlusion;
	}

	return finalColor;
}
