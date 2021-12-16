#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <GL/glut.h>
#include <iostream>
#include "cmath"
#include "imageloader.h"
#include "glm.h"

static int  handsRotation = 0, shoulder = 0, elbow = 0, elbow2 = 0, down_Leg1Part = 0, leg1_Yrotation = 0,
down_Leg2Part = 0, leg_Zrotation = 0, leg2_Yrotation = 0, f1base = 0, f1up = 0, f2base = 0, f2up = 0, frame = 0,
moving, startx, starty, zoom = 100, ball_rotationX = 0, obj1_x = 10, obj1_y = -5, obj1_z = 10, obj2_x = -10, obj2_y = 0, obj2_z = 10,
obj1_scalex = 40, obj1_scaley = 40, obj1_scalez = 40, obj2_scalex = 50, obj2_scaley = 100, obj2_scalez = 50, obj_Rotation = 180, body_Ztranslation = 0;


double center[] = { 0,0,0 }, look[3], eye[] = { 0,0,-3.0 }, up[] = { 0, 1.0, 0 }, Cross[3], body_Xtranslation = 0;

Image* image;
Image* image2;
// RGBA
GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat light_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
// x , y, z, w
GLfloat light_position[] = { 0.5, 100.0, 2.0, 1.0 };
GLfloat lightPos1[] = { 0, 0.0, -2.0, 1.0 };

GLfloat angle = 0.0;   /* in degrees */
GLfloat angle2 = 0.0;   /* in degrees */
GLfloat cameraRotation = 0.1;   /* in degrees */
const char* modelname = "data/snowman_finish.obj";
const char* modelname2 = "data/Tree2.obj";

bool animation1 = true;
bool animation2 = true;
bool animation3 = true;

int animate1[10][9] = {
    {0,0,0,0,0,-40,20,0,0},
    {0,0,-20,0,0,-40,0,0,0},
    {0,-2,-20,0,-10,-25,0,0,0},
    {0,-2,-20,0,-20,-15,0,0,0},
    {0,-2,15,0,-20,0,0,0,0},
    {0,-2,40,0,-20,20,0,0,0},
    {0,-2,40,0,0,20,10,0,0},
    {0,-4,25,0,0,20,20,0,0},
    {0,-4,25,0,0,-15,20,0,0},
    {0,-4,15,0,0,0,0,0,0}
};

int animate2[10][9] = {
    {30,0,0,5,0,0,0,0,0},
    {40,0,0,15,0,0,0,1,0},
    {90,0,0,30,0,0,0,2,0},
    {90,0,0,30,0,0,0,1,0},
    {90,0,0,30,0,0,0,0,0},
    {60,0,0,15,0,0,0,1,0},
    {30,0,0,0,0,0,0,2,0},
    {0,0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0}
};

int animate3[10][9] = {
    {0,0,-20,0,0,0,0,0,0},
    {0,0,-20,0,-10,0,0,0,0},
    {0,0,-20,0,-20,0,0,0,0},
    {0,0,20,0,-20,0,0,0,0},
    {0,0,40,0,-20,0,0,0,10},
    {0,0,60,0,-20,0,0,0,20},
    {0,0,60,0,0,0,0,0,30},
    {0,0,40,0,0,0,0,0,40},
    {0,0,20,0,0,0,0,0,50},
    {0,0,0,0,0,0,0,0,60}
};

void drawmodel(char* filename)
{
    GLMmodel* model = glmReadOBJ(filename);
    glmUnitize(model);
    glmFacetNormals(model);
    glmVertexNormals(model, 90.0);
    glmScale(model, .15);
    glmDraw(model, GLM_SMOOTH | GLM_MATERIAL);
}

