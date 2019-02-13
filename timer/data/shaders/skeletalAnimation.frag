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

//CONSTS=======================================================================
#include "include/commons.glsl"

#define SAMPLE_BIAS 0

//INPUTS=======================================================================
in block{
	vec4 velocityVertex;
	vec4 prevVelocityVertex;	
	vec3 vertex;
	vec3 modelVertex;
	vec3 normal;
	vec3 tangent;
	vec2 uv;		
	
	vec3 color;
	vec3 heightColor;
}IN;

//TEXTURES=====================================================================
layout (binding = 0) uniform sampler2D u_albedoTexture;
layout (binding = 1) uniform sampler2D u_normalTexture;
layout (binding = 2) uniform sampler2D u_roughnessTexture;
layout (binding = 3) uniform sampler2D u_metallicTexture;
layout (binding = 4) uniform sampler2D u_occlusionTexture;
layout (binding = 5) uniform sampler2D u_heightTexture;

//UNIFORMS=====================================================================
uniform vec3 u_color;

//OUTPUTS======================================================================
layout (location = 0) out vec4 o_locationDepth;
layout (location = 1) out vec3 o_normal;
layout (location = 2) out vec2 o_velocity;
layout (location = 3) out vec4 o_albedo;
layout (location = 4) out vec4 o_RMOE;


//CODE=========================================================================
void main()
{	
	o_locationDepth = vec4(IN.vertex, linearizeDepth(gl_FragCoord.z));
	o_locationDepth = vec4(0,0,0,0);
	
	float heightFactor = (IN.modelVertex.z + 30.0) / 60.0;
	
	vec3 albedo = texture(u_albedoTexture, IN.uv).rgb;
	o_albedo = vec4(albedo + u_color, 1.0);
	//o_albedo.rgb = vec3(0.25, 0.7, 0.4);
	o_albedo = vec4(0.15, 0.026, 0.075, 1.0);
	//o_albedo = vec4(0.85, 0.21, 0.66, 1.0);
	o_albedo = vec4(0.7);
	
	//o_albedo = vec4(0);
	//o_albedo = vec4(1);
	//o_albedo = vec4(IN.color, 1.0);
	//o_albedo = vec4(IN.uv, 0.0, 1.0);
	//o_albedo = vec4(0.0, 1.0, 0.0, 1.0);
	//o_albedo = vec4(heightFactor, 0.0, 1.0 - heightFactor, 1.0) * 0.8;
	
	//o_albedo = vec4(IN.heightColor, 1.0) * 0.8;
	
	//o_albedo = vec4(0.8,0.9,0,1);
	//o_albedo = vec4(0.9, 0.9, 0.9, 1.0);
	
	vec3 textureNormal = normalize(texture(u_normalTexture, IN.uv, 0).rgb * 2.0 - 1.0);
	vec3 normal = normalize(IN.normal);
	vec3 tangent = normalize(IN.tangent);
	mat3 TBN = mat3(tangent, cross(tangent, normal), normal);
	o_normal = TBN * textureNormal;
	o_normal = normal;
	//o_normal = vec3(0.0, 1.0, 0.0);
	
	vec2 currVertex = (IN.velocityVertex.xy / IN.velocityVertex.w) * 0.5 + 0.5;
	vec2 prevVertex = (IN.prevVelocityVertex.xy / IN.prevVelocityVertex.w) * 0.5 + 0.5;
	o_velocity = (currVertex - prevVertex) * 0.25f;
	o_velocity = vec2(0.0);

	//Will blackscreen without certain minimum
	//o_RMOE.r = max(0.015, pow(texture(u_roughnessTexture, IN.uv, SAMPLE_BIAS).r, 1.0 / 2.2));	
	o_RMOE.r = max(0.015, texture(u_roughnessTexture, IN.uv * 3, SAMPLE_BIAS ).r); //ROUGHNESS	
	o_RMOE.g = 0.0; texture(u_metallicTexture, IN.uv, SAMPLE_BIAS).r; //METALLIC
	o_RMOE.b = texture(u_occlusionTexture, IN.uv, SAMPLE_BIAS).r; //OCCLUSION
	o_RMOE.a = 0.0; //dot(u_color, vec3(0.21, 0.71, 0.08)); //EMISSIVE
	
	//o_RMOE.r = smoothstep(0.2, 1.0, o_RMOE.r);
	//o_RMOE.r = pow(o_RMOE.r, 0.7);
	//o_RMOE.r = 0.85;
	
	//o_albedo.rgb = pow(o_RMOE.r, 2.0).rrr * 0.25;
}
