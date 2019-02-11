
layout (location = 0) out vec4 fragColor;

in vec2 outUV;

layout (binding = 0) uniform sampler2D sampler0;
layout (binding = 1) uniform sampler2D sampler1;


void main()
{
	vec4 texture1 = texture(sampler0, outUV);
	vec4 texture2 = texture(sampler1, outUV);
	fragColor = texture1 + texture2;
}






