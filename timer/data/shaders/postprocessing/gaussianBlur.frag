
layout (location = 0) out vec4 fragColor;

in vec2 outUV;

layout (binding = 0) uniform sampler2D screenTexture;
uniform int horizontal;

const int NUM_WEIGHTS = 5;
const float weights[NUM_WEIGHTS] = float[] 
	(0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
	//(0.20, 0.15, 0.11, 0.09, 0.05);

void main()
{
	vec2 pixelSize = 1.0 / textureSize(screenTexture, 0).xy;
	vec4 finalColor = texture(screenTexture, outUV).rgba * weights[0];
	
	if(horizontal == 1){
		for(int i = 1; i < NUM_WEIGHTS; ++i){
			vec2 offset = vec2(pixelSize.x * i, 0.0);
			finalColor += texture(screenTexture, outUV + offset).rgba * weights[i];
			finalColor += texture(screenTexture, outUV - offset).rgba * weights[i];	
		}
	}else{
		for(int i = 1; i < NUM_WEIGHTS; ++i){
			vec2 offset = vec2(0.0, pixelSize.y * i);
			finalColor += texture(screenTexture, outUV + offset).rgba * weights[i];
			finalColor += texture(screenTexture, outUV - offset).rgba * weights[i];		
		}
	}
		
	fragColor = finalColor;
	//fragColor.rgb = vec3(1.0, 1.0, 0);
}






