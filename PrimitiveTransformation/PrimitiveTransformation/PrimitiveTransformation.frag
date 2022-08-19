#version 430 core
in vec4 vertColor;
out vec4 fragColor;

void main()
{
	if (vertColor.r > 0)
	{
		fragColor.r = 0.8f;
	}
	else
	{
		fragColor.r = 0.2f;
	}
	if (vertColor.g > 0)
	{
		fragColor.g = 0.8f;
	}
	else
	{
		fragColor.g = 0.2f;
	}
//	fragColor = vertColor;
}