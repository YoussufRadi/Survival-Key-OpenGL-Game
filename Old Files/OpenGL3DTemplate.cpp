#include <math.h>
#include <string.h>
#include <glut.h>

#define PI 3.14159265
#define radToDeg  (180.0 / PI);
#define degToRad  (PI / 180.0);

GLUquadricObj *quadratic;

double screenX = glutGet(GLUT_SCREEN_WIDTH);
double screenY = glutGet(GLUT_SCREEN_HEIGHT);

double eyeX = 2;
double eyeY = 2;
double eyeZ = 6;

double cameraX = 0;
double cameraY = 0;
double cameraZ = -60;

double angle = 0;

void startAgain(){
	eyeX = 2;
	eyeY = 2;
	eyeZ = 6;
	cameraX = 0;
	cameraY = 0;
	cameraZ = -60;
	angle = 0;
}

void setupLights() {
	GLfloat ambient[] = { 0.9f, 0.9f, 0.9f, 1 };
	GLfloat diffuse[] = { 0.8f, 0.8f, 0.8f, 0.9f };
	GLfloat specular[] = { 0.8f, 0.8f, 0.8f, 1 };
	GLfloat shininess[] = { 60 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	GLfloat lightIntensity[] = { 1, 1, 1, 1.0f };
	GLfloat lightPosition[] = { -7.0f, 6.0f, 8.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
}

void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-0.5, 0.5, -0.5, 0.5, -1, 1);
	gluPerspective(60, screenX / screenY, 0.001, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ,   //eye
			 cameraX, cameraY, cameraZ,   //center
			  0.0, 1.0, 0.0);  //up
}

void passM(int x, int y)
{
	angle = ((2 * x - screenX) / screenX) * 62;
	double radAngle = angle * degToRad;
	cameraX = 50* tan(radAngle);		
	glutPostRedisplay();
}

void actM(int button, int state, int x, int y)
{
	glutPostRedisplay();
}

void spe(int k, int x, int y)
{
	if (k == GLUT_KEY_UP)
		eyeY++;
	if (k == GLUT_KEY_DOWN)
		eyeY--;
	glutPostRedisplay();
}

void key(unsigned char k, int x, int y)
{
	if (k == 'w')
		eyeZ--;
	if (k == 'a')
		eyeX--;
	if (k == 's')
		eyeZ++;
	if (k == 'd')
		eyeX++;
	if (k == (char)27)
		exit(0);
	if (k == (char)13)
		startAgain();
	glutPostRedisplay();
}

void print(int x, int y, char *string)
{
	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
}

void drawTarget(){
	glColor3f(1.0, 0.0, 0.0);
	gluDisk(quadratic, 0, 5, 100, 100);
	gluCylinder(quadratic, 5, 5, 1, 100, 100);
	glPushMatrix();
	glTranslated(0, 0, 1);
	glColor3f(1.0, 0.0, 0.0);
	gluDisk(quadratic, 4, 5, 100, 100);
	glColor3f(1.0, 1.0, 1.0);
	gluDisk(quadratic, 3, 4, 100, 100);
	glColor3f(1.0, 0.0, 0.0);
	gluDisk(quadratic, 2, 3, 100, 100);
	glColor3f(1.0, 1.0, 1.0);
	gluDisk(quadratic, 1, 2, 100, 100);
	glColor3f(1.0, 0.0, 0.0);
	gluDisk(quadratic, 0, 1, 100, 100);
	glColor3f(1.0, 1.0, 1.0);
	glPopMatrix();
}

void drawWall(double thickness) {
	glPushMatrix();
	glColor3f(0.7, 0.55, 0.0);
	glScaled(1.0, 5 * thickness, 1.0);
	glutSolidCube(10);
	glColor3f(1.0, 1.0, 1.0);
	glPopMatrix();
}

void drawRoom(){
	//Front Wall
	glPushMatrix();
	glTranslated(0, 0, -60);
	glScaled(10, 4, 1);
	glRotated(-90, 1.0, 0.0, 0.0);
	drawWall(0.02);
	glPopMatrix();
	//Backward Wall
	glPushMatrix();
	glTranslated(0, 0, 35);
	glScaled(10, 4, 1);
	glRotated(-90, 1.0, 0.0, 0.0);
	drawWall(0.02);
	glPopMatrix();
	//Downward Wall
	glPushMatrix();
	glTranslated(0, -20, -25);
	glScaled(10, 4, 12);
	drawWall(0.02);
	glPopMatrix();
	//Upper Wall
	glPushMatrix();
	glTranslated(0, 20, -25);
	glScaled(10, 4, 12);
	drawWall(0.02);
	glPopMatrix();
	//Right Wall
	glPushMatrix();
	glTranslated(50, 0, -25);
	glScaled(10, 4, 12);
	glRotated(90, 0.0, 0.0, 1.0);
	drawWall(0.02);
	glPopMatrix();
	//Left Wall
	glPushMatrix();
	glTranslated(-50, 0, -25);
	glScaled(10, 4, 12);
	glRotated(-90, 0.0, 0.0, 1.0);
	drawWall(0.02);
	glPopMatrix();
}
void Display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	setupLights();
	setupCamera();
	
	glPushMatrix();
	glScaled(2, 2, 0.5);
	drawTarget();
	glPopMatrix();

	drawRoom();

	glFlush();
}

void Anim() {
	glutPostRedisplay();
}

void main(int argc, char** argv) {
	
	glutInit(&argc, argv);
	glutInitWindowSize(screenX, screenY);
	glutInitWindowPosition(0, 0);

	glutCreateWindow("Assignment 2");
	quadratic = gluNewQuadric();
	glutDisplayFunc(Display);
	glutIdleFunc(Anim);

	glutFullScreen();
	glutSetCursor(GLUT_CURSOR_NONE);
	glutPassiveMotionFunc(passM);
	glutKeyboardFunc(key);
	glutSpecialFunc(spe);	
	glutMouseFunc(actM);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	
	glutMainLoop();
}
