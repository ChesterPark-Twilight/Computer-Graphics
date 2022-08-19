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
	//���嶥������
	GLfloat R = 0.6;//�����������Բ�뾶
	GLfloat r = -R * 2 * sqrt(2.0) / 3.0;//�����������Բ�뾶
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

	//���嶥����ɫ
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

	//������������
	GLuint indexArray[] = {
		1, 2, 3, 0, 1, 2,
		6, 4, 2, 0, 3, 1, 7, 5,
		0xFFFF,
		1, 0, 5, 4, 7, 6, 3, 2
	};

	//����VAO��������������
	glGenVertexArrays(VAONum, VAO);
	glBindVertexArray(VAO[0]);

	//����VBO�����Դ��д洢��������
	glGenBuffers(VBONum, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tetrahedronArray) + sizeof(tetrColorArray) + sizeof(cubeArray) + sizeof(cubeColorArray), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(tetrahedronArray), tetrahedronArray);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(tetrahedronArray), sizeof(tetrColorArray), tetrColorArray);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(tetrahedronArray) + sizeof(tetrColorArray), sizeof(cubeArray), cubeArray);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(tetrahedronArray) + sizeof(tetrColorArray) + sizeof(cubeArray), sizeof(cubeColorArray), cubeColorArray);

	//����EBO�����Դ��д洢��������
	glGenBuffers(EBONum, EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexArray), indexArray, GL_STATIC_DRAW);

	//������ɫ��
	ShaderInfo shaderInfo[] = {
		{ GL_VERTEX_SHADER, "ObjectRendering.vert" },
		{ GL_FRAGMENT_SHADER, "ObjectRendering.frag" },
		{ GL_NONE, NULL}
	};
	GLuint program = LoadShaders(shaderInfo);
	glUseProgram(program);

	//���Ӷ������ԣ�������δ���������
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

	float time = float(GetTickCount64() & 0x1FFF) / float(0x1FFF);//ʱ��δ�

	mat4 modelModel(rotate(time * 360.0f, Y) * rotate(time * 360.0f, Z));//ģ�;���
	mat4 viewModel(translate(0.0f, 0.0f, -2.4f));//�ӽǾ���
	mat4 projectionModel;
	if (projectionType == frustumTag)
	{
		projectionModel = vmath::customFrustum(-1.0f, 1.0f, -ratio, ratio, 1.0f, 100.0f);//��ͶӰ����
	}
	else
	{
		projectionModel = vmath::customPerspective(-1.0f, 1.0f, -ratio, ratio, 1.6f, 100.0f);//͸��ͶӰ����
	}
	glUniformMatrix4fv(modelMatirx, 1, GL_FALSE, modelModel);
	glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, viewModel);
	glUniformMatrix4fv(projectionMatrix, 1, GL_FALSE, projectionModel);

	cullFace ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);//����ü�
	depthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);//��Ȳ���

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//׼��Buffers��ɫ����
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
		glUniform1ui(objectType, tetrPos);//����������
		renderType = tetrPos;
		break;
	case 'c':
		glUniform1ui(objectType, cubePos);//����������
		renderType = cubePos;
		break;
	case 'p':
		if (projectionType == frustumTag)//ͶӰ��ʽ�л�
		{
			projectionType = perspectiveTag;
			cout << "����͸��ͶӰ" << endl;
		}
		else
		{
			projectionType = frustumTag;
			cout << "������ͶӰ" << endl;
		}
		break;
	case '1':							//����ü�����
		if (cullFace)
		{
			cullFace = false;
			cout << "�رձ���ü�" << endl;
		}
		else
		{
			cullFace = true;
			cout << "��������ü�" << endl;
		}
		break;
	case '2':							//��Ȳ��Կ���
		if (depthTest)
		{
			depthTest = false;
			cout << "�ر���Ȳ���" << endl;
		}
		else
		{
			depthTest = true;
			cout << "������Ȳ���" << endl;
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