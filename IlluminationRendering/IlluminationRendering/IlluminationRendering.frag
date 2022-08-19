#version 430 core
in vec3 originPosition;
in vec3 vertNormal;
in vec4 vertColor;
in vec3 vertPosition;
in vec3 cameraPosition;

in vec4 gouraudColor;

out vec4 fragColor;

uniform uint lightType;
uniform vec3 lightPosition;
uniform vec3 lightColor;

void main()
{
	if (lightType == 0)
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

		fragColor = resultColor * objectColor;
	}
	else
	{
		fragColor = gouraudColor;
	}
}