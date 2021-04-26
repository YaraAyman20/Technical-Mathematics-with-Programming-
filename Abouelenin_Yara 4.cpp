/*
 Name : Yara Abouelenin - section: 1
 Id: 21901282
 HW : 4
 **********************************************
 Additional features:
 1- The orbit of the planet is ellipse which is closer to real life orbit than a circle 
 2- I made random speed everytime you restart(run) the game and random speed while in the orbit as given
 3- I did other plantes with different orbits and different colors only on the edges (red / green) and they get the sun reflection as well
 4- backround changes by pressing "v" for view. 
   
*/ 

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "vec.h"

#define WINDOW_WIDTH  900
#define WINDOW_HEIGHT 600

#define TIMER_PERIOD 10 // Period for the timer.
#define TIMER_ON 1 // 0:disable timer, 1:enable timer
#define DURATION 40
#define D2R 0.0174532


/* Global Variables  */
int  winWidth, winHeight;
bool animation = true;
int earth_x, earth_y; //basic planet
int marse_x, marse_y;  
int mercury_x, mercury_y; //smallest planet
bool state = 1; //background place 

int sunState = 1;

int obj_y = 0;
int obj_x = 0;

bool stopped = false;
bool statemove = 1;

bool up = false, down = false, right = false, left = false;

//light part
bool upT = true; //light source 

typedef struct {
	float speed;
	int direction;

} planet_t;

planet_t earth;

typedef struct {
	float r, g, b;
} color_t;

typedef struct {
	vec_t pos;
	vec_t N;
} vertex_t;

typedef struct {
	vec_t   pos;
	color_t color;
	vec_t   vel;
} light_t;


light_t light = { {0, 0}, {sunState,sunState,sunState}, {0,0}}; //light source. the sun

color_t mulColor(float k, color_t c) {
	color_t tmp = { k * c.r, k * c.g, k * c.b };
	return tmp;
}

color_t addColor(color_t c1, color_t c2) {
	color_t tmp = { c1.r + c2.r, c1.g + c2.g, c1.b + c2.b };
	return tmp;
}

double distanceImpact(double d) {
	return (-1.0 / 350.0) * d + 1.0;
}

color_t calculateColor(light_t source, vertex_t v) {
	vec_t L = subV(source.pos, v.pos);
	vec_t uL = unitV(L);
	float factor = dotP(uL, v.N) * distanceImpact(magV(L));
	return mulColor(factor, source.color);
}

float A = 210, //amplitude
fq = 1,  //frequency
C = 0,   //horizontal phase shift
B = 0;   //vertical phase shift
float angle = 0;

typedef struct {
	float x, y;
} pos_t;

pos_t p0, //red point: vertical harmonic motion
p1, //green point: horizontal harmonic motion
p2; //composite animation for color, radius and position 


//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
bool object = 1;
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);
	if (key == ' ') {
		sunState = !sunState;
		
	}

		if (key == 'v') //check key is view, change view
			state = !state;
	

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);


	// to refresh the window it calls display() function
	glutPostRedisplay();
}


//circle Functions

void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void print(int x, int y, const char *string, void *font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}
void vprint(int x, int y, void *font, const char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}
void vprint2(int x, int y, float size, const char *string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}

//Drawing stars 
void stars(int x, int y, int size) {

	glColor3ub(255, 255, 245);

	glBegin(GL_POLYGON);

	glVertex2f(x + 1 * size, y - 1 * size);
	glVertex2f(x + 2 * size, y - 1 * size);
	glVertex2f(x + 1 * size, y - 2 * size);
	glVertex2f(x + 1 * size, y - 3 * size);
	glVertex2f(x, y - 2 * size);
	glVertex2f(x - 1 * size, y - 2 * size);
	glVertex2f(x, y - 1 * size);
	glVertex2f(x, y);


	glEnd();
}


