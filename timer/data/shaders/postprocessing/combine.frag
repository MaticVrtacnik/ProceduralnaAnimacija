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






