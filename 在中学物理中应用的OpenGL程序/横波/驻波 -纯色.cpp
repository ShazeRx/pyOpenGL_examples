#include <math.h>
#include <gl/glut.h>
#include <stdlib.h>

float xBall = 0.0f; //某个球的位置横坐标
float xDraw = 0.0f; //绘制坐标
float yBall = 0.0f; //某个球的位置纵坐标
float d_t = 0.0f;//计算时间间隔
float tHz = 0.6f;//多少毫秒刷新一次
static float xRot = 50.0f;
static float yRot = 0.0f;
static float zRot = 100.0f;
//视角位置
float border_line = 80.0f;

int clourOne = 0;//模式1下的取余数
float x_Max = 0.0f ;//传播时的位置
void RenderScene(void) {
	//用当前的清除色清除窗口
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//如果这一条被注释，那么前一帧的图像不会被清楚
	//保存矩阵状态并进行旋转
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
		gluLookAt(xRot, yRot, zRot, xRot, yRot, 0, 0, 1, 0);
	glBegin(GL_LINES);
	glColor3ub(255, 0, 0);
	glLineWidth(8);
	glShadeModel(GL_SMOOTH);
	glVertex3f( border_line, -80.0f, 0.0f);
	glVertex3f( border_line,  80.0f, 0.0f);
 	glEnd();//绘制坐标系
	for(xBall=0;xBall<160;xBall+=0.1f)
	{
		if(xBall<x_Max)
		{
			if(xBall<border_line)
				yBall=sin(xBall-d_t*0.1)*5;//计算纵坐标
			else
				yBall=sin(xBall-d_t*0.1)*5;
		}
		else
			yBall=0.0f;
		if(xBall<border_line)
		{
			yBall+=30;
			xDraw=xBall;
		}
		else
		{
			xDraw=2*border_line-xBall;
		}
		glTranslatef(xDraw,yBall,0.0f);//移动到绘制球的点
		
		
		glColor3ub(0, 0, 255);//蓝色
		glutSolidSphere(0.1f, 8, 8);
		glTranslatef(-xDraw,-yBall,0.0f);//恢复矩阵
		if(x_Max<border_line)
		{
			if(xBall<border_line)
			{
				if(xBall>x_Max)
					yBall=0.0f;
				else
					yBall=sin(xBall-d_t*0.1)*5;
				yBall-=30;
				xDraw=xBall;
				glTranslatef(xDraw,yBall,0.0f);
				glutSolidSphere(0.1f, 8, 8);
				glTranslatef(-xDraw,-yBall,0.0f);
			}//绘制驻波
		}
		else
		{
			if(xBall>border_line)
			{
				if(xBall>x_Max)
					yBall=sin((2*border_line-xBall)-d_t*0.1)*5;
				else
					yBall=sin(xBall-d_t*0.1)*5+sin((2*border_line-xBall)-d_t*0.1)*5;
				yBall-=30;
				xDraw=2*border_line-xBall;
				glTranslatef(xDraw,yBall,0.0f);
				glutSolidSphere(0.1f, 8, 8);
				glTranslatef(-xDraw,-yBall,0.0f);
			}//绘制驻波
		}
		
	}
	
	if(x_Max<161.0f)
	{
		if(x_Max<border_line)
			x_Max=x_Max+tHz*0.1f;
		else
			x_Max=x_Max+tHz*0.1f;
	}
	d_t+=tHz;
	if(d_t>31415)
		d_t-=31415;
	//恢复矩阵状态
	glPopMatrix();
	glutSwapBuffers();//交换两个缓冲区的指针
} 

void SetupRC() {
	glEnable(GL_DEPTH_TEST);	// 启用之后，gl只会绘制最前面一层，gl会检查当前像素前面是否有别的像素，如果别的像素挡道了它，那它就不会绘制
	glFrontFace(GL_CCW);
	//GL_CCW 表示窗口坐标上投影多边形的顶点顺序为逆时针方向的表面为正面。GL_CW 表示顶点顺序为顺时针方向的表面为正面。
	glEnable(GL_CULL_FACE);		// 开启剔除操作效果
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glClearColor(0.3f, 0.3f, 0.3f, 0.0f );
} 

void TimerFunc(int value) {
   glutPostRedisplay();//将当前的窗口重新绘制
   glutTimerFunc(30, TimerFunc, 1);//(毫秒数、回调函数指针、区别值）
}


void ChangeSize(int w, int h) {//保证窗口拉伸后不发生形变
    float fAspect;
    if (h == 0)  h = 1;
    glViewport(0, 0, w, h);
    fAspect = (float)w/(float)h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, fAspect, 1.0, 455.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void myMouse(int button, int state, int x, int y) {
	switch (button) {
	case GLUT_RIGHT_BUTTON:
		if(zRot<150.0f)zRot += 5.0f; break;
	case GLUT_LEFT_BUTTON:
		if(zRot>50.0f)zRot -= 5.0f; break;
	}
}

void keyboard (unsigned char key, int x, int y) {
   switch (key) {
	   case 'a':  xRot -= 1.0f;  break;
	   case 'A':  xRot -= 1.0f;  break;
	   case 'd':  xRot += 1.0f;  break;
	   case 'D':  xRot += 1.0f;  break;
	   case 'w':  yRot += 1.0f;  break;
	   case 'W':  yRot += 1.0f;  break;
	   case 's':  yRot -= 1.0f;  break;
	   case 'S':  yRot -= 1.0f;  break;
	   case 'z': if(tHz>0.1f)tHz -= 0.2f;  break;
	   case 'Z': if(tHz>0.1f)tHz -= 0.2f;  break;
	   case 'x': if(tHz<6.0f)tHz += 0.2f;  break;
	   case 'X': if(tHz<6.0f)tHz += 0.2f;  break;
	   case 'q': x_Max = 0.0f;d_t = 0.0f; break;
	   case 'Q': x_Max = 0.0f;d_t = 0.0f; break;
	   case 27:   exit(0); 
	   default:   break;
   }
	glutPostRedisplay();
}
int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(800, 600);
	glutCreateWindow("驻波-纯色，wasd控制视角，zx控制速度，c控制颜色，q重置");
	glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(myMouse);
	glutDisplayFunc(RenderScene);
	glutTimerFunc(250, TimerFunc, 1);
	SetupRC();
	glutMainLoop();
	return 0;
}
