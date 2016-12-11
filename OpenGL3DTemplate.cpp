#include <math.h>
#include <string>
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

double shootingAngle = 10;

double bulletX = 0;
double bulletY = 0;
double bulletZ = 0;
int bulletAngle = 0;

double grenadeX = 0;
double grenadeY = 0;
double grenadeZ = 0;
int grenadeAngle = 0;

double shurikenX = 0;
double shurikenY = 0;
double shurikenZ = 0;
int shurikenAngle = 0;

double targetX = 0;
double targetY = 0;
double targetZ = -55;

double* p = new double[3];
int p0[3];
int p1[3];
int p2[3];
int p3[3];

int weapone = 0;
bool hit = false;
bool replay = false;
bool done = false;
float startFire = 0;
bool passTarget = false;

int s = 0;

void startAgain(){
	eyeX = 2;
	eyeY = 2;
	eyeZ = 6;
	cameraX = 0;
	cameraY = 0;
	cameraZ = -60;
	shootingAngle = 0;
	bulletX = 0;
	bulletY = 0;
	bulletZ = 0;
	bulletAngle = 0;
	grenadeX = 0;
	grenadeY = 0;
	grenadeZ = 0;
	grenadeAngle = 0;
	shurikenX = 0;
	shurikenY = 0;
	shurikenZ = 0;
	shurikenAngle = 0;
	targetX = 0;
	targetY = 0;
	targetZ = -55;
	weapone = 0;
	hit = false;
	replay = false;
	done = false;
	startFire = 0;
	passTarget = false;
}

void replayScene(){
	eyeX = 0;
	eyeY = 0;
	eyeZ = 10;
	cameraX = 0;
	cameraY = 0;
	cameraZ = -60;
	bulletX = 0;
	bulletY = 0;
	bulletZ = 0;
	bulletAngle = 0;
	grenadeX = 0;
	grenadeY = 0;
	grenadeZ = 0;
	grenadeAngle = 0;
	shurikenX = 0;
	shurikenY = 0;
	shurikenZ = 0;
	shurikenAngle = 0;
	replay = true;
	startFire = 0;
	passTarget = false;
}

void setupLights() {
	GLfloat lmodel_ambient[] = { 1.0f, 1.0f, 1.0f, 0.1f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	GLfloat l0Diffuse[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	GLfloat l0Ambient[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	GLfloat l0Position[] = { 00.0f, 20.0f, -10.0f, false };
	GLfloat l0Direction[] = { 0.0, -1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l0Diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, l0Ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, l0Position);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 10.0);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 90.0);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, l0Direction);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
}

void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-0.5, 0.5, -0.5, 0.5, -1, 1);
	gluPerspective(60, screenX / screenY, 0.001, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotated(shootingAngle, 0, 1, 0);
	gluLookAt(eyeX, eyeY, eyeZ,   //eye
			 cameraX, cameraY, cameraZ,   //center
			  0.0, 1.0, 0.0);  //up
}

int xOld = 0;
void passM(int x, int y)
{
	
	if (!hit){
		shootingAngle += ((2 * (xOld-x) - screenX) / screenX) * 180;
		xOld = x;
		printf("Angle %f    ", xOld - x);
		double radAngle = shootingAngle * degToRad;
		//cameraX = 50* tan(radAngle);
	}
		
	glutPostRedisplay();
}

void actM(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !hit)
		hit = true;
	glutPostRedisplay();
}

void spe(int k, int x, int y)
{
	if (k == GLUT_KEY_UP){
		eyeY++;
		if (eyeY > 0)
			eyeZ--;
		else
			eyeZ++;
	}
	if (k == GLUT_KEY_DOWN){
		eyeY--;
		if (eyeY < 0)
			eyeZ--;
		else
			eyeZ++;
	}
	if (k == GLUT_KEY_LEFT){
		eyeX++;
	}
	if (k == GLUT_KEY_RIGHT)
		eyeX--;
	glutPostRedisplay();
}

