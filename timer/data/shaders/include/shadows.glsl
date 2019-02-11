#ifndef SHADOWS_GLSL
#define SHADOWS_GLSL

//CONSTS=======================================================================
#define VARIANCE_SHADOW
//#define EXPONENTIAL_SHADOW
//#define BASIC_SHADOW


//CODE=========================================================================
highp float linearStep(float a, float b, float value)
{
	return clamp((value - a) / (b - a), 0.0, 1.0);
}

/*highp float map_01(float x, float v0, float v1)
{
  return (x - v0) / (v1 - v0);
}*/


float getShadowFactor(sampler2D shadowTexture, vec4 vertex, float lowerLimit = 0.0)
{
#ifdef VARIANCE_SHADOW
	{
		vec3 lightVertex = vertex.xyz / vertex.w;
		lightVertex = lightVertex * 0.5 + 0.5;

		vec2 moments = texture(shadowTexture, lightVertex.xy).xy;
		
		float p = step(lightVertex.z, moments.x);
		//float variance = max(moments.y - moments.x * moments.x, 0.0000002);
		float variance = max(moments.y - moments.x * moments.x, 0.0000002);
		
		float d = lightVertex.z - moments.x;
		//float pMax = linearStep(0.96, 1.0, variance / (variance + d * d));
		float pMax = linearStep(0.5, 1.0, variance / (variance + d * d));
		
		return clamp(max(p, pMax), lowerLimit , 1.0);
	}
#elif defined(EXPONENTIAL_SHADOW) 
	{	
		/*vec3 lightUV = (outLightVertex.xyz / outLightVertex.w) * 0.5 + 0.5;

		const vec3 lightLocation = vec3(viewMatrix * vec4(150.0, 200.0, 100.0, 1.0));
		float distanceToLight = distance(outVertex, lightLocation);
		distanceToLight = (distanceToLight - NEAR_PLANE) / (FAR_PLANE - NEAR_PLANE);
		
		
		float occluder = texture(shadowTexture, lightUV.xy).x;
		
		float diff = occluder - distanceToLight;
		
		float EXPONENT = 50.0;
		float shadow = clamp(exp(EXPONENT * diff), 0.0, 1.0);
		shadow = pow(shadow + 1.0, 12.0) - 1.0;
		shadow = smoothstep(0.7, 1.0, shadow);*/
		

		vec3 lightUV = (vertex.xyz / vertex.w) * 0.5 + 0.5;
		
		float occluder = texture(shadowTexture, lightUV.xy).x;
		//return occluder;
		
		const float LIGHT_NEAR = 0.1f;
		const float LIGHT_FAR = 100.0f;
		const float EXPONENT = 10.0f;
		
		
		float lVertexDepth = (lightUV.z - NEAR_PLANE) / (FAR_PLANE - NEAR_PLANE);
		
		float diff = abs(occluder - lVertexDepth);
		float visible = 0.05 / (1.0 + pow(saturate(exp(-EXPONENT * diff)), 1.0));
		
		return visible;
		
		//float visible = saturate(exp(-
		
		//float receiver = map_01(vertex.z, LIGHT_NEAR, LIGHT_FAR);
		

		float receiver = linearStep(NEAR_PLANE, FAR_PLANE, vertex.z);
		//return linearVertex * 1000.0;
		
		//float diff = occluder - linearVertex;
		//return pow(diff, 0.5);
		

		//float receiver = saturate(exp(EXPONENT * diff));
		//return receiver * 0.2;
		
		//float visible = clamp(occluder * exp(-EXPONENT * receiver), 0.0, 1.0);
		
		//float visible = clamp(exp(EXPONENT * receiver), 0.0, 1.0);
		
		
		return visible;
		//return max(lowerLimit, visible);	
	}
#elif defined(BASIC_SHADOW) 
	#define PCF
	#define MIN_SHADOW_BIAS 0.0000015
	#define MAX_SHADOW_BIAS 0.0000030
	{
		/*float finalBias = clamp( maxShadowBias * 
		   tan(acos(clamp(dot(lights[0].position.xyz, normal), 0.0, 1.0))), 
			minShadowBias, maxShadowBias);*/

		float shadow = 0.0;
		vec3 lightVertex = vertex.xyz / vertex.w;
	#ifdef PCF
		for(float offsetX = -0.0001; offsetX <= 0.0001; offsetX += 0.00005){
			for(float offsetY = -0.0001; offsetY <= 0.0001; offsetY += 0.00005){
				vec2 offset = vec2(offsetX, offsetY);
				float z = texture(shadowTexture, lightVertex.xy + offset).r;
				shadow += z > (lightVertex.z - bias) ? 1.0 : 0.0;
			}
		}shadow /= 25.0;
	#else //!PCF
		float z = texture(shadowTexture, lightVertex.xy).z;
		shadow = z > (lightVertex.z - bias) ? 1.0 : 0.0;
		
	#endif //PCF
	
		return max(lowerLimit, shadow);		
	}
#else
	{
		return 1.0;
	}
	
#endif //SHADOW_TYPE
}


#endif //SHADOWS_GLSL