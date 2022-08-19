#version 430 core
layout (location = 0) in vec3 flagPosition;
layout (location = 1) in vec4 vColor;
layout (location = 2) in vec2 vTexture;
layout (location = 3) in vec3 roadPosition;
out vec4 vertColor;
out vec2 texturePosition;

uniform uint renderType;
uniform float textureRatio;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    if (renderType == 0)
    {
//        gl_Position = projectionMatrix * ( viewMatrix * (modelMatrix * vec4(flagPosition, 1.0)));
        gl_Position = projectionMatrix * ( viewMatrix * (modelMatrix * vec4(flagPosition.x, flagPosition.y * textureRatio, flagPosition.z, 1.0)));
        vertColor = vColor;
        texturePosition = vTexture;
    }
    else
    {
        gl_Position = projectionMatrix * ( viewMatrix * (modelMatrix * vec4(roadPosition, 1.0)));
        texturePosition = vTexture;
    }
}