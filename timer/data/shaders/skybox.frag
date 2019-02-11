
//CONSTS=======================================================================
#include "include/commons.glsl"

//INPUTS=======================================================================
in block{
	vec3 vertex;
}IN;

//TEXTURES=====================================================================
layout (binding = 0) uniform samplerCube u_texture;

//UNIFORMS=====================================================================
uniform mat4 u_modelViewProjectionMatrix;

//OUTPUTS======================================================================
layout (location = 0) out vec4 o_fragColor;
layout (location = 1) out vec4 o_bloomColor;


//CODE=========================================================================
void main()
{
	vec3 textureColor = textureLod(u_texture, IN.vertex, 0).rgb;
	o_fragColor = vec4(pow(textureColor, vec3(2.2)), 1.0);
	
	const float brigthnessFalloff = 1.0;
	float brightness = pow(getBrightness(o_fragColor.rgb), brigthnessFalloff);
	//float brightness = min(1.0, pow(getBrightness(o_fragColor), brigthnessFalloff));
	
	o_bloomColor = vec4(o_fragColor.rgb * (0.2 + brightness), 1.0);
	//o_bloomColor = vec4(o_fragColor.rgb, 1.0);
	//o_bloomColor = vec4(0, 0, 0, 1);
}
