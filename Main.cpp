#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <glut.h>

#define PI 3.14159265
#define degToRad(x) ((x)*PI/180)
//#pragma  comment(lib, "legacy_stdio_definitions.lib")

int WIDTH = glutGet(GLUT_SCREEN_WIDTH);
int HEIGHT = glutGet(GLUT_SCREEN_HEIGHT);

GLuint tex;
char title[] = "Key To Survival";

class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
};

bool playSound = true;
float soundPlaying = 10;

int keyCount = 0;
const int keysAmount = 5;
Vector key0(12.15, 1, 0.4);
Vector key1(-79.6, 0, 0.36);
Vector key2(-70.76, 0, 30);
Vector key3(89, 2, 76);
Vector key4(0.984, 0, -74.58);
Vector keyLocations[] = { key0, key1, key2, key3, key4};
bool keysTaken[] = { false, false, false, false, false};

const int batteriesAmount = 4;
Vector battery0(-70.5987, 0, -2.65);
Vector battery1(-69, 0, -71);
Vector battery2(87.19, 0, -54.37);
Vector battery3(-84.804683, 0, 31.835105);
Vector batteryLocations[] = { battery0, battery1, battery2, battery3 };
bool batteriesTaken[] = { false, false, false, false};


// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.01;
GLdouble zFar = 1000;
Vector Eye(20, 2, 20);
Vector At(Eye.x, Eye.y, Eye.z - 10);
Vector Up(0, 1, 0);
GLdouble upAngle = 0;
GLdouble sideAngle = 180;
double skyAngle = 90;
int mouseXOld = 0;
int mouseYOld = 0;
int gameMode = 1;
bool won = false;

double jumpFactor = 280;
bool jumpFlag = false;
double batteryLife = 100;

// Model Variables
Model_3DS asian_house;
Model_3DS barrels;
Model_3DS tree_house;
Model_3DS bathroom;
Model_3DS swamp_house;
Model_3DS maple_tree;
Model_3DS tree;
Model_3DS house;
Model_3DS key;
Model_3DS battery;

#pragma region

double treesPos[2][2];
double fencesPos[2][2];

Vector D0(-52, 0, 30);
Vector D1(-88, 0, 30);
Vector D2(-88, 0, 66);
Vector D3(-52, 0, 66);
Vector E0(-83, 0, 2);
Vector E1(-62, 0, 3);
Vector E2(-70, 0, -10);
Vector E3(-70, 0, -10);
Vector I0(-65, 0, -68);
Vector I1(-65, 0, -81);
Vector I2(-84, 0, -81);
Vector I3(-84, 0, -68);
Vector H0(-2, 0, 2);
Vector H1(14, 0, 2);
Vector H2(14, 0, -2);
Vector H3(-2, 0, -2);
Vector C0(62, 0, 56);
Vector C1(62, 0, 94);
Vector C2(88, 0, 94);
Vector C3(88, 0, 56);
Vector B0(95, 0, -27);
Vector B1(95, 0, -64);
Vector B2(60, 0, -64);
Vector B3(58, 0, -27);

Vector buildingsPos[24] = { D0, D1 ,D2, D3, E0, E1 ,E2, E3 , I0, I1 ,I2, I3 , H0, H1 ,H2, H3, C0, C1 ,C2, C3 , B0, B1 ,B2, B3 };
#pragma endregion

// Textures
GLTexture tex_ground;
GLTexture tex_key;


void startAgain(){
	keyCount = 0;
	keysTaken[0] = false;
	keysTaken[1] = false;
	keysTaken[2] = false;
	keysTaken[3] = false;
	keysTaken[4] = false;
	batteriesTaken[0] = false;
	batteriesTaken[1] = false;
	batteriesTaken[2] = false;
	batteriesTaken[3] = false;
	Eye.x = 20;
	Eye.y = 2;
	Eye.z = 20;
	upAngle = 0;
	sideAngle = 180;
	skyAngle = 90;
	jumpFactor = 280;
	jumpFlag = false;
	batteryLife = 100;
	gameMode = 1;
	won = false;
}

