#include "stdafx.h"
using namespace vmath;

#define PRIMITIVE_RESTART true;

static const vmath::vec3 X(1.0f, 0.0f, 0.0f);
static const vmath::vec3 Y(0.0f, 1.0f, 0.0f);
static const vmath::vec3 Z(0.0f, 0.0f, 1.0f);

const GLuint tetrVerticeNum = 4;
const GLuint cubeVerticeNum = 8;
const GLuint coordinateLen = 3;
const GLuint colorLen = 4;

const GLuint VAONum = 1;
GLuint VAO[VAONum];

const GLuint VBONum = 1;
GLuint VBO[VBONum];

const GLuint EBONum = 1;
GLuint EBO[EBONum];

const GLuint tetrPos = 0;
const GLuint tetrColor = 1;
const GLuint cubePos = 2;
const GLuint cubeColor = 3;

GLuint objectType;
GLuint modelMatirx;
GLuint viewMatrix;
GLuint projectionMatrix;

GLuint renderType = tetrPos;

const GLuint frustumTag = 0;
const GLuint perspectiveTag = 1;

GLuint projectionType = frustumTag;
GLboolean cullFace = true;
GLboolean depthTest = true;

void init()
{
	//定义顶点数组
	GLfloat R = 0.6;//正四面体外接圆半径
	GLfloat r = -R * 2 * sqrt(2.0) / 3.0;//正三角形外接圆半径
	GLfloat originX = r * sqrt(3.0) / 2.0;
	GLfloat originY = -R / 3.0;
	GLfloat originZ = -r / 2.0;
	GLfloat tetrahedronArray[tetrVerticeNum][coordinateLen] = {
		{ 0.0, R, 0.0},
		{ 0.0, originY, r},
		{ originX, originY, originZ},
		{ -originX, originY, originZ}
	};

	GLfloat sideLen = 0.6;
	GLfloat cubeArray[cubeVerticeNum][coordinateLen] = {
		{ -sideLen, -sideLen, -sideLen},
		{ -sideLen, -sideLen, sideLen},
		{ -sideLen, sideLen, -sideLen},
		{ -sideLen, sideLen, sideLen},
		{ sideLen, -sideLen, -sideLen},
		{ sideLen, -sideLen, sideLen},
		{ sideLen, sideLen, -sideLen},
		{ sideLen, sideLen, sideLen}
	};

	//定义顶点颜色
	GLfloat tetrColorArray[tetrVerticeNum][colorLen] = {
		{ 0.0f, 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 0.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 1.0f, 1.0f }
	};

	GLfloat cubeColorArray[cubeVerticeNum][colorLen] = {
		{ 1.0f, 1.0f, 1.0f, 1.0f},
		{ 1.0f, 1.0f, 0.0f, 1.0f},
		{ 1.0f, 0.0f, 1.0f, 1.0f},
		{ 1.0f, 0.0f, 0.0f, 1.0f},
		{ 0.0f, 1.0f, 1.0f, 1.0f},
		{ 0.0f, 1.0f, 0.0f, 1.0f},
		{ 0.0f, 0.0f, 1.0f, 1.0f},
		{ 0.6f, 0.6f, 0.6f, 1.0f}
	};

	//定义索引数组
	GLuint indexArray[] = {
		1, 2, 3, 0, 1, 2,
		6, 4, 2, 0, 3, 1, 7, 5,
		0xFFFF,
		1, 0, 5, 4, 7, 6, 3, 2
	};

	//关联VAO：保存数据配置
	glGenVertexArrays(VAONum, VAO);
	glBindVertexArray(VAO[0]);

	//关联VBO：在显存中存储顶点数据
	glGenBuffers(VBONum, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tetrahedronArray) + sizeof(tetrColorArray) + sizeof(cubeArray) + sizeof(cubeColorArray), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(tetrahedronArray), tetrahedronArray);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(tetrahedronArray), sizeof(tetrColorArray), tetrColorArray);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(tetrahedronArray) + sizeof(tetrColorArray), sizeof(cubeArray), cubeArray);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(tetrahedronArray) + sizeof(tetrColorArray) + sizeof(cubeArray), sizeof(cubeColorArray), cubeColorArray);

	//关联EBO：在显存中存储索引数据
	glGenBuffers(EBONum, EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexArray), indexArray, GL_STATIC_DRAW);

	//链接着色器
	ShaderInfo shaderInfo[] = {
		{ GL_VERTEX_SHADER, "ObjectRendering.vert" },
		{ GL_FRAGMENT_SHADER, "ObjectRendering.frag" },
		{ GL_NONE, NULL}
	};
	GLuint program = LoadShaders(shaderInfo);
	glUseProgram(program);

	//链接顶点属性：定义如何处理顶点数据
	glVertexAttribPointer(tetrPos, coordinateLen, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer(tetrColor, colorLen, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(tetrahedronArray)));
	glVertexAttribPointer(cubePos, coordinateLen, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(tetrahedronArray) + sizeof(tetrColorArray)));
	glVertexAttribPointer(cubeColor, colorLen, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(tetrahedronArray) + sizeof(tetrColorArray) + sizeof(cubeArray)));
	glEnableVertexAttribArray(tetrPos);
	glEnableVertexAttribArray(tetrColor);
	glEnableVertexAttribArray(cubePos);
	glEnableVertexAttribArray(cubeColor);

	objectType = glGetUniformLocation(program, "objectType");
	modelMatirx = glGetUniformLocation(program, "modelMatrix");
	viewMatrix = glGetUniformLocation(program, "viewMatrix");
	projectionMatrix = glGetUniformLocation(program, "projectionMatrix");
}

