#ifndef TONE_MAPPING_GLSL
#define TONE_MAPPING_GLSL

//CONSTS=======================================================================
//#define REINHARD_TONEMAP //washed out
//#define UNCHARTED_TONEMAP
#define ACES_TONEMAP
//#define ACES_TONEMAP_ADVANCED
//#define FILMIC_TONEMAP


//CODE=========================================================================
vec3 toneMapping(vec3 HDRcolor, float exposure, const float gamma = 1.0 / 2.2)
{
#ifdef REINHARD_TONEMAP
	{
		vec3 exposedColor = HDRcolor * exposure;
		vec3 LDRcolor = vec3(1.0) - exp(-exposedColor);
		LDRcolor = pow(LDRcolor, vec3(gamma));

		return LDRcolor;
	}
#elif defined(UNCHARTED_TONEMAP)
	{
		const float A = 0.15;
		const float B = 0.50;
		const float C = 0.10;
		const float D = 0.20;
		const float E = 0.02;
		const float F = 0.30;
		const float W = 11.2;
		const float exposureBias = 16.0;

		#define Uncharted2Tone(x)\
			((x *(A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F
	
		vec3 exposedColor = HDRcolor * exposure * exposureBias;
		vec3 LDRcolor = Uncharted2Tone(exposedColor) / Uncharted2Tone(vec3(W));
		LDRcolor = pow(LDRcolor, vec3(gamma));

		return LDRcolor;
	}
#elif defined(ACES_TONEMAP)
	{
		const float A = 2.51;
		const float B = 0.03;
		const float C = 2.43;
		const float D = 0.59;
		const float E = 0.14;
	
		vec3 exposedColor = HDRcolor * exposure;
		vec3 LDRcolor = exposedColor * (A * exposedColor + B);
		LDRcolor /= exposedColor * (C * exposedColor + D) + E;
		LDRcolor = clamp(LDRcolor, 0.0, 1.0);
		LDRcolor = pow(LDRcolor, vec3(gamma));

		return LDRcolor;
	}	
#elif defined(ACES_TONEMAP_ADVANCED)
	{
		const mat3 ACESInputMat = mat3(
			0.59719, 0.35458, 0.04823,
			0.07600, 0.90834, 0.01566,
			0.02840, 0.13383, 0.83777
		);

		const mat3 ACESOutputMat = mat3(
			 1.60475, -0.53108, -0.07367,
			-0.10208,  1.10813, -0.00605,
			-0.00327, -0.07276,  1.07602
		);

		#define RRTAndODTFit(v)\
			(v * (v + 0.0245786f) - 0.000090537f) / (v * (0.983729f * v + 0.4329510f) + 0.238081f)


		vec3 exposedColor = HDRcolor * exposure;
		exposedColor = ACESInputMat * exposedColor;

		vec3 LDRcolor = RRTAndODTFit(exposedColor);
		LDRcolor = ACESOutputMat * LDRcolor;
		LDRcolor = clamp(LDRcolor, 0.0, 1.0);
		LDRcolor = pow(LDRcolor, vec3(gamma));

		return LDRcolor;
	}
#elif defined(FILMIC_TONEMAP)
	{
		vec3 exposedColor = pow(HDRcolor * exposure, vec3(1.0)); // 1.3
		exposedColor = max(vec3(0.0), exposedColor - 0.004);
		vec3 LDRcolor = exposedColor * (6.2 * exposedColor + 0.5); 
		LDRcolor /= exposedColor * (6.2 * exposedColor + 1.7) + 0.06;
		//LDRcolor = pow(LDRcolor, vec3(2.2));

		return LDRcolor;
	}
#else //LINEAR
	{
		vec3 exposedColor = HDRcolor * exposure;
		vec3 LDRcolor = clamp(exposedColor, 0.0, 1.0);
		LDRcolor = pow(LDRcolor, vec3(gamma));

		return LDRcolor;
	}	
#endif //TONEMAP_TYPE

	//Should not get here
	return vec3(0.0);
}


#endif //TONE_MAPPING_GLSL