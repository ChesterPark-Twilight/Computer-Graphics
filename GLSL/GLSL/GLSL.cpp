// GLSL.cpp : �������̨Ӧ�ó������ڵ㡣

#include "stdafx.h"
enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };
GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];
const GLuint NumVertices = 9;//���鳤��
GLint Mode_View_Loc;
GLint Mode_Size_loc;
GLfloat Mode_Size = 10.0 /10.0;
GLuint Render_Mode_loc;
GLuint Render_Mode = 0;
const GLuint Max_Render_Mode = 20;

//��ʼ��
void init(void) {
	glGenVertexArrays(NumVAOs, VAOs);//����NumVAOs��array�����ƣ�����VAOs������
	glBindVertexArray(VAOs[Triangles]);//�������array�����붥�����������������������arrayִ�в�ͬ�Ĺ�������

	GLfloat pi = 3.1415;
	GLfloat radian = pi / 180.0;
	GLfloat R = 0.8;
	GLfloat r = R * sin(18 * radian) / cos(36 * radian);
	GLfloat firstAbscissa = R * cos(18 * radian);
	GLfloat firstOrdinate = R * sin(18 * radian);
	GLfloat secondAbscissa = R * sin(36 * radian);
	GLfloat secondOrdinate = R * cos(36 * radian);
	GLfloat shortAbscissa = r * sin(72 * radian);
	GLfloat shortOrdinate = r * cos(72 * radian);
	GLfloat vertices[NumVertices][2] = {
		{ 0.0, R},
		{ -secondAbscissa, -secondOrdinate},
		{ shortAbscissa, -shortOrdinate},
		{ 0.0, R},
		{-shortAbscissa, -shortOrdinate},
		{ secondAbscissa, -secondOrdinate},
		{ -firstAbscissa, firstOrdinate},
		{ firstAbscissa, firstOrdinate},
		{ 0.0, -r}
	};//���嶥����������

	glGenBuffers(NumBuffers, Buffers);//����NumBuffers��δʹ�õĻ���������ƣ�����Buffers������
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);//�������buffer���������������������������������bufferִ�в�ͬ�Ĺ�������
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//�����ݿ������Դ��У�1.�������������� 2.����Ŀռ��С 3.ָ���ڴ��ָ�� 4.Usage

	ShaderInfo shader[] = {
		{ GL_VERTEX_SHADER, "GLSL.vert" },
		{ GL_FRAGMENT_SHADER, "GLSL.frag" },
		{ GL_NONE, NULL },
	};//��ɫ���ṹ��
	GLuint program = LoadShaders(shader);//װ�ء����������롢������ɫ��������Program
	glUseProgram(program);//����Program

	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));//���嶥����������: 1.����λ��ֵ(��Ӧ������ɫ��position) 2.�����С 3.������������ 4.��׼�� 5.����(0����OpenGL����) 6.ƫ����
	glEnableVertexAttribArray(vPosition);

	Mode_Size_loc = glGetUniformLocation(program, "size");
	Render_Mode_loc = glGetUniformLocation(program, "render_type");//��λ��ɫ���е�uniform����

	glUniform1f(Mode_Size_loc, Mode_Size);
}

GLuint i;

//���õ���˵�
void setMenu()
{
	glutAddMenuEntry("org", 0);
	glutAddMenuEntry("abs", 1);
	glutAddMenuEntry("red", 2);
	glutAddMenuEntry("abs(red)", 3);
	glutAddMenuEntry("abs(green)", 4);
	glutAddMenuEntry("abs(pos)", 5);
	glutAddMenuEntry("gray", 6);
	glutAddMenuEntry("another_gray", 7);
	glutAddMenuEntry("gl_FragCoord", 8);
	glutAddMenuEntry("cos", 9);
	glutAddMenuEntry("sin", 10);
	glutAddMenuEntry("sin+cos", 11);
	glutAddMenuEntry("cos(color)", 12);
	glutAddMenuEntry("sin+cos", 13);
	glutAddMenuEntry("sin*cos", 14);
	glutAddMenuEntry("customOne", 15);
	glutAddMenuEntry("customTwo", 16);
	glutAddMenuEntry("customThree", 17);
	glutAddMenuEntry("customFour", 18);
	glutAddMenuEntry("customFive", 19);
	glutAttachMenu(GLUT_LEFT_BUTTON);
}

//���ò˵�����¼�
void processMenu(int key)
{
	Render_Mode = key;
	glUniform1ui(Render_Mode_loc, Render_Mode);
	glFlush();
	glutPostRedisplay();
}

//��ʾͼ��
void display() {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);//׼��Buffers��ɫ����
	glBindVertexArray(VAOs[VAO_IDs::Triangles]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);//����ͼԪ��1.ͼԪװ������ 2.����Ԫ����ʼλ�� 3.���鳤��
	glFlush();//����ǿ�������Կ������������أ�����CPU��GPU��ͬ��
}

//���ð����¼�
void keyfunc(GLboolean key, GLint x, GLint y) {
	switch (key)
	{
	case 'd':
		glutFullScreen();
		break;
	case 'e':
		glutReshapeWindow(700, 700);
		glutPositionWindow(100, 100);
		break;
	case 'q':
		glutExit();
		break;
	case 'a':
		if (Render_Mode < Max_Render_Mode-1)
			Render_Mode++;
		else
			Render_Mode = 0;
		glUniform1ui(Render_Mode_loc, Render_Mode);
		cout << "Render_Mode :" << Render_Mode << endl;;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);//��ʼ��GLUT��
	glutInitDisplayMode(GLUT_RGBA);//���ô�������
	glutInitWindowSize(700, 700);//���ô��ڴ�С
	glutInitContextVersion(4, 3);//����GLSL�汾
	glutInitContextProfile(GLUT_CORE_PROFILE);//���ú���ģʽ
	glutCreateWindow("GLSL Windows");//��������
	glewExperimental = GL_TRUE;
	if (glewInit())
	{
		cerr << "Unable to Initialize GLEW" << endl;
	}
	init();//��ʼ������
	glutKeyboardFunc(keyfunc);//���ð������Ч��
	glutCreateMenu(processMenu);//���ö�Ӧ���Ч��
	setMenu();//���õ���˵�
	glutDisplayFunc(display);//�ص���ʾ����
	glutMainLoop();//GLUT��ѭ��
	return 0;
}