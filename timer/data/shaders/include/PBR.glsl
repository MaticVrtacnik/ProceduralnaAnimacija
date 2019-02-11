#ifndef PBR_GLSL
#define PBR_GLSL

//CONSTS=======================================================================
#define PI 3.141592653589
#define INVERSE_PI 0.318309886183


//CODE=========================================================================
vec3 fresnelFactor(vec3 F0, float product)
{
    return F0 + (1.0 - F0) * pow(1.01 - product, 5.0);
}

vec3 fresnelSchlickRoughness(vec3 F0, float cosTheta, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
} 

float GGX(in float roughness, in float NdotH)
{
    float m = roughness * roughness;
    float m2 = m * m;
    float d = (NdotH * m2 - NdotH) * NdotH + 1.0;
    return m2 / (PI * d * d);
}

float schlickG(in float roughness, in float NdotV, in float NdotL)
{
    float k = roughness * roughness * 0.5;
    float V = NdotV * (1.0 - k) + k;
    float L = NdotL * (1.0 - k) + k;
    return 0.25 / (V * L);
}

vec3 cooktorranceSpecular(in float NdotL, in float NdotV, 
	in float NdotH, in vec3 specular, in float roughness)
{
    float D = GGX(roughness, NdotH);
    float G = schlickG(roughness, NdotV, NdotL);
    float rim = mix(1.0 - roughness * 0.9, 1.0, NdotV);
    return (1.0 / rim) * specular * G * D;
}


#endif //PBR_GLSL