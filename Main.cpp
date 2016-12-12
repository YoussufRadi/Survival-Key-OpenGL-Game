#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <math.h>
#include <stdio.h>
#include <glut.h>

#define PI 3.14159265
#define degToRad(x) ((x)*PI/180)
#pragma  comment(lib, "legacy_stdio_definitions.lib")

int WIDTH = glutGet(GLUT_SCREEN_WIDTH);
int HEIGHT = glutGet(GLUT_SCREEN_HEIGHT);

GLuint tex;
char title[] = "3D Survival Game";

//int keyLocations [4][4] = { { 20, 1, 3, 0},
//							{ -8, 1, -5, 0},
//							{ 20, 74, 67, 0},
//							{ 11, 18, 14, 0} };



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

int keyCount = 0;
const int keysAmount = 3;
Vector key0(20, 1, 3);
Vector key1(-8, 1, -5);
Vector key2(10, 1, -23);
Vector keyLocations[] = { key0, key1, key2 };
bool keysTaken[] = { false,false,false };


// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 1000;
Vector Eye(20, 5, 20);
Vector At(Eye.x, Eye.y, Eye.z-10);
Vector Up(0, 1, 0);
GLdouble upAngle = -90;
GLdouble sideAngle = 180;
int mouseXOld = 0;
int mouseYOld = 0;

int cameraZoom = 0;

// Model Variables
Model_3DS model_house;
Model_3DS model_tree;

// Textures
GLTexture tex_ground;
GLTexture tex_key;

void setUpLights()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat ambient[] = { 0.1f, 0.1f, 0.1, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

void setUpMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
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
	setUpLights();
	setUpMaterial();

}

void RenderGround()
{
	glDisable(GL_LIGHTING);	// Disable lighting 
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

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.
	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

void drawKeys() {
	
	glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, tex_key.texture[0]);
	glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
	glDisable(GL_TEXTURE_GEN_T);

	for (int i = 0; i < keysAmount; i++)
	{
		if (keysTaken[i] == false) {
			glPushMatrix();
			glTranslated(keyLocations[i].x, keyLocations[i].y, keyLocations[i].z);
			glScaled(0.5, 0.5, 0.5);
			glutSolidTeapot(1);
			glPopMatrix();
		}
	}
}

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glRotated(upAngle, 1, 0, 0);
	glRotated(sideAngle, 0, 1, 0);
	gluLookAt(Eye.x, Eye.y, Eye.z, Eye.x, Eye.y, Eye.z-10, Up.x, Up.y, Up.z);

	GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
	GLfloat lightPosition[] = {0.0f, 100.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);

	glPushMatrix();
	drawKeys();
	glPopMatrix();

	// Draw Ground
	RenderGround();
	
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



	//glPushMatrix();
	//glScaled(0.9, 0.9, 0.9);
	//glTranslated(keyLocations[0].x-8.8, keyLocations[0].y, keyLocations[0].z-1.3);
	////glRotatef(90.f, 1, 0, 0);
	//glutSolidCube(4);
	//glPopMatrix();
	

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
	//glDisable(GL_TEXTURE_2D);

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

	//printf("Eye.x: %f	 Eye.z: %f\n", Eye.x, Eye.z);
	//printf("Range in %i to %i\n", tx - 1, tx + 1);

	glutPostRedisplay();
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
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		for (int i = 0; i < keysAmount; i++)
		{
			if (Eye.x <= keyLocations[i].x + 5 && Eye.x >= keyLocations[i].x - 5
				&& Eye.z <= keyLocations[i].z + 5 && Eye.z >= keyLocations[i].z - 5
				&& keysTaken[i] == false) {
					keysTaken[i] = true;
					keyCount++;
					printf("Key Count: %i", keyCount);
					printf("\n");
			}
		}
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
	model_tree.Load("Models/tree/Tree1.3ds");

	// Loading texture files
	tex_ground.Load("Textures/ground.bmp");
	tex_key.Load("Textures/gold.bmp");
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
	glutMouseFunc(actM);
	glutIdleFunc(anim);
	glutReshapeFunc(myReshape);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	setUpCamera();
	LoadAssets();
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}