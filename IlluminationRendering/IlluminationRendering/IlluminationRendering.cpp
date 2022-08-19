#include "stdafx.h"
using namespace vmath;

static const vmath::vec3 X(1.0f, 0.0f, 0.0f);
static const vmath::vec3 Y(0.0f, 1.0f, 0.0f);
static const vmath::vec3 Z(0.0f, 0.0f, 1.0f);

const GLuint verticeNum = 36;
const GLuint surfaceNum = 36;
const GLuint coordinateLen = 3;
const GLuint colorLen = 4;

const GLuint VAONum = 1;
GLuint VAO[VAONum];

const GLuint VBONum = 1;
GLuint VBO[VBONum];

const GLuint EBONum = 1;
GLuint EBO[EBONum];

const GLuint vPosition = 0;
const GLuint vNormal = 1;
const GLuint vColor = 2;

GLuint lightType;
GLuint modelMatirx;
GLuint viewMatrix;
GLuint projectionMatrix;

GLuint lightPosition;
GLuint lightColor;

void init()
{
	//定义顶点数组
	GLfloat sideLen = 0.4f;
	/*GLfloat verticeArray[verticeNum][coordinateLen] = {
		{ -sideLen, -sideLen, -sideLen},
		{ -sideLen, -sideLen, sideLen},
		{ -sideLen, sideLen, -sideLen},
		{ -sideLen, sideLen, sideLen},
		{ sideLen, -sideLen, -sideLen},
		{ sideLen, -sideLen, sideLen},
		{ sideLen, sideLen, -sideLen},
		{ sideLen, sideLen, sideLen}
	};*/

	GLfloat verticeArray[verticeNum][coordinateLen] = {
		//1
		{ -sideLen, -sideLen, -sideLen},
		{ sideLen, -sideLen, -sideLen},
		{ sideLen, sideLen, -sideLen},
		{ sideLen, sideLen, -sideLen},
		{ -sideLen, sideLen, -sideLen},
		{ -sideLen, -sideLen, -sideLen},

		//2
		{ -sideLen, -sideLen, sideLen},
		{ sideLen, -sideLen, sideLen},
		{ sideLen, sideLen, sideLen},
		{ sideLen, sideLen, sideLen},
		{ -sideLen, sideLen, sideLen},
		{ -sideLen, -sideLen, sideLen},

		//3
		{ -sideLen, sideLen, sideLen},
		{ -sideLen, sideLen, -sideLen},
		{ -sideLen, -sideLen, -sideLen},
		{ -sideLen, -sideLen, -sideLen},
		{ -sideLen, -sideLen, sideLen},
		{ -sideLen, sideLen, sideLen},

		//4
		{ sideLen, sideLen, sideLen},
		{ sideLen, sideLen, -sideLen},
		{ sideLen, -sideLen, -sideLen},
		{ sideLen, -sideLen, -sideLen},
		{ sideLen, -sideLen, sideLen},
		{ sideLen, sideLen, sideLen},

		//5
		{ -sideLen, -sideLen, -sideLen},
		{ sideLen, -sideLen, -sideLen},
		{ sideLen, -sideLen, sideLen},
		{ sideLen, -sideLen, sideLen},
		{ -sideLen, -sideLen, sideLen},
		{ -sideLen, -sideLen, -sideLen},
		
		//6
		{ -sideLen, sideLen, -sideLen},
		{ sideLen, sideLen, -sideLen},
		{ sideLen, sideLen, sideLen},
		{ sideLen, sideLen, sideLen},
		{ -sideLen, sideLen, sideLen},
		{ -sideLen, sideLen, -sideLen}
	};

	//定义法线数组
	/*GLfloat normalArray[surfaceNum][coordinateLen] = {
		{ 1.0f, 0.0f, 0.0f},
		{ 0.0f, 1.0f, 0.0f},
		{ 0.0f, 0.0f, 1.0f},
		{ -1.0f, 0.0f, 0.0f},
		{ 0.0f, -1.0f, 0.0f},
		{ 0.0f, 0.0f, -1.0f}
	};*/

	GLfloat normalArray[surfaceNum][coordinateLen] = {
		//1
		{ 0.0f, 0.0f, -1.0f},
		{ 0.0f, 0.0f, -1.0f},
		{ 0.0f, 0.0f, -1.0f},
		{ 0.0f, 0.0f, -1.0f},
		{ 0.0f, 0.0f, -1.0f},
		{ 0.0f, 0.0f, -1.0f},

		//2
		{ 0.0f, 0.0f, 1.0f},
		{ 0.0f, 0.0f, 1.0f},
		{ 0.0f, 0.0f, 1.0f},
		{ 0.0f, 0.0f, 1.0f},
		{ 0.0f, 0.0f, 1.0f},
		{ 0.0f, 0.0f, 1.0f},

		//3
		{ -1.0f, 0.0f, 0.0f},
		{ -1.0f, 0.0f, 0.0f},
		{ -1.0f, 0.0f, 0.0f},
		{ -1.0f, 0.0f, 0.0f},
		{ -1.0f, 0.0f, 0.0f},
		{ -1.0f, 0.0f, 0.0f},

		//4
		{ 1.0f, 0.0f, 0.0f},
		{ 1.0f, 0.0f, 0.0f},
		{ 1.0f, 0.0f, 0.0f},
		{ 1.0f, 0.0f, 0.0f},
		{ 1.0f, 0.0f, 0.0f},
		{ 1.0f, 0.0f, 0.0f},

		//5
		{ 0.0f, -1.0f, 0.0f},
		{ 0.0f, -1.0f, 0.0f},
		{ 0.0f, -1.0f, 0.0f},
		{ 0.0f, -1.0f, 0.0f},
		{ 0.0f, -1.0f, 0.0f},
		{ 0.0f, -1.0f, 0.0f},

		//6
		{ 0.0f, 1.0f, 0.0f},
		{ 0.0f, 1.0f, 0.0f},
		{ 0.0f, 1.0f, 0.0f},
		{ 0.0f, 1.0f, 0.0f},
		{ 0.0f, 1.0f, 0.0f},
		{ 0.0f, 1.0f, 0.0f}
	};

	//定义顶点颜色
	/*GLfloat colorArray[verticeNum][colorLen] = {
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 0.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f, 1.0f },
		{ 0.0f, 1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f, 1.0f },
		{ 0.6f, 0.6f, 0.6f, 1.0f }
	};*/

	GLfloat colorArray[verticeNum][colorLen] = {
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 0.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f, 1.0f },
		{ 0.0f, 1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f, 1.0f }
	};

	//定义索引数组
	GLuint indexArray[] = {
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticeArray) + sizeof(normalArray) + sizeof(colorArray), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verticeArray), verticeArray);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(verticeArray), sizeof(normalArray), normalArray);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(verticeArray) + sizeof(normalArray), sizeof(colorArray), colorArray);

	//关联EBO：在显存中存储索引数据
	glGenBuffers(EBONum, EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexArray), indexArray, GL_STATIC_DRAW);

	//链接着色器
	ShaderInfo shaderInfo[] = {
		{ GL_VERTEX_SHADER, "IlluminationRendering.vert" },
		{ GL_FRAGMENT_SHADER, "IlluminationRendering.frag" },
		{ GL_NONE, NULL}
	};
	GLuint program = LoadShaders(shaderInfo);
	glUseProgram(program);

	//链接顶点属性：定义如何处理顶点数据
	glVertexAttribPointer(vPosition, coordinateLen, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer(vNormal, coordinateLen, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(verticeArray)));
	glVertexAttribPointer(vColor, colorLen, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(verticeArray) + sizeof(normalArray)));
	glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(vNormal);
	glEnableVertexAttribArray(vColor);

	lightType = glGetUniformLocation(program, "lightType");
	modelMatirx = glGetUniformLocation(program, "modelMatrix");
	viewMatrix = glGetUniformLocation(program, "viewMatrix");
	projectionMatrix = glGetUniformLocation(program, "projectionMatrix");

	lightPosition = glGetUniformLocation(program, "lightPosition");
	lightColor = glGetUniformLocation(program, "lightColor");

	glUniform3fv(lightPosition, 1, vec3(0.0f, 1.0f, 1.0f));//光源位置
	glUniform3fv(lightColor, 1, vec3(1.0f, 1.0f, 1.0f));//光源颜色
}