void key(unsigned char k, int x, int y)
{
	if (k == '1' && !hit)
		weapone = 0;
	if (k == '2' && !hit)
		weapone = 1;
	if (k == '3' && !hit)
		weapone = 2;
	if (k == 'w' && !hit && targetZ > -58)
		targetZ -= 2;
	if (k == 'a' && !hit && targetX > -33)
		targetX -= 2;
	if (k == 's' && !hit && targetZ < -20)
		targetZ += 2;
	if (k == 'd' && !hit && targetX < 33)
		targetX += 2;
	if (k == (char)27)
		exit(0);
	if (k == (char)13)
		startAgain();
	if (k == (char)32 && done){
		replayScene();
	}

	glutPostRedisplay();
}

void scoredisplay(float x, float y, void *font, const char *string){
	const char *c;
	glRasterPos2f(x, y);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void bezier(float t, int* p0, int* p1, int* p2, int* p3, double* p)
{
	p[0] = pow((1 - t), 3)*p0[0] + 3 * t*pow((1 - t), 2)*p1[0] + 3 * pow(t, 2)*(1 - t)*p2[0] + pow(t, 3)*p3[0];
	p[1] = pow((1 - t), 3)*p0[1] + 3 * t*pow((1 - t), 2)*p1[1] + 3 * pow(t, 2)*(1 - t)*p2[1] + pow(t, 3)*p3[1];
	p[2] = pow((1 - t), 3)*p0[2] + 3 * t*pow((1 - t), 2)*p1[2] + 3 * pow(t, 2)*(1 - t)*p2[2] + pow(t, 3)*p3[2];
}

void drawBullet(){
	glColor3d(0.6, 0.4, 0.2);
	glutSolidTorus(0.05, 0.29, 100, 100);
	glutSolidTorus(0.05, 0.14, 100, 100);
	gluCylinder(quadratic, 0.3, 0.3, 2.8, 100, 10);
	gluDisk(quadratic, 0, 0.3, 100, 100);
	glPushMatrix();
	glTranslated(0, 0, 2.8);
	gluCylinder(quadratic, 0.3, 0.23, 0.27, 100, 10);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0, 0, 3.07);
	gluCylinder(quadratic, 0.23, 0.23, 0.5, 100, 10);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0, 0, 3.57);
	gluCylinder(quadratic, 0.23, 0.14, 0.255, 100, 10);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0, 0, 3.79);
	glutSolidSphere(0.142, 100, 100);
	glPopMatrix();
	glColor3d(1, 1, 1);
}

void drawGrenade(){
	glColor3d(0.1, 0.5, 0.2);
	glutSolidSphere(4, 100, 100);
	glColor3d(0.1, 0.2, 0.2);
	glutSolidTorus(0.2, 4, 100, 100);
	glPushMatrix();
	glTranslated(0, 0, 2.5);
	gluCylinder(quadratic, 1.2, 1.2, 2.2, 100, 100);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0, 0, 4.7);
	gluDisk(quadratic, 0, 1.2, 100, 100);
	glPopMatrix();
	glColor3d(1, 1, 1);
}

void drawShurikenPart(){
	glPushMatrix();
	glTranslated(0, 0, 1.48);
	glScaled(1, 0.4, 1);
	glutSolidCone(0.5, 2.5, 4, 1);
	glPushMatrix();
	glRotated(180, 0, 1, 0);
	glColor3d(0.5, 0.5, 0.5);
	glutSolidCone(0.5, 1, 4, 1);
	glPopMatrix();
	glPopMatrix();
	glColor3d(1, 1, 1);
}

void drawShuriken(){

	glutSolidTorus(0.07, 0.43, 100, 100);

	glColor3d(0.1, 0.4, 0.1);
	glutSolidCone(0.38, 0.25, 4, 1);
	
	glPushMatrix();
	glRotated(180, 0, 1, 0);
	glutSolidCone(0.38, 0.25, 4, 1);
	glPopMatrix();
	glColor3d(1, 1, 1);

	glPushMatrix();
	glRotated(-90, 1, 0, 0);
	drawShurikenPart();
	glPopMatrix();

	glPushMatrix();
	glRotated(-120, 0, 0, 1);
	glRotated(-90, 1, 0, 0);
	drawShurikenPart();
	glPopMatrix();

	glPushMatrix();
	glRotated(120, 0, 0, 1);
	glRotated(-90, 1, 0, 0);
	drawShurikenPart();
	glPopMatrix();
}

