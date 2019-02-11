
//CONSTS=======================================================================
#define MAX_BONES 50
//TODO include defines with MAX_BONES

//INPUTS=======================================================================
layout (location = 0) in vec4 i_vertexColorR;
layout (location = 1) in vec4 i_normalColorG;
layout (location = 2) in vec4 i_tangentColorB;
layout (location = 3) in vec4 i_uv;
layout (location = 4) in ivec4 i_boneIds;
layout (location = 5) in vec4 i_weights;

//UNIFORMS=====================================================================
uniform mat4 u_bones[MAX_BONES];
uniform mat4 u_modelViewProjectionMatrix;
uniform vec3 u_scale;


//CODE=================================================================
void main()
{	
	mat4 boneTransform = u_bones[i_boneIds.x] * i_weights.x;
	boneTransform += u_bones[i_boneIds.y] * i_weights.y;
	boneTransform += u_bones[i_boneIds.z] * i_weights.z;
	boneTransform += u_bones[i_boneIds.w] * i_weights.w;
	//boneTransform = mat4(1.0);
	
	vec4 skinnedVertex = boneTransform * vec4(i_vertexColorR.xyz, 1.0);

	gl_Position = u_modelViewProjectionMatrix * skinnedVertex;
}
