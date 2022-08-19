#version 430 core
layout (location = 0) in vec3 tetrPos;
layout (location = 1) in vec4 tetrColor;
layout (location = 2) in vec3 cubePos;
layout (location = 3) in vec4 cubeColor;
out vec4 vertColor;

uniform uint objectType;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    if(objectType == 0)
    {
        gl_Position = projectionMatrix * ( viewMatrix * (modelMatrix * vec4(tetrPos, 1.0)));
        vertColor = tetrColor;
    }
    else
    {
        gl_Position = projectionMatrix * ( viewMatrix * (modelMatrix * vec4(cubePos, 1.0)));
        vertColor = cubeColor;
    }
}