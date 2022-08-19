#version 430 core //设定版本
in vec4 color; // 从上一个阶段传进
out vec4 fColor; // 输出至下个阶段
uniform uint render_type;
float getMax(float a, float b)
{
	return a>b ? a : b;
}
void main()
{
	float b = 3.1f;
	mat4 m = mat4(1);
	vec4 v = m[0];
	float gray = dot(color.rgb, vec3(0.299,0.587, 0.114));
	switch(render_type){
		case 0:
			fColor = color;
			break;
		case 1:
			fColor = abs(vec4(1.0, 1.0, 1.0, 0.0)*color)*b;
			break;
		case 2:
			fColor = vec4(1.0, 1.0, 1.0, 1.0)*v;
			break;
		case 3:
			if(color.r < 0)
			{
				fColor.r = abs(color.r);
			}
			else
				fColor = color;
			break;
		case 4:
			if(color.g <0)
			{
				fColor.g = abs(color.g);
				fColor.r = color.r;
				fColor.b = color.b;
				fColor.a = color.a;
			}
			else
				fColor = color;
			break;
		case 5:
			fColor = abs(color);
			break;
		case 6:
			fColor = vec4(gray,gray,gray,1);
			break;
		case 7:
			fColor = vec4(gray);
			fColor.a = 1;
			break;
		case 8:
			fColor = vec4(gl_FragCoord.x / 250,gl_FragCoord.y / 250,0,1);
			break;
		case 9:
			fColor = cos(gl_FragCoord);
			break;
		case 10:
			fColor = sin(gl_FragCoord);
			break;
		case 11:
			fColor = sin(gl_FragCoord) + cos(gl_FragCoord);
			break;
		case 12:
			fColor = cos(color);
		    break;
		case 13:
			fColor = sin(color)+cos(color);
			break;
		case 14:
			fColor = sin(color)*cos(color);
			break;
		case 15:
			fColor.r = color.r;
			fColor.g = color.g;
			fColor.b = 0.6;
			fColor.a = 1;
			break;
		case 16:
			if(color.r < 0) 
			{
				fColor.r = 1.0f;
			}
			else
			{
				fColor.g = 1.0f;
			}
			break;
		case 17:
			fColor.r = 1 - abs(color.r);
			fColor.g = 1 - abs(color.g);
			fColor.b = 1 - abs(color.b);
			break;
		case 18:
			fColor.r = 1 - abs(color.r);
			fColor.g = 1 - abs(color.r);
			fColor.b = 1 - abs(color.r);
			break;
		case 19:
			fColor.r = 1 - getMax(abs(color.r), abs(color.g));
			fColor.g = 1 - getMax(abs(color.r), abs(color.g));
			fColor.b = 1 - getMax(abs(color.r), abs(color.g));
			break;
		default:
			fColor = vec4(1.0,0,0,1);
			break;
	}
}