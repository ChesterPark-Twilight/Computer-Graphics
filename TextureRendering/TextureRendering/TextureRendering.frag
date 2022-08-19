#version 430 core
in vec4 vertColor;
in vec2 texturePosition;
out vec4 fragColor;

uniform uint renderType;
uniform sampler2D flagTexture;
uniform sampler2D roadTexture;

void main()
{
	if (renderType == 0)
    {
        fragColor = texture(flagTexture, texturePosition);
//        fragColor = vec4(0.3f, 0.6f, 1.0f, 1.0f);
    }
    else
    {
        fragColor = texture(roadTexture, texturePosition);
    }
	
}