void print(int x, int y, char *string)
{
	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
}

void nightLights() {
	GLfloat light_diffuse[4] = { 0.03, 0.03, 0.05, 0.3 };
	GLfloat light_ambient[4] = { 0.03, 0.03, 0.05, 0.5 };
	GLfloat light_specular[4] = { 0.3, 0.3, 0.3, 0.1 };
	GLfloat light_position[4] = { 0, 0, -10, 0.0 };
	GLfloat light_direction[3] = { 0, 0, 10 };
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_direction);
}

void drawTree(double x, double z) {
	glPushMatrix();
	glTranslatef(x, 8.5, z);
	glScalef(6, 6, 6);
	maple_tree.Draw();
	glPopMatrix();
}

void setUpLights()
{
	GLfloat light_diffuse[4] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_ambient[4] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light_specular[4] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[4] = { Eye.x, Eye.y+0.5, Eye.z, 1.0 };
	GLfloat spot_direction[3] = { -cos(degToRad(270 - sideAngle)) * 20, -sin(degToRad(upAngle)) * 20-.5, sin(degToRad(270 - sideAngle)) * 20 };
	GLfloat spot_cutoff = batteryLife / 10;
	GLfloat spot_exponent = 10;
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
	gluLookAt(Eye.x, Eye.y, Eye.z, Eye.x, Eye.y, Eye.z - 10, Up.x, Up.y, Up.z);
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
	glVertex3f(-200, 0, -200);
	glTexCoord2f(10, 0);
	glVertex3f(200, 0, -200);
	glTexCoord2f(10, 10);
	glVertex3f(200, 0, 200);
	glTexCoord2f(0, 10);
	glVertex3f(-200, 0, 200);
	glEnd();
	glPopMatrix();

	//glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.
	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

void drawBatteries() {
	for (int i = 0; i < batteriesAmount; i++)
	{
		if (batteriesTaken[i] == false) {
			glPushMatrix();
			glTranslated(batteryLocations[i].x, batteryLocations[i].y, batteryLocations[i].z);
			glScaled(0.5, 0.5, 0.5);
			battery.Draw();
			glPopMatrix();
		}
	}
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
			glScaled(0.05, 0.05, 0.05);
			//glutSolidTeapot(1);
			key.Draw();
			glPopMatrix();
		}
		else {
			glDisable(GL_LIGHTING);
			glPushMatrix();
			glTranslated(Eye.x, Eye.y, Eye.z);
			glRotated(-sideAngle, 0, 1, 0);
			glRotated(-upAngle + 15, 1, 0, 0);
			glTranslated(0.01*(i-3), 0, -0.1);
			glRotated(90, 0, 1, 0);
			glRotated(90, 0, 0, 1);
			glScaled(0.0007, 0.0007, 0.0007);
			//glutSolidTeapot(1);
			key.Draw();
			glPopMatrix();
			glEnable(GL_LIGHTING);
		}
	}
}

