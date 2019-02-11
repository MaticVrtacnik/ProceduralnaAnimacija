
layout (location = 0) out vec4 fragColor;

in vec2 outUV;

layout (binding = 0) uniform sampler2D screenTexture;

void main()
{
	fragColor = vec4(texture(screenTexture, outUV).rgb, 1.0);
}






