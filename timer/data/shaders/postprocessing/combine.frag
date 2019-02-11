
layout (location = 0) out vec4 fragColor;

noperspective in vec2 outUV;

//uniform sampler2D screenTexture;
layout (binding = 0) uniform sampler2D screenTexture;
layout (binding = 2) uniform sampler2D lightScatteringTexture;
layout (binding = 3) uniform sampler2D sunScatteringTexture;

vec3 saturation(vec3 rgb, float adjustment){
    const vec3 W = vec3(0.2125, 0.7154, 0.0721);
    vec3 intensity = vec3(dot(rgb, W));
    return mix(intensity, rgb, adjustment);
}

void main()
{
	vec3 sunScatteringColor = texture(sunScatteringTexture, outUV).rgb;
	sunScatteringColor = pow(sunScatteringColor, vec3(2.2)) * 1.0;
	//sunScatteringColor = saturation(sunScatteringColor, 0.0);
	
	vec3 screenColor = texture(screenTexture, outUV).rgb;
	
	fragColor.rgb = screenColor;
	fragColor.rgb += sunScatteringColor;
	fragColor.a = 1.0;
}