void drawTarget(){
	glColor3f(1.0, 0.0, 0.0);
	//gluDisk(quadratic, 4, 5, 100, 100);
	//glColor3f(1.0, 1.0, 1.0);
	//gluDisk(quadratic, 3, 4, 100, 100);
	//glColor3f(1.0, 0.0, 0.0);
	//gluDisk(quadratic, 2, 3, 100, 100);
	//glColor3f(1.0, 1.0, 1.0);
	//gluDisk(quadratic, 1, 2, 100, 100);
	//glColor3f(1.0, 0.0, 0.0);
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
	glColor3f(1, 1.0, 0.8);
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

void drawBulletPath(){
	glColor3d(0.5, 0.5, 0.5);
	double radAngle = shootingAngle * degToRad;
	double slope = tan(radAngle);
	double xPosition = 2 * sin(radAngle);
	for (double i = bulletZ - 2; i > targetZ && !hit; i--){
		glPushMatrix();
		xPosition += slope;
		glTranslated(xPosition, 0, i);
		glutSolidSphere(0.1, 100, 100);
		glPopMatrix();
	}
	glColor3d(1, 1, 1);
}

void drawGrenadePath(){
	glColor3f(0.5, 0.5, 0.5);
	double radAngle = shootingAngle * degToRad;
	p0[0] = 0;
	p0[1] = 0;
	p0[2] = 0;

	p1[0] = 2;
	p1[1] = 15;
	p1[2] = 15 * tan(radAngle);
	
	p2[0] = 72;
	p2[1] = 10;
	p2[2] = 50 * tan(radAngle);
	
	p3[0] = 62;
	p3[1] = -30;
	p3[2] = 60 * tan(radAngle);
	
	for (float t = 0; t<1 && !hit; t += 0.02)
	{
		bezier(t, p0, p1, p2, p3, p);
		glPushMatrix();
		glTranslated(p[2], p[1], -p[0]);
		glutSolidSphere(0.1, 100, 100);
		glPopMatrix();
	}
	glColor3d(1, 1, 1);
}

void drawShurikenPath(){
	glColor3f(0.5, 0.5, 0.5);
	double radAngle = shootingAngle * degToRad;
	p0[0] = 0;
	p0[1] = 0;

	p1[0] = 30;
	p1[1] = 15 + 30 * tan(radAngle);

	p2[0] = 55;
	p2[1] = 30 +55 * tan(radAngle);

	p3[0] = 60;
	p3[1] = 0 + 60 * tan(radAngle);

	for (float t = 0.02; t<1 && !hit; t += 0.02)
	{
		bezier(t, p0, p1, p2, p3, p);
		glPushMatrix();
		glTranslated(p[1], 0, -p[0]);
		glutSolidSphere(0.1, 100, 100);
		glPopMatrix();
	}
	glColor3d(1, 1, 1);
}

void Display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*if (shootingAngle > 120)
		shootingAngle++;*/
	//setupLights();
	setupCamera();
	
	switch (weapone)
	{
	case 0:
		drawBulletPath();
		glPushMatrix();
		glTranslated(bulletX, bulletY, bulletZ);
		glScaled(0.6, 0.6, 0.5);
		glRotated(-shootingAngle, 0, 1, 0);
		glRotated(180, 0, 1, 0);
		glRotated(bulletAngle, 0, 0, 1);
		drawBullet();
		glPopMatrix();
		break;
	case 1:
		drawGrenadePath();
		glPushMatrix();
		glTranslated(grenadeX, grenadeY, grenadeZ);
		glScaled(0.1, 0.1, 0.1);
		glRotated(-45, 1, 0, 0);
		glRotated(shootingAngle, 0, 1, 0);
		glRotated(grenadeAngle, 1, 1, 0);
		drawGrenade();
		glPopMatrix();
		break;
	case 2:
		drawShurikenPath();
		glPushMatrix();
		glTranslated(shurikenX, shurikenY, shurikenZ);
		glRotated(shootingAngle, 0, 1, 0);
		glRotated(90, 1, 0, 0);
		glRotated(shurikenAngle, 0, 0, 1);
		glScaled(0.2, 0.2, 0.2);
		drawShuriken();
		glPopMatrix();
		break;
	}

	glPushMatrix();
	glTranslated(targetX, targetY, targetZ);
	glScaled(2, 2, 0.5);
	drawTarget();
	glPopMatrix();
	
	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslated(cameraX-4, 12, targetZ + 3);
	scoredisplay(0, 0, (void *)GLUT_BITMAP_TIMES_ROMAN_24, ("Score is: " + std::to_string(s)).c_str() );
	//print(0, 0, ("Score is: " + std::to_string(s)).~basic_string);
	glColor3f(1, 1, 1);
	glPopMatrix();

	drawRoom();

	glFlush();
}



void Anim() {
	if (hit){
		//variables for Bullet Firing	
		double radAngle = shootingAngle * degToRad;
		double slope = tan(radAngle);
		double zStop = 0;

		//variables for Gernade & Shuriken Firing	
		if (weapone != 2){
			p0[0] = 0;
			p0[1] = 0;
			p0[2] = 0;

			p1[0] = 2;
			p1[1] = 15;
			p1[2] = 15 * tan(radAngle);

			p2[0] = 72;
			p2[1] = 10;
			p2[2] = 50 * tan(radAngle);

			p3[0] = 62;
			p3[1] = -30;
			p3[2] = 60 * tan(radAngle);
		}
		else{
			p1[0] = 30;
			p1[1] = 15 + 30 * tan(radAngle);

			p2[0] = 55;
			p2[1] = 30 + 55 * tan(radAngle);

			p3[0] = 60;
			p3[1] = 0 + 60 * tan(radAngle);
		}
		if ( !passTarget && (targetX + 10 > targetZ * -tan(radAngle) && targetX - 10 < targetZ * -tan(radAngle)))
			zStop = targetZ;
		else
			zStop = -60;
		switch (weapone)
		{
		case 0:
			if (bulletZ > zStop + 1 && bulletX < 43 && bulletX > -43){
				bulletX += 1 * slope;
				bulletZ = startFire;
				startFire -= 1;
				bulletAngle += 40;
				bulletAngle = bulletAngle % 360;
			}
			else{
				if (!done && zStop == targetZ)
					s++;
				done = true;
				if (!replay)
					replayScene();
			}
			if (replay)
			{
				eyeX = bulletX + 2;
				eyeY = bulletY + 2;
				eyeZ = 5 + bulletZ;
				cameraX = bulletX;
				cameraY = bulletY;
				cameraZ = bulletZ;
			}
			break;
		case 1:
			if (grenadeY > sqrt(10*10 - (targetX - grenadeX)*(targetX - grenadeX))){
				passTarget = true;
				zStop = -60;
			}
			else if (!passTarget)
				zStop = targetZ ;
			
			if (grenadeZ > zStop + 1 && grenadeX < 43 && grenadeX > -43){
				bezier(startFire, p0, p1, p2, p3, p);
				grenadeX = p[2];
				grenadeY = p[1];
				grenadeZ = -p[0];
				startFire += 0.01;
				grenadeAngle += 8;
				grenadeAngle = grenadeAngle % 360;
			}
			else{
				if (!done && zStop == targetZ)
					s++;
				done = true;
				if (!replay)
					replayScene();
			}
			if (replay)
			{
				eyeX = grenadeX + 2;
				eyeY = grenadeY-1;
				eyeZ = 5 + grenadeZ;
				cameraX = grenadeX;
				cameraY = grenadeY;
				cameraZ = grenadeZ;
			}
			break;
		case 2:
			if ( !passTarget && shurikenX < targetX + 10 && shurikenX > targetX -10)
				zStop = targetZ;
			else{
				if (shurikenZ == targetZ)
					passTarget = true;
				zStop = -60;
			}
			if (shurikenZ > zStop + 1 && shurikenX < 43 && shurikenX > -43){
				bezier(startFire, p0, p1, p2, p3, p);
				shurikenX = p[1];
				shurikenZ = -p[0];
				startFire += 0.01;
				shurikenAngle += 20;
				shurikenAngle = shurikenAngle % 360;
			}
			else{
				if (!done && zStop == targetZ)
					s++;
				done = true;
				if (!replay)
					replayScene();
			}
			if (replay)
			{
				eyeX = shurikenX + 2;
				eyeY = shurikenY + 1;
				eyeZ = shurikenZ + 2;
				cameraX = shurikenX;
				cameraY = shurikenY;
				cameraZ = shurikenZ;
			}
			break;
		}
		
	}
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
