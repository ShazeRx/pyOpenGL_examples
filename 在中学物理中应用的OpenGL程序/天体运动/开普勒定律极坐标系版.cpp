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
float a_1 = 0.9f;//天体1轨道半长轴
float b_1 = 0.8f;//天体1轨道半短轴
float c_1 = 0.0f;//焦点到中心距离
float e_1 = 0.0f;//极坐标方程中的离心率，e=c/a,a为半长轴，c=sqrt(a^2-b^2),b是半短轴
float p_1 = 0.0f;//极坐标方程中的准线x=a^2/c,p=a^2/c+c,即准线到焦点的距离
float r_1 = 0.0f;//天体1距离中心天体的距离，用坐标方程计算
float dAngle_1 = 2.0f;//单位时间内转过的角度，即角速度，这里直接给出初始角速度
float v_1 = 0.0f;//天体1的速度
float cW_1 = 0.0f;//天体1的角动量常数
float cE_1 = 0.0f;//天体1的能量常数
float angle_2 = 0.0f;
float a_2 = 1.1f;
float b_2 = 0.9f;
float c_2 = 0.0f;
float e_2 = 0.0f;
float p_2 = 0.0f;
float r_2 = 0.0f;
float dAngle_2 = 2.0f;
float v_2 = 0.0f;
float cW_2 = 0.0f;
float cE_2 = 0.0f;//天体2对应的各个参数
float ellipseAng = 0.0f;//椭圆角度，用于绘制椭圆轨道
//用于计算天体和中心天体之间的距离
static float xRot = 0.5f;
static float yRot = 0.0f;
static float zRot = 6.0f;
//视角位置
int trackLines = 1;//可以切换轨道是否显示
int invisible_2 = 1;//控制天体2是否隐身

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//如果这一条被注释，那么前一帧的图像不会被清楚
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	gluLookAt(xRot, yRot, zRot, xRot, yRot, 0, 0, 1, 0);//第4~6个变量可以用来追踪某一个星星
	glRasterPos3f(xRot+1.0f,yRot-1.5f,0.0f);//显示文字的坐标
	sprintf(show_1,"w=%f",dAngle_1);//唯一用到stdio.h的函数
    drawString(show_1);//这里显示文字
	glRasterPos3f(xRot+1.0f,yRot-1.8f,0.0f);
	sprintf(show_1,"r1=%f",r_1);
	drawString(show_1);//显示距离
	glRasterPos3f(xRot+1.0f,yRot-2.1f,0.0f);
	sprintf(show_1,"v1=%f",v_1);
	drawString(show_1);//显示速度
	glTranslatef(0.0f,0.0f,0.0f);	//(x,y,z)(x右，y上，z指向观众）
	glColor3ub(255, 2, 0);//轨道颜色
	if(trackLines==1)
	{
//		glPointSize(1);
		for(ellipseAng=0.0f;ellipseAng<360.0f;ellipseAng+=0.1)
		{
		glRotatef(0.1f, 0.0f, 0.0f, 1.0f);
		r_1=e_1*p_1/(1-e_1*cos(ellipseAng/180.0f*3.14159));//椭圆极坐标方程
		glBegin(GL_POINTS);
		glVertex3f( r_1, 0.0f, 0.0f);
		glEnd();
		if(invisible_2==0)
		{
			glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
			r_2=e_2*p_2/(1-e_2*cos(ellipseAng/180.0f*3.14159));
			glBegin(GL_POINTS);
			glVertex3f( r_2, 0.0f, 0.0f);
			glEnd();
			glRotatef(-30.0f, 0.0f, 0.0f, 1.0f);
		}
		}//绘制一个轨道
	ellipseAng=0.0f;
	}
	glColor3ub(255, 255, 0);  //太阳颜色：黄色
	glutSolidSphere(0.06f, 18, 17);  // 绘制太阳//渲染一个球体，球心为原点（半径（double）、以Z轴上线段为直径分布的圆周线的条数、围绕在Z轴周围的线的条数）
	r_1=e_1*p_1/(1-e_1*cos(angle_1/180.0f*3.14159));//椭圆极坐标方程
	glColor3ub(0,0,255);   // 天体1颜色
	glRotatef(angle_1, 0.0f, 0.0f, 1.0f);
	glTranslatef(r_1,0.0f,0.0f);//天体1坐标
	glutSolidSphere(0.06f, 30, 17);
	glTranslatef(-r_1,0.0f,0.0f);
	glRotatef(-angle_1, 0.0f, 0.0f, 1.0f);//坐标还原
	dAngle_1 = cW_1/(r_1*r_1);//角动量守恒，即开普勒第二定律
	v_1 = sqrt(2*(cE_1+(grav*m_0/(r_1*r_1))));//利用能量守恒定律计算天体速度，注意，如果用角动量乘以r只能计算该速度在垂直r方向上的分量
	angle_1 += dAngle_1;
	if(angle_1>=360.0f)angle_1 -= 360.0f;//将角度数值控制在0到360，防止数据溢出
	if(invisible_2==0)
	glColor3ub(255,0,255);
	else
	glColor3f(0.3f,0.3f,0.3f);
	r_2=e_2*p_2/(1-e_2*cos(angle_2/180.0f*3.14159));
	glRotatef((angle_2+30.0f), 0.0f, 0.0f, 1.0f);//假设轨道不共轴
	glTranslatef(r_2,0.0f,0.0f);
	glutSolidSphere(0.06f, 30, 17);
	dAngle_2 = cW_2/(r_2*r_2);
	angle_2 += dAngle_2;
	if(angle_2>=360.0f)angle_2 -= 360.0f;
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
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f );
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
	   case 'e':  if(invisible_2==1)invisible_2=0;else invisible_2=1;break;
	   case 'E':  if(invisible_2==1)invisible_2=0;else invisible_2=1;break;
	   case GLUT_KEY_F2: trackLines = 1;  break;
	   case GLUT_KEY_F3: trackLines = 0;  break;

	   case 27:   exit(0); 
	   default:   break;
   }
	glutPostRedisplay();
}
int main(int argc, char* argv[]) {
	c_1 = sqrt(a_1*a_1-b_1*b_1);
	e_1 = c_1/a_1;
	p_1 = a_1*a_1/c_1+c_1;
	c_2 = sqrt(a_2*a_2-b_2*b_2);
	e_2 = c_2/a_2;
	p_2 = a_2*a_2/c_2+c_2;//根据半长轴和半短轴计算离心率和准线
	r_1 =  e_1*p_1/(1-e_1*cos(0));//计算天体1的初始至中心天体的距离
	cW_1 = dAngle_1*r_1*r_1;//计算天体1的角动量常数
	cE_1 = (r_1*dAngle_1*r_1*dAngle_1)/2-grav*m_0/(r_1*r_1);//计算天体1的能量常数
	r_2 =  e_2*p_2/(1-e_2*cos(0));
	cW_2 = dAngle_2*r_2*r_2;
	cE_2 = (r_2*dAngle_2*r_2*dAngle_2)/2-grav*m_0/(r_2*r_2);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(800, 600);
	glutCreateWindow("Solar System");
	glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(myMouse);
	glutDisplayFunc(RenderScene);
	glutTimerFunc(250, TimerFunc, 1);
	SetupRC();
	glutMainLoop();
	return 0;
}
