#include <iostream>
#include <gl/glut.h>
#include "ViewerTrackball.h" 

int xBall = 0; //某个球的位置横坐标
float yBall = 0.0f; //某个球的位置纵坐标
float zBall = 0.0f;//用于三维演示
float d_t = 0.0f;//计算时间间隔
int tHz = 50;//多少毫秒刷新一次
int clourMode = 1;//着色模式
int clourOne = 0;//模式1下的取余数
static float xRot = 0.0f;
static float yRot = 0.0f;
static float zRot = 100.0f;
//视角位置
namespace ViewerN {
//公共变量
bool   isListCreated (GL_FALSE);             //控制是否重新生成显示列表 
float  bgdColor[3] = {0.7f, 0.7f, 0.7f};     //背景色
float  fgdColor[3] = {0.0f, 0.0f, 1.0f};     //前景色 
int    flag(0);
Trackball trackball; 

void init(void);
void display(void);
void resize(int _width, int _height);
void mouse(int _button, int _state, int _x, int _y);
void motion(int _x, int _y);
void idle(void); 
 
void init(void) { //OGL初始化函数
   trackball.init(GLUT_LEFT_BUTTON);

	glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LESS);
 
}

void display(void) {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	gluLookAt(xRot, yRot, zRot, xRot, yRot, 0, 0, 1, 0);
	trackball.matrix(); //旋转  // <<================= !!!!
	trackball.isRedraw_ = GL_FALSE;
	glBegin(GL_LINES);
	glLineWidth(8);
	glShadeModel(GL_SMOOTH);
	glColor3ub(255, 0, 0);
	glVertex3f(-160.0f,  0.0f, 0.0f);
	glVertex3f( 160.0f,  0.0f, 0.0f);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f( 0.0f, -12.0f, 0.0f);
	glVertex3f( 0.0f,  12.0f, 0.0f);
	glEnd();//绘制坐标系
	glTranslatef(-100.0f,0,0);
	for(xBall=0;xBall<255;xBall++)
	{
		yBall=sin(xBall+d_t*0.1)*5;//计算纵坐标
		zBall=cos(xBall+d_t*0.1)*5;
		glTranslatef(0.0f,yBall,zBall);//移动到绘制球的点
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
			clourOne=(xBall)%7;
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
		glTranslatef(1.0f,-yBall,-zBall);//移动到下一个球的平衡点
	}
	d_t++;
	glPopMatrix();	
	glutSwapBuffers();
}
void resize(int _width, int _height) {   //重新确定窗口大小
   trackball.reshape(_width, _height);
   glViewport(0, 0, _width, _height);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   float XYratio = (float)_width/(float)_height;
   gluPerspective(20.2, XYratio, 1.0,-1000);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void mouse(int _button, int _state, int _x, int _y) { 
   trackball.mouse(_button, _state, _x, _y); 
   glutPostRedisplay();
}

void motion(int _x, int _y) { 
   trackball.motion(_x, _y); 
   glutPostRedisplay();
}
} 
void SetupRC() {
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glClearColor(0.3f, 0.3f, 0.3f, 0.0f );
} 

void TimerFunc(int value) {
   glutPostRedisplay();//将当前的窗口重新绘制
   glutTimerFunc(tHz, TimerFunc, 1);//(毫秒数、回调函数指针、区别值）
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
	   case 27:   exit(0); 
	   default:   break;
   }
	glutPostRedisplay();
}
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]) { 
   glutInitWindowPosition(400, 0);
   glutInitWindowSize(620,620);
   glutInitDisplayMode (GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE );  
   if (glutCreateWindow ("横波三维演示") == GL_FALSE)
      exit (1);
	ViewerN::init(); 
	glutDisplayFunc(ViewerN::display);
   glutReshapeFunc(ViewerN::resize);
   	glutKeyboardFunc(keyboard);
   glutMouseFunc(ViewerN::mouse);
   glutMotionFunc(ViewerN::motion);
   glutTimerFunc(250, TimerFunc, 1);
   SetupRC();
   glutMainLoop();
   return 0;
}