GLuint loadTexture(Image* image)
{
    GLuint textureId;
    glGenTextures(1, &textureId);			 //Make room for our texture
    glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
    //Map the image to the texture
    glTexImage2D(GL_TEXTURE_2D,				  //Always GL_TEXTURE_2D
        0,							  //0 for now
        GL_RGB,					  //Format OpenGL uses for image
        image->width, image->height, //Width and height
        0,							  //The border of the image
        GL_RGB,					  //GL_RGB, because pixels are stored in RGB format
        GL_UNSIGNED_BYTE,			  //GL_UNSIGNED_BYTE, because pixels are stored
                                     //as unsigned numbers
        image->pixels);			  //The actual pixel data
    return textureId;						  //Returns the id of the texture
}
GLuint _textureId; //The id of the texture
GLuint _textureId2;

void init(void)
{
    image = loadBMP("images/floor1.bmp");
    _textureId = loadTexture(image);
    image2 = loadBMP("images/Wall.bmp");
    _textureId2 = loadTexture(image2);
    delete image;
    delete image2;
    // Turn on the power
    glEnable(GL_LIGHTING);
    // Flip light switch
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    // assign light parameters
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    // Material Properties
    GLfloat lightColor1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor1);
    glEnable(GL_NORMALIZE);
    //Enable smooth shading
    glShadeModel(GL_SMOOTH);
    // Enable Depth buffer
    glEnable(GL_DEPTH_TEST);

}

void print() {
    std::cout << "hands shoulder : " << shoulder << std::endl;
    std::cout << "elbow1 : " << elbow << std::endl;
    std::cout << "elbow2 : " << elbow2 << std::endl;
    std::cout << "leg1 Yrotation : " << leg1_Yrotation << std::endl;
    std::cout << "leg Zrotation : " << leg_Zrotation << std::endl;
    std::cout << "leg1 downpart : " << down_Leg1Part << std::endl;
    std::cout << "leg2 Yrotation : " << leg2_Yrotation << std::endl;
    std::cout << "leg2 downpart : " << down_Leg2Part << std::endl;
    std::cout << "F1BASE : " << f1base << std::endl;
    std::cout << "F1UP : " << f1up << std::endl;
    std::cout << "body translation : " << body_Xtranslation << std::endl;
    std::cout << "####################################" << std::endl;

}

