#include<GL/glut.h>
#include<GL/glu.h>
#include<GL/gl.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<GL/freeglut.h>
#include<AL/alut.h>
#include "imageloader.h"

float pillarSphereRadius = 3.0, pillarCylinderRadius = 2.0, pillarHeight = 20, pillarTriangle = 50; //pillar
float beadV = 0.05, time = 0, slAngle = 0; //car
float spAlpha = 0, spTheta = 0, spRadius = 150, R1 = 0, R2 = 0, R3 = 0, alpha = 0, copyAlpha = 0, xf = 0, yf = 0, xi = 0, yi = 0; //LookAt
float l1Angle = 0; //light 
float posiColor = 0;
float ambientLightIntensity = 0.1;
float fogIntensity = -50;
const float RADIUS = 4.0f; //The radius of the sphere

int smoothShadingFlag = 0;
int fogFlag = 0;
int flag = 0, flag1 = 0, flag2 = 1;
int rotatingTheta = 0;
int windmillAngle = 0;

bool _highShininess = false; //Whether the shininess parameter is high
bool _lowSpecularity = false; //Whether the specularity parameter is high
bool _emission = false; //Whether the emission parameter is turned on

void check(float, float);

GLuint loadTexture(Image* image) {
    GLuint textureId;
    glGenTextures(1, &textureId); //Make room for our texture
    glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
    //Map the image to the texture
    glTexImage2D(GL_TEXTURE_2D, //Always GL_TEXTURE_2D
            0, //0 for now
            GL_RGB, //Format OpenGL uses for image
            image->width, image->height, //Width and height
            0, //The border of the image
            GL_RGB, //GL_RGB, because pixels are stored in RGB format
            GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
            //as unsigned numbers
            image->pixels); //The actual pixel data
    return textureId; //Returns the id of the texture
}

GLuint _textureId[50]; //The id of the texture

class building {
public:
    float cx, cy, cz; //coordinates of the center of the floor of 1 storey structure
    /*	building(float x, float y, float z){
                    cx = x;
                    cy = y;
                    cz = z;
            }
     */ void draw(float x, float y, float z, int i, int j);
};

void building::draw(float x, float y, float z, int i, int j) {
    cx = x;
    cy = y;
    cz = z;
    glPushMatrix();
    glTranslatef(cx, cy, cz);
    glColor3f(1, 1, 1);

    //roof 
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId[j]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glTexCoord2f(0, 1);
    glVertex3f(-7.5, 15, -7.5);
    glTexCoord2f(1, 1);
    glVertex3f(7.5, 15, -7.5);
    glTexCoord2f(1, 0);
    glVertex3f(7.5, 15, 7.5);
    glTexCoord2f(0, 0);
    glVertex3f(-7.5, 15, 7.5);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //boundaries for the whole floor
    glLineWidth(2);
    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_LINE_LOOP);
    glVertex3f(-7.5, 0, -7.5);
    glVertex3f(7.5, 0, -7.5);
    glVertex3f(7.5, 0, 7.5);
    glVertex3f(-7.5, 0, 7.5);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex3f(-7.5, 15, -7.5);
    glVertex3f(7.5, 15, -7.5);
    glVertex3f(7.5, 15, 7.5);
    glVertex3f(-7.5, 15, 7.5);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(-7.5, 15, -7.5);
    glVertex3f(-7.5, 0, -7.5);
    glVertex3f(7.5, 15, -7.5);
    glVertex3f(7.5, 0, -7.5);
    glVertex3f(7.5, 15, 7.5);
    glVertex3f(7.5, 0, 7.5);
    glVertex3f(-7.5, 15, 7.5);
    glVertex3f(-7.5, 0, 7.5);
    glEnd();

    //windows on each wall..
    //-x facing..
    glPushMatrix();
    glTranslatef(-7.55, 7.5, 0);
    glBegin(GL_LINE_LOOP);
    glVertex3f(0, 3, 1.5);
    glVertex3f(0, 3, 4.5);
    glVertex3f(0, -3, 4.5);
    glVertex3f(0, -3, 1.5);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(0, 3, -1.5);
    glVertex3f(0, 3, -4.5);
    glVertex3f(0, -3, -4.5);
    glVertex3f(0, -3, -1.5);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(0, 2.5, 1.5);
    glVertex3f(0, 2.5, 4.5);
    glVertex3f(0, -2.5, 4.5);
    glVertex3f(0, -2.5, 1.5);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(0, 2.5, -1.5);
    glVertex3f(0, 2.5, -4.5);
    glVertex3f(0, -2.5, -4.5);
    glVertex3f(0, -2.5, -1.5);
    glEnd();


    glPopMatrix();
    //x facing..
    glPushMatrix();
    glTranslatef(7.55, 7.5, 0);
    glBegin(GL_LINE_LOOP);
    glVertex3f(0, 3, 1.5);
    glVertex3f(0, 3, 4.5);
    glVertex3f(0, -3, 4.5);
    glVertex3f(0, -3, 1.5);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(0, 3, -1.5);
    glVertex3f(0, 3, -4.5);
    glVertex3f(0, -3, -4.5);
    glVertex3f(0, -3, -1.5);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(0, 2.5, 1.5);
    glVertex3f(0, 2.5, 4.5);
    glVertex3f(0, -2.5, 4.5);
    glVertex3f(0, -2.5, 1.5);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(0, 2.5, -1.5);
    glVertex3f(0, 2.5, -4.5);
    glVertex3f(0, -2.5, -4.5);
    glVertex3f(0, -2.5, -1.5);
    glEnd();
    glPopMatrix();
    //-z facing..
    glPushMatrix();
    glTranslatef(0, 7.5, -7.55);
    glBegin(GL_LINE_LOOP);
    glVertex3f(1.5, 3, 0);
    glVertex3f(4.5, 3, 0);
    glVertex3f(4.5, -3, 0);
    glVertex3f(1.5, -3, 0);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(-1.5, 3, 0);
    glVertex3f(-4.5, 3, 0);
    glVertex3f(-4.5, -3, 0);
    glVertex3f(-1.5, -3, 0);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(1.5, 2.5, 0);
    glVertex3f(4.5, 2.5, 0);
    glVertex3f(4.5, -2.5, 0);
    glVertex3f(1.5, -2.5, 0);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(-1.5, 2.5, 0);
    glVertex3f(-4.5, 2.5, 0);
    glVertex3f(-4.5, -2.5, 0);
    glVertex3f(-1.5, -2.5, 0);
    glEnd();

    glPopMatrix();
    //z facing..
    glPushMatrix();
    glTranslatef(0, 7.5, 7.55);
    glBegin(GL_LINE_LOOP);
    glVertex3f(1.5, 3, 0);
    glVertex3f(4.5, 3, 0);
    glVertex3f(4.5, -3, 0);
    glVertex3f(1.5, -3, 0);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(-1.5, 3, 0);
    glVertex3f(-4.5, 3, 0);
    glVertex3f(-4.5, -3, 0);
    glVertex3f(-1.5, -3, 0);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(1.5, 2.5, 0);
    glVertex3f(4.5, 2.5, 0);
    glVertex3f(4.5, -2.5, 0);
    glVertex3f(1.5, -2.5, 0);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(-1.5, 2.5, 0);
    glVertex3f(-4.5, 2.5, 0);
    glVertex3f(-4.5, -2.5, 0);
    glVertex3f(-1.5, -2.5, 0);
    glEnd();
    glPopMatrix();


    glColor3f(1, 1, 1);
    glLineWidth(1);

    //floor
    glBegin(GL_QUADS);
    glNormal3f(0, -1, 0);
    glVertex3f(-7.5, 0, -7.5);
    glVertex3f(7.5, 0, -7.5);
    glVertex3f(7.5, 0, 7.5);
    glVertex3f(-7.5, 0, 7.5);
    glEnd();

    //left wall 
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId[i]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glTexCoord2f(1, 0);
    glVertex3f(-7.5, 0, -7.5);
    glTexCoord2f(1, 1);
    glVertex3f(-7.5, 15, -7.5);
    glTexCoord2f(0, 1);
    glVertex3f(-7.5, 15, 7.5);
    glTexCoord2f(0, 0);
    glVertex3f(-7.5, 0, 7.5);
    glEnd();

    //right wall

    glBegin(GL_QUADS);
    glNormal3f(1, 0, 0);
    glTexCoord2f(1, 0);
    glVertex3f(7.5, 0, -7.5);
    glTexCoord2f(1, 1);
    glVertex3f(7.5, 15, -7.5);
    glTexCoord2f(0, 1);
    glVertex3f(7.5, 15, 7.5);
    glTexCoord2f(0, 0);
    glVertex3f(7.5, 0, 7.5);
    glEnd();

    glBegin(GL_QUADS);
    glNormal3f(0, 0, -1);
    glTexCoord2f(0, 0);
    glVertex3f(-7.5, 0, -7.5);
    glTexCoord2f(0, 1);
    glVertex3f(-7.5, 15, -7.5);
    glTexCoord2f(1, 1);
    glVertex3f(7.5, 15, -7.5);
    glTexCoord2f(1, 0);
    glVertex3f(7.5, 0, -7.5);
    glEnd();

    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glTexCoord2f(0, 0);
    glVertex3f(-7.5, 0, 7.5);
    glTexCoord2f(0, 1);
    glVertex3f(-7.5, 15, 7.5);
    glTexCoord2f(1, 1);
    glVertex3f(7.5, 15, 7.5);
    glTexCoord2f(1, 0);
    glVertex3f(7.5, 0, 7.5);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