void display()
{
	float width = glutGet(GLUT_WINDOW_WIDTH);
	float height = glutGet(GLUT_WINDOW_HEIGHT);
	float ratio = width / height;

	float time = float(GetTickCount64() & 0x1FFF) / float(0x1FFF);//时间滴答

	mat4 modelModel(rotate(time * 360.0f, Y) * rotate(time * 360.0f, Z));//模型矩阵
	mat4 viewModel(translate(0.0f, 0.0f, -2.4f));//视角矩阵
	mat4 projectionModel;
	if (projectionType == frustumTag)
	{
		projectionModel = vmath::customFrustum(-1.0f, 1.0f, -ratio, ratio, 1.0f, 100.0f);//正投影矩阵
	}
	else
	{
		projectionModel = vmath::customPerspective(-1.0f, 1.0f, -ratio, ratio, 1.6f, 100.0f);//透视投影矩阵
	}
	glUniformMatrix4fv(modelMatirx, 1, GL_FALSE, modelModel);
	glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, viewModel);
	glUniformMatrix4fv(projectionMatrix, 1, GL_FALSE, projectionModel);

	cullFace ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);//背面裁剪
	depthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);//深度测试

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//准备Buffers颜色输入
	glBindVertexArray(VAO[0]);
	if (renderType == tetrPos)
	{
		glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, 0);
	}
	else
	{
#if	PRIMITIVE_RESTART
		glEnable(GL_PRIMITIVE_RESTART);
		glPrimitiveRestartIndex(0xFFFF);
		glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_INT, (const GLvoid*)(6 * sizeof(GLuint)));
#else
		glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_INT, (const GLvoid*)(6 * sizeof(GLuint)));
		glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_INT, (const GLvoid*)(15 * sizeof(GLuint)));
#endif
	}
	
	glFlush();
	glutPostRedisplay();
}

void keyboardFunc(GLboolean key, GLint x, GLint y)
{
	switch (key)
	{
	case 't':
		glUniform1ui(objectType, tetrPos);//绘制四面体
		renderType = tetrPos;
		break;
	case 'c':
		glUniform1ui(objectType, cubePos);//绘制立方体
		renderType = cubePos;
		break;
	case 'p':
		if (projectionType == frustumTag)//投影方式切换
		{
			projectionType = perspectiveTag;
			cout << "开启透视投影" << endl;
		}
		else
		{
			projectionType = frustumTag;
			cout << "开启正投影" << endl;
		}
		break;
	case '1':							//背面裁剪开关
		if (cullFace)
		{
			cullFace = false;
			cout << "关闭背面裁剪" << endl;
		}
		else
		{
			cullFace = true;
			cout << "开启背面裁剪" << endl;
		}
		break;
	case '2':							//深度测试开关
		if (depthTest)
		{
			depthTest = false;
			cout << "关闭深度测试" << endl;
		}
		else
		{
			depthTest = true;
			cout << "开启深度测试" << endl;
		}
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
	glutCreateWindow("Object Rendering Windows");
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