void display(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
    glRotatef(angle2, 1.0, 0.0, 0.0);
    glRotatef(angle, 0.0, 1.0, 0.0);
    glPushMatrix();
    glTranslatef(0.0, -6.4, 3.0);

    glPushMatrix();
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glPopMatrix();
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBegin(GL_QUADS);
    glNormal3f(0.0, 1.0, 0.0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-30, -2, 30);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(30, -2, 30);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(30, -2, -30);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-30, -2, -30);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();

    glPushMatrix();
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glPopMatrix();
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBegin(GL_QUADS);
    glNormal3f(0.0, 1.0, 0.0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-26, -8, 30);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-26, 20, 30);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-26, 20, -26);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-26, -8, -26);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-26, 20, 30);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(26, 20, 30);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(26, 20, -26);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-26, 20, -26);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(26, 20, 30);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(26, -8, 30);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(26, -8, -26);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(26, 20, -26);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-26, -8, 30);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-26, 20, 30);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(26, 20, 30);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(26, -8, 30);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-26, -8, -26);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-26, 20, -26);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(26, 20, -26);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(26, -8, -26);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    glPopMatrix();

    glTranslatef(0.0, 1.0, 3.0);

    if (modelname != NULL) {

        glPushMatrix();

        glRotatef(ball_rotationX, 1.0, 0.0, 0.0);

        glTranslatef(obj1_x, obj1_y, obj1_z);
        glRotatef(obj_Rotation, 0.0, 1.0, 0.0);

        glScalef(obj1_scalex, obj1_scaley, obj1_scalez);
        drawmodel((char*)modelname);
        glPopMatrix();
    }
    
    if (modelname2 != NULL) {
        glPushMatrix();
        glTranslatef(obj2_x, obj2_y, obj2_z);
        glScalef(obj2_scalex, obj2_scaley, obj2_scalez);
        drawmodel((char*)modelname2);
        glPopMatrix();
    }

    glPushMatrix();
    glTranslatef(0, (GLfloat)body_Xtranslation, 0);
    glTranslatef(0, 0, (GLfloat)body_Ztranslation);

    //Body Head
    glPushMatrix();
    glTranslatef(0, 1.8, 0);
    glutSolidSphere(0.8, 10, 10);
    glPopMatrix();

    //Body Trunk
    glPushMatrix();
    glPushMatrix();
    glTranslatef(0.0, -2, 0.0);
    glScalef(1.0, 2.5, 0.5);
    glutSolidCube(2.0);
    glPopMatrix();

    //Right hand
    glPushMatrix();
    glTranslatef(1.3, 0.3, 0.0);
    glRotatef(-90, 0.0, 0.0, 1.0);
    glRotatef((GLfloat)shoulder, 0.0, 0.0, 1.0);
    glRotatef((GLfloat)handsRotation, 1.0, 0.0, 0.0);
    glTranslatef(1.0, 0.0, 0.0);
    glPushMatrix();
    glScalef(3.2, 0.6, 1.0);
    glutSolidCube(0.6);
    glPopMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef((GLfloat)elbow, 0.0, 0.0, 1.0);
    glTranslatef(1.0, 0.0, 0.0);
    glPushMatrix();
    glScalef(3.2, 0.6, 1.0);
    glutSolidCube(0.6);
    glPopMatrix();


    //Draw finger flang 1 
    glPushMatrix();
    glTranslatef(1.0, 0.1, -0.2);
    glRotatef((GLfloat)-f1base, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.3, 0.1, 0.1);
    glutSolidCube(1);
    glPopMatrix();


    //Draw finger flang 1 
    glTranslatef(0.15, 0.0, 0.0);
    glRotatef((GLfloat)-f1up, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.3, 0.1, 0.1);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();


    //Draw finger flang 2
    glPushMatrix();
    glTranslatef(1.0, 0.1, -0.05);
    glRotatef((GLfloat)-f1base, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.3, 0.1, 0.1);
    glutSolidCube(1);
    glPopMatrix();


    //Draw finger flang 2 
    glTranslatef(0.15, 0.0, 0.0);
    glRotatef((GLfloat)-f1up, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.3, 0.1, 0.1);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();


    //Draw finger flang 3
    glPushMatrix();
    glTranslatef(1.0, 0.1, 0.1);
    glRotatef((GLfloat)-f1base, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.3, 0.1, 0.1);
    glutSolidCube(1);
    glPopMatrix();


    //Draw finger flang 3
    glTranslatef(0.15, 0.0, 0.0);
    glRotatef((GLfloat)-f1up, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.3, 0.1, 0.1);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();


    //Draw finger flang 4
    glPushMatrix();
    glTranslatef(1.0, 0.1, 0.25);
    glRotatef((GLfloat)-f1base, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.3, 0.1, 0.1);
    glutSolidCube(1);
    glPopMatrix();


    //Draw finger flang 4
    glTranslatef(0.15, 0.0, 0.0);
    glRotatef((GLfloat)-f1up, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.3, 0.1, 0.1);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();


    //Draw finger flang 5
    glPushMatrix();
    glTranslatef(0.9, -0.1, 0.15);
    glRotatef((GLfloat)-f1base, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.2, 0.1, 0.1);
    glutSolidCube(1);
    glPopMatrix();



    //Draw finger flang 5
    glTranslatef(0.05, 0.0, 0.0);
    glRotatef((GLfloat)-f1up, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.2, 0.1, 0.1);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();

    glPopMatrix();

    //Left Hand

    glPushMatrix();
    glTranslatef(-1.3, 0.3, 0.0);
    glRotatef(270, 0.0, 0.0, 1.0);
    glRotatef((GLfloat)-shoulder, 0.0, 0.0, 1.0);
    glRotatef((GLfloat)-handsRotation, 1.0, 0.0, 0.0);
    glTranslatef(1.0, 0.0, 0.0);
    glPushMatrix();
    glScalef(3.2, 0.6, 1.0);
    glutSolidCube(0.6);
    glPopMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef((GLfloat)-elbow2, 0.0, 0.0, 1.0);
    glTranslatef(1.0, 0.0, 0.0);
    glPushMatrix();
    glScalef(3.2, 0.6, 1.0);
    glutSolidCube(0.6);
    glPopMatrix();

    //Draw finger flang 1 
    glPushMatrix();
    glTranslatef(1.0, -0.1, -0.2);
    glRotatef((GLfloat)f2base, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.3, 0.1, 0.1);
    glutSolidCube(1);
    glPopMatrix();


    //Draw finger flang 1 
    glTranslatef(0.15, 0.0, 0.0);
    glRotatef((GLfloat)f2up, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.3, 0.1, 0.1);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();


    //Draw finger flang 2
    glPushMatrix();
    glTranslatef(1.0, -0.1, -0.05);
    glRotatef((GLfloat)f2base, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.3, 0.1, 0.1);
    glutSolidCube(1);
    glPopMatrix();


    //Draw finger flang 2 
    glTranslatef(0.15, 0.0, 0.0);
    glRotatef((GLfloat)f2up, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.3, 0.1, 0.1);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();


    //Draw finger flang 3
    glPushMatrix();
    glTranslatef(1.0, -0.1, 0.1);
    glRotatef((GLfloat)f2base, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.3, 0.1, 0.1);
    glutSolidCube(1);
    glPopMatrix();


    //Draw finger flang 3
    glTranslatef(0.15, 0.0, 0.0);
    glRotatef((GLfloat)f2up, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.3, 0.1, 0.1);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();


    //Draw finger flang 4
    glPushMatrix();
    glTranslatef(1.0, -0.1, 0.25);
    glRotatef((GLfloat)f2base, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.3, 0.1, 0.1);
    glutSolidCube(1);
    glPopMatrix();


    //Draw finger flang 4
    glTranslatef(0.15, 0.0, 0.0);
    glRotatef((GLfloat)f2up, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.3, 0.1, 0.1);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();


    //Draw finger flang 5
    glPushMatrix();
    glTranslatef(0.9, 0.1, 0.15);
    glRotatef((GLfloat)f2base, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.2, 0.1, 0.1);
    glutSolidCube(1);
    glPopMatrix();


    //Draw finger flang 5
    glTranslatef(0.05, 0.0, 0.0);
    glRotatef((GLfloat)f2up, 0.0, 0.0, 1.0);
    glTranslatef(0.15, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.2, 0.1, 0.1);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();

    glPopMatrix();
    glPopMatrix();

    //Right leg

    glPushMatrix();

    glTranslatef(0.7, -4.5, 0.0);
    glRotatef(270, 0.0, 0.0, 1.0);
    glRotatef((GLfloat)leg1_Yrotation, 0.0, 1.0, 0.0);
    glRotatef((GLfloat)leg_Zrotation, 0.0, 0.0, 1.0);
    glTranslatef(1.0, 0.0, 0.0);
    glPushMatrix();
    glScalef(2.0, 0.6, 1.0);
    glutSolidCube(1.0);
    glPopMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef((GLfloat)down_Leg1Part, 0.0, 1.0, 0.0);
    glTranslatef(1.0, 0.0, 0.0);
    glPushMatrix();
    glScalef(2.0, 0.6, 1.0);
    glutSolidCube(1.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(1.4, 0.0, 0.0);
    glScalef(0.8, 0.6, 2.0);
    glutSolidCube(1.0);
    glPopMatrix();
    glPopMatrix();

    //Left Leg

    glPushMatrix();

    glTranslatef(-0.7, -4.5, 0.0);
    glRotatef(270, 0.0, 0.0, 1.0);
    glRotatef((GLfloat)-leg2_Yrotation, 0.0, 1.0, 0.0);
    glRotatef((GLfloat)-leg_Zrotation, 0.0, 0.0, 1.0);
    glTranslatef(1.0, 0.0, 0.0);
    glPushMatrix();
    glScalef(2.0, 0.6, 1.0);
    glutSolidCube(1.0);
    glPopMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef((GLfloat)-down_Leg2Part, 0.0, 1.0, 0.0);
    glTranslatef(1.0, 0.0, 0.0);
    glPushMatrix();
    glScalef(2.0, 0.6, 1.0);
    glutSolidCube(1.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(1.4, 0.0, 0.0);
    glScalef(0.8, 0.6, 2.0);
    glutSolidCube(1.0);
    glPopMatrix();
    glPopMatrix();

    glPopMatrix();

    glPopMatrix();
    print();
    glutSwapBuffers();
}

void Rotate(double Up[], double theta, double Eye[])
{
    double temp[3];
    temp[0] = Eye[0];
    temp[1] = Eye[1];
    temp[2] = Eye[2];

    temp[0] = -Up[2] * Eye[1] + Up[1] * Eye[2];
    temp[1] = Up[2] * Eye[0] - Up[0] * Eye[2];
    temp[2] = -Up[1] * Eye[0] + Up[0] * Eye[1];

    temp[0] *= sin(theta);
    temp[1] *= sin(theta);
    temp[2] *= sin(theta);

    temp[0] += (1 - cos(theta)) * (Up[0] * Up[0] * Eye[0] + Up[0] * Up[1] * Eye[1] + Up[0] * Up[2] * Eye[2]);
    temp[1] += (1 - cos(theta)) * (Up[0] * Up[1] * Eye[0] + Up[1] * Up[1] * Eye[1] + Up[1] * Up[2] * Eye[2]);
    temp[2] += (1 - cos(theta)) * (Up[0] * Up[2] * Eye[0] + Up[1] * Up[2] * Eye[1] + Up[2] * Up[2] * Eye[2]);

    temp[0] += cos(theta) * Eye[0];
    temp[1] += cos(theta) * Eye[1];
    temp[2] += cos(theta) * Eye[2];

    Eye[0] = temp[0];
    Eye[1] = temp[1];
    Eye[2] = temp[2];
}

void crossProduct_Function(double Up[], double look[], double crossProduct[])
{
    crossProduct[0] = Up[1] * look[2] - Up[2] * look[1];
    crossProduct[1] = Up[2] * look[0] - Up[0] * look[2];
    crossProduct[2] = Up[0] * look[1] - Up[1] * look[0];
}


void Normalize(double crossProduct[]) {
    double norm;
    norm = crossProduct[0] * crossProduct[0] + crossProduct[1] * crossProduct[1] + crossProduct[2] * crossProduct[2];
    norm = sqrt(norm);
    crossProduct[0] /= norm;
    crossProduct[1] /= norm;
    crossProduct[2] /= norm;
}

void horizontalRotate(double theta, double Eye[], double Up[]) {
    look[0] = center[0] - eye[0];
    look[1] = center[1] - eye[1];
    look[2] = center[2] - eye[2];

    crossProduct_Function(Up, look, Cross);
    Normalize(Cross);
    Rotate(Cross, theta, Eye);
    Rotate(Cross, theta, Up);
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(zoom, (GLfloat)w / (GLfloat)h, 1.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 1.0, -10.0);
}

void setFrames(int animate[10][9], int frame) {
    shoulder = animate[frame][0];
    body_Ztranslation = animate[frame][1];
    leg1_Yrotation = animate[frame][2];
    leg_Zrotation = animate[frame][3];
    down_Leg1Part = animate[frame][4];
    leg2_Yrotation = animate[frame][5];
    down_Leg2Part = animate[frame][6];
    body_Xtranslation = animate[frame][7];
    ball_rotationX = animate[frame][8];

}

void timer1(int value) {
    if (animation1) {
        frame = frame % 10;
        setFrames(animate1, frame);
        frame++;
        glutPostRedisplay();
        glutTimerFunc(150, timer1, 0);
    }
    else {
        glutTimerFunc(150, NULL, 0);

    }
}

void timer2(int value) {
    if (animation2) {
        frame = frame % 10;
        setFrames(animate2, frame);
        frame++;
        glutPostRedisplay();
        glutTimerFunc(150, timer2, 0);
    }
    else {
        glutTimerFunc(150, NULL, 0);
    }
}

void timer3(int value) {
    if (animation3) {
        frame = frame % 10;
        setFrames(animate3, frame);
        frame++;
        glutPostRedisplay();
        glutTimerFunc(150, timer3, 0);
    }
    else {
        glutTimerFunc(150, NULL, 0);
    }
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'm':
        body_Xtranslation += 0.2;
        glutPostRedisplay();
        break;

    case 'M':
        body_Xtranslation -= 0.2;
        glutPostRedisplay();
        break;

    case 'z':
        zoom -= 5;
        if (zoom <= 0) {
            zoom = 0;
        }
        reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        glutPostRedisplay();
        break;

    case 'Z':
        zoom += 5;
        if (zoom >= 180) {
            zoom = 180;
        }
        reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        glutPostRedisplay();
        break;

    case 'q':
        shoulder = (shoulder + 5) % 360;
        if (shoulder >= 180) { shoulder = 180; }
        glutPostRedisplay();

        break;

    case 'Q':
        shoulder = (shoulder - 5) % 360;
        if (shoulder <= 0) { shoulder = 0; }
        glutPostRedisplay();

        break;

    case 'w':
        elbow = (elbow + 5) % 360;
        if (elbow >= 150) { elbow = 150; }        // maxmize the angle to 150 to simulate the human arm
        glutPostRedisplay();

        break;

    case 'W':
        elbow = (elbow - 5) % 360;
        if (elbow <= -100) { elbow = -100; }        // minimize the angle to 0 to simulate the human arm
        glutPostRedisplay();

        break;

    case 'l':
        elbow2 = (elbow2 + 5) % 360;
        if (elbow2 >= 150) { elbow2 = 150; }        // maxmize the angle to 150 to simulate the human arm
        glutPostRedisplay();

        break;

    case 'L':
        elbow2 = (elbow2 - 5) % 360;
        if (elbow2 <= -100) { elbow2 = -100; }        // minimize the angle to 0 to simulate the human arm
        glutPostRedisplay();

        break;

    case 'e':
        handsRotation = (handsRotation + 5) % 360;
        if (handsRotation >= 90) { handsRotation = 90; }
        glutPostRedisplay();

        break;

    case 'E':
        handsRotation = (handsRotation - 5) % 360;
        if (handsRotation <= 0) { handsRotation = 0; }
        glutPostRedisplay();

        break;

    case 'r':
        leg_Zrotation = (leg_Zrotation + 5) % 360;
        if (leg_Zrotation >= 80) { leg_Zrotation = 80; }

        glutPostRedisplay();

        break;

    case 'R':
        leg_Zrotation = (leg_Zrotation - 5) % 360;
        if (leg_Zrotation <= -5) { leg_Zrotation = -5; }
        glutPostRedisplay();

        break;

    case 'y':
        leg1_Yrotation = (leg1_Yrotation + 5) % 360;
        if (leg1_Yrotation >= 80) { leg1_Yrotation = 80; }
        glutPostRedisplay();

        break;

    case 'Y':
        leg1_Yrotation = (leg1_Yrotation - 5) % 360;
        if (leg1_Yrotation <= -60) { leg1_Yrotation = -60; }
        glutPostRedisplay();

        break;

    case 'u':
        down_Leg1Part = (down_Leg1Part - 5) % 360;
        if (down_Leg1Part <= -80) { down_Leg1Part = -80; }
        glutPostRedisplay();

        break;

    case 'U':
        down_Leg1Part = (down_Leg1Part + 5) % 360;
        if (down_Leg1Part >= 0) { down_Leg1Part = 0; }
        glutPostRedisplay();

        break;

    case 'i':
        leg2_Yrotation = (leg2_Yrotation - 5) % 360;
        if (leg2_Yrotation <= -80) { leg2_Yrotation = -80; }
        glutPostRedisplay();

        break;

    case 'I':
        leg2_Yrotation = (leg2_Yrotation + 5) % 360;
        if (leg2_Yrotation >= 60) { leg2_Yrotation = 60; }
        glutPostRedisplay();

        break;

    case 'o':
        down_Leg2Part = (down_Leg2Part + 5) % 360;
        if (down_Leg2Part >= 80) { down_Leg2Part = 80; }
        glutPostRedisplay();

        break;

    case 'O':
        down_Leg2Part = (down_Leg2Part - 5) % 360;
        if (down_Leg2Part <= 0) { down_Leg2Part = 0; }
        glutPostRedisplay();

        break;

    case 'v':
        Rotate(up, cameraRotation, eye);
        glutPostRedisplay();

        break;

    case 'V':
        Rotate(up, -cameraRotation, eye);
        glutPostRedisplay();

        break;

    case 'h':
        horizontalRotate(cameraRotation, eye, up);
        glutPostRedisplay();
        break;

    case 'H':

        horizontalRotate(-cameraRotation, eye, up);
        glutPostRedisplay();
        break;

    case 27:
        exit(0);
        break;
    default:
        break;
    }

}