class hut {
public:
    float cx, cy, cz;
    void draw(float x, float y, float z, int wall, int roof, int win, int door);
};

void hut::draw(float x, float y, float z, int wall, int roof, int win, int door) {
    cx = x;
    cy = y;
    cz = z;

    glPushMatrix();
    glTranslatef(cx, cy, cz);
    glColor3f(1, 1, 1);
    //floor
    glBegin(GL_QUADS);
    glNormal3f(0, -1, 0);
    glVertex3f(-5, 0, -7);
    glVertex3f(5, 0, -7);
    glVertex3f(5, 0, 7);
    glVertex3f(-5, 0, 7);
    glEnd();
    //boundary	
    glLineWidth(2);
    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_LINE_LOOP);
    glVertex3f(-5, 0, -7);
    glVertex3f(5, 0, -7);
    glVertex3f(5, 0, 7);
    glVertex3f(-5, 0, 7);
    glEnd();
    glColor3f(1, 1, 1);
    glLineWidth(1);

    //left wall
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId[wall]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0, 0);
    glVertex3f(-5, 0, -7);
    glTexCoord2f(0, 1);
    glVertex3f(-5, 10, -7);
    glTexCoord2f(1, 1);
    glVertex3f(-5, 10, 7);
    glTexCoord2f(1, 0);
    glVertex3f(-5, 0, 7);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //boundary
    glLineWidth(2);
    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_LINE_LOOP);
    glVertex3f(-5, 0, -7);
    glVertex3f(-5, 10, -7);
    glVertex3f(-5, 10, 7);
    glVertex3f(-5, 0, 7);
    glEnd();
    //windows
    glBegin(GL_LINE_LOOP);
    glVertex3f(-5.5, 3, -5);
    glVertex3f(-5.5, 7.5, -5);
    glVertex3f(-5.5, 7.5, -2);
    glVertex3f(-5.5, 3, -2);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(-5.5, 3, 5);
    glVertex3f(-5.5, 7.5, 5);
    glVertex3f(-5.5, 7.5, 2);
    glVertex3f(-5.5, 3, 2);
    glEnd();
    glColor3f(1, 1, 1);
    glLineWidth(1);

    //right wall 
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId[wall]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBegin(GL_QUADS);
    glNormal3f(1, 0, 0);
    glTexCoord2f(1, 0);
    glVertex3f(5, 0, -7);
    glTexCoord2f(1, 1);
    glVertex3f(5, 10, -7);
    glTexCoord2f(0, 1);
    glVertex3f(5, 10, 7);
    glTexCoord2f(0, 0);
    glVertex3f(5, 0, 7);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //boundary
    glLineWidth(2);
    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_LINE_LOOP);
    glVertex3f(5, 0, -7);
    glVertex3f(5, 10, -7);
    glVertex3f(5, 10, 7);
    glVertex3f(5, 0, 7);
    glEnd();

    //windows
    glBegin(GL_LINE_LOOP);
    glVertex3f(5.5, 3, -5);
    glVertex3f(5.5, 7.5, -5);
    glVertex3f(5.5, 7.5, -2);
    glVertex3f(5.5, 3, -2);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(5.5, 3, 5);
    glVertex3f(5.5, 7.5, 5);
    glVertex3f(5.5, 7.5, 2);
    glVertex3f(5.5, 3, 2);
    glEnd();
    glColor3f(1, 1, 1);
    glLineWidth(1);


    //back window
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId[win]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, -1);
    glTexCoord2f(1, 0);
    glVertex3f(-5, 0, -7);
    glTexCoord2f(1, 1);
    glVertex3f(-5, 10, -7);
    //    glVertex3f(0, 15, -7);
    glTexCoord2f(0, 1);
    glVertex3f(5, 10, -7);
    glTexCoord2f(0, 0);
    glVertex3f(5, 0, -7);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //boundary
    glLineWidth(2);
    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_LINE_LOOP);
    glVertex3f(-5, 0, -7);
    glVertex3f(-5, 10, -7);
    glVertex3f(0, 15, -7);
    glVertex3f(5, 10, -7);
    glVertex3f(5, 0, -7);
    glEnd();
    glColor3f(1, 1, 1);
    glLineWidth(1);

    //front door
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId[door]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glTexCoord2f(0, 0);
    glVertex3f(-5, 0, 7);
    glTexCoord2f(0, 1);
    glVertex3f(-5, 10, 7);
    //  glVertex3f(0, 15, 7);
    glTexCoord2f(1, 1);
    glVertex3f(5, 10, 7);
    glTexCoord2f(1, 0);
    glVertex3f(5, 0, 7);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glColor3f(0, 0, 0);
    glBegin(GL_TRIANGLES);
    glNormal3f(0, 0, 1);
    glVertex3f(-5, 10, 7);
    glVertex3f(5, 10, 7);
    glVertex3f(0, 15, 7);
    glNormal3f(0, 0, -1);
    glVertex3f(-5, 10, -7);
    glVertex3f(5, 10, -7);
    glVertex3f(0, 15, -7);
    glEnd();

    //boundary
    glLineWidth(2);
    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_LINE_LOOP);
    glVertex3f(-5, 0, 7);
    glVertex3f(-5, 10, 7);
    glVertex3f(0, 15, 7);
    glVertex3f(5, 10, 7);
    glVertex3f(5, 0, 7);
    glEnd();

    /*    //window
        glBegin(GL_LINE_LOOP);
        glVertex3f(-1.5, 3, 7.5);
        glVertex3f(1.5, 3, 7.5);
        glVertex3f(1.5, 8, 7.5);
        glVertex3f(-1.5, 8, 7.5);
        glEnd();
        glColor3f(1, 1, 1);
        glLineWidth(1);
     **/

    //roof...
    //left panel
    glPushMatrix();
    glTranslatef(-5, 10, 0);
    glRotatef(45, 0, 0, 1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId[roof]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBegin(GL_QUADS);
    glNormal3f(1, 1, 0);
    glTexCoord2f(0, 0);
    glVertex3f(-1, 0, -7);
    glTexCoord2f(1, 0);
    glVertex3f(-1, 0, 7);
    glTexCoord2f(1, 1);
    glVertex3f(7.1, 0, 7);
    glTexCoord2f(0, 1);
    glVertex3f(7.1, 0, -7);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    //boundary
    glLineWidth(2);
    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_LINE_LOOP);
    glVertex3f(-1, 0, -7);
    glVertex3f(-1, 0, 7);
    glVertex3f(7.1, 0, 7);
    glVertex3f(7.1, 0, -7);
    glEnd();
    glColor3f(1, 1, 1);
    glLineWidth(1);

    glPopMatrix();
    //right panel
    glPushMatrix();
    glTranslatef(5, 10, 0);
    glRotatef(-45, 0, 0, 1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId[roof]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBegin(GL_QUADS);
    glNormal3f(-1, 1, 0);
    glTexCoord2f(1, 0);
    glVertex3f(1, 0, -7);
    glTexCoord2f(0, 0);
    glVertex3f(1, 0, 7);
    glTexCoord2f(0, 1);
    glVertex3f(-7.1, 0, 7);
    glTexCoord2f(1, 1);
    glVertex3f(-7.1, 0, -7);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    //boundary	
    glLineWidth(2);
    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_LINE_LOOP);
    glVertex3f(1, 0, -7);
    glVertex3f(1, 0, 7);
    glVertex3f(-7.1, 0, 7);
    glVertex3f(-7.1, 0, -7);
    glEnd();
    glColor3f(1, 1, 1);
    glLineWidth(1);

    glPopMatrix();


    glPopMatrix();

}

