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