void displayRocket(int x, int y) {
	//rectangular body
	glColor3ub(255, 215, 0);
	glRectf(x - 30 * 0.5, y - 90 * 0.5, x + 40 * 0.5, y + 80 * 0.5);

	//traingular head with gradient 
	glColor3f(1, 0, 0);
	glBegin(GL_TRIANGLES);
	glVertex2f(x - 30 * 0.5, y + 80 * 0.5);
	glColor3ub(219, 112, 147);
	glVertex2f(x + 40 * 0.5, y + 80 * 0.5);
	glColor3ub(128, 0, 0);
	glVertex2f(x + 6 * 0.5, y + 130 * 0.5);

	glEnd();

	//Rocket window
	glColor3f(0, 0, 0); //window boarder
	circle(x + 5.5*0.5, y + 5 * 0.5, 25 * 0.5);

	glColor3ub(135, 206, 235);
	circle(x + 5.5*0.5, y + 5 * 0.5, 22 * 0.5);


}


//Drawing clouds
void drawCloud(int x, int y, double size) {
	glColor3f(1, 1, 1); //white
	circle(x, y + 13 * size, 20 * size);
	circle(x - 33 * size, y, 24 * size);
	circle(x + 30 * size, y - 2 * size, 20 * size);
	circle(x, y - 22 * size, 20 * size);
	circle(x + 20 * size, y - 20 * size, 20 * size);
	circle(x - 30 * size, y - 24 * size, 18 * size);
}


//space Ship (UFO)
void dispShip(int x, int y, double size) //UFO 
{
	//Space ship legs and antenna
	glColor3ub(0, 0, 0);
	glBegin(GL_LINES);

	// line 1
	glVertex2f(x, y);
	glVertex2f(x - 20 * size, y - 80 * size);
	// line 2
	glVertex2f(x + 20 * size, y);
	glVertex2f(x + 40 * size, y - 80 * size);
	// line 3 up (antenna)
	glColor3b(0, 0, 0);
	glVertex2f(x + 10 * size, y);
	glVertex2f(x + 10 * size, y + 70 * size);

	glEnd();

	//Space Ship body : circle / Rectangle 
	glColor3f(0.4, 0.4, 0.4);
	circle(x + 10 * size, y - 20 * size, 41 * size);

	glColor3f(1, 1, 1); //window
	circle(x + 9 * size, y + 7 * size, 35 * size);

	// alien body 
	glColor3ub(0, 100, 0);
	glRectf(x + 14 * size, y + 14 * size, x + 3 * size, y - 10 * size);

	// ship body
	glColor3f(0.4, 0.4, 0.4);
	glRectf(x - 40 * size, y, x + 60 * size, y - 40 * size);

	//ship's 3 middle circle design AND changing colors randomly
	int r, g, b;
	srand(time(NULL));
	r = 90 + rand() % 200;
	g = 90 + rand() % 200;
	b = 90 + rand() % 200;

	glColor3ub(r, g, b);
	circle(x - 20 * size, y - 20 * size, 8 * size);
	circle(x + 9 * size, y - 20 * size, 8 * size);
	circle(x + 40 * size, y - 20 * size, 8 * size);

	// alien face
	glColor3ub(0, 100, 0);
	circle(x + 8 * size, y + 14 * size, 10 * size);

	// alien eyes
	glColor3f(0, 0, 0);
	circle(x + 5 * size, y + 14 * size, 3 * size);
	glColor3f(0, 0, 0);
	circle(x + 9.5* size, y + 14 * size, 3 * size);


}


