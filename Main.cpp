#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <math.h>
#include <stdio.h>
#include <glut.h>

#define PI 3.14159265
#define degToRad(x) ((x)*PI/180)
//#pragma  comment(lib, "legacy_stdio_definitions.lib")

int WIDTH = glutGet(GLUT_SCREEN_WIDTH);
int HEIGHT = glutGet(GLUT_SCREEN_HEIGHT);

GLuint tex;
char title[] = "3D Survival Game";

class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	//================================================================================================//
	// Operator Overloading; In C++ you can override the behavior of operators for you class objects. //
	// Here we are overloading the += operator to add a given value to all vector coordinates.        //
	//================================================================================================//
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
};

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.01;
GLdouble zFar = 1000;
Vector Eye(20, 1, 20);
Vector At(Eye.x, Eye.y, Eye.z-10);
Vector Up(0, 1, 0);
GLdouble upAngle = -90;
GLdouble sideAngle = 180;
int mouseXOld = 0;
int mouseYOld = 0;

int cameraZoom = 0;

double batteryLife = 100;

// Model Variables
Model_3DS model_house;
Model_3DS model_tree;

// Textures
GLTexture tex_ground;

void print(int x, int y, char *string)
{
	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
}

void sunLights() {
	GLfloat light_diffuse[4] = { 0.1, 0.1, 0.0, 0.1 };
	GLfloat light_ambient[4] = { 0.2, 0.2, 0.1, 0.1 };
	GLfloat light_specular[4] = { 0.3, 0.3, 0.0, 0.3 };
	GLfloat light_position[4] = { 0, 0, -10, 0.0 }; // <- w = 1
	GLfloat light_direction[3] = { 0, 0, 10};
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_direction);
}

void setUpLights()
{
	GLfloat light_diffuse[4] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_ambient[4] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light_specular[4] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[4] = { Eye.x + 5, Eye.y + 0.3, Eye.z + 15, 1.0 };
	GLfloat spot_direction[3] = { -cos(degToRad(270 - sideAngle)), -sin(degToRad(upAngle)), sin(degToRad(270 - sideAngle)) };
	GLfloat spot_cutoff = batteryLife/10;
	GLfloat spot_exponent = 0.01;
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, spot_exponent);
}

void setUpMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	GLfloat lmodel_ambient[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Will be applied to all objects
	GLfloat specular[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 30.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

void setUpCamera()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, aspectRatio, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	setUpMaterial();

}

void RenderGround()
{
	//glDisable(GL_LIGHTING);	// Disable lighting 
	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit
	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing
	glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture
	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-20, 0, -20);
	glTexCoord2f(5, 0);
	glVertex3f(20, 0, -20);
	glTexCoord2f(5, 5);
	glVertex3f(20, 0, 20);
	glTexCoord2f(0, 5);
	glVertex3f(-20, 0, 20);
	glEnd();
	glPopMatrix();

	//glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.
	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	glRotated(upAngle, 1, 0, 0);
	glRotated(sideAngle, 0, 1, 0);
	gluLookAt(Eye.x, Eye.y, Eye.z, Eye.x, Eye.y, Eye.z-10, Up.x, Up.y, Up.z);

	setUpLights();

	// Draw Ground
	RenderGround();

	//Drawing Battery Life Bar
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glColor3d(0.2, 1, 0.2);
	for (int i = 0; i < batteryLife / 10; i++){
		glPushMatrix();
		glTranslated(Eye.x+.03, Eye.y+.03, Eye.z);
		glRotated(-upAngle, 1, 0, 0);
		glRotated(-sideAngle, 0, 1, 0);
		glTranslated(0.005*i, 0, -0.1);
		glScaled(0.005, 0.005, 0.005);
		glutSolidCube(1);
		glPopMatrix();
	}
	glEnable(GL_LIGHTING);
	glColor3d(1, 1, 1);
	glPopMatrix();

	// Draw Tree Model
	glPushMatrix();
	glTranslatef(10, 0, 0);
	glScalef(0.7, 0.7, 0.7);
	model_tree.Draw();
	glPopMatrix();

	// Draw house Model
	glPushMatrix();
	glRotatef(90.f, 1, 0, 0);
	model_house.Draw();
	glPopMatrix();

	//sky box
	glPushMatrix();
	GLUquadricObj * qobj;
	qobj = gluNewQuadric();
	glTranslated(50,0,0);
	glRotated(90,1,0,1);
	glBindTexture(GL_TEXTURE_2D, tex);
	gluQuadricTexture(qobj,true);
	gluQuadricNormals(qobj,GL_SMOOTH);
	gluSphere(qobj,100,100,100);
	gluDeleteQuadric(qobj);
	glPopMatrix();

	glutSwapBuffers();
}

