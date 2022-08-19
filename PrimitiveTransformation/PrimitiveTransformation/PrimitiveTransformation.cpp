#include "stdafx.h"

const GLuint verticeNum = 8;
const GLuint coordinateLen = 3;

const GLuint VAONum = 1;
GLuint VAO[VAONum];

const GLuint VBONum = 1;
GLuint VBO[VBONum];

const GLuint EBONum = 1;
GLuint EBO[EBONum];

const GLuint vPosition = 0;

GLuint transformType;

void init()
{
	//定义顶点数组
	GLfloat pi = 3.1415;
	GLfloat radian = pi / 180.0;
	GLfloat R = 0.6;
	GLfloat r = R * sin(18 * radian) / cos(36 * radian);
	GLfloat firstAbscissa = R * cos(18 * radian);
	GLfloat firstOrdinate = R * sin(18 * radian);
	GLfloat secondAbscissa = R * sin(36 * radian);
	GLfloat secondOrdinate = R * cos(36 * radian);
	GLfloat shortAbscissa = r * sin(72 * radian);
	GLfloat shortOrdinate = r * cos(72 * radian);
	GLfloat verticeArray[verticeNum][coordinateLen] = {
		{ 0.0, R, 1.0},
		{ -secondAbscissa, -secondOrdinate, 1.0},
		{ shortAbscissa, -shortOrdinate, 1.0},
		{-shortAbscissa, -shortOrdinate, 1.0},
		{ secondAbscissa, -secondOrdinate, 1.0},
		{ -firstAbscissa, firstOrdinate, 1.0},
		{ firstAbscissa, firstOrdinate, 1.0},
		{ 0.0, -r, 1.0}
	};

	//定义索引数组
	GLuint indexArray[] = {
		0, 1, 2,
		0, 3, 4,
		5, 6, 7
	};

	//关联VAO：保存数据配置
	glGenVertexArrays(VAONum, VAO);
	glBindVertexArray(VAO[0]);

	//关联VBO：在显存中存储顶点数据
	glGenBuffers(VBONum, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticeArray), verticeArray, GL_STATIC_DRAW);

	//关联EBO：在显存中存储索引数据
	glGenBuffers(EBONum, EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexArray), indexArray, GL_STATIC_DRAW);

	//链接着色器
	ShaderInfo shaderInfo[] = {
		{ GL_VERTEX_SHADER, "PrimitiveTransformation.vert" },
		{ GL_FRAGMENT_SHADER, "PrimitiveTransformation.frag" },
		{ GL_NONE, NULL}
	};
	GLuint program = LoadShaders(shaderInfo);
	glUseProgram(program);

	//链接顶点属性：定义如何处理顶点数据
	glVertexAttribPointer(vPosition, coordinateLen, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vPosition);

	transformType = glGetUniformLocation(program, "transformType");
}

void display()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);//准备Buffers颜色输入
	glBindVertexArray(VAO[0]);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
	glFlush();
}

void keyboardFunc(GLboolean key, GLint x, GLint y)
{
	switch (key)
	{
	case 'b':
		glUniform1ui(transformType, 0);//初始绘制状态
		break;
	case 't':
		glUniform1ui(transformType, 1);//平移
		break;
	case '1':
		glUniform1ui(transformType, 2);//绕原点旋转
		break;
	case '2':
		glUniform1ui(transformType, 3);//以原点为中心缩放
		break;
	case '3':
		glUniform1ui(transformType, 7);//饶(0.2, 0.2)旋转
		break;
	case '4':
		glUniform1ui(transformType, 8);//以(0.2, 0.2)为中心缩放
		break;
	case 'c':
		glUniform1ui(transformType, 4);//组合
		break;
	case 'h':
		glUniform1ui(transformType, 5);//错切
		break;
	case 'f':
		glUniform1ui(transformType, 6);//翻转
		break;
	default:
		glUniform1ui(transformType, 0);//初始绘制状态
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
	glutCreateWindow("Space Conversion Windows");
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