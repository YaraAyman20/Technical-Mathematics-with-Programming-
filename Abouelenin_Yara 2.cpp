#define _CRT_SECURE_NO_WARNINGS
/*****
  Name: Yara Abouelenin - section 1
  Id: 21901282
  HW : 2
***************************
 Additional Features:
 1- The left/right keys on the keyboard makes the cannon ball coming from the tank (weapon) go faster(right) or slower(left).It is wriiten down in the keys area in the game. So player can slow down the ball
 or make it faster depending on situation.
 2- Every time you hit the UFO the score is calculated and then restarts to 0 when the game restarts. (located under the time remaining).
 3- The ufoes are going in random speeds each. Some slow down suddenly and go very slow and others go fast randomly and so on.To make game more fun.
 4- After 10 secounds in the game it becomes night time so the screen turns darker and the background changes from clouds TO stars (star clusters that change in a random manner) and moon.
 5- The Ufo's spaceship lights (3 circles in the middle) change color randomly throughout the game and even when it ends to capture the player's attention.
 6- When the background becomes dark the cannon ball fired becomes red.
*********************/


#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  900
#define WINDOW_HEIGHT 600

#define TIMER_PERIOD   20 //period for the timer
#define TIMER_ON        1 //0:disable timer, 1:enable timer

#define DURATION 20
#define D2R 0.0174532



typedef struct {
	int y_axis;
	int x_axis;
	double r, b, g;

}ufo_t;

ufo_t arr[5];


/* Global Variables  */
int  winWidth, winHeight;
bool animation = true;
bool cannonMovement = false;


bool stopped = false;
bool statemove = 1;
bool color = 1;
bool up = false, down = false;
int tank_y;
int cx = -320, cy = tank_y - 74;
int cannonSpeed = 5;

int timeCounter = DURATION;
int score = 0, t = 0, gameOver = 0;

double r = 0.3, g = 0.5, b = 0.8;


//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
bool object = 1;
void onKeyDown(unsigned char key, int x, int y)
{

	if (key == ' ') {

		cannonMovement = true;
	}

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


void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = true;
		break;
	case GLUT_KEY_DOWN: down = true;
		break;
	case GLUT_KEY_LEFT:
		if (cannonSpeed > 3)
			cannonSpeed -= 2; break;
	case GLUT_KEY_RIGHT:
		if (cannonSpeed < 10)
			cannonSpeed += 2; break;


	case GLUT_KEY_F1:
		if (gameOver == 0)
			animation = !animation;
		else
		{
			timeCounter = DURATION + 1;
			animation = !animation;
			gameOver = 0;
			score = 0;
			r = 0.3;
			g = 0.5;
			b = 0.8;

		}



		break;

		// to refresh the window it calls display() function 
		glutPostRedisplay();

	}
}


void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	}

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

//Gradiant of the background 
void drawGradient(int x1, int y1, int w, int h, float r, float g, float b) {
	glBegin(GL_QUADS);
	glColor3f(r, g, b);
	glVertex2f(x1, y1);
	glVertex2f(x1 + w, y1);
	glColor3f(r + 0.4, g + 0.4, b + 0.4);
	glVertex2f(x1 + w, y1 - h);
	glVertex2f(x1, y1 - h);
	glEnd();
	glColor3f(0.1, 0.1, 0.1);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x1, y1);
	glVertex2f(x1 + w, y1);
	glVertex2f(x1 + w, y1 - h);
	glVertex2f(x1, y1 - h);
	glEnd();
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
	glColor3f(0.5, 0.5, 0.5);
	circle(x + 10 * size, y - 20 * size, 41 * size);

	glColor3f(1, 1, 1); //window
	circle(x + 9 * size, y + 7 * size, 35 * size);

	// alien body 
	glColor3ub(0, 100, 0);
	glRectf(x + 14 * size, y + 14 * size, x + 3 * size, y - 10 * size);

	// ship body
	glColor3f(0.5, 0.5, 0.5);
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

//cannon ball
void cannon(int x, int y) {
	int r, g, b;
	if (timeCounter < 10)
	{
		glColor3ub(255, 0, 0);
	}
	else {
		glColor3ub(0, 0, 0);
	}

	circle(x, y, 8);
}

