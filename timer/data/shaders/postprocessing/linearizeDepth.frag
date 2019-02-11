
#include "include/commons.glsl"

layout (location = 0) out float o_linearDepth;

in vec2 outUV;

layout (binding = 0) uniform sampler2D u_depthTexture;

void main()
{
	float depth = texture(u_depthTexture, outUV).r;
	o_linearDepth = -linearizeDepth(depth);
}






