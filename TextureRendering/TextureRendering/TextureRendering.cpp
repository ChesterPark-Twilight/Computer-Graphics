#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "stdafx.h"
using namespace vmath;

const GLuint flagVertNum = 4;
const GLuint roadVertNum = 4;
const GLuint coordinateLen = 3;
const GLuint colorLen = 4;
const GLuint textureLen = 2;

const GLuint VAONum = 1;
GLuint VAO[VAONum];

const GLuint VBONum = 1;
GLuint VBO[VBONum];

const GLuint EBONum = 1;
GLuint EBO[EBONum];

const GLuint TextureNum = 1;
GLuint Flag[TextureNum];
GLuint Road[TextureNum];

const GLuint flagPosition = 0;
const GLuint vColor = 1;
const GLuint vTexture = 2;
const GLuint roadPosition = 3;

GLuint renderType;
float textureRatio;

GLuint modelMatirx;
GLuint viewMatrix;
GLuint projectionMatrix;

GLuint flagTexture;
GLuint roadTexture;

GLuint filterTag = 0;//过滤属性设置
GLuint renderTag = 0;//绘制图形设置

void init()
{
	//定义顶点数组
	GLfloat verticeArray[flagVertNum][coordinateLen] = {
		{ 1.0, 1.0, 1.0},
		{ -1.0, 1.0, 1.0},
		{ -1.0, -1.0, 1.0},
		{ 1.0, -1.0, 1.0},
	};
	//定义顶点颜色
	GLfloat colorArray[flagVertNum][colorLen] = {
		{ 0.0f, 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 0.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 1.0f, 1.0f }
	};
	//定义纹理坐标
	GLfloat textureLocation = 2.0;
	GLfloat textureArray[4][textureLen] = {
		{ textureLocation, 0.0f },
		{ 0.0f, 0.0f },
		{ 0.0f, textureLocation },
		{ textureLocation, textureLocation },
	};
	//定义索引数组
	GLuint indexArray[] = {
		0, 1, 3, 2,
		4, 5, 7, 6,
		8, 9, 11, 10
	};
	
	//定义公路顶点数组
	GLfloat nearDist = 1.0f;
	GLfloat farDist = -24.6f;
	GLfloat nearHeight = 0.8f;
	GLfloat farHeight = 2.0f;
	GLfloat roadVerticeArray[roadVertNum][coordinateLen] = {
		//路面
		{ 1.0, 1.0, farDist},
		{ -1.0, 1.0, farDist},
		{ -1.0, -1.0, nearDist},
		{ 1.0, -1.0, nearDist},
		////左侧景色
		//{ -1.0, 1.0, farDist},
		//{ -1.0, farHeight, farDist},
		//{ -1.0, nearHeight, nearDist},
		//{ -1.0, -1.0, nearDist},
		////右侧景色
		//{ 1.0, 1.0, farDist},
		//{ 1.0, farHeight, farDist},
		//{ 1.0, nearHeight, nearDist},
		//{ 1.0, -1.0, nearDist}
	};

	//关联VAO：保存数据配置
	glGenVertexArrays(VAONum, VAO);
	glBindVertexArray(VAO[0]);

	//关联VBO：在显存中存储顶点数据
	glGenBuffers(VBONum, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticeArray) + sizeof(colorArray) + sizeof(textureArray) + sizeof(roadVerticeArray), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verticeArray), verticeArray);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(verticeArray), sizeof(colorArray), colorArray);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(verticeArray) + sizeof(colorArray), sizeof(textureArray), textureArray);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(verticeArray) + sizeof(colorArray) + sizeof(textureArray), sizeof(roadVerticeArray), roadVerticeArray);

	//关联EBO：在显存中存储索引数据
	glGenBuffers(EBONum, EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexArray), indexArray, GL_STATIC_DRAW);

	//链接着色器
	ShaderInfo shaderInfo[] = {
		{ GL_VERTEX_SHADER, "TextureRendering.vert" },
		{ GL_FRAGMENT_SHADER, "TextureRendering.frag" },
		{ GL_NONE, NULL}
	};
	GLuint program = LoadShaders(shaderInfo);
	glUseProgram(program);

	//链接顶点属性：定义如何处理顶点数据
	glVertexAttribPointer(flagPosition, coordinateLen, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribPointer(vColor, colorLen, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(verticeArray)));
	glVertexAttribPointer(vTexture, textureLen, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(verticeArray) + sizeof(colorArray)));
	glVertexAttribPointer(roadPosition, coordinateLen, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(verticeArray) + sizeof(colorArray) + sizeof(textureArray)));
	glEnableVertexAttribArray(flagPosition);
	glEnableVertexAttribArray(vColor);
	glEnableVertexAttribArray(vTexture);
	glEnableVertexAttribArray(roadPosition);

	//创建国旗纹理
	glGenTextures(TextureNum, Flag);
	glBindTexture(GL_TEXTURE_2D, Flag[0]);
	int flagWidth, flagHeight, flagChannel;
	unsigned char* textureFlag = stbi_load("Pic_MotherChina.jpg", &flagWidth, &flagHeight, &flagChannel, 0);
	if (textureFlag)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, flagWidth, flagHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureFlag);
		glGenerateMipmap(GL_TEXTURE_2D);
		//设置范围超出纹理展示模式：S, T, R分别对应X, Y, Z，模式包括REPEAT, MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER，最后一种模式需要设计边缘颜色
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//选择最后一种模式需要设计边缘颜色
		//float borderColor[] = { 1.0f, 0.6f, 0.3f, 1.0f };
		//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		//设置各向异性
		GLfloat anisotropy;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
	}
	else
	{
		cout << "纹理加载失败..." << endl;
	}
	stbi_image_free(textureFlag);
	
	//创建道路纹理
	glGenTextures(TextureNum, Road);
	glBindTexture(GL_TEXTURE_2D, Road[0]);
	int roadWidth, roadHeight, roadChannel;
	unsigned char* textureRoad = stbi_load("Pic_Road.jpg", &roadWidth, &roadHeight, &roadChannel, 0);
	if (textureRoad)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, roadWidth, roadHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureRoad);
		glGenerateMipmap(GL_TEXTURE_2D);
		//设置范围超出纹理展示模式：S, T, R分别对应X, Y, Z，模式包括REPEAT, MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER，最后一种模式需要设计边缘颜色
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//选择最后一种模式需要设计边缘颜色
		//float borderColor[] = { 1.0f, 0.6f, 0.3f, 1.0f };
		//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		//设置各向异性
		GLfloat anisotropy;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
	}
	else
	{
		cout << "纹理加载失败..." << endl;
	}
	stbi_image_free(textureRoad);

	renderType = glGetUniformLocation(program, "renderType");
	textureRatio = glGetUniformLocation(program, "textureRatio");
	modelMatirx = glGetUniformLocation(program, "modelMatrix");
	viewMatrix = glGetUniformLocation(program, "viewMatrix");
	projectionMatrix = glGetUniformLocation(program, "projectionMatrix");
	flagTexture = glGetUniformLocation(program, "flagTexture");
	roadTexture = glGetUniformLocation(program, "roadTexture");

	float ratio = (float)flagHeight / (float)flagWidth;
	glUniform1f(textureRatio, ratio);

	//激活纹理单元
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Flag[0]);
	glUniform1i(flagTexture, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, Road[0]);
	glUniform1i(roadTexture, 1);
}

