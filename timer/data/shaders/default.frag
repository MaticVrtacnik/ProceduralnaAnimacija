
//INPUTS=======================================================================
in block{
	vec2 uv;
}IN;

//TEXTURES=====================================================================
layout (binding = 0) uniform sampler2D u_texture;

//OUTPUTS======================================================================
layout (location = 0) out vec4 o_fragColor;


//CODE=========================================================================
void main()
{
	vec3 textureColor = texture(u_texture, IN.uv).rgb;
	o_fragColor = vec4(textureColor, 1.0);
}
