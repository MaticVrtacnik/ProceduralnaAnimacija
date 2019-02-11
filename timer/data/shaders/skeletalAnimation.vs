
//CONSTS=======================================================================
#define MAX_BONES 50

//INPUTS=======================================================================
layout (location = 0) in vec4 i_vertexColorR;
layout (location = 1) in vec4 i_normalColorG;
layout (location = 2) in vec4 i_tangentColorB;
layout (location = 3) in vec4 i_uv;
layout (location = 4) in ivec4 i_boneIds;
layout (location = 5) in vec4 i_weights;

//UNIFORMS=====================================================================
uniform mat4 u_bones[MAX_BONES];
uniform mat4 u_prevBones[MAX_BONES];
uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_modelViewMatrix;
uniform mat4 u_modelViewProjectionMatrix;
uniform mat4 u_prevModelViewProjectionMatrix;
uniform vec3 u_scale;

//OUTPUTS======================================================================
out block{
	vec4 velocityVertex;
	vec4 prevVelocityVertex;	
	vec3 vertex;
	vec3 modelVertex;
	vec3 normal;
	vec3 tangent;
	vec2 uv;	
	
	vec3 color;
	
	vec3 heightColor;
	
}OUT;


//CODE=========================================================================
float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{	
	/*mat4 boneTransform = u_bones[i_boneIds.x] * step(0.01, i_weights.x);
	boneTransform += u_bones[i_boneIds.y]  * step(0.01, i_weights.y);
	boneTransform += u_bones[i_boneIds.z]  * step(0.01, i_weights.z);
	boneTransform += u_bones[i_boneIds.w]  * step(0.01, i_weights.w);*/
	//boneTransform = mat4(1.0);
	
	//mat4 boneTransform = u_bones[i_boneIds.x] * max(0.01, i_weights.x);
	mat4 boneTransform = u_bones[i_boneIds.x] * i_weights.x;
	boneTransform += u_bones[i_boneIds.y] * i_weights.y;
	boneTransform += u_bones[i_boneIds.z] * i_weights.z;
	boneTransform += u_bones[i_boneIds.w] * i_weights.w;
	//boneTransform = mat4(1.0);
	
	/*if(i_boneIds.x == 0){
		boneTransform = mat4(1.0);
	}*/
	
	mat4 prevBoneTransform = u_prevBones[i_boneIds.x] * max(0.01, i_weights.x);
	prevBoneTransform += u_prevBones[i_boneIds.y] * i_weights.y;
	prevBoneTransform += u_prevBones[i_boneIds.z] * i_weights.z;
	prevBoneTransform += u_prevBones[i_boneIds.w] * i_weights.w;
	//prevBoneTransform = mat4(1.0);
	
	//prevBoneTransform = boneTransform;
	
	vec3 vertex = i_vertexColorR.xyz;
	OUT.modelVertex = vertex;
	
	float heightFactor = (OUT.modelVertex.z + 30.0) / 60.0;
	OUT.heightColor = vec3(heightFactor, 0.0, 1.0 - heightFactor);
	//vertex.z *= 0.85;
	/*if(i_boneIds.x == 2)*///vertex *= 2.5; //vec3(1.5, 1.0, 1.5);
	
	vec4 prevSkinnedVertex = prevBoneTransform * vec4(vertex, 1.0);
	vec4 skinnedVertex = boneTransform * vec4(vertex, 1.0);
	//skinnedVertex.xyz *= 0.8;
	///*if(i_boneIds.x == 0)*/skinnedVertex.xyz *= 1.5;
	//skinnedVertex.z *= -1.0f;
	//skinnedVertex.xyz *= u_scale;
	//skinnedVertex.xyz *= 2.0;
	
	//Hip transform
	mat3 normalMatrix = transpose(inverse(mat3(u_modelViewMatrix * boneTransform)));
	
	
	OUT.velocityVertex = u_modelViewProjectionMatrix * skinnedVertex;
	OUT.prevVelocityVertex = u_prevModelViewProjectionMatrix * prevSkinnedVertex;
	//OUT.prevVelocityVertex = OUT.velocityVertex;
	
	OUT.vertex = vec3(u_modelViewMatrix * skinnedVertex);
	OUT.normal = normalMatrix * i_normalColorG.xyz;
	OUT.normal = mat3(u_viewMatrix) * vec3(0.0, 1.0, 0.0);
	
	OUT.tangent = normalMatrix * i_tangentColorB.xyz;
	OUT.tangent = OUT.tangent - dot(OUT.tangent, OUT.normal) * OUT.normal;
	OUT.uv = i_uv.xy;
	
	OUT.color = vec3(rand(i_boneIds.xy), rand(i_boneIds.xz), rand(i_boneIds.zy));
	OUT.color = max(vec3(0.5), OUT.color);
	
	gl_Position = OUT.velocityVertex;
}