void display()
{
	//绘制物体
	float width = glutGet(GLUT_WINDOW_WIDTH);
	float height = glutGet(GLUT_WINDOW_HEIGHT);
	float ratio = width / height;

	float time = float(GetTickCount64() & 0x1FFF) / float(0x1FFF);//时间滴答

	mat4 modelModel(
		translate(0.0f, 0.0f, 0.0f)
		//* rotate(45.0f, Y)
		* rotate(time * 360.0f, Y)
	);//模型矩阵
	mat4 viewModel(
		translate(0.0f, 0.0f, -2.0f) 
		* rotate(16.0f, X)
	);//视角矩阵
	mat4 projectionModel(vmath::customFrustum(-1.0f, 1.0f, -ratio, ratio, 0.1f, 100.0f));//正投影矩阵
	//mat4 projectionModel(vmath::customPerspective(-1.0f, 1.0f, -ratio, ratio, 1.8f, 500.0f));//透视投影矩阵

	glUniformMatrix4fv(modelMatirx, 1, GL_FALSE, modelModel);
	glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, viewModel);
	glUniformMatrix4fv(projectionMatrix, 1, GL_FALSE, projectionModel);

	glDisable(GL_CULL_FACE);//背面裁剪
	glEnable(GL_DEPTH_TEST);//深度测试

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//准备Buffers颜色输入
	glBindVertexArray(VAO[0]);
	/*glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFF);
	glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_INT, 0);*/
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glFlush();

	glutPostRedisplay();
}

void keyboardFunc(GLboolean key, GLint x, GLint y)
{
	switch (key)
	{
	case 'a':
		glUniform1ui(lightType, 0);
		cout << "Phong光照模型" << endl;
		break;
	case 's':
		glUniform1ui(lightType, 1);
		cout << "Gouraud光照模型" << endl;
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