class windmill {
	public:
		float cx, cy, cz;
		void drawWindmill(float x, float y, float z, int angle);
};

void windmill::drawWindmill(float x, float y, float z, int angle) {
	cx = x;
	cy = y;
	cz = z;

	glPushMatrix();
	glTranslatef(cx, cy, cz);
        glRotatef(angle, 0, 1, 0);
	glColor3f(0.3, 0.3, 0.4);
	glLineWidth(3);

	//the tower..
	glBegin(GL_LINE_LOOP);
	glVertex3f(-5,0,-5);
	glVertex3f(5,0,-5);
	glVertex3f(5,0,5);
	glVertex3f(-5,0,5);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(-5,0,-5);
	glVertex3f(0,50,0);
	glVertex3f(5,0,-5);
	glVertex3f(0,50,0);
	glVertex3f(5,0,5);
	glVertex3f(0,50,0);
	glVertex3f(-5,0,5);
	glVertex3f(0,50,0);
	glEnd();
	for(int i=0;i<50;i+=5){
		glBegin(GL_LINE_LOOP);
		glVertex3f(-5+i*0.1,i,-5+i*0.1);
		glVertex3f(5-i*0.1,i,-5+i*0.1);
		glVertex3f(5-i*0.1,i,5-i*0.1);
		glVertex3f(-5+i*0.1,i,5-i*0.1);
		glEnd();
	}
	
	//the fan
	glPushMatrix();
	glTranslatef(0,50,0);
        glPushMatrix();
        glRotatef(-90,0,1,0);
        gluCylinder(gluNewQuadric(), 0.5, 0.5, 5, 100, 100);
        glPopMatrix();
	glTranslatef(-5,0,0);
        glRotatef(windmillAngle,1,0,0);
	glBegin(GL_LINES);
	glVertex3f(0,0,0);
	glVertex3f(0,5,0);
	glVertex3f(0,0,0);
	glVertex3f(0,-5,0);
	glVertex3f(0,0,0);
	glVertex3f(0,0,5);
	glVertex3f(0,0,0);
	glVertex3f(0,0,-5);
	glEnd();

	glBegin(GL_LINE_LOOP);
	for(int i=0;i<360;i++){
		glVertex3f(cos(i*3.1452/180),0,sin(i*3.1452/180));
	}
	glEnd();

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureId[25]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);
	glNormal3f(-1,0,0);
        glTexCoord2f(0,0);
        glVertex3f(0,5,0);
        glTexCoord2f(1,0);
	glVertex3f(0,5,5);
        glTexCoord2f(1,1);
	glVertex3f(0,25,5);
        glTexCoord2f(0,1);
	glVertex3f(0,25,0);
	glEnd();
        

	glBegin(GL_QUADS);
	glNormal3f(-1,0,0);
        glTexCoord2f(1,1);
	glVertex3f(0,-5,0);
        glTexCoord2f(0,1);
	glVertex3f(0,-5,-5);
        glTexCoord2f(0,0);
	glVertex3f(0,-25,-5);
        glTexCoord2f(1,0);
	glVertex3f(0,-25,0);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(-1,0,0);
        glTexCoord2f(0,1);
	glVertex3f(0,0,5);
        glTexCoord2f(0,0);
	glVertex3f(0,-5,5);
        glTexCoord2f(1,0);
	glVertex3f(0,-5,25);
        glTexCoord2f(1,1);
	glVertex3f(0,0,25);
	glEnd();
	
	glBegin(GL_QUADS);
	glNormal3f(-1,0,0);
        glTexCoord2f(1,0);
	glVertex3f(0,0,-5);
        glTexCoord2f(1,1);
	glVertex3f(0,5,-5);
        glTexCoord2f(0,1);
	glVertex3f(0,5,-25);
        glTexCoord2f(0,0);
	glVertex3f(0,0,-25);
	glEnd();
        glDisable(GL_TEXTURE_2D);
        
	glPopMatrix();
	
        windmillAngle++;

	glColor3f(1,1,1);
	glLineWidth(1);

	glPopMatrix();

}

class car {
public:
    float cx, cy, cz, steerAngle, theta, carV, wheelAngle, constant;
    int carFlag;

    car(float x, float y, float z, int carF) {
        cx = x, cy = y, cz = z, carFlag = carF;
        theta = 0, steerAngle = 0, carV = 0, wheelAngle = 0, constant = 0, carFlag = carF;

    }

    void drawCar(void);
    void drawWheel(float, float, float);
    void collisionDetect(void);
    void check(float, float);
};

