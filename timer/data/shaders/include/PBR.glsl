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


#ifndef PBR_GLSL
#define PBR_GLSL

//CONSTS=======================================================================
#define PI 3.141592653589
#define INVERSE_PI 0.318309886183


//CODE=========================================================================
vec3 fresnelFactor(vec3 F0, float product)
{
    return F0 + (1.0 - F0) * pow(1.01 - product, 5.0);
}

vec3 fresnelSchlickRoughness(vec3 F0, float cosTheta, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
} 

float GGX(in float roughness, in float NdotH)
{
    float m = roughness * roughness;
    float m2 = m * m;
    float d = (NdotH * m2 - NdotH) * NdotH + 1.0;
    return m2 / (PI * d * d);
}

float schlickG(in float roughness, in float NdotV, in float NdotL)
{
    float k = roughness * roughness * 0.5;
    float V = NdotV * (1.0 - k) + k;
    float L = NdotL * (1.0 - k) + k;
    return 0.25 / (V * L);
}

vec3 cooktorranceSpecular(in float NdotL, in float NdotV, 
	in float NdotH, in vec3 specular, in float roughness)
{
    float D = GGX(roughness, NdotH);
    float G = schlickG(roughness, NdotV, NdotL);
    float rim = mix(1.0 - roughness * 0.9, 1.0, NdotV);
    return (1.0 / rim) * specular * G * D;
}


#endif //PBR_GLSL