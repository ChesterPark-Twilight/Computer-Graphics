// GLSL.cpp : 定义控制台应用程序的入口点。

#include "stdafx.h"
enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };
GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];
const GLuint NumVertices = 9;//数组长度
GLint Mode_View_Loc;
GLint Mode_Size_loc;
GLfloat Mode_Size = 10.0 /10.0;
GLuint Render_Mode_loc;
GLuint Render_Mode = 0;
const GLuint Max_Render_Mode = 20;

//初始化
void init(void) {
	glGenVertexArrays(NumVAOs, VAOs);//生成NumVAOs个array的名称，放在VAOs数组里
	glBindVertexArray(VAOs[Triangles]);//将输入的array对象与顶点数组对象关联，根据输入的array执行不同的关联操作

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
	};//定义顶点坐标数组

	glGenBuffers(NumBuffers, Buffers);//生成NumBuffers个未使用的缓存对象名称，放在Buffers数组里
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);//将输入的buffer对象与已命名缓存对象关联，根据输入的buffer执行不同的关联操作
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//将数据拷贝到显存中：1.拷贝的数据类型 2.分配的空间大小 3.指向内存的指针 4.Usage

	ShaderInfo shader[] = {
		{ GL_VERTEX_SHADER, "GLSL.vert" },
		{ GL_FRAGMENT_SHADER, "GLSL.frag" },
		{ GL_NONE, NULL },
	};//着色器结构体
	GLuint program = LoadShaders(shader);//装载、解析、编译、链接着色器，返回Program
	glUseProgram(program);//激活Program

	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));//定义顶点属性数据: 1.顶点位置值(对应顶点着色器position) 2.顶点大小 3.顶点数据类型 4.标准化 5.步长(0则交由OpenGL处理) 6.偏移量
	glEnableVertexAttribArray(vPosition);

	Mode_Size_loc = glGetUniformLocation(program, "size");
	Render_Mode_loc = glGetUniformLocation(program, "render_type");//定位着色器中的uniform数据

	glUniform1f(Mode_Size_loc, Mode_Size);
}

GLuint i;

//设置点击菜单
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

//设置菜单点击事件
void processMenu(int key)
{
	Render_Mode = key;
	glUniform1ui(Render_Mode_loc, Render_Mode);
	glFlush();
	glutPostRedisplay();
}

//显示图形
void display() {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);//准备Buffers颜色输入
	glBindVertexArray(VAOs[VAO_IDs::Triangles]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);//绘制图元：1.图元装配类型 2.数组元素起始位置 3.数组长度
	glFlush();//命令强制输入显卡并且立即返回，导致CPU和GPU不同步
}

//设置按键事件
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
	glutInit(&argc, argv);//初始化GLUT库
	glutInitDisplayMode(GLUT_RGBA);//设置窗口类型
	glutInitWindowSize(700, 700);//设置窗口大小
	glutInitContextVersion(4, 3);//设置GLSL版本
	glutInitContextProfile(GLUT_CORE_PROFILE);//设置核心模式
	glutCreateWindow("GLSL Windows");//创建窗口
	glewExperimental = GL_TRUE;
	if (glewInit())
	{
		cerr << "Unable to Initialize GLEW" << endl;
	}
	init();//初始化操作
	glutKeyboardFunc(keyfunc);//设置按键点击效果
	glutCreateMenu(processMenu);//设置对应点击效果
	setMenu();//设置点击菜单
	glutDisplayFunc(display);//回调显示函数
	glutMainLoop();//GLUT主循环
	return 0;
}