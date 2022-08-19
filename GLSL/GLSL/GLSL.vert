#version 430 core //设定版本
layout(location = 0) in vec2 vPosition;//从buffer传进
out vec4 color;//输出至下个阶段（细分，几何，或者直接是片元）
//此为最简单的Gouraund着色
uniform float size;
uniform uint render_type;
void main()
{ 
	gl_Position = vec4(vPosition.x*size,vPosition.y*size,1,1);
	
	color = vec4(vec2(gl_Position),0,1);

	/*
	color.r = gl_Position.x;
	color.g = gl_Position.y;
	color.b = 0.0f;
	color.a = 1.0f;
	*/
}