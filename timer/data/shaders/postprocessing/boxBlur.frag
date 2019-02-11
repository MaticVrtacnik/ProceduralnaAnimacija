
layout (location = 0) out vec4 fragColor;

in vec2 outUV;

layout (binding = 0) uniform sampler2D screenTexture;

#define NUM_HALF_DIR_SAMPLES 2
#define NUM_DIR_SAMPLES (NUM_HALF_DIR_SAMPLES * 2 + 1)
#define NUM_SAMPLES (NUM_DIR_SAMPLES * NUM_DIR_SAMPLES)

void main()
{
	vec2 pixelSize = 1.0 / textureSize(screenTexture, 0).xy;
	vec3 finalColor = vec3(0.0);
	
	#pragma unroll
	for(int i = -NUM_HALF_DIR_SAMPLES; i <= NUM_HALF_DIR_SAMPLES; ++i){
		for(int j = -NUM_HALF_DIR_SAMPLES; j <= NUM_HALF_DIR_SAMPLES; ++j){
			vec2 offset = vec2(pixelSize.x * float(j), pixelSize.y * float(i));
			finalColor += texture(screenTexture, outUV + offset).rgb;
		}
	}
	
	fragColor = vec4(finalColor.rgb / float(NUM_SAMPLES), 1.0);
}






