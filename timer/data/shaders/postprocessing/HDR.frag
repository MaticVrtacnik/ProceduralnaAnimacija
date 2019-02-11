
#include "include/toneMapping.glsl"

#define BLOOM

layout (location = 0) out vec4 fragColor;
layout (location = 1) out float average;

in vec2 outUV;

layout (binding = 0) uniform sampler2D screenTexture;
layout (binding = 1) uniform sampler2D bloomTexture1;
layout (binding = 2) uniform sampler2D bloomTexture2;
layout (binding = 3) uniform sampler2D bloomTexture3;
layout (binding = 4) uniform sampler2D bloomTexture4;
layout (binding = 5) uniform sampler2D previousAverage;

const float interpolationFactor = 0.05;
uniform float exposureFactor;
uniform float fpsFactor;

float getBrightness(vec3 color){
	return dot(color, vec3(0.2125, 0.7154, 0.0721));
}

vec3 saturation(vec3 rgb, float adjustment){ 
    return mix(getBrightness(rgb).rrr, rgb, adjustment);
}


float getVignetteFactor(vec2 uv)
{
	const float RADIUS = 0.9;
	const float SOFTNESS = 0.4;

	float distanceFromCenter = length(uv - vec2(0.5));
	float vignette = smoothstep(RADIUS, RADIUS - SOFTNESS, distanceFromCenter);
	
	//return 1.0;
	return vignette;
}

vec3 sepia(vec3 color, float f){
	vec3 newColor;
	newColor.r = min(1.0, (color.r * (1.0 - (0.607 * f))) + (color.g * (0.769 * f)) + (color.b * (0.189 * f)));
	newColor.g = min(1.0, (color.r * 0.349 * f) + (color.g * (1.0 - (0.314 * f))) + (color.b * 0.168 * f));
	newColor.b = min(1.0, (color.r * 0.272 * f) + (color.g * 0.534 * f) + (color.b * (1.0 - (0.869 * f))));
	
	return newColor;
}

vec3 colorGrade(vec3 color, float f){
	//const vec3 W = vec3(0.2125, 0.7154, 0.0721);
	const vec3 W = vec3(0.2125, 0.0721, 0.7154);
	float brightness = dot(color, W);
	
    return mix(brightness.rrr, color, f);
}

void main()
{
	vec3 averageColor = textureLod(screenTexture, outUV, 16).rgb;
	//vec3 averageColor = textureLod(screenTexture, vec2(0.5), 0).rgb;
	vec3 textureColor = textureLod(screenTexture, outUV, 0).rgb;	
	
	averageColor = pow(averageColor, vec3(1.0 / 2.2));
	float avgBrigthness = dot(averageColor, vec3(0.21, 0.71, 0.08));
	//avgBrigthness = max(0.0, avgBrigthness);
	//avgBrigthness = max(0.2, avgBrigthness);
	
	//avgBrigthness = clamp(avgBrigthness, 0.025, 2.0);
	

#ifdef BLOOM
	/*const float bloomFactor = 0.125;
	vec4 bloom1 = texture(bloomTexture1, outUV);
	vec4 bloom2 = texture(bloomTexture2, outUV);
	vec4 bloom3 = texture(bloomTexture3, outUV);
	vec4 bloom4 = texture(bloomTexture4, outUV);//*/

	const float bloomFactor = 1.25;
	vec4 bloom1 = texture(bloomTexture1, outUV) * 0.3;
	vec4 bloom2 = texture(bloomTexture2, outUV) * 0.3;
	vec4 bloom3 = texture(bloomTexture3, outUV) * 0.2;
	vec4 bloom4 = texture(bloomTexture4, outUV) * 0.2;//*/
	
	vec3 bloomColor = bloom1.rgb * bloom1.a * 1.0;
	bloomColor += bloom2.rgb * bloom2.a;
	bloomColor += bloom3.rgb * bloom3.a;
	bloomColor += bloom4.rgb * bloom4.a;

	//exp2(bloomColor)
	vec3 combinedBloom = bloomColor * bloomFactor;
	combinedBloom = saturation(combinedBloom, 1.5);
	//combinedBloom = saturation(combinedBloom, 1.1);
	textureColor += combinedBloom;	
	
#endif //BLOOM

	//SATURATE
	//textureColor = czm_saturation(textureColor, 1.0);
	
	
	float previousAvgBrigthness = texture(previousAverage, outUV).r;
	
	float interpolationVelocity = interpolationFactor * fpsFactor;	
	float change = (avgBrigthness - previousAvgBrigthness) * interpolationVelocity;
	avgBrigthness =  max(previousAvgBrigthness + change, 0.00001f);
	
	//float exposure = log2(max(exposureFactor / avgBrigthness, 0.00001f));
	//exposure = exp2(exposure);
	
	

	//float exposure = exposureFactor / avgBrigthness;
	float exposure = exposureFactor * 2.5;
	
	fragColor.rgb = toneMapping(textureColor, exposure);
	fragColor.rgb = saturation(fragColor.rgb, 1.4);
	
	//fragColor.rgb = colorGrade(fragColor.rgb, 1.0);
	
	
	//fragColor.rgb = sepia(fragColor.rgb, 0.5);
	
	
	//fragColor.rgb = step(0.75, getBrightness(fragColor.rgb)).rrr;
	
	//fragColor.rgb *= getVignetteFactor(outUV);

	 //FXAA luma
	//vec3 linear = pow(fragColor.rgb, vec3(2.2));
	fragColor.a = dot(fragColor.rgb, vec3(0.299, 0.587, 0.114));
	fragColor.a = 1.0;

	
	//fragColor = vec4(textureColor, 1.0);
	average = avgBrigthness;
}
