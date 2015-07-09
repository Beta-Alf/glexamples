#version 150 core
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec3 a_vertex_frame1;
layout(location = 1) in vec3 a_normal_frame1;
layout(location = 2) in vec3 a_vertex_frame2;
layout(location = 3) in vec3 a_normal_frame2;

out vec3 v_normal;

uniform mat4 transform;
uniform float interpol;

void main()
{
	vec3 interpolatedPosition = mix(a_vertex_frame1, a_vertex_frame2, interpol);
	vec3 interpolatedNormal = mix(a_normal_frame1, a_normal_frame2, interpol);
	gl_Position = transform * vec4(interpolatedPosition, 1.0);
	v_normal = interpolatedNormal;
	/*gl_Position = transform * vec4(a_vertex_frame2, 1.0);
	v_normal = a_normal_frame2;*/
}