static void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            moving = 1;
            startx = x;
            starty = y;
        }
        if (state == GLUT_UP) {
            moving = 0;
        }
    }
}

static void motion(int x, int y)
{

    if (moving) {
        angle = angle + (x - startx);
        angle2 = angle2 + (y - starty);
        startx = x;
        starty = y;
        glutPostRedisplay();
    }
}

void reset()
{
    double e[] = { 0.0, 0.0, -3.0 };
    double c[] = { 0.0, 0.0, 0.0 };
    double u[] = { 0.0, 1.0, 0.0 };
    for (int i = 0; i < 3; i++)
    {
        eye[i] = e[i];
        center[i] = c[i];
        up[i] = u[i];
    }
}

void screen_menu(int value)
{

    switch (value)
    {
    case '1':

        image = loadBMP("images/floor1.bmp");
        _textureId = loadTexture(image);
        delete image;
        modelname = "data/snowman_finish.obj";
        modelname2 = "data/Tree2.obj";
        obj1_x = 10, obj1_y = -5, obj1_z = 10, obj1_scalex = 40, obj1_scaley = 40, obj1_scalez = 40, obj_Rotation = 180;
        animation1 = true;
        animation2 = false;
        animation3 = false;
        frame = 0;
        glutTimerFunc(0, timer1, 0);
        break;

    case '2':

        image = loadBMP("images/floor2.bmp");
        _textureId = loadTexture(image);
        delete image;
        modelname = "data/soccerball.obj";
        obj1_x = 1.0, obj1_y = -8.0, obj1_z = -2.5, obj1_scalex = 10, obj1_scaley = 10, obj1_scalez = 10, obj_Rotation = 0;
        modelname2 = "data/Tree/Tree.obj";

        animation1 = false;
        animation2 = false;
        animation3 = true;
        frame = 0;
        glutTimerFunc(0, timer3, 0);
        break;

    case '3':

        image = loadBMP("images/floor3.bmp");
        _textureId = loadTexture(image);
        delete image;
        modelname = "data/Rock.obj";
        obj1_x = 10, obj1_y = -5, obj1_z = 10, obj1_scalex = 30, obj1_scaley = 30, obj1_scalez = 30, obj_Rotation = 180;
        modelname2 = NULL;
        animation1 = false;
        animation2 = true;
        animation3 = false;
        frame = 0;
        glutTimerFunc(0, timer2, 0);
        break;

    }
    reset();
    glutPostRedisplay();
}

void attachMenu()
{
    glutCreateMenu(screen_menu);
    glutAddMenuEntry("Scene1", '1');
    glutAddMenuEntry("Scene2", '2');
    glutAddMenuEntry("Scene3", '3');
    glutAttachMenu(GLUT_RIGHT_BUTTON);;
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Animation");
    init();
    glMatrixMode(GL_PROJECTION);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutTimerFunc(0, timer1, 0);
    attachMenu();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}