void car::drawCar(void) {
    glPushMatrix();
    glTranslatef(cx, cy, cz);
    glRotatef(theta, 0, 1, 0);
    glColor3f(1, 1, 1);

    if (carFlag == 1) {
        //LEFT HEADLIGHT
        // set light position
        GLfloat lightPos1[] = {-2, 1.25, -5.01, 1.0};
        glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
        // set spot light parameters
        GLfloat spotDir1[] = {0.0, 0.0, -1.0}; // define spot direction
        //GLfloat spotDir[]={sin(rotatingTheta*3.14/180),0,cos(rotatingTheta*3.14/180)};            	// define spot direction
        GLfloat lightColor1[] = {0, 1, 1, 1.0f};
        glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDir1);
        glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 10.0); // set cutoff angle
        glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 10.0); // set focusing strength

        //RIGHT HEADLIGHT
        // set light position
        GLfloat lightPos2[] = {2, 1.25, -5.01, 1.0};
        glLightfv(GL_LIGHT2, GL_POSITION, lightPos2);
        // set spot light parameters
        GLfloat spotDir2[] = {0.0, 0.0, -1.0}; // define spot direction
        //GLfloat spotDir[]={sin(rotatingTheta*3.14/180),0,cos(rotatingTheta*3.14/180)};            	// define spot direction
        GLfloat lightColor2[] = {0, 1, 1, 1.0f};
        glLightfv(GL_LIGHT2, GL_DIFFUSE, lightColor2);
        glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotDir2);
        glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 10.0); // set cutoff angle
        glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 10.0); // set focusing strength
    } else if (carFlag == 2) {
        //LEFT HEADLIGHT
        // set light position
        GLfloat lightPos4[] = {-2, 1.25, -5.01, 1.0};
        glLightfv(GL_LIGHT1, GL_POSITION, lightPos4);
        // set spot light parameters
        GLfloat spotDir4[] = {0.0, 0.0, -1.0}; // define spot direction
        //GLfloat spotDir[]={sin(rotatingTheta*3.14/180),0,cos(rotatingTheta*3.14/180)};            	// define spot direction
        GLfloat lightColor4[] = {0, 1, 1, 1.0f};
        glLightfv(GL_LIGHT4, GL_DIFFUSE, lightColor4);
        glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, spotDir4);
        glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 10.0); // set cutoff angle
        glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 10.0); // set focusing strength

        //RIGHT HEADLIGHT
        // set light position
        GLfloat lightPos5[] = {2, 1.25, -5.01, 1.0};
        glLightfv(GL_LIGHT5, GL_POSITION, lightPos5);
        // set spot light parameters
        GLfloat spotDir5[] = {0.0, 0.0, -1.0}; // define spot direction
        //GLfloat spotDir[]={sin(rotatingTheta*3.14/180),0,cos(rotatingTheta*3.14/180)};            	// define spot direction
        GLfloat lightColor5[] = {0, 1, 1, 1.0f};
        glLightfv(GL_LIGHT5, GL_DIFFUSE, lightColor5);
        glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, spotDir5);
        glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 10.0); // set cutoff angle
        glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 10.0); // set focusing strength        
    }
    //front
    glBegin(GL_QUADS);
    glNormal3f(0, 0, -1);
    glVertex3f(-2.5, 0, -5);
    glVertex3f(-2.5, 2.50, -5);
    glVertex3f(2.5, 2.50, -5);
    glVertex3f(2.5, 0, -5);
    glEnd();

    //front upper
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glVertex3f(-2.5, 2.5, -5);
    glVertex3f(2.5, 2.5, -5);
    glVertex3f(2.5, 2.5, -3);
    glVertex3f(-2.5, 2.5, -3);
    glEnd();

    //front left
    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glVertex3f(-2.5, 0, -5);
    glVertex3f(-2.5, 2.5, -5);
    glVertex3f(-2.5, 2.5, -3);
    glVertex3f(-2.5, 0, -3);
    glEnd();

    //front right
    glBegin(GL_QUADS);
    glNormal3f(1, 0, 0);
    glVertex3f(2.5, 0, -5);
    glVertex3f(2.5, 2.5, -5);
    glVertex3f(2.5, 2.5, -3);
    glVertex3f(2.5, 0, -3);
    glEnd();

    //front window
    glLineWidth(2);
    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_LINE_LOOP);
    glVertex3f(-2.5, 2.5, -3);
    glVertex3f(2.5, 2.5, -3);
    glVertex3f(2.5, 5, -3);
    glVertex3f(-2.5, 5, -3);
    glEnd();
    glLineWidth(1);

    glColor3f(1, 1, 1);

    //upper part
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glVertex3f(-2.5, 5, 5);
    glVertex3f(-2.5, 5, -3);
    glVertex3f(2.5, 5, -3);
    glVertex3f(2.5, 5, 5);
    glEnd();

    //back
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glVertex3f(-2.5, 3, 5);
    glVertex3f(2.5, 3, 5);
    glVertex3f(2.5, 0, 5);
    glVertex3f(-2.5, 0, 5);
    glEnd();
    //back window
    glColor3f(0.3, 0.3, 0.3);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    glVertex3f(-2.5, 2.5, 5);
    glVertex3f(-2.5, 5, 5);
    glVertex3f(2.5, 5, 5);
    glVertex3f(2.5, 2.5, 5);
    glEnd();
    glLineWidth(1);
    glColor3f(1, 1, 1);

    //left
    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glVertex3f(-2.5, 2.5, 5);
    glVertex3f(-2.5, 2.5, -3);
    glVertex3f(-2.5, 0, -3);
    glVertex3f(-2.5, 0, 5);
    glEnd();
    //left window
    glColor3f(0.3, 0.3, 0.3);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    glVertex3f(-2.5, 2.5, -3);
    glVertex3f(-2.5, 5, -3);
    glVertex3f(-2.5, 5, 5);
    glVertex3f(-2.5, 2.5, 5);
    glEnd();
    glLineWidth(1);
    glColor3f(1, 1, 1);


    //right
    glBegin(GL_QUADS);
    glNormal3f(1, 0, 0);
    glVertex3f(2.5, 0, 5);
    glVertex3f(2.5, 0, -3);
    glVertex3f(2.5, 2.5, -3);
    glVertex3f(2.5, 2.5, 5);
    glEnd();
    //right window
    glColor3f(0.3, 0.3, 0.3);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    glVertex3f(2.5, 2.5, -3);
    glVertex3f(2.5, 5, -3);
    glVertex3f(2.5, 5, 5);
    glVertex3f(2.5, 2.5, 5);
    glEnd();
    glLineWidth(1);
    glColor3f(1, 1, 1);

    //bottom
    glBegin(GL_QUADS);
    glNormal3f(0, -1, 0);
    glVertex3f(-2.5, 0, 5);
    glVertex3f(2.5, 0, 5);
    glVertex3f(2.5, 0, -5);
    glVertex3f(-2.5, 0, -5);
    glEnd();



    //wheels
    glPushMatrix();

    //back left
    glColor3f(0.5, 0.5, 0.5);
    drawWheel(-2.52, 0, 4);

    //back right
    drawWheel(2.52, 0, 4);

    //front left
    glPushMatrix();
    glTranslatef(-2.51, 0, -4);
    glRotatef(steerAngle, 0, 1, 0);
    drawWheel(0, 0, 0);
    glPopMatrix();

    //front right
    glPushMatrix();
    glTranslatef(2.51, 0, -4);
    glRotatef(steerAngle, 0, 1, 0);
    drawWheel(0, 0, 0);
    glPopMatrix();

    glPopMatrix();

    //steering wheel
    glColor3f(0.5, 0.5, 0.5);
    glLineWidth(2);
    glBegin(GL_LINES);
    for (int i = 0; i < 360; i += 1) {
        glVertex3f(0.5 * cos(i * (3.14 / 180)), 2.5 + 0.5 * sin(i * (3.14 / 180)), -2.5);
    }
    glLineWidth(1.5);
    glEnd();
    glBegin(GL_LINES);
    glVertex3f(0.5 * cos((60 + steerAngle * 8)*(3.14 / 180)), 2.5 + 0.5 * sin((60 + steerAngle * 8)*(3.14 / 180)), -2.5);
    glVertex3f(0, 2.5, -2.5);
    glVertex3f(0.5 * cos((180 + steerAngle * 8)*(3.14 / 180)), 2.5 + 0.5 * sin((180 + steerAngle * 8)*(3.14 / 180)), -2.5);
    glVertex3f(0, 2.5, -2.5);
    glVertex3f(0.5 * cos((300 + steerAngle * 8)*(3.14 / 180)), 2.5 + 0.5 * sin((300 + steerAngle * 8)*(3.14 / 180)), -2.5);
    glVertex3f(0, 2.5, -2.5);
    glEnd();
    glLineWidth(1);
    glColor3f(1, 1, 1);

    //upper box
    //upper part
    glPushMatrix();
    glTranslatef(0 + 2 * sin(time), 0, 0);

    glColor3f(0, 0, 1);
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glVertex3f(-0.5, 6, -0.5);
    glVertex3f(0.5, 6, -0.5);
    glVertex3f(0.5, 6, 0.5);
    glVertex3f(-0.5, 6, 0.5);
    glEnd();

    //back
    glColor3f(1, 0, 1);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glVertex3f(-0.5, 6, 0.5);
    glVertex3f(0.5, 6, 0.5);
    glVertex3f(0.5, 5, 0.5);
    glVertex3f(-0.5, 5, 0.5);
    glEnd();

    //front
    glBegin(GL_QUADS);
    glNormal3f(0, 0, -1);
    glVertex3f(-0.5, 5, -0.5);
    glVertex3f(0.5, 5, -0.5);
    glVertex3f(0.5, 6, -0.5);
    glVertex3f(-0.5, 6, -0.5);
    glEnd();

    //left
    glColor3f(1, 1, 0);
    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glVertex3f(-0.5, 6, -0.5);
    glVertex3f(-0.5, 6, 0.5);
    glVertex3f(-0.5, 5, 0.5);
    glVertex3f(-0.5, 5, -0.5);
    glEnd();

    //right
    glBegin(GL_QUADS);
    glNormal3f(1, 0, 0);
    glVertex3f(0.5, 6, -0.5);
    glVertex3f(0.5, 6, 0.5);
    glVertex3f(0.5, 5, 0.5);
    glVertex3f(0.5, 5, -0.5);
    glEnd();

    //searchLight
    glPushMatrix();
    glTranslatef(0, 6 + 0.25 * cos(30 * (3.14 / 180)), 0);
    glRotatef(slAngle, 0, 1, 0);
    glRotatef(210, 1, 0, 0);
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        glVertex3f(0.25 * cos(i * 3.14 / 180), 0.25 * sin(i * 3.14 / 180), 0);
    }
    glEnd();
    gluCylinder(gluNewQuadric(), 0.25, 0.25, 1, 100, 100);
    if (carFlag == 1) {
        //SearchLIGHT
        GLfloat lightPos3[] = {0, 0, 0.1, 1.0};
        glLightfv(GL_LIGHT3, GL_POSITION, lightPos3);
        
        GLfloat spotDir3[] = {0.0, 0.0, 1.0}; 
        GLfloat lightColor3[] = {1, 1, 1, 1};
        glLightfv(GL_LIGHT3, GL_DIFFUSE, lightColor3);
        glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spotDir3);
        glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 10.0); // set cutoff angle
        glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 10.0); // set focusing strength
    } else if (carFlag == 2) {
        //Searchlight
        GLfloat lightPos6[] = {0, 0, 0.1, 1.0};
        glLightfv(GL_LIGHT6, GL_POSITION, lightPos6);
        
        GLfloat spotDir6[] = {0.0, 0.0, 1.0}; 
        GLfloat lightColor6[] = {1, 1, 1, 1};
        glLightfv(GL_LIGHT6, GL_DIFFUSE, lightColor6);
        glLightfv(GL_LIGHT6, GL_SPOT_DIRECTION, spotDir6);
        glLightf(GL_LIGHT6, GL_SPOT_CUTOFF, 10.0); // set cutoff angle
        glLightf(GL_LIGHT6, GL_SPOT_EXPONENT, 10.0); // set focusing strength
    }
    glPopMatrix();

    glPopMatrix();

    glPopMatrix();

    cz = cz - carV * cos(theta * (3.14 / 180));
    cx = cx - carV * sin(theta * (3.14 / 180));

    if (steerAngle > 0.3) {
        steerAngle -= 0.5;
    } else if (steerAngle < -0.3) {
        steerAngle += 0.5;
    } else {
        steerAngle = 0;
    }

    collisionDetect();
}

