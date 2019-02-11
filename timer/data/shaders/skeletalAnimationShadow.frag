
//CONSTS=======================================================================
#include "include/commons.glsl"

//OUTPUTS======================================================================
layout (location = 0) out vec2 o_fragColor;


//CODE=========================================================================
void main()
{	
	float moment1 = gl_FragCoord.z;
	float dx = dFdx(moment1);
	float dy = dFdy(moment1);
	
	float moment2 = moment1 * moment1 + 0.25 * (dx * dx + dy * dy);

	o_fragColor = vec2(moment1, moment2);
	//o_fragColor = vec2(1.0);
	
	
	/*const float LIGHT_NEAR = 0.1f;
	const float LIGHT_FAR = 1000.0f;
	const float EXPONENT = 15.0f;

	float depthDivisor = (1.0 / gl_FragCoord.z);
	float mappedDivisor = map_01(depthDivisor, LIGHT_NEAR, LIGHT_FAR);
	 	
	o_fragColor.r = exp(EXPONENT * mappedDivisor);
	
	o_fragColor.r = linearizeDepth(gl_FragCoord.z);
	//o_fragColor.r = mappedDivisor;
	//o_fragColor.r = 0.5;*/
}
