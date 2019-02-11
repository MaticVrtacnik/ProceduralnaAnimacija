
//INPUTS=======================================================================
in block{
	vec3 vertex;
	vec3 normal;
	vec3 cubemapUv;
	vec2 uv;
}IN;

//TEXTURES=====================================================================
layout (binding = 0) uniform samplerCube u_cubemapTexture;
layout (binding = 1) uniform sampler2D u_diffuseTexture; 

//UNIFORMS=====================================================================
uniform vec3 u_cameraLocation;

//OUTPUTS======================================================================
layout (location = 0) out vec4 o_fragColor;


//CODE=========================================================================
void main()
{
	const float refractionFactor = 1.0 / 1.31;
	vec3 incident = normalize(IN.vertex - u_cameraLocation);
	vec3 normal = normalize(IN.normal);
	
	vec3 reflected = reflect(incident, normal);
	vec3 refracted = refract(incident, normal, refractionFactor);
	
	vec4 textureColor = texture(u_diffuseTexture, IN.uv);
	float grayscale = dot(textureColor.rgb, vec3(0.33333));
	
	o_fragColor = texture(u_cubemapTexture, reflected);
	o_fragColor = mix (o_fragColor, textureColor, 1 - grayscale);
	o_fragColor = texture(u_cubemapTexture, IN.cubemapUv);
	//o_fragColor = vec4(normal, 1.0);
}