void car::drawWheel(float x, float y, float z) {
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i++) {
        glVertex3f(x, y + 1.5 * sin(i * (3.14 / 180)), z + 1.5 * cos(i * (3.14 / 180)));
    }
    glEnd();
    glLineWidth(1.5);
    glBegin(GL_LINES);
    glVertex3f(x, y + 1.5 * sin(90 * (3.14 / 180) + 0.01 * constant * carV / 0.3), z + 1.5 * cos(90 * (3.14 / 180) + 0.01 * constant * carV / 0.3));
    glVertex3f(x, y + 1.5 * sin(270 * (3.14 / 180) + 0.01 * constant * carV / 0.3), z + 1.5 * cos(270 * (3.14 / 180) + 0.01 * constant * carV / 0.3));
    glVertex3f(x, y + 1.5 * sin(180 * (3.14 / 180) + 0.01 * constant * carV / 0.3), z + 1.5 * cos(180 * (3.14 / 180) + 0.01 * constant * carV / 0.3));
    glVertex3f(x, y + 1.5 * sin(360 * (3.14 / 180) + 0.01 * constant * carV / 0.3), z + 1.5 * cos(360 * (3.14 / 180) + 0.01 * constant * carV / 0.3));
    glEnd();
    glLineWidth(1);

    constant++;
}

void car::collisionDetect(void) {
    for (float i = -2.5, j = 0; i <= 2.5; i += 0.1, j += 0.2) {
        check(cx + i * cos(theta * (3.14 / 180)) + 5 * sin(theta * (3.14 / 180)), cz + i * sin(theta * (3.14 / 180)) + 5 * cos(theta * (3.14 / 180)));
        check(cx + i * cos(theta * (3.14 / 180)) - 5 * sin(theta * (3.14 / 180)), cz + i * sin(theta * (3.14 / 180)) - 5 * cos(theta * (3.14 / 180)));
        check(cx + 2.5 * cos(theta * (3.14 / 180)) - j * sin(theta * (3.14 / 180)), cz - 2.5 * sin(theta * (3.14 / 180)) - j * cos(theta * (3.14 / 180)));
        check(cx - 2.5 * cos(theta * (3.14 / 180)) - j * sin(theta * (3.14 / 180)), cz + 2.5 * sin(theta * (3.14 / 180)) - j * cos(theta * (3.14 / 180)));
    }
}

void car::check(float x, float z) {
    float radius = 0;
    if (z >= 0)
        radius = sqrt(pow(x, 2) + pow(z - 50, 2));
    else
        radius = sqrt(pow(x, 2) + pow(z + 50, 2));

    if ((z <= 50 && z >= -50 && ((x >= 25 && x <= 55) || (x >= -55 && x <= -25))) || ((z >= -15 && z <= 15 && x >= -55 && x <= 55) && sqrt(pow(x, 2) + pow(z, 2)) >= 2.5)
            || ((z > 50 || z<-50) && (radius > 25 && radius < 55))) {
    } else {
        carV = 0;
    }

}

car a(45, 3, 0, 1);
car b(35, 3, 0, 2);

building f1[3];
building f2[2];
building f3[4];

hut h1;

windmill w1;

void drawSigns(float x, float y, float z, int flag) {

    glPushMatrix();
    if (flag == 1) {
        glTranslatef(x, y, z);
        glRotatef(180, 0, 1, 0);
        glTranslatef(-x, -y, -z);
    }
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_QUADS);
    glVertex3f(x + 0.5, y, z);
    glVertex3f(x + 1.5, y, z);
    glVertex3f(x + 1.5, y + 5, z);
    glVertex3f(x + 0.5, y + 5, z);
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId[4]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(x - 1, y + 5, z);
    glTexCoord2f(1, 0);
    glVertex3f(x + 3, y + 5, z);
    glTexCoord2f(1, 1);
    glVertex3f(x + 3, y + 10, z);
    glTexCoord2f(0, 1);
    glVertex3f(x - 1, y + 10, z);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glColor3f(0, 0, 0);

    glPopMatrix();
}

void initialize(void) {
   
    //sound
    ALuint Buffer, Source;
  
        alutInit (0, NULL);
        Buffer = alutCreateBufferFromFile("superbad.wav");
        alGenSources (1, &Source);
        alSourcei(Source, AL_BUFFER, Buffer);
        alSourcei(Source, AL_LOOPING, AL_TRUE);
        alSourcePlay (Source);
    
    
    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(120, 1, 0.01, 1000);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE); //Automatically normalize normals
    glEnable(GL_LIGHTING); //Enable lighting
    glEnable(GL_LIGHT0); //Enable light #0
    glEnable(GL_LIGHT1); //Enable light #1
    glEnable(GL_LIGHT2); //Enable light #2
    glEnable(GL_LIGHT3); //Enable light #2
    glEnable(GL_LIGHT4); //Enable light #2
    glEnable(GL_LIGHT5); //Enable light #2
    glEnable(GL_LIGHT6); //Enable light #2
    if (smoothShadingFlag) {
        glEnable(GL_SMOOTH);
    } else {
        glEnable(GL_FLAT);
    }
    if (fogFlag) {
        glEnable(GL_FOG);
        glClearColor(0.5, 0.5, 0.5, 1);
    }
    
    Image* image = loadBMP("./world/f.bmp");
    _textureId[0] = loadTexture(image);
    image = loadBMP("./world/lr.bmp");
    _textureId[1] = loadTexture(image);
    image = loadBMP("./world/b.bmp");
    _textureId[2] = loadTexture(image);
    image = loadBMP("./world/g.bmp");
    _textureId[3] = loadTexture(image);
    image = loadBMP("./world/rightTurn.bmp");
    _textureId[4] = loadTexture(image);

    //building texture
    image = loadBMP("./building/w1.bmp"); //w1
    _textureId[11] = loadTexture(image);
    image = loadBMP("./building/r1.bmp"); //r1
    _textureId[12] = loadTexture(image);
    image = loadBMP("./building/w2.bmp"); //w2
    _textureId[13] = loadTexture(image);
    image = loadBMP("./building/w3.bmp"); //w3
    _textureId[14] = loadTexture(image);

    //hut
    image = loadBMP("./hut/s1.bmp"); //s1
    _textureId[21] = loadTexture(image);
    image = loadBMP("./hut/s2.bmp"); //s2
    _textureId[22] = loadTexture(image);
    //   image = loadBMP("./hut/r1.bmp");       //r1
    //    _textureId[25] = loadTexture(image);
    image = loadBMP("./hut/r2.bmp"); //r2
    _textureId[25] = loadTexture(image);
    image = loadBMP("./hut/w1.bmp"); //w1
    _textureId[31] = loadTexture(image);
    image = loadBMP("./hut/w2.bmp"); //w2
    _textureId[32] = loadTexture(image);
    image = loadBMP("./hut/w3.bmp"); //w3
    _textureId[33] = loadTexture(image);
    image = loadBMP("./hut/w4.bmp"); //w4
    _textureId[34] = loadTexture(image);
    image = loadBMP("./hut/d1.bmp"); //d1
    _textureId[35] = loadTexture(image);
    image = loadBMP("./hut/d2.bmp"); //d2
    _textureId[36] = loadTexture(image);

    delete image;
    //loadTexture(0,"rightTurn.bmp",texture_id);
    //loadTexture(1,"sun.bmp",texture_id);
    //	loadTexture(2,"key.bmp",texture_id);
    //	loadTexture(3,"base.bmp",texture_id);

}

