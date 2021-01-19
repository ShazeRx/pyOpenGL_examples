#include <math.h>
#include <gl/glut.h>
#include <stdlib.h>
#include <windows.h>//该库用于绘制文字
#include <stdio.h>
// ASCII字符总共只有0到127，一共128种字符
#define MAX_CHAR 128
#define grav 6.67
//万有引力常数，量级为10的-11次方牛顿平方米每平方千克
float m_0 = 1.0f;//默认星体的质量为太阳质量（中心天体），量级为10的30次方千克
float angle_1 = 0.0f;//天体1旋转的角度
float r_1 = 1.0f;
float dAngle_1 = 6.0f;//单位时间内转过的角度，即角速度，这里直接给出初始角速度
int n_1 = 0;//计数器
int n_2 = 0;//计数器
int n_3 = 0;//计数器
float angle_2 = 0.0f;
float r_2 = 1.5f;
float dAngle_2 = 0.0f;//在主程序计算
float dAngle_3 = 0.0f;//角速度的差值
float angle_3 = 0.0f;//角速度差值计数器
float ellipseAng=0.0f;//角度累加器
static float xRot = 0.5f;
static float yRot = 0.0f;
static float zRot = 6.0f;
//视角位置
int trackLines = 1;//可以切换轨道是否显示
int pause = 0 ;//暂停控制器
float bgc = 0.3f;//背景色
int sun_color = 255;//太阳颜色变量1
int flash_mode = 20;//闪烁指示器
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//如果这一条被注释，那么前一帧的图像不会被清除
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	gluLookAt(xRot, yRot, zRot, xRot, yRot, 0, 0, 1, 0);//第4~6个变量可以用来追踪某一个星星
	glRasterPos3f(xRot+1.0f,yRot-1.5f,0.0f);//显示文字的坐标
	sprintf(show_1,"N1=%d",n_1);//唯一用到stdio.h的函数
    drawString(show_1);//这里显示文字
	glColor3ub(255, 0, 255);
	glRasterPos3f(xRot+1.0f,yRot-1.8f,0.0f);
	sprintf(show_1,"N2=%d",n_2);
	drawString(show_1);
	glColor3ub(0, 255, 255);
	glRasterPos3f(xRot+1.0f,yRot-2.1f,0.0f);
	sprintf(show_1,"MEET=%d",n_3);
	drawString(show_1);
	glTranslatef(0.0f,0.0f,0.0f);	//(x,y,z)(x右，y上，z指向观众）
	glColor3ub(255, 2, 0);//轨道颜色
	if(trackLines==1)
	{
//		glPointSize(1);
		for(ellipseAng=0.0f;ellipseAng<360.0f;ellipseAng+=0.1)
		{
		glRotatef(0.1f, 0.0f, 0.0f, 1.0f);
		glBegin(GL_POINTS);
		glVertex3f( r_1, 0.0f, 0.0f);
		glEnd();
			glBegin(GL_POINTS);
			glVertex3f( r_2, 0.0f, 0.0f);
			glEnd();
		}//绘制一个轨道
	ellipseAng=0.0f;
	}
	glColor3ub(255, sun_color, 0);  //太阳颜色：黄色
	glutSolidSphere(0.06f, 18, 17);  // 绘制太阳//渲染一个球体，球心为原点（半径（double）、以Z轴上线段为直径分布的圆周线的条数、围绕在Z轴周围的线的条数）
	glColor3ub(0,0,255);   // 天体1颜色
	glRotatef(angle_1, 0.0f, 0.0f, 1.0f);
	glTranslatef(r_1,0.0f,0.0f);//天体1坐标
	glutSolidSphere(0.06f, 30, 17);
	glTranslatef(-r_1,0.0f,0.0f);
	glRotatef(-angle_1, 0.0f, 0.0f, 1.0f);//坐标还原
	angle_1 += (dAngle_1*pause);
	if(angle_1>=360.0f){angle_1 -= 360.0f;n_1++;}//将角度数值控制在0到360，防止数据溢出
	glColor3ub(255,0,255);
	glRotatef(angle_2, 0.0f, 0.0f, 1.0f);
	glTranslatef(r_2,0.0f,0.0f);
	glutSolidSphere(0.06f, 30, 17);
	glRotatef(-angle_2, 0.0f, 0.0f, 1.0f);
	angle_2 += (dAngle_2*pause);
	if(angle_2>=360.0f){angle_2 -= 360.0f;n_2++;}
	angle_3 += (dAngle_3*pause);
	if(angle_3>=360.0f){angle_3 -= 360.0f;n_3++;flash_mode = 0;dAngle_1 = 0.0f;dAngle_2 = 0.0f;dAngle_3 = 0.0f;}
	if(flash_mode < 20){flash_mode++;if((flash_mode%2)==0)sun_color = 125;else sun_color = 0;if(flash_mode == 20){sun_color = 255;dAngle_1 = 6.0f;}dAngle_2 = dAngle_1*sqrt(r_1*r_1*r_1/r_2/r_2/r_2);dAngle_3 = dAngle_1 - dAngle_2;}
		glColor3ub(0, 0, 255); //文字的颜色
	glPopMatrix();	
	glutSwapBuffers();
}  
void SetupRC() {
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glClearColor(bgc, bgc, bgc, 0.0f );
} 
void TimerFunc(int value) {
   glutPostRedisplay();//将当前的窗口重新绘制
   glutTimerFunc(50, TimerFunc, 1);//(毫秒数、回调函数指针、区别值）
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
		if(zRot<20.0f)zRot += 0.5f; break;
	case GLUT_LEFT_BUTTON:
		if(zRot>1.0f)zRot -= 0.5f; break;
	}

}

void keyboard (unsigned char key, int x, int y) {
   switch (key) {
	   case 'a':  xRot -= 0.2f;  break;
	   case 'A':  xRot -= 0.2f;  break;
	   case 'd':  xRot += 0.2f;  break;
	   case 'D':  xRot += 0.2f;  break;
	   case 'w':  yRot += 0.2f;  break;
	   case 'W':  yRot += 0.2f;  break;
	   case 's':  yRot -= 0.2f;  break;
	   case 'S':  yRot -= 0.2f;  break;
	   case 'q':  if(trackLines==1)trackLines=0;else trackLines=1;break;
	   case 'Q':  if(trackLines==1)trackLines=0;else trackLines=1;break;
	   case 'p':  if(pause==1)pause=0;else pause=1;break;
	   case 'P':  if(pause==1)pause=0;else pause=1;break;
//	   case GLUT_KEY_F2: trackLines = 1;  break;
//	   case GLUT_KEY_F3: trackLines = 0;  break;

	   case 27:   exit(0); 
	   default:   break;
   }
	glutPostRedisplay();
}
int main(int argc, char* argv[]) {
	dAngle_2 = dAngle_1*sqrt(r_1*r_1*r_1/r_2/r_2/r_2);
	dAngle_3 = dAngle_1 - dAngle_2;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(800, 600);
	glutCreateWindow("按P开始/暂停，按Q关闭/打开轨道线");
	glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(myMouse);
	glutDisplayFunc(RenderScene);
	glutTimerFunc(250, TimerFunc, 1);
	SetupRC();
	glutMainLoop();
	return 0;
}