//TANK
void drawTank(int x, int y, double size) {


	//tank base borders
	glColor3ub(255, 250, 205);
	circle(x - 400 * size, y - 220 * size, 35 * size);
	glRectf(x - 400 * size, y - 255 * size, x - 310 * size, y - 195 * size);
	circle(x - 300 * size, y - 220 * size, 35 * size);

	//tank base 
	glColor3ub(51, 51, 0);
	circle(x - 400 * size, y - 220 * size, 30 * size);
	glRectf(x - 400 * size, y - 250 * size, x - 300 * size, y - 200 * size);
	circle(x - 300 * size, y - 220 * size, 30 * size);

	//Wheels
	glColor3ub(85, 107, 47);
	circle(x - 390 * size, y - 230 * size, 15 * size);
	circle(x - 350 * size, y - 230 * size, 15 * size);
	circle(x - 310 * size, y - 230 * size, 15 * size);

	circle(x - 410 * size, y - 205 * size, 10 * size);//small on the side
	circle(x - 295 * size, y - 205 * size, 10 * size);//small on the side

	//inner circles
	glColor3ub(75, 83, 32);
	circle(x - 390 * size, y - 230 * size, 10 * size);
	circle(x - 350 * size, y - 230 * size, 10 * size);
	circle(x - 310 * size, y - 230 * size, 10 * size);

	circle(x - 410 * size, y - 205 * size, 7 * size);//small on the side
	circle(x - 295 * size, y - 205 * size, 7 * size);//small on the side

	//connecting the wheels 
	glColor3ub(0, 0, 0);
	glRectf(x - 365 * size, y - 230 * size, x - 375 * size, y - 235 * size);
	glRectf(x - 335 * size, y - 230 * size, x - 325 * size, y - 235 * size);

	//triangles above wheels (pointy edges)
	glColor3ub(75, 83, 32);
	glBegin(GL_TRIANGLES);
	glVertex2f(x - 300 * 1 * size, y - 170 * size * 1);
	glVertex2f(x - 270 * 1 * size, y - 186 * 1 * size);
	glVertex2f(x - 300 * size * 1, y - 200 * size * 1);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3ub(75, 83, 32);
	glVertex2f(x - 400 * 1 * size, y - 170 * size * 1);
	glVertex2f(x - 430 * size, y - 194 * size * 1);
	glVertex2f(x - 400 * 1 * size, y - 200 * size * 1);
	glEnd();


	//tank body
	glColor3ub(85, 107, 47);
	glRectf(x - 400 * size, y - 190 * size, x - 300 * size, y - 130 * size);

	//tank shooter
	glColor3ub(75, 83, 32);
	glRectf(x - 300 * size, y - 160 * size, x - 190 * size, y - 140 * size);
	glColor3ub(85, 107, 47);
	glRectf(x - 250 * size, y - 165 * size, x - 190 * size, y - 133 * size);


	//above the wheels rectangle 
	glColor3ub(0, 0, 0);
	glRectf(x - 390 * size, y - 210 * size, x - 310 * size, y - 195 * size);
	glColor3ub(75, 83, 32);
	glRectf(x - 400 * size, y - 199 * size, x - 300 * size, y - 170 * size);


}


//mornining background
void drawBackground() {

	drawGradient(-500, 300, 1000, 600, r, g, b); //sky

	//draw Clouds
	drawCloud(-300, 220, 1);
	drawCloud(-98, 240, 1);
	drawCloud(100, 230, 1);
	drawCloud(-200, 270, 0.3);
	drawCloud(-10, 190, 0.3);


	glColor3f(1, 1, 1);
	print(340, 219, "REMAINING", GLUT_BITMAP_HELVETICA_12);
	print(360, 200, "TIME :", GLUT_BITMAP_HELVETICA_12);
	glColor3f(1, 1, 1);
	vprint2(370, 175, 0.12, "%d", timeCounter);
	vprint2(330, 135, 0.15, "Score: %d", score);


}

//Night background
void drawBackground2() {

	drawGradient(-500, 300, 1000, 600, 0.09, 0.09, 0.4); //darker sky

	//draw stars 
	//random star clusters 
	srand(time(NULL));
	for (int i = 0; i < 10; i++)
	{
		stars(6 * i + rand() % 100, i * 20, 7);

	}


	// moon
	glColor3f(0.9607, 0.9529, 0.808);
	circle(-160, 140, 50);

	glColor3f(0.663, 0.663, 0.663);
	circle(-160, 145, 9);
	glColor3f(0.663, 0.663, 0.663);
	circle(-190, 160, 7);
	glColor3f(0.663, 0.663, 0.663);
	circle(-180, 120, 5);


	glColor3f(1, 1, 1);
	print(340, 219, "REMAINING", GLUT_BITMAP_HELVETICA_12);
	print(360, 200, "TIME :", GLUT_BITMAP_HELVETICA_12);
	glColor3f(1, 1, 1);
	vprint2(370, 175, 0.12, "%d", timeCounter);
	vprint2(330, 135, 0.15, "Score: %d", score);

}