void light(void) {
    //Add ambient light
    GLfloat ambientColor[] = {ambientLightIntensity, ambientLightIntensity, ambientLightIntensity, 1.0f}; //Color (0.2, 0.2, 0.2)
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
      
//	GLfloat lightColor[] = {0.6f, 0.6f, 0.6f, 1.0f};
	GLfloat lightColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat lightPos[] = {cos(l1Angle * 3.14 / 180), sin(l1Angle * 3.14 / 180), 1, 1.0f};
	//Diffuse (non-shiny) light component
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	//Specular (shiny) light component
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    if (posiColor >= 1) {
        posiColor = 0;
    }

}

void drawPillar(void) {

    float specularity;
	if (_lowSpecularity) {
		specularity = 0.3f;
	}
	else {
		specularity = 1;
	}
	
	float emissivity;
	if (_emission) {
		emissivity = 0.05f;
	}
	else {
		emissivity = 0;
	}
	
	float shininess;
	if (_highShininess) {
		shininess = 25;
	}
	else {
		shininess = 12;
	}
        
        //The color of the sphere
	GLfloat materialColor[] = {0.2f, 0.2f, 1.0f, 1.0f};
	//The specular (shiny) component of the material
	GLfloat materialSpecular[] = {specularity, specularity, specularity, 1.0f};
	//The color emitted by the material
	GLfloat materialEmission[] = {emissivity, emissivity, emissivity, 1.0f};
	
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess); //The shininess parameter
        
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    gluCylinder(gluNewQuadric(), pillarCylinderRadius, pillarCylinderRadius, pillarHeight, pillarTriangle, pillarTriangle);
    glRotatef(90, 1, 0, 0);
    glTranslatef(0, pillarHeight + pillarSphereRadius * cos(asin(pillarCylinderRadius / pillarSphereRadius)), 0);
    
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess); //The shininess parameter
        
    gluSphere(gluNewQuadric(), pillarSphereRadius, pillarTriangle, pillarTriangle);
    glPopMatrix();
}

