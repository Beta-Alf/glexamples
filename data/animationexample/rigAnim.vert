#version 150 core
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec3 a_vertex;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 tex_coords;
layout(location = 3) in ivec4 bone_indices;
layout(location = 4) in vec4 bone_weights;

out vec3 v_normal;
out vec4 weights;

const int MAX_BONES = 100;

uniform mat4 transform;
uniform mat4 gBones[MAX_BONES];

void main()
{
	gl_Position = transform * vec4(a_vertex, 1.0);
    v_normal = a_normal;

	weights = bone_weights;
}
