#version 430 core
layout (location = 0) in vec3 vPosition;
out vec4 vertColor;

uniform uint transformType;
uniform vec3 position;

mat4 translate(float x, float y, float z)
{
    return 
    mat4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        x, y, z, 1
    );
}

mat4 rotate(float Angle)
{
    float pi = 3.1415;
	float radian = pi / 180.0;
    float Radian = Angle * radian;
    return
    mat4(
        cos(Radian), sin(Radian), 0, 0,
        -sin(Radian), cos(Radian), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
}

mat4 rotateFreely(float x, float y)
{
    return translate(x, y, 0) * rotate(180) * translate(-x, -y, 0);
}

mat4 scaleFreely(float xMultiple, float yMultiple, float x, float y)
{
    return
    mat4(
        xMultiple, 0, 0, 0,
        0, yMultiple, 0, 0,
        0, 0, 1, 0,
        x*(1-xMultiple), y*(1-yMultiple), 0, 1
    );
}

mat4 shear(float xAngle, float yAngle, float zAngle)
{
    float pi = 3.1415;
	float radian = pi / 180.0;
    float xRadian = xAngle * radian;
    return
    mat4(
        1, 0, 0, 0,
        tan(xRadian), 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
}

mat4 flip(bool x, bool y)
{
    float xNum = x ? -1 : 1;
    float yNum = y ? -1 : 1;
    return
    mat4(
        xNum, 0, 0, 0,
        0, yNum, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
}

void main()
{

    vec4 originCoordinate = vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
    switch(transformType){
        case 0:
            gl_Position = vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
            break;
        case 1:
            gl_Position = translate(0.1, 0.1, 0.0) * originCoordinate;
            break;
        case 2:
            gl_Position = rotate(30) * originCoordinate;
            break;
        case 3:
            gl_Position = scaleFreely(0.5, 0.5, 0, 0) * originCoordinate;
            break;
        case 4:
            gl_Position = translate(0.1, 0.1, 0.0) *  rotate(30) * scaleFreely(0.5, 0.5, 0, 0) * originCoordinate;
            break;
        case 5:
            gl_Position = shear(30, 0, 0) * originCoordinate;
            break;
        case 6:
            gl_Position = flip(false, true) * originCoordinate;
            break;
        case 7:
            gl_Position = rotateFreely(0.2, 0.2) * originCoordinate;
            break;
        case 8:
            gl_Position = scaleFreely(0.5, 0.5, 0.2, 0.2) * originCoordinate;
            break;
        default:
            gl_Position = vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
            break;
    }
    vertColor = vec4(vPosition, 1.0f);
}