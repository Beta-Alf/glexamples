#version 150 core

in vec3 v_normal;
in vec3 weights;

out vec4 fragColor;

void main()
{

	fragColor = vec4(weights,1.0);
}