void pc(void) {
    glClearColor(0, 0, 0, 1);
    light();

    a.drawCar();
    b.drawCar();

    for (int i = 0; i < 3; i++) {
        f1[i].draw(70, i * 15, 10, 11, 12);
    }
    for (int i = 0; i < 2; i++) {
        f2[i].draw(10, i * 15, 35, 13, 12);
    }
    for (int i = 0; i < 4; i++) {
        f3[i].draw(-70, i * 15, -35, 14, 12);
    }
    //21-22,25,31-34,35-36
    h1.draw(10, 0, -35, 21, 25, 31, 35);
    h1.draw(150, 0, -50, 21, 25, 32, 36);
    h1.draw(-10, 0, 150, 22, 25, 33, 36);
    h1.draw(-50, 0, -150, 21, 25, 34, 35);
    h1.draw(100, 0, 80, 22, 25, 33, 36);
    
    w1.drawWindmill(180,0.5,-180, 90);
    w1.drawWindmill(180,0.5,-150, 90);
    w1.drawWindmill(180,0.5,-120, 90);

    w1.drawWindmill(150,0.5,-180, 90);
    w1.drawWindmill(150,0.5,-150, 90);
    w1.drawWindmill(150,0.5,-120, 90);
    
    w1.drawWindmill(120,0.5,-180, 90);
    w1.drawWindmill(120,0.5,-150, 90);
    w1.drawWindmill(120,0.5,-120, 90);    
    if (fogFlag) {
        //GLfloat fogColor[] = {0.5f, 0.5f, 0.5f, 1};
        GLfloat fogColor[] = {0.5f, 0.5f, 0.5f, 1};
        glFogfv(GL_FOG_COLOR, fogColor);
        glFogi(GL_FOG_MODE, GL_LINEAR);
        glFogf(GL_FOG_START, fogIntensity);
        glFogf(GL_FOG_END, 200.0f);
    }

    //world
    //ground
    glColor3f(0, 1, 0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId[3]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glTexCoord2f(0, 0);
    glVertex3f(-200, -0.4, 200);
    glTexCoord2f(0, 1);
    glVertex3f(-200, -0.4, -200);
    glTexCoord2f(1, 1);
    glVertex3f(200, -0.4, -200);
    glTexCoord2f(1, 0);
    glVertex3f(200, -0.4, 200);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //left wall
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0, 1);
    glVertex3f(-200, 200, 200);
    glTexCoord2f(1, 1);
    glVertex3f(-200, 200, -200);
    glTexCoord2f(1, 0);
    glVertex3f(-200, 0, -200);
    glTexCoord2f(0, 0);
    glVertex3f(-200, 0, 200);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //right wall
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glNormal3f(1, 0, 0);
    glTexCoord2f(0, 1);
    glVertex3f(200, 200, 200);
    glTexCoord2f(1, 1);
    glVertex3f(200, 200, -200);
    glTexCoord2f(1, 0);
    glVertex3f(200, 0, -200);
    glTexCoord2f(0, 0);
    glVertex3f(200, 0, 200);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //front wall
    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glTexCoord2f(1, 1);
    glVertex3f(-200, 200, 200);
    glTexCoord2f(0, 1);
    glVertex3f(200, 200, 200);
    glTexCoord2f(0, 0);
    glVertex3f(200, 0, 200);
    glTexCoord2f(1, 0);
    glVertex3f(-200, 0, 200);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //back wall
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, -1);
    glTexCoord2f(0, 1);
    glVertex3f(-200, 200, -200);
    glTexCoord2f(1, 1);
    glVertex3f(200, 200, -200);
    glTexCoord2f(1, 0);
    glVertex3f(200, 0, -200);
    glTexCoord2f(0, 0);
    glVertex3f(-200, 0, -200);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    /*	//roof
            glColor3f(0,0,1);
            glBegin(GL_QUADS);
            glVertex3f(-200,200,200);
            glVertex3f(-200,200,-200);
            glVertex3f(200,200,-200);
            glVertex3f(200,200,200);
            glEnd();  */

    //track
    glColor3f(0, 1, 1);
    //
    //
    glBegin(GL_QUADS);
  //  glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
    glNormal3f(0, 1, 0);
    glVertex3f(25, 0, 50);
    glVertex3f(25, 0, -50);
    glVertex3f(55, 0, -50);
    glVertex3f(55, 0, 50);
    glEnd();

    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glVertex3f(-25, 0, 50);
    glVertex3f(-25, 0, -50);
    glVertex3f(-55, 0, -50);
    glVertex3f(-55, 0, 50);
    glEnd();

    //join track
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glVertex3f(25, 0, 15);
    glVertex3f(25, 0, -15);
    glVertex3f(-25, 0, -15);
    glVertex3f(-25, 0, 15);
    glEnd();

    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    glTranslatef(0, 50, 0);
    glNormal3f(0, 1, 0);
    gluPartialDisk(gluNewQuadric(), 25, 55, 100, 100, -90, 180);
    glPopMatrix();

    glPushMatrix();
    glRotatef(-270, 1, 0, 0);
    glTranslatef(0, 50, 0);
    glNormal3f(0, 1, 0);
    gluPartialDisk(gluNewQuadric(), 25, 55, 100, 100, -90, 180);
    glPopMatrix();

    //inner track
    glColor3f(0, 1, 1);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex3f(-25, 1, -15);
    glVertex3f(25, 1, -15);
    glVertex3f(25, 1, -15);
    glVertex3f(25, 1, -50);
    for (int i = 180; i < 360; i++) {
        glVertex3f(25 * cos(i * (3.14 / 180)), 1, -50 + 25 * sin(i * (3.14 / 180)));
    }

    glVertex3f(-25, 1, -50);
    glVertex3f(-25, 1, -15);
    glEnd();

    glBegin(GL_LINES);
    glVertex3f(25, 0, 50);
    glVertex3f(25, 0, 15);
    glVertex3f(25, 0, 15);
    glVertex3f(-25, 0, 15);
    glVertex3f(-25, 0, 15);
    glVertex3f(-25, 0, 50);
    for (int i = 0; i < 180; i++) {
        glVertex3f(25 * cos(i * (3.14 / 180)), 1, 50 + 25 * sin(i * (3.14 / 180)));
    }
    glEnd();

    //inner fencing
    glColor3f(0.5, 0.5, 0.5);
    for (int i = 50; i > 15; i -= 2) {
        glBegin(GL_QUADS);
        glNormal3f(1, 0, 0);
        glVertex3f(25, 1, i);
        glVertex3f(25, 1, i - 1);
        glVertex3f(25, 5, i - 1);
        glVertex3f(25, 5, i);
        glEnd();

        glBegin(GL_QUADS);
        glNormal3f(-1, 0, 0);
        glVertex3f(-25, 1, i);
        glVertex3f(-25, 1, i - 1);
        glVertex3f(-25, 5, i - 1);
        glVertex3f(-25, 5, i);
        glEnd();
    }

    for (int i = -15; i > -50; i -= 2) {
        glBegin(GL_QUADS);
        glNormal3f(1, 0, 0);
        glVertex3f(25, 1, i);
        glVertex3f(25, 1, i - 1);
        glVertex3f(25, 5, i - 1);
        glVertex3f(25, 5, i);
        glEnd();

        glBegin(GL_QUADS);
        glNormal3f(-1, 0, 0);
        glVertex3f(-25, 1, i);
        glVertex3f(-25, 1, i - 1);
        glVertex3f(-25, 5, i - 1);
        glVertex3f(-25, 5, i);
        glEnd();
    }

    for (int i = 25; i > -25; i -= 2) {
        glBegin(GL_QUADS);
        glNormal3f(0, 0, 1);
        glVertex3f(i, 1, -15);
        glVertex3f(i - 1, 1, -15);
        glVertex3f(i - 1, 5, -15);
        glVertex3f(i, 5, -15);
        glEnd();

        glBegin(GL_QUADS);
        glNormal3f(0, 0, -1);
        glVertex3f(i, 1, 15);
        glVertex3f(i - 1, 1, 15);
        glVertex3f(i - 1, 5, 15);
        glVertex3f(i, 5, 15);
        glEnd();
    }

    for (int i = 180; i < 360; i += 2) {
        glBegin(GL_QUADS);
        glNormal3f(abs(cos((i - 180)*3.14 / 180)), 1, -sin(i - 180)*3.14 / 180);
        glVertex3f(25 * cos(i * (3.14 / 180)), 1, -50 + 25 * sin(i * (3.14 / 180)));
        glVertex3f(25 * cos((i + 1) * (3.14 / 180)), 1, -50 + 25 * sin((i + 1) * (3.14 / 180)));
        glVertex3f(25 * cos((i + 1) * (3.14 / 180)), 5, -50 + 25 * sin((i + 1) * (3.14 / 180)));
        glVertex3f(25 * cos(i * (3.14 / 180)), 5, -50 + 25 * sin(i * (3.14 / 180)));
        glEnd();
    }

    for (int i = 0; i < 180; i += 2) {
        glBegin(GL_QUADS);
        glNormal3f(abs(cos((i - 180)*3.14 / 180)), 1, -sin(i - 180)*3.14 / 180);
        glVertex3f(25 * cos(i * (3.14 / 180)), 1, 50 + 25 * sin(i * (3.14 / 180)));
        glVertex3f(25 * cos((i + 1) * (3.14 / 180)), 1, 50 + 25 * sin((i + 1) * (3.14 / 180)));
        glVertex3f(25 * cos((i + 1) * (3.14 / 180)), 5, 50 + 25 * sin((i + 1) * (3.14 / 180)));
        glVertex3f(25 * cos(i * (3.14 / 180)), 5, 50 + 25 * sin(i * (3.14 / 180)));
        glEnd();
    }

    //outer track
    glColor3f(0, 1, 1);
    glBegin(GL_LINES);
    glVertex3f(55, 1, 50);
    glVertex3f(55, 1, -50);
    for (int i = 180; i < 360; i++) {
        glVertex3f(55 * cos(i * (3.14 / 180)), 1, -50 + 55 * sin(i * (3.14 / 180)));
    }
    glVertex3f(-55, 1, -50);
    glVertex3f(-55, 1, 50);
    for (int i = 0; i < 180; i++) {
        glVertex3f(55 * cos(i * (3.14 / 180)), 1, 50 + 55 * sin(i * (3.14 / 180)));
    }
    glEnd();
    glLineWidth(1);

    //outer fencing
    glColor3f(0.5, 0.5, 0.5);
    for (int i = 50; i > -55; i -= 2) {
        glBegin(GL_QUADS);
        glNormal3f(-1, 0, 0);
        glVertex3f(55, 1, i);
        glVertex3f(55, 1, i - 1);
        glVertex3f(55, 5, i - 1);
        glVertex3f(55, 5, i);
        glEnd();
    }

    for (int i = 50; i > -55; i -= 2) {
        glBegin(GL_QUADS);
        glNormal3f(1, 0, 0);
        glVertex3f(-55, 1, i);
        glVertex3f(-55, 1, i - 1);
        glVertex3f(-55, 5, i - 1);
        glVertex3f(-55, 5, i);
        glEnd();
    }

    for (int i = 180; i < 360; i += 2) {
        glBegin(GL_QUADS);
        glNormal3f(-abs(cos((i - 180)*3.14 / 180)), 1, sin(i - 180)*3.14 / 180);
        glVertex3f(55 * cos(i * (3.14 / 180)), 1, -50 + 55 * sin(i * (3.14 / 180)));
        glVertex3f(55 * cos((i + 1) * (3.14 / 180)), 1, -50 + 55 * sin((i + 1) * (3.14 / 180)));
        glVertex3f(55 * cos((i + 1) * (3.14 / 180)), 5, -50 + 55 * sin((i + 1) * (3.14 / 180)));
        glVertex3f(55 * cos(i * (3.14 / 180)), 5, -50 + 55 * sin(i * (3.14 / 180)));
        glEnd();
    }

    for (int i = 0; i < 180; i += 2) {
        glBegin(GL_QUADS);
        glNormal3f(abs(cos((i - 180)*3.14 / 180)), 1, -sin(i - 180)*3.14 / 180);
        glVertex3f(55 * cos(i * (3.14 / 180)), 1, 50 + 55 * sin(i * (3.14 / 180)));
        glVertex3f(55 * cos((i + 1) * (3.14 / 180)), 1, 50 + 55 * sin((i + 1) * (3.14 / 180)));
        glVertex3f(55 * cos((i + 1) * (3.14 / 180)), 5, 50 + 55 * sin((i + 1) * (3.14 / 180)));
        glVertex3f(55 * cos(i * (3.14 / 180)), 5, 50 + 55 * sin(i * (3.14 / 180)));
        glEnd();
    }

    drawPillar();

    drawSigns(60, 0, -50, 0);
    drawSigns(60, 0, -45, 0);
    drawSigns(60, 0, -40, 0);
    drawSigns(60, 0, -55, 0);
    drawSigns(60, 0, -60, 0);
    drawSigns(-60, 0, 40, 1);
    drawSigns(-60, 0, 45, 1);
    drawSigns(-60, 0, 50, 1);
    drawSigns(-60, 0, 55, 1);
    drawSigns(-60, 0, 60, 1);

    time += 0.01;

    if (slAngle == 360) {
        slAngle = 0;
    }
    slAngle += 1;

}

