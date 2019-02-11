#ifndef COMMONS_GLSL
#define COMMONS_GLSL

//CONSTS=======================================================================
#define NEAR_PLANE 0.1 //TODO uniforms
#define FAR_PLANE 1000.0 //TODO uniforms


//CODE=========================================================================
float linearizeDepth(float value)
{
	float NDC = value * 2.0 - 1.0;
	float planeDiff = FAR_PLANE - NEAR_PLANE;
	float planeProduct = NEAR_PLANE * FAR_PLANE;
	float planeSum = NEAR_PLANE + FAR_PLANE;
	
	return (2.0 * planeProduct) / (planeSum - NDC * planeDiff);
}


//TODO convert gBuffer
vec3 PositionFromDepth(float depth, vec2 uv, mat4 inverseProjectionMatrix)
{
    float z = depth * 2.0 - 1.0;

    vec4 clipSpacePosition = vec4(uv * 2.0 - 1.0, z, 1.0);
    vec4 viewSpacePosition = inverseProjectionMatrix * clipSpacePosition;

    // Perspective division
    viewSpacePosition /= viewSpacePosition.w;

    return viewSpacePosition.xyz;
}

float rand(vec2 co)
{
    return fract(sin(dot(vec2(floor(co.x),floor(co.y)) ,vec2(12.9898,78.233))) * 43758.5453);
}

vec3 correctGamma(vec3 color, float gamma)
{
    return pow(color, vec3(1.0 / gamma));
}

float saturate(float x)
{
	return clamp(x, 0.0, 1.0);
}

float getBrightness(vec3 x)
{
	return dot(x, vec3(0.2125, 0.7154, 0.0721));
}

vec3 saturation(vec3 x, float adjustment)
{
    vec3 brightness = vec3(getBrightness(x));
    return mix(brightness, x, adjustment);
}


#endif //COMMONS_GLSL