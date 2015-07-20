#version 150 core

in vec3 v_normal;
in vec4 weights;

out vec4 fragColor;

void main()
{

	fragColor = vec4(weights.xyz,1.0);
}