//background 1
void display_background() {

	glClearColor(0.5, 0.5, 0.5, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	// label
	glColor3f(0, 0, 0);
	vprint(-290, 270, GLUT_BITMAP_8_BY_13, "HW -4- By Yara Abouelenin");
	vprint(-290, 250, GLUT_BITMAP_8_BY_13, "21901282");



	glColor3f(1, 1, 1);
	vprint2(-360, -290, 0.12, "Press Spacebar to turn ON the sun's light ----- Press v to change the background");
	

	//spaceship
	dispShip(-320, -90 , 0.7);
	dispShip(350, -200, 0.7);



	//star cluster 
	srand(time(NULL));
	for (int i = 0; i < 10; i++)
	{
		stars(20 * i + rand() % 100, i * 40, 7);

	}


	
}


//background 2

void display_background2() {

	glClearColor(0.09, 0.09 , 0.4, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	// label
	glColor3f(1, 1, 1);
	vprint(-290, 270, GLUT_BITMAP_8_BY_13, "HW -4- By Yara Abouelenin");
	vprint(-290, 250, GLUT_BITMAP_8_BY_13, "21901282");



	glColor3f(1, 1, 1);
	vprint2(-360, -290, 0.12, "Press Spacebar to turn ON the sun's light ----- Press v to change the background");

	
	displayRocket(-290, 100);
	drawCloud(-290, 60, 0.6);

	displayRocket(280, -200);
	drawCloud(285, -250, 0.6);



}


void sun() {
	glColor3f(sunState, sunState, sunState);
	circle(0, 0, 50);

}


float f(float x) {
	return A * sin((fq * x + C) * D2R) + B;
}

void drawAngle() {
	float r = fabs(A); //amplitude


	//glColor3f(1, 1, 1);
	//vprint(-100, 30, GLUT_BITMAP_8_BY_13, "%0.f", angle); //white angle

	glColor3f(0, 0, 0);

	earth_x = r * cos(angle * D2R) ;
	earth_y = r * sin(angle*D2R) ;


	marse_x = r * cos(angle * D2R) + 50;
	marse_y = r * sin(angle*D2R) + 40;
	

	mercury_x =  r * cos(angle * D2R) + 70;
	mercury_y = r * sin(angle*D2R) + 80;
	
	
}

void circleUsingTriangles(double x, double y, int r, color_t color)
{

#define PI 3.1415
	float angle;
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0, 0, 0);
	glVertex2f(x , y);
	color_t res{ 0,0,0 };

	for (int i = 0; i < 360; i++)
	{
		vertex_t P = { { x + 30 * cos(i * D2R),y + 30 * sin(i * D2R)}, { 0, 0} };
		P.N = unitV(subV(P.pos, { x,y - 10 }));
		res = calculateColor(light, P);
		glColor3f(res.r+color.r, res.g+color.g, res.b+color.b); // just add 0.2 on any one of these and you'll have a unique colored planet
		
		angle = 2 * PI*i / 360;

		vec_t ev = { x + r * cos(angle), y + r * sin(angle) };
		res = addColor(res, calculateColor(light, P));
		if (sunState == 0 )
		{
			glColor3f(0,0,0);
		}
		
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}


// main display function 
void display() {

	//state to change the background and object
	if (state == 1)
	{
		display_background();
		//dispShip(obj_x, -15);
	}
	else if (state == 0)
	{
		display_background2();
		//displayRocket(0, obj_y);
	}


	sun();


	drawAngle();

	glColor3f(light.color.r, light.color.g, light.color.b);


	circleUsingTriangles(earth_x, earth_y, 30, {0,0,0});
	circleUsingTriangles(marse_x, marse_y, 20, { 0 + 0.4,0,0 });
	circleUsingTriangles(mercury_x, mercury_y, 10, { 0 ,0 + 0.5,0 });
	


	glutSwapBuffers();
}

void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.

}


void Init() {

	srand(time(NULL));

	earth.direction =  rand()% 2;
	earth.speed = rand() % 2+1;
	printf("%f", earth.speed);

	printf("%d", earth.direction);



}




#if TIMER_ON == 1

void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);

	if (earth.direction == 1)
	{
		srand(time(NULL));
		angle += 0.75*earth.speed * (rand() % 2+1) *0.6; //random speed in the orbit itself
		if (angle > 360)
			angle -= 360;
	}
	
	else {
		angle -= 0.75*earth.speed;
		if (angle < 0)
			angle += 360;

	}

	//earth
	p0.x = -350;
	p0.y = f(angle);
	p0.y += 25; //25 is added to position on the coordinate system

	//only show obejects and animation if the game is NOT paused
	if (animation)
	{

	
	}


	//to refresh the window it calls display() function
	glutPostRedisplay(); //display()
	glutPostRedisplay();
}
#endif




void main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("HW-1 Flying into the outer space by Yara");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);

	glutKeyboardUpFunc(onKeyUp);



#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);

#endif

	Init();

	glutMainLoop();
}