#include <math.h>
#include <gl/glut.h>
#include <stdlib.h>

int xBall = 0; //某个球的位置横坐标
float yBall = 0.0f; //某个球的位置纵坐标
float d_t = 0.0f;//计算时间间隔
int tHz = 30;//多少毫秒刷新一次
static float xRot = 50.0f;
static float yRot = 0.0f;
static float zRot = 100.0f;
//视角位置
int clourMode = 1;//着色模式
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
	glLineWidth(8);
	glShadeModel(GL_SMOOTH);
	glColor3ub(255, 0, 0);
	glVertex3f(-6.0f,  0.0f, 0.0f);
	glVertex3f( 720.0f,  0.0f, 0.0f);
	glEnd();
//	glBegin(GL_LINES);
//	glVertex3f( 70.0f, -12.0f, 0.0f);
//	glVertex3f( 70.0f,  12.0f, 0.0f);
// 	glEnd();//绘制坐标系
	for(xBall=0;xBall<255;xBall++)
	{
		if(xBall<x_Max)
			yBall=sin(xBall-d_t*0.1)*5;//计算纵坐标
		else
			yBall=0.0f;
		glTranslatef(0.0f,yBall,0.0f);//移动到绘制球的点
		if(clourMode==0)
		{
			if(yBall>4.0f)
				glColor3ub(255, 0, 0);
				else if(yBall>3.0f)
				glColor3ub(255, 125, 0);
				else if(yBall>2.0f)
				glColor3ub(255, 255, 0);
				else if(yBall>1.0f)
				glColor3ub(0, 255, 0);
				else if(yBall>0.0f)
				glColor3ub(0, 0, 255);
				else if(yBall>-1.0f)
				glColor3ub(0, 255, 255);
				else if(yBall>-2.0f)
				glColor3ub(255, 0, 255);
				else if(yBall>-4.0f)
				glColor3ub(255, 125, 255);
				else
					glColor3ub(255, 255, 255);


		}
		else if(clourMode==1)
		{
			clourOne=xBall%7;
			switch(clourOne)
			{
			case 0:glColor3ub(255, 0, 0);break;//红色
			case 1:glColor3ub(255, 125, 0);break;//橙色
			case 2:glColor3ub(255, 255, 0);break;//黄色
			case 3:glColor3ub(0, 255, 0);break;//绿色
			case 4:glColor3ub(0, 0, 255);break;//蓝色
			case 5:glColor3ub(0, 255, 255);break;//靛色
			case 6:glColor3ub(255, 0, 255);break;//紫色
			}
		}

		glutSolidSphere(0.5f, 18, 17);
		glTranslatef(1.0f,-yBall,0.0f);//移动到下一个球的平衡点
	}
	if(x_Max<256.0f)
	x_Max=x_Max+0.1f;
	d_t++;
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
   glutTimerFunc(tHz, TimerFunc, 1);//(毫秒数、回调函数指针、区别值）
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
	   case 'z': if(tHz>9)tHz -= 5;  break;
	   case 'Z': if(tHz>9)tHz -= 5;  break;
	   case 'x': if(tHz<50)tHz += 5;  break;
	   case 'X': if(tHz>9)tHz += 5;  break;
	   case 'c': {clourMode++;if(clourMode>2)clourMode=0;} break;
	   case 'C': {clourMode++;if(clourMode>2)clourMode=0;} break;
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
	glutCreateWindow("横波演示，wasd控制视角，zx控制速度，c控制颜色，q重置");
	glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(myMouse);
	glutDisplayFunc(RenderScene);
	glutTimerFunc(250, TimerFunc, 1);
	SetupRC();
	glutMainLoop();
	return 0;
}