GLfloat dist = 0.0f;

void display()
{
	//设置纹理过滤模式：主要包括GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR
	if (filterTag == 0)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	else if (filterTag == 1)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	else if (filterTag == 2)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	}
	else if (filterTag == 3)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	}
	else if (filterTag == 4)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	}
	else if (filterTag == 5)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else
	{
		cout << "过滤参数出错..." << endl;
	}

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);//准备Buffers颜色输入

	glBindVertexArray(VAO[0]);

	if (renderTag == 0)
	{
		float width = glutGet(GLUT_WINDOW_WIDTH);
		float height = glutGet(GLUT_WINDOW_HEIGHT);
		float ratio = width / height;

		mat4 modelModel(vmath::translate(0.0f, 0.0f, 0.0f));//模型矩阵
		mat4 viewModel(vmath::translate(0.0f, 0.0f, -1.6f));//视角矩阵
		mat4 projectionModel(vmath::customPerspective(-1.0f, 1.0f, -ratio, ratio, 1.0f, 100.0f));

		glUniformMatrix4fv(modelMatirx, 1, GL_FALSE, modelModel);
		glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, viewModel);
		glUniformMatrix4fv(projectionMatrix, 1, GL_FALSE, projectionModel);
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
	}
	else if (renderTag == 1)
	{
		float time = float(GetTickCount64() & 0x1FFF) / float(0x1FFF);//时间滴答
		float width = glutGet(GLUT_WINDOW_WIDTH);
		float height = glutGet(GLUT_WINDOW_HEIGHT);
		float ratio = width / height;

		dist += 0.0002f;

		mat4 modelModel(vmath::translate(0.0f, 0.0f, 0.0f));//模型矩阵
		mat4 viewModel(vmath::translate(0.0f, -dist / 12.8f, dist));//视角矩阵
		mat4 projectionModel(vmath::customPerspective(-1.0f, 1.0f, -ratio, ratio, 1.0f, 100.0f));

		glUniformMatrix4fv(modelMatirx, 1, GL_FALSE, modelModel);
		glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, viewModel);
		glUniformMatrix4fv(projectionMatrix, 1, GL_FALSE, projectionModel);

		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid*)(4 * sizeof(GLuint)));
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (const GLvoid*)(8 * sizeof(GLuint)));
	}
	else
	{
		cout << "绘制参数出错..." << endl;
	}
	
	glFlush();

	glutPostRedisplay();
}

void keyboardFunc(GLboolean key, GLint x, GLint y)
{
	switch (key)
	{
	case '1':
		filterTag = 0;
		break;
	case '2':
		filterTag = 1;
		break;
	case '3':
		filterTag = 2;
		break;
	case '4':
		filterTag = 3;
		break;
	case '5':
		filterTag = 4;
		break;
	case '6':
		filterTag = 5;
		break;
	case 'f':
		glUniform1ui(renderType, 0);
		renderTag = 0;
		break;
	case 'r':
		glUniform1ui(renderType, 1);
		renderTag = 1;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(600, 600);
	glutInitContextVersion(1, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Texture Rendering Windows");
	glewExperimental = GL_TRUE;
	if (glewInit())
	{
		cerr << "Unable to Initialize GLEW" << endl;
	}

	init();
	glutKeyboardFunc(keyboardFunc);

	glutDisplayFunc(display);

	glutMainLoop();

	return 0;
}