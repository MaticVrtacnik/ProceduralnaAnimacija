
layout (location = 0) out vec4 fragColor;

in vec2 outUV;

layout (binding = 0) uniform sampler2D screenTexture;
layout (binding = 1) uniform sampler2D velocityTexture;

//uniform float velocityScale; //TODO adjust for FPS
const int MAX_SAMPLES = 8;

uniform float fpsFactor;

void main()
{	
	vec2 pixelSize = 1.0 / textureSize(screenTexture, 0);
	
	vec2 velocity = texture(velocityTexture, outUV).rg;
	velocity *= 0.3/*fpsFactor*/; //0.5
	
	float speed = length(velocity / pixelSize);
	int numSamples = clamp(int(speed), 1, MAX_SAMPLES);
	
	vec3 finalColor = texture(screenTexture, outUV).rgb;
	for(int i = 1; i < numSamples; ++i){
		vec2 offset = velocity * (float(i) / float(numSamples - 1) - 0.5);
		finalColor += texture(screenTexture, outUV + offset).rgb;
	}
	
	finalColor /= float(numSamples);
	fragColor = vec4(finalColor, 1.0);
}






