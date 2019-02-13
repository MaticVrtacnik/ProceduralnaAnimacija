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

//INPUTS=======================================================================
in block{
	vec3 vertex;
	vec3 normal;
	vec3 cubemapUv;
	vec2 uv;
}IN;

//TEXTURES=====================================================================
layout (binding = 0) uniform samplerCube u_cubemapTexture;
layout (binding = 1) uniform sampler2D u_diffuseTexture; 

//UNIFORMS=====================================================================
uniform vec3 u_cameraLocation;

//OUTPUTS======================================================================
layout (location = 0) out vec4 o_fragColor;


//CODE=========================================================================
void main()
{
	const float refractionFactor = 1.0 / 1.31;
	vec3 incident = normalize(IN.vertex - u_cameraLocation);
	vec3 normal = normalize(IN.normal);
	
	vec3 reflected = reflect(incident, normal);
	vec3 refracted = refract(incident, normal, refractionFactor);
	
	vec4 textureColor = texture(u_diffuseTexture, IN.uv);
	float grayscale = dot(textureColor.rgb, vec3(0.33333));
	
	o_fragColor = texture(u_cubemapTexture, reflected);
	o_fragColor = mix (o_fragColor, textureColor, 1 - grayscale);
	o_fragColor = texture(u_cubemapTexture, IN.cubemapUv);
	//o_fragColor = vec4(normal, 1.0);
}