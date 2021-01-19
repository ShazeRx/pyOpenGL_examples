#include <math.h>
#include <gl/glut.h>
#include <stdlib.h>
#include <windows.h>//该库用于绘制文字
#include <stdio.h>
// ASCII字符总共只有0到127，一共128种字符
#define MAX_CHAR 128
float xPoint = 0; 
float yPoint = 0.0f; 
float d_t = 0.0f;//计算时间间隔
int tHz = 20;//多少毫秒刷新一次
static float xRot = 70.0f;
static float yRot = 0.0f;
static float zRot = 100.0f;
//视角位置
int pause = 0;//暂停模式
float Amplitude = 1.0f;//振幅
float Omega = 1.0f;//ω
float velocity = 1.0f;//波速u
float Phase = 0.0f;//相位φ
//横波的三个参数
char show_1[10] ;
void drawString(const char* str) {
    static int isFirstCall = 1;
    static GLuint lists;
    if( isFirstCall ) { // 如果是第一次调用，执行初始化
                        // 为每一个ASCII字符产生一个显示列表
        isFirstCall = 0;
        // 申请MAX_CHAR个连续的显示列表编号
        lists = glGenLists(MAX_CHAR);
        // 把每个字符的绘制命令都装到对应的显示列表中
        wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
    }
    // 调用每个字符对应的显示列表，绘制每个字符
    for(; *str!='\0'; ++str)
        glCallList(lists + *str);
}
void RenderScene(void) {
	//用当前的清除色清除窗口
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//如果这一条被注释，那么前一帧的图像不会被清楚
	//保存矩阵状态并进行旋转
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	gluLookAt(70.0f, 0.0f, 100.0f, 70.0f, 0.0f, 0, 0, 1, 0);
	glRasterPos3f(100.0f,-15.0f,0.0f);//显示文字的坐标
	sprintf(show_1,"A=%f",Amplitude);//唯一用到stdio.h的函数
    drawString(show_1);//这里显示文字
	glRasterPos3f(100.0f,-18.0f,0.0f);
	sprintf(show_1,"w=%f",Omega);
    drawString(show_1);
	glRasterPos3f(100.0f,-21.0f,0.0f);
	sprintf(show_1,"u=%f",velocity);
    drawString(show_1);
	glRasterPos3f(100.0f,-24.0f,0.0f);
	sprintf(show_1,"p=%f",Phase);
    drawString(show_1);
	glPointSize(2);
	glLineWidth(3);
	glShadeModel(GL_SMOOTH);
	glBegin(GL_LINES);
	glColor3ub(255, 0, 0);
	glVertex3f(-6.0f,  0.0f, 0.0f);
	glVertex3f( 160.0f,  0.0f, 0.0f);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f( 70.0f, -12.0f, 0.0f);
	glVertex3f( 70.0f,  12.0f, 0.0f);
	glEnd();//绘制坐标系
	for(xPoint=0;xPoint<255;xPoint+=0.1f)
	{
		yPoint=Amplitude*sin(Omega*(d_t-xPoint/velocity)+Phase)*5;//计算纵坐标
		glColor3ub(0, 255, 0);		
		glBegin(GL_POINTS);
		glVertex3f(xPoint, yPoint, 0.0f);
		glEnd();

	}
	if(pause==0)d_t++;else;
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
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f );
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

void keyboard (unsigned char key, int x, int y) {
   switch (key) {
	   case 'p':  if(pause == 0)pause = 1;else pause = 0;  break;
	   case 'P':  if(pause == 0)pause = 1;else pause = 0;  break;
	   case 'q':  Amplitude += 0.1f;  break;
	   case 'Q':  Amplitude += 0.1f;  break;
	   case 'w':  Amplitude -= 0.1f;  break;
	   case 'W':  Amplitude -= 0.1f;  break;
	   case 'a':  Omega += 0.1f;  break;
	   case 'A':  Omega += 0.1f;  break;
	   case 's':  Omega -= 0.1f;  break;
	   case 'S':  Omega -= 0.1f;  break;
	   case 'z': velocity += 0.1f;  break;
	   case 'Z': velocity += 0.1f;  break;
	   case 'x': if(velocity>0.1f)velocity -= 0.1f;  break;
	   case 'X': if(velocity>0.1f)velocity -= 0.1f;  break;
	   case 'e': Phase += 0.3927f;  break;
	   case 'E': Phase += 0.3927f;  break;
	   case 'r': Phase -= 0.3927f;  break;
	   case 'R': Phase -= 0.3927f;  break;
	   case 27:   exit(0); 
	   default:   break;
   }
	glutPostRedisplay();
}
int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(800, 600);
	glutCreateWindow("波的参数，p暂停，qw振幅，as角速度，zx波速，er相位");
	glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(RenderScene);
	glutTimerFunc(250, TimerFunc, 1);
	SetupRC();
	glutMainLoop();
	return 0;
}