void draw(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();


    if (flag == 0) {
        gluLookAt(a.cx - 1 * sin(a.theta * (3.14 / 180)), 6.5, a.cz - 1 * cos(a.theta * (3.14 / 180)), a.cx - sin(a.theta * (3.14 / 180)) - 4 * sin(a.theta * (3.14 / 180)), 5.5, a.cz - 5 * cos(a.theta * (3.14 / 180)), 0, 1, 0);
    }//rooftop view
    else if (flag == 1) {
        gluLookAt(a.cx + 5 * sin(a.theta * (3.14 / 180)), 10, a.cz + 5 * cos(a.theta * (3.14 / 180)), a.cx, 10, a.cz, 0, 1, 0);
    }//sliding box view
    else if (flag == 2) {
        gluLookAt(a.cx + 4 * sin(a.theta * (3.14 / 180)) + 2 * sin(time) * cos(a.theta * (3.14 / 180)), 10, 4 * cos(a.theta * (3.14 / 180)) + a.cz - 2 * sin(time) * sin(a.theta * (3.14 / 180)), a.cx + 2 * sin(time) * cos(a.theta * (3.14 / 180)), 10, a.cz - 2 * sin(time) * sin(a.theta * (3.14 / 180)), 0, 1, 0);
    }//search light view
    else if (flag == 3) {
        gluLookAt(a.cx + 2 * sin(time) * cos(a.theta * (3.14 / 180)), 9 + 0.25 * cos(30 * (3.14 / 180)), a.cz - 2 * sin(time) * sin(a.theta * (3.14 / 180)),
                a.cx - sin((a.theta + slAngle)*(3.14 / 180)) + 2 * sin(time) * cos(a.theta * (3.14 / 180)), 9 + 0.25 * cos(30 * (3.14 / 180)) + 1 * tan(30 * (3.14 / 180)), a.cz - cos((a.theta + slAngle)*(3.14 / 180)) - 2 * sin(time) * sin(a.theta * (3.14 / 180)), 0, 1, 0);
    }//wheel view
    else if (flag == 4) {
        gluLookAt(a.cx - 4 * sin(a.theta * (3.14 / 180)), 5.5, 0 + a.cz - 4 * cos(a.theta * (3.14 / 180)),
                a.cx - 4 * sin((a.theta)*(3.14 / 180)) - 1 * sin((a.theta + a.steerAngle / 4.0)*(3.14 / 180)), 5.5, 0 + a.cz - 4 * cos((a.theta)*(3.14 / 180)) - 1 * cos((a.theta + a.steerAngle / 4.0)*(3.14 / 180)), 0, 1, 0);
    }//overhead view
    else if (flag == 5) {
        gluLookAt(a.cx + 10 * sin(a.theta * (3.14 / 180)), 15, 0 + a.cz + 10 * cos(a.theta * (3.14 / 180)), a.cx - sin(a.theta * (3.14 / 180)), 10, 0 + a.cz - cos(a.theta * (3.14 / 180)), 0, 1, 0);
    }//helicopter view
    else if (flag == 6) {
        gluLookAt(0 + spRadius * cos(spAlpha * (3.14 / 180)) * sin(spTheta * (3.14 / 180)), 0 + spRadius * sin(spAlpha * (3.14 / 180)), 0 + spRadius * cos(spAlpha * (3.14 / 180)) * cos(spTheta * (3.14 / 180)), 0, 0, 0, 0, 1, 0);
    } else if (flag == 7) {
        gluLookAt(0, 100, 0, 0, 0, 0, 0, 0, -1);
    } else if (flag == 7) {
        gluLookAt(0, 100, 0, 0, 0, 0, 0, 0, -1);
    }


    pc();
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x1, int y1) {

    if (key == '1') {
        flag = 0;
    } else if (key == '2') {
        flag = 1;
    } else if (key == '3') {
        flag = 2;
    } else if (key == '4') {
        flag = 3;
    } else if (key == '5') {
        flag = 4;
    } else if (key == '6') {
        flag = 5;
    } else if (key == '7') {
        flag = 6;
    } else if (key == '8') {
        flag = 7;
    }
    else if (key == 'w') {
        if (b.carV < 0.9)
            b.carV += 0.01;
    } else if (key == 'a') {
        if (b.steerAngle < 45)
            b.steerAngle++;
        b.wheelAngle++;
        if (b.carV >= 0)
            b.theta += 2;
        else {
            b.theta -= 2;
        }
    } else if (key == 'd') {
        if (b.steerAngle > -45)
            b.steerAngle--;
        b.wheelAngle--;
        if (b.carV >= 0)
            b.theta -= 2;
        else
            b.theta += 2;
    } else if (key == 's') {
        if (b.carV>-0.1)
            b.carV -= 0.01;
    } else if (key == 'q') {
        l1Angle += 5;
    } else if (key == 'e') {
        l1Angle -= 5;
    } else if (key == '+') {
        if (ambientLightIntensity < 1) {
            ambientLightIntensity += 0.1;
        }
    } else if (key == '-') {
        if (ambientLightIntensity > 0.1)
            ambientLightIntensity -= 0.1;
    } else if (key == 'f') {
        if (!smoothShadingFlag) {
            smoothShadingFlag = 1;
            glEnable(GL_SMOOTH);
        } else {
            smoothShadingFlag = 0;
            glEnable(GL_FLAT);
        }
    } else if (key == 'o') {
        if (fogFlag == 0) {
            fogFlag = 1;
            glEnable(GL_FOG);
        } else {
            fogFlag = 0;
            glDisable(GL_FOG);
        }
    } else if (key == 'i') {
        if (fogIntensity > -200) {
            fogIntensity -= 20;
        }
    } else if (key == 'p') {
        if (fogIntensity < -50) {
            fogIntensity += 20;
        }
    } else if (key == 'z') {
        if (pillarTriangle < 100) {
            pillarTriangle += 5;
        }
    } else if (key == 'x') {
        if (pillarTriangle > 5) {
            pillarTriangle -= 5;
        }
    } else if (key == 27) { //Escape key
        exit(0);
    } else if (key == 'c') {
        _highShininess = !_highShininess;
    } else if (key == 'v') {
        _lowSpecularity = !_lowSpecularity;
    } else if (key == 'b') {
        _emission = !_emission;
    } 

}

void SpecialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {
        if (a.carV < 0.9)
            a.carV += 0.01;
    } else if (key == GLUT_KEY_LEFT) {
        if (a.steerAngle < 45)
            a.steerAngle++;
        a.wheelAngle++;
        if (a.carV >= 0)
            a.theta += 2;
        else {
            a.theta -= 2;
        }
    } else if (key == GLUT_KEY_RIGHT) {
        if (a.steerAngle > -45)
            a.steerAngle--;
        a.wheelAngle--;
        if (a.carV >= 0)
            a.theta -= 2;
        else
            a.theta += 2;
    } else if (key == GLUT_KEY_DOWN) {
        if (a.carV>-0.1)
            a.carV -= 0.01;
    }
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN)
            flag1 = 1;
        else if (state == GLUT_UP) {
            flag1 = 0;
            flag2 = 1;
        }
    } else if (button == 3) {
        if (spRadius < 250)
            spRadius++;
    } else if (button == 4) {
        if (spRadius > 10)
            spRadius--;
    }

}

void motion(int x, int y) {
    xf = x;
    yf = y;
    if (flag2 == 1) {
        xi = x;
        yi = y;
    }
    if (flag1 && flag == 6) {
        spTheta += (xf - xi) / 10.0;
        spAlpha -= (yf - yi) / 10.0;
        if (spAlpha <= -90 || spAlpha >= 90)
            spAlpha += (yf - yi) / 10.0;
    }
    xi = x;
    yi = y;
    flag2 = 0;

}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(1, 1);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutCreateWindow("PC");
    glutDisplayFunc(draw);
    glutIdleFunc(draw);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutSpecialFunc(SpecialKeys);
    glutFullScreen();
    initialize();
    glutMainLoop();
    return 0;
}