void drawBatteryBar() {
	glDisable(GL_LIGHTING);
	glColor3d(0.2, 1, 0.2);
	glPushMatrix();
	glTranslated(Eye.x , Eye.y, Eye.z);
	glRotated(-sideAngle, 0, 1, 0);
	glRotated(-upAngle+20, 1, 0, 0);
	glTranslated(0, 0, -0.2);
	glRotated(-90, 0, 1, 0);
	glScaled(0.005, 0.005, 0.001*batteryLife);
	glutSolidCube(1);
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glColor3d(1, 1, 1);
}

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (gameMode){
		case 0:
			glDisable(GL_LIGHTING);
			glLoadIdentity();
			gluLookAt(Eye.x, Eye.y, Eye.z, Eye.x, Eye.y, Eye.z - 10, Up.x, Up.y, Up.z);
			glPushMatrix();
			glTranslated(Eye.x -.4, Eye.y, Eye.z-2);
			glColor3d(1, 0, 0);
			print(0, 0, "Game Over You Lost :( Press Enter To Start Again");
			glPushMatrix();
			glScaled(1, 0.1, 1);
			glTranslated(.38, 0, -.5);
			glutSolidCube(1);
			glPopMatrix();
			glPopMatrix();
			glEnable(GL_LIGHTING);
			break;
		case 1:
			if (!won){
				if (gameMode == 1){
				glLoadIdentity();
				glRotated(upAngle, 1, 0, 0);
				glRotated(sideAngle, 0, 1, 0);
				gluLookAt(Eye.x, Eye.y, Eye.z, Eye.x, Eye.y, Eye.z - 10, Up.x, Up.y, Up.z);

				setUpLights();
				}
			}
			else {
				glDisable(GL_LIGHTING);
				glPushMatrix();
				glTranslated(Eye.x - .4, Eye.y, Eye.z - 2);
				glColor3d(1, 0, 0);
				print(0, 0, "Congrats You Won :D  Press Enter To Start Again");
				glPushMatrix();
				glScaled(1, 0.1, 1);
				glTranslated(.38, 0, -.5);
				glutSolidCube(1);
				glPopMatrix();
				glPopMatrix();
				glEnable(GL_LIGHTING);
			}

			drawBatteries();

			drawKeys();

			// Draw Ground
			RenderGround();

			//Drawing Battery Life Bar
			if (batteryLife > 0)
				drawBatteryBar();

			//Draw Swamp House Model
			glPushMatrix();
			glTranslated(-75, 0, 75);
			glScaled(0.75, 0.75, 0.75);
			asian_house.Draw();
			glPopMatrix();

			//Draw Barrels Model
			glPushMatrix();
			glTranslated(-75, 0, 0);
			glScaled(0.4, 0.4, 0.4);
			barrels.Draw();
			glPopMatrix();

			//Draw tree house Model
			glPushMatrix();
			glTranslated(-75, 0, -75);
			glRotated(90, 1, 0, 0);
			glScaled(3, 3, 3);
			tree_house.Draw();
			glPopMatrix();

			// Draw Tree Model
			glPushMatrix();
			glTranslatef(0, 8.5, 75);
			glScalef(6, 6, 6);
			maple_tree.Draw();
			glPopMatrix();

			//Draw Bathroom Model
			glPushMatrix();
			glTranslated(0, 0, 0);
			glScaled(0.02, 0.02, 0.02);
			bathroom.Draw();
			glPopMatrix();

			//Draw Bathroom Model
			glPushMatrix();
			glTranslated(4, 0, 0);
			glScaled(0.02, 0.02, 0.02);
			bathroom.Draw();
			glPopMatrix();

			//Draw Bathroom Model
			glPushMatrix();
			glTranslated(8, 0, 0);
			glScaled(0.02, 0.02, 0.02);
			bathroom.Draw();
			glPopMatrix();

			//Draw Bathroom Model
			glPushMatrix();
			glTranslated(12, 0, 0);
			glScaled(0.02, 0.02, 0.02);
			bathroom.Draw();
			glPopMatrix();

			// Draw Tree Model
			glPushMatrix();
			glTranslatef(0, 8.5, -75);
			glScalef(6, 6, 6);
			maple_tree.Draw();
			glPopMatrix();

			//Draw House Model
			glPushMatrix();
			glTranslated(75, 0, 75);
			glScaled(0.05, 0.05, 0.05);
			house.Draw();
			glPopMatrix();


			// Draw Tree Model
			glPushMatrix();
			glTranslatef(75, 8.5, 0);
			glScalef(6, 6, 6);
			maple_tree.Draw();
			glPopMatrix();

			// Draw Swamp House Model
			glPushMatrix();
			glTranslated(75, 0, -75);
			swamp_house.Draw();
			glPopMatrix();

			//sky box
			glPushMatrix();
			GLUquadricObj * qobj;
			qobj = gluNewQuadric();
			glTranslated(0, 0, 0);
			glRotated(skyAngle, 0, 1, 0);
			glRotated(90, 1, 0, 1);
			glBindTexture(GL_TEXTURE_2D, tex);
			gluQuadricTexture(qobj, true);
			gluQuadricNormals(qobj, GL_SMOOTH);
			gluSphere(qobj, 200, 100, 100);
			gluDeleteQuadric(qobj);
			glPopMatrix();
			//glDisable(GL_TEXTURE_2D);
			break;
	case 2:
			glDisable(GL_LIGHTING);
			glLoadIdentity();
			gluLookAt(Eye.x, Eye.y, Eye.z, Eye.x, Eye.y, Eye.z - 10, Up.x, Up.y, Up.z);
			glPushMatrix();
			glTranslated(Eye.x - .4, Eye.y, Eye.z - 2);
			glColor3d(1, 0, 0);
			print(0, 0, "Congrats You Won :D Press Enter To Start Again");
			glPushMatrix();
			glScaled(1, 0.1, 1);
			glTranslated(.38, 0, -.5);
			glutSolidCube(1);
			glPopMatrix();
			glPopMatrix();
			glEnable(GL_LIGHTING);
			break;
	}

	glutSwapBuffers();
}