void resetCannon() {  //reset cannon ball function
	cannonMovement = false;
	//initial coordinates of the ball
	cx = -320;
	cy = tank_y - 74;
}

void CMovement()
{
	cx += cannonSpeed;


}


// main display function 
void display() {

	if (timeCounter < 10)
	{
		drawBackground2();
	}
	else
		drawBackground();


	//display ships
	for (int i = 0; i < 5; i++)
	{
		arr[i].x_axis = i * 80;
		dispShip(arr[i].x_axis, arr[i].y_axis, 0.6);
	}


	//display cannon ball
	cannon(cx, cy);

	//display Tank 
	drawTank(-210, tank_y, 0.5);


	//bottom info panel
	glColor3f(0, 0, 0);
	glRectf(-500, -280, 500, -300);

	//info message
	glColor3f(1, 1, 1);
	print(-365, -295, " up/down: move the tank,     <SpaceBar> Fire,      <F1> Pause/Restart,     Left/Right: Increase/Decrease speed of the ball", GLUT_BITMAP_HELVETICA_12);

	glutSwapBuffers();
}

//moving object function
void Move()
{

	srand(time(NULL));
	for (int i = 0; i < 5; i++)
	{
		if (arr[i].y_axis > 590)
		{
			arr[i].y_axis = -590 - i * 20;
		}
		arr[i].y_axis += rand() % 8 + 1;
	}

	if (!cannonMovement)
	{

		if (up == true && tank_y < 360)
		{
			tank_y += 5;
			cy += 5;
		}

		if (down == true && tank_y > -160)
		{
			tank_y -= 5;
			cy -= 5;
		}

	}


}

void Init() {

	tank_y = 0;
	resetCannon();

	srand(time(NULL));


	for (int i = 0; i < 5; i++)
	{
		arr[i].y_axis = -550 - i * 20;

		arr[i].r = rand() % 255;
		arr[i].g = rand() % 255;
		arr[i].b = rand() % 255;
	}



}


int si = 52; //area of intersection 
bool checkIntercept(int x, int y) //check the intercept between ball and ufo
{
	if ((cx >= x && cx <= x + si) && (cy >= y - si && cy <= y + si))
	{
		return true; //if intercept return true 
	}
	else
		return false;
}

void reset(int i)
{
	arr[i].y_axis = -590 - i * 20;
	resetCannon();

}

#if TIMER_ON == 1

void onTimer(int v) {


	glutTimerFunc(TIMER_PERIOD, onTimer, 0);


	t += 10; // time counter 



	// check if animation is true
	if (animation) {
		Move();

		if (cannonMovement) {

			CMovement();
			//check if the ball reaches the end of the screen
			if (cx > (WINDOW_WIDTH / 2 - 10) || cx < (-WINDOW_WIDTH / 2 + 10) || cy < (-WINDOW_HEIGHT / 2 + 10) || cy >(WINDOW_HEIGHT / 2 - 10))
				resetCannon();

			for (int i = 0; i < 5; i++)
			{
				if (checkIntercept(arr[i].x_axis, arr[i].y_axis) == true) //check intercept 
				{
					reset(i);
					score++;


				}



			}


		}

		if (gameOver == 0) {
			if (t >= 1000) {
				timeCounter--;

				if (timeCounter < 10)
				{
					r = 0;
					g = 0;
					b = 0;
				}

				if (timeCounter == 0)
				{
					gameOver = 1;
					animation = false;
				}


				t = 0;
			}
		}
	}





	glutPostRedisplay();
}
#endif






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



void main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("HW-2 Keeping the Aliens Away(blowing UFO) by Yara");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//
	// keyboard registration
	//
	glutSpecialFunc(onSpecialKeyDown);
	glutKeyboardFunc(onKeyDown);
	glutSpecialUpFunc(onSpecialKeyUp);



#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);

#endif

	glutKeyboardUpFunc(onKeyUp);



	glutMainLoop();
}
