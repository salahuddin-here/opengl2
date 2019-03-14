/*********
CTIS164 - Circle Hitting Game
----------
STUDENT :	MOHAMMAD SALAHUDDIN KURD
SECTION :	003
HOMEWORK:	HOMEWORK- 3
----------
PROBLEMS: If your program does not function correctly,
explain here which parts are not running.

Please add GL/ in the #include <glut.h> otherwise it will not work. 
*********/

#include <glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include<time.h>

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 1000

#define TIMER_PERIOD  25 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

typedef struct {
	int x, y, radius;
	int r, b, g, direction, speed;
	float angle, Cx;
	bool hit = false;
}movingObj_t;				// structure for the moving circles 

movingObj_t mObj[3];


typedef struct {
	float x, y;
	float angle;
}firingObj_t;			// cannon's structure 

firingObj_t fObj{ 0,0,0 }, bul{ 0,0,45 };			// first initialization is of cannon. Second is of bullet. I am using same structure for both things.


typedef struct {
	float x, y;
}posi_t;				// structure for the vertex function

typedef struct {
	int timeSec = 0, timeMin = 0;		// time counters 
}timet_t;

timet_t timet, best_time;


bool bult = false;		// bool for the bullet movement (STOP = FALSE) (MOVE = TRUE)

bool timer = false;			// program activator

int game_no = 0;



void initialize() {
	for (int i = 0; i < 3; i++)
	{
		mObj[i].angle = rand() % 361;
		mObj[i].radius = rand() % 20 + 20;
		mObj[i].r = rand() % 256;
		mObj[i].g = rand() % 256;
		mObj[i].b = rand() % 256;
		mObj[i].direction = rand() % 2 + 0;
		mObj[i].speed = rand() % 4 + 1;
		mObj[i].hit = false;
	}
	mObj[0].Cx = 200;
	mObj[1].Cx = 250;
	mObj[2].Cx = 300;
	timer = false;
	bult = false;
	timet.timeSec = 0;
	timet.timeMin = 0;
	bul.x = 0;
	bul.y = 0;
	bul.angle = 45;
}			// initialization of all the points of the so that they return to their original positions after the program is finished

void vertex(posi_t P, posi_t Tr, double angle) {
	double xp = (P.x * cos(angle) - P.y * sin(angle)) + Tr.x;
	double yp = (P.x * sin(angle) + P.y * cos(angle)) + Tr.y;
	glVertex2d(xp, yp);
}		// for changing the vertexes for the rotation of the objects 
bool testCollision(firingObj_t fr, movingObj_t t) {
	float dx = t.x - fr.x;
	float dy = t.y - fr.y;
	float d = sqrt(dx*dx + dy*dy);
	return d <= t.radius;
} // to check whether the object is hit by the bullet 

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
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
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}

