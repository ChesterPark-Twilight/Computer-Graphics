#version 430 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec4 vColor;
out vec3 originPosition;
out vec3 vertNormal;
out vec4 vertColor;
out vec3 vertPosition;
out vec3 cameraPosition;

out vec4 gouraudColor;

uniform uint lightType;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec3 lightPosition;
uniform vec3 lightColor;
void main()
{
	gl_Position = projectionMatrix * ( viewMatrix * (modelMatrix * vec4(vPosition, 1.0)));
	originPosition = vPosition;
	vertNormal = vec3(modelMatrix * vec4(vNormal, 0.0));
	vertColor = vColor;
	vertPosition = vec3(modelMatrix * vec4(vPosition, 1.0));
	cameraPosition = vec3(0.0f, 0.0f, 2.0f);
	if (lightType == 1) 
	{
		//环境光
		float ambientStrength = 0.3;
		vec3 ambientColor = ambientStrength * lightColor;

		//漫反射光
		vec3 stanNormal = normalize(vertNormal);
		vec3 stanLightDirection = normalize(lightPosition - vertPosition);
		float diffuseWeight= max(dot(stanNormal, stanLightDirection), 0.0);
		vec3 diffuseColor = diffuseWeight * lightColor;

		//镜面光
		float specularStrength = 0.3;
		vec3 stanCameraDirection = normalize(cameraPosition - vertPosition);
		vec3 reflectDirection = reflect(-stanLightDirection, stanNormal);
		float specularWeight = pow(max(dot(stanCameraDirection, reflectDirection), 0.0), 64);
		vec3 specularColor = specularStrength * specularWeight * lightColor;

		vec4 resultColor = vec4((ambientColor + diffuseColor + specularColor), 1.0f);
		vec4 objectColor;

		objectColor = vec4(step(vec3(0), originPosition), 1) + vec4(0.16);
//		objectColor = vec4(0.8f, 0.2f, 0.2f, 1.0f);
	
		gouraudColor = resultColor * objectColor;
	}
}