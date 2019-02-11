
//CONSTS=======================================================================
#include "include/commons.glsl"

#define SAMPLE_BIAS 0

//INPUTS=======================================================================
in block{
	vec4 velocityVertex;
	vec4 prevVelocityVertex;
	vec3 vertex;
	vec3 normal;
	vec3 tangent;
	vec3 color;
	vec2 uv;
}IN;

//TEXTURES=====================================================================
layout (binding = 0) uniform sampler2D u_albedoTexture;
layout (binding = 1) uniform sampler2D u_normalTexture;
layout (binding = 2) uniform sampler2D u_roughnessTexture;
layout (binding = 3) uniform sampler2D u_metallicTexture;
layout (binding = 4) uniform sampler2D u_occlusionTexture;
layout (binding = 5) uniform sampler2D u_heightTexture;

//UNIFORMS=====================================================================
uniform mat4 u_inverseViewMatrix;
uniform mat3 u_inverseNormalMatrix;
uniform vec4 u_material; //TODO
uniform vec3 u_cameraLocation;
uniform vec3 u_color;
uniform float u_displacementScale;

//OUTPUTS======================================================================
layout (location = 0) out vec4 o_locationDepth;
layout (location = 1) out vec3 o_normal;
layout (location = 2) out vec2 o_velocity;
layout (location = 3) out vec4 o_albedo;
layout (location = 4) out vec4 o_RMOE;


//CODE=========================================================================
void main()
{
	vec3 normal = normalize(IN.normal);
	vec3 tangent = normalize(IN.tangent);
	vec3 bitangent = normalize(cross(tangent, normal));
	mat3 TBN = mat3(tangent, bitangent, normal);
	
	//TODO maybe wrong because of mat3(inverseView)
	mat3 transposeTBN = transpose(mat3(u_inverseViewMatrix) * TBN);

	
	vec3 worldVertex = vec3(u_inverseViewMatrix * vec4(IN.vertex, 1.0));
	vec3 TBNVertex = transposeTBN * worldVertex;
	vec3 TBNcameraLocation = transposeTBN * u_cameraLocation;
	vec3 worldViewDirection = -normalize(TBNcameraLocation - TBNVertex);

	vec2 uv = IN.uv;
	//uv = getParallax(u_heightTexture, IN.uv,  worldViewDirection, u_displacementScale);      
    //if(uv.x > 1.0 || uv.y > 1.0 || uv.x < 0.0 || uv.y < 0.0)discard;


	vec3 textureNormal = normalize(texture(u_normalTexture, uv, SAMPLE_BIAS).rgb * 2.0 - 1.0);
	o_normal = normalize(TBN * textureNormal);
	//o_normal = normal;
	//o_normal = vec3(0.0, 1.0, 0.0);
	
	o_locationDepth = vec4(IN.vertex, linearizeDepth(gl_FragCoord.z));
	//o_locationDepth = vec4(0,0,0,1);
	
	vec4 albedo = texture(u_albedoTexture, uv, SAMPLE_BIAS);	
	if(albedo.a < 0.5)discard; //Discard opaque
	
	vec3 randColor = vec3(rand(uv), rand(textureNormal.xz), rand(textureNormal.yz));
	o_albedo = vec4(albedo.rgb, 1.0);
	o_albedo.rgb = pow(o_albedo.rgb, vec3(1.0)); // - randColor;
	o_albedo.rgb = saturation(o_albedo.rgb, 1.0);
	//o_albedo.rgb = IN.color;
	//o_albedo.rgb = vec3(1.0, 0.0, 0.0);
	//o_albedo.rgb = vec3(1.0);
	//o_albedo.rgb = vec3(0.25);
	
	//o_albedo.rgb = vec3(0.1, 0.4, 0.02);
	//o_albedo.rgb = IN.color;
	//o_albedo.rgb = vec3(1.0);
		
	vec2 currVertex = (IN.velocityVertex.xy / IN.velocityVertex.w) * 0.5 + 0.5;
	vec2 prevVertex = (IN.prevVelocityVertex.xy / IN.prevVelocityVertex.w) * 0.5 + 0.5;
	o_velocity = currVertex - prevVertex;
	//o_velocity = vec2(0.0);

	//Will blackscreen without certain minimum
	//o_RMOE.r = max(0.015, pow(texture(u_roughnessTexture, uv, SAMPLE_BIAS).r, 1.0 / 2.2));	
	o_RMOE.r = max(0.015, texture(u_roughnessTexture, uv, SAMPLE_BIAS).r); //o_RMOE.r = 0.85; //ROUGHNESS	
	o_RMOE.g = texture(u_metallicTexture, uv, SAMPLE_BIAS).r; //o_RMOE.g = 1.0; //METALLIC
	o_RMOE.b = texture(u_occlusionTexture, uv, SAMPLE_BIAS).r; //o_RMOE.b *= 0.25; //OCCLUSION
	o_RMOE.a = 0.0; //dot(color, vec3(0.21, 0.71, 0.08)); //EMISSIVE
	
	o_RMOE.r = pow(o_RMOE.r, 1.0 / (1.0 + o_RMOE.g * 1.2));
	
	//o_RMOE.r = pow(o_RMOE.r, 0.25);
}