void print(int x, int y, char *string, void *font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void *font, char *string, ...)
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

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, char *string, ...) {
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
void displayBackgound() {
	glBegin(GL_QUADS);
	//black colour
	glColor3ub(153, 0, 0);
	glVertex2f(-400, 400);
	glVertex2f(400, 400);
	//grey color
	glColor3f(.5, .5, .5);
	glVertex2f(400, -400);
	glVertex2f(-400, -400);
	glEnd();

	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	glVertex2f(WINDOW_WIDTH / 2, 0);
	glVertex2f(-WINDOW_WIDTH / 2, 0);
	glVertex2f(0, WINDOW_HEIGHT / 2);
	glVertex2f(0, -WINDOW_HEIGHT / 2);
	glEnd();

	circle_wire(0, 0, 200);
	circle_wire(0, 0, 250);
	circle_wire(0, 0, 300);
	

	vprint(350 * cos(fObj.angle * D2R), 350 * sin(fObj.angle * D2R), GLUT_BITMAP_9_BY_15, "%0.0f", fObj.angle);
	glColor3f(1, 1, 1);

	vprint(-360, 360, GLUT_BITMAP_TIMES_ROMAN_24, "TIMER");
	vprint(-350, 330, GLUT_BITMAP_TIMES_ROMAN_24, "%02d:%02d", timet.timeMin, timet.timeSec);

	vprint(250, 380, GLUT_BITMAP_HELVETICA_12, "NUMBER OF GAMES");
	vprint(310, 360, GLUT_BITMAP_HELVETICA_12, "%d", game_no);

} // back gorund fucntion showing  lines and circulat paths 
//revolving circles function(it is function for only one circle)
void movingObject(movingObj_t obj) {
	glColor3ub(obj.r, obj.g, obj.b);
	circle(obj.x, obj.y, obj.radius);
	glColor3f(0, 0, 0);
	vprint(obj.x - 12, obj.y-3, GLUT_BITMAP_9_BY_15, "%0.0f", obj.angle);
}// function of the balls/ circles that are moving in circular path 
//cannon's structure fucntion
void hittingObject(firingObj_t firing_Obj) {
	glColor3f(.6, 0, 0);
	circle(0, 0, 25);
	float fangle = firing_Obj.angle * D2R; // new angle or changing the rotation
	glBegin(GL_QUADS);
	vertex({ 0, 25 }, { 0, 0 }, fangle);
	vertex({ 0, -25 }, { 0, 0 }, fangle);
	glColor3f(.1, 0.1, 0.1);

	vertex({ 50, -15 }, { 0, 0 }, fangle);
	vertex({ 50, 15 }, { 0, 0 }, fangle);
	glEnd();

	glColor3f(1, 1, 0);
	glBegin(GL_LINES);

	vertex({ 0, 25 }, { 0, 0 }, fangle);
	vertex({ 0, -25 }, { 0, 0 }, fangle);

	vertex({ 20, 23 }, { 0, 0 }, fangle);
	vertex({ 20, -23 }, { 0, 0 }, fangle);

	vertex({ 0, 0 }, { 0, 0 }, fangle);
	vertex({ 50, 0 }, { 0, 0 }, fangle);

	glEnd();


	// line aim that are the stripe lines for the aiming purposes 
	glColor3f(1, 1, 0);  // line color
	glLineWidth(3);   // line thickness
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(3, 0xf0f0);
	glBegin(GL_LINES);
	glVertex2f(20 * cos(fObj.angle *D2R), 20 * sin(fObj.angle *D2R));
	glVertex2f(320 * cos(fObj.angle *D2R), 320 * sin(fObj.angle *D2R));
	glEnd();
	glLineWidth(1);   // line thickness
	glDisable(GL_LINE_STIPPLE);



}// firing object / cannon function
//bullet function
void bullet(firingObj_t bul) {
	glColor3f(1, 1, 0);
	circle(bul.x, bul.y, 10);

}



//
// To display onto window using OpenGL commands
//
void display() {

	//
	// clear window to black
	//
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	displayBackgound(); // displaying 4 circles with the help of FOR Loop
	for (int i = 0; i < 3; i++)
	{
		if (mObj[i].hit == false)
		{
			mObj[i].x = mObj[i].Cx * cos(mObj[i].angle * D2R); // new X coordinate for the rotation
			mObj[i].y = mObj[i].Cx * sin(mObj[i].angle * D2R); // new Y coordinate for the rotation
			movingObject(mObj[i]);

		}

	}

	hittingObject(fObj);
	if (bult == true) // bullet will only be displayed when the mouse is clicked 
		bullet(bul);
	if (timer == false)		// Game message 
	{
		glColor3f(.4, .4, .4);
		glRectf(-160, -180, 190, -230);
		glColor3f(0, 0, 0);
		vprint(-150, -200, GLUT_BITMAP_TIMES_ROMAN_24, "CLICK TO START THE GAME");
	}



	glutSwapBuffers();

}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);


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

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = true; break;
	case GLUT_KEY_DOWN: down = true; break;
	case GLUT_KEY_LEFT: left = true; break;
	case GLUT_KEY_RIGHT: right = true; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
	if (timer == false && button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) // starting the game by clicking on the START GAME box in yellow color
	{
		float x2 = x - winWidth / 2;
		float y2 = winHeight / 2 - y;
		if ((x2 >= -160 && x2 <= 190) && (y2 <= -180 && y2 >= -230))
			timer = true;

	}


	else if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) // moving the bullet 
	{
		if (bult == false)
			bul.angle = fObj.angle;
		bult = true;
	}
	if (timer == false && timet.timeSec != 0)// initializing everything at the end of the game 
		initialize();


	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
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

void onMoveDown(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function   
	glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
	// Write your codes here.
	int x2 = x - winWidth / 2;
	int y2 = winHeight / 2 - y;
	float angle = (atan2f(y2, x2))*(180 / PI);
	if (angle < 0)
		angle += 360;
	fObj.angle = angle;


	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	if (timer == true)
	{
		timet.timeSec++;
		if (timet.timeSec % 100 == 0) // calculating time
		{
			timet.timeMin++;
			timet.timeSec = 0;
		}

		for (int i = 0; i < 3; i++)	// moving objects in circular manner by incrementing or decrementing depending on the direction
		{
			if (mObj[i].direction == 1)				// Anti- clockwise movement of the circle
				mObj[i].angle += mObj[i].speed;


			else
				mObj[i].angle -= mObj[i].speed;  // Clockwise movement of the circle 

			if (mObj[i].angle > 360)			// displaying the angle of the circles 
				mObj[i].angle -= 360;
			else if (mObj[i].angle < 0)			// displaying the angle of the circles 
				mObj[i].angle += 360;

			if (testCollision(bul, mObj[i]) == true)		// checking if the bullet hit the object 
				mObj[i].hit = true;


		}
		if (bult == true)			//moving the bullet by increasing X  AND Y 
		{

			bul.x += 15 * cos(bul.angle * D2R);
			bul.y += 15 * sin(bul.angle * D2R);
			if ((bul.x > 400 || bul.x < -400) || (bul.y > 400 || bul.y < -400)) //Checking if the bullet has not hit any circle it should re - appear when it leaves the screen
			{
				bult = false;
				bul.x = 0;
				bul.y = 0;
			}
		}
		if (mObj[0].hit == true && mObj[1].hit == true && mObj[2].hit == true) //if all the objects are hit, the game is over 
		{
			game_no++;
			
		
			timer = false;
			bult = false;
		}

	}






	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
#endif 

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	initialize();

} 

void main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("HITTER BY SALAHUDDIN");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);
	srand(time(NULL));
	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}