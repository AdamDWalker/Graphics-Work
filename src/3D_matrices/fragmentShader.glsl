#version 330
in vec4 fragmentColor;
out vec4 outputColor;
void main()
{
	float ambientStrength = 0.5f;
	vec4 ambient = ambientStrength * fragmentColor;
	 outputColor = ambient;
}