bool checkForCollision(double x, double z) {
	for (int i = 0; i < 23; i+=4)
	{
		double x1 = buildingsPos[i].x;
		double z1 = buildingsPos[i].z;
		double x2 = buildingsPos[i + 2].x;
		double z2 = buildingsPos[i + 2].z;

		double minx = min(x1, x2);
		double maxx = max(x1, x2);

		double minz = min(z1, z2);
		double maxz = max(z1, z2);

		if (x >= minx && x <= maxx && z >= minz && z <= maxz)return true;
	}
	return false;
}

void jump(){
	jumpFactor = 90; 
	jumpFlag = true;
}

void myKeyboard(unsigned char button, int x, int y)
{

	double prevX = Eye.x;
	double prevZ = Eye.z;

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
		if (Eye.z > 120)
			Eye.z = 120;
		if (Eye.z < -120)
			Eye.z = -120;
		if (Eye.x > 120)
			Eye.x = 120;
		if (Eye.x < -120)
			Eye.x = -120;
		if (playSound){
			PlaySoundA((LPCSTR) "player-footstep.wav", NULL, SND_FILENAME | SND_ASYNC);
			playSound = false;
			soundPlaying = 0;
		}	
		break;
	case  'a':
		Eye.x += cos(degToRad(180 - sideAngle));
		Eye.z += -sin(degToRad(180 - sideAngle));
		if (Eye.z > 120)
			Eye.z = 120;
		if (Eye.z < -120)
			Eye.z = -120;
		if (Eye.x > 120)
			Eye.x = 120;
		if (Eye.x < -120)
			Eye.x = -120;
		if (playSound){
			PlaySoundA((LPCSTR) "player-footstep.wav", NULL, SND_FILENAME | SND_ASYNC);
			playSound = false;
			soundPlaying = 0;
		}
		break;
	case  's':
		Eye.z += cos(degToRad(sideAngle));
		Eye.x += -sin(degToRad(sideAngle));
		if (Eye.z > 120)
			Eye.z = 120;
		if (Eye.z < -120)
			Eye.z = -120;
		if (Eye.x > 120)
			Eye.x = 120;
		if (Eye.x < -120)
			Eye.x = -120;
		if (playSound){
			PlaySoundA((LPCSTR) "player-footstep.wav", NULL, SND_FILENAME | SND_ASYNC);
			playSound = false;
			soundPlaying = 0;
		}
		break;
	case  'd':
		Eye.x += -cos(degToRad(180 - sideAngle));
		Eye.z += sin(degToRad(180 - sideAngle));
		if (Eye.z > 120)
			Eye.z = 120;
		if (Eye.z < -120)
			Eye.z = -120;
		if (Eye.x > 120)
			Eye.x = 120;
		if (Eye.x < -120)
			Eye.x = -120;
		if (playSound){
			PlaySoundA((LPCSTR) "player-footstep.wav", NULL, SND_FILENAME | SND_ASYNC);
			playSound = false;
			soundPlaying = 0;
		}
		break;
	case 27:
		exit(0);
		break; 
	case 32:
		if (!jumpFlag){
			if (playSound){
				PlaySoundA((LPCSTR) "player-footstep.wav", NULL, SND_FILENAME | SND_ASYNC);
				playSound = false;
				soundPlaying = 0;
			}
			jump();
		}
			break;
	case 13:
		startAgain();
		break;
	default:
			break;
	}

	if (checkForCollision(Eye.x, Eye.z)) {
		Eye.x = prevX;
		Eye.z = prevZ;
	}


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
				if (keyCount == keysAmount){
					gameMode = 2;
					PlaySoundA((LPCSTR) "success.wav", NULL, SND_FILENAME | SND_ASYNC);
				}
				else
					PlaySoundA((LPCSTR) "keys-collect.wav", NULL, SND_FILENAME | SND_ASYNC);
			}
		}
		for (int i = 0; i < batteriesAmount; i++)
		{
			if (Eye.x <= batteryLocations[i].x + 5 && Eye.x >= batteryLocations[i].x - 5
				&& Eye.z <= batteryLocations[i].z + 5 && Eye.z >= batteryLocations[i].z - 5
				&& batteriesTaken[i] == false) {
				batteriesTaken[i] = true;
				batteryLife += 50;
				if (batteryLife > 100)
					batteryLife = 100;
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
	asian_house.Load("Models/house6/house.3ds");
	barrels.Load("Models/barrel/barrel.3ds");
	tree_house.Load("Models/house/house.3ds");
	maple_tree.Load("Models/tree1/tree.3ds");
	bathroom.Load("Models/bathroom/bathroom.3ds");
	house.Load("Models/house4/house.3ds");
	swamp_house.Load("Models/house1/house.3ds");
	key.Load("Models/key/key.3ds");
	battery.Load("Models/battery/battery.3ds");
	tree.Load("Models/tree2/tree.3ds");
	// Loading texture files
	tex_ground.Load("Textures/grassground.bmp");
	tex_key.Load("Textures/gold.bmp");
	loadBMP(&tex, "Textures/night-sky.bmp", true);

}

void rotateCamera() {
	if (mouseXOld > (9 * WIDTH / 10))
		sideAngle += 3;
	if (mouseXOld < (WIDTH / 10))
		sideAngle -= 3;
	if (sideAngle > 360)
		sideAngle -= 360;
	if (sideAngle < -360)
		sideAngle += 360;
}

void anim() {
	if (soundPlaying == 7)
		playSound = true;
	else
		soundPlaying++;

	if (jumpFactor <= 270){
		Eye.y += sin(degToRad(jumpFactor));
		jumpFactor += 10;
		if (jumpFactor == 270)
			jumpFlag = false;
	}
	if (batteryLife > 0)
		batteryLife -= 0.1;
	else if (gameMode == 1){
		gameMode = 0;
		PlaySoundA((LPCSTR) "monster-rawr.wav", NULL, SND_FILENAME | SND_SYNC);
		PlaySoundA((LPCSTR) "monster-footsteps.wav", NULL, SND_FILENAME | SND_SYNC);
		PlaySoundA((LPCSTR) "monster-attack.wav", NULL, SND_FILENAME | SND_SYNC);

	}
	skyAngle += 0.1;
	rotateCamera();
	glutPostRedisplay();
}

void checkLookAtKey() {
	float x = Eye.x - At.x;
	float y = Eye.y - At.y;
	float z = Eye.z - At.z;
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

	glClearColor(1, 1, 1, 1);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);

	setUpCamera();
	LoadAssets();
	nightLights();

	glutMainLoop();
}