void myKeyboard(unsigned char button, int x, int y)
{
	switch (button)
	{
	case 'x':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'c':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 'w':
		Eye.z += -cos(degToRad(sideAngle));
		Eye.x += sin(degToRad(sideAngle));
		break;
	case  'a':
		Eye.x += cos(degToRad(180-sideAngle));
		Eye.z += -sin(degToRad(180-sideAngle));
		break;
	case  's':
		Eye.z += cos(degToRad(sideAngle));
		Eye.x += -sin(degToRad(sideAngle));
		break;
	case  'd':
		Eye.x += -cos(degToRad(180 - sideAngle));
		Eye.z += sin(degToRad(180 - sideAngle));
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void myMotion(int x, int y)
{
	y = HEIGHT - y;

	if (cameraZoom - y > 0)
	{
		Eye.x += -0.1;
		Eye.z += -0.1;
	}
	else
	{
		Eye.x += 0.1;
		Eye.z += 0.1;
	}
	cameraZoom = y;
	glLoadIdentity();	//Clear Model_View Matrix
	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);	//Setup Camera with modified paramters

	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glutPostRedisplay();	//Re-draw scene 
}

void passM(int x, int y)
{
	int diffX = x - mouseXOld;
	int diffY = y - mouseYOld;
	sideAngle += diffX*0.25;
	upAngle += diffY*0.5;
	mouseXOld = x;
	mouseYOld = y;
	if (upAngle >= 90)
		upAngle = 90;
	if (upAngle <= -90)
		upAngle = -90;
	glutPostRedisplay();
}

void actM(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		cameraZoom = y;
	}
}

void myReshape(int w, int h)
{
	if (h == 0) {
		h = 1;
	}

	WIDTH = w;
	HEIGHT = h;

	// set the drawable region of the window
	glViewport(0, 0, w, h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
}

void LoadAssets()
{
	// Loading Model files
	model_house.Load("Models/house/house.3ds");
	model_tree.Load("Models/tree/tree1.3ds");

	// Loading texture files
	tex_ground.Load("Textures/ground.bmp");
	loadBMP(&tex, "Textures/sky4-jpg.bmp", true);
}

void rotateCamera(){
	if (mouseXOld > (9 * WIDTH / 10))
		sideAngle+=0.5;
	if (mouseXOld < (WIDTH / 10))
		sideAngle-=0.5;
	if (sideAngle > 360)
		sideAngle -= 360;
	if (sideAngle < -360)
		sideAngle += 360;
}

void anim(){
	batteryLife -= 0.01;
	rotateCamera();
	glutPostRedisplay();
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow(title);
	//glutFullScreen();
	glutSetCursor(GLUT_CURSOR_NONE);

	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);
	glutPassiveMotionFunc(passM);
	glutMotionFunc(myMotion);
	glutMouseFunc(actM);
	glutIdleFunc(anim);
	glutReshapeFunc(myReshape);

	glClearColor(0.0, 0.0, 0.0, 0.0);

	glPushMatrix();
	
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
	//glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);

	setUpCamera();
	LoadAssets();
	sunLights();
	
	glPopMatrix();

	glutMainLoop();
}

//glDisable(GL_LIGHTING);
//glPushMatrix();
//glTranslated(Eye.x, Eye.y, Eye.z);
//glRotated(-upAngle, 1, 0, 0);
//glRotated(-sideAngle, 0, 1, 0);
//glTranslated(0.005, 0, -0.1);
//glScaled(0.005, 0.005, 0.005);
//glPushMatrix();
//glTranslated(0, 0, 1);
//print(0, 0, "sadasdsads sdad wqewq e sadasd sa d ");
//glPopMatrix();
//glPushMatrix();
//glTranslated(2.3, 0, 0);
//glScaled(7, 0.7, 1);
//glutSolidCube(1);
//glPopMatrix();
//glPopMatrix();
//glEnable(GL_LIGHTING);