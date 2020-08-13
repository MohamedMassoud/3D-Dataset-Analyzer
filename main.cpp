#define _GNU_SOURCE
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <unistd.h>
void mainDisplay();

bool rot = false;
int rotX, rotZ = 0;
float data[100000][3];
float normalizedData[100000][3];
int rows=0;
int win;
float minValue=0;
float maxValue=0;
float scale=1;

void init( void )
{

    glEnable(GL_DEPTH);
    //glEnable(GL_LIGHTING);

    glClearColor (0.0, 0.0, 0.0, 0.0);
    glViewport(0, 0, 500, 500);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-250, 250, -250, 250, -250, 250);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //x-axis
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
        glVertex3i(-10000, 0, 0);
        glVertex3i(10000, 0, 0);
    glEnd();

    //y-axis
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
        glVertex3i(0, -10000, 0);
        glVertex3i(0, 10000, 0);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    if(rot){
        printf("%d   %d   ", rotX, rotZ);
        glRotatef(3 ,rotX , rotZ, 0);
    }


    for(int i=0; i<rows; i++){
        glTranslatef(-normalizedData[i][0], -normalizedData[i][1], -normalizedData[i][2]);
        glutSolidSphere(scale, 10, 10);
        glTranslatef(normalizedData[i][0], normalizedData[i][1], normalizedData[i][2]);
    }

    glutSwapBuffers();

}


void mouse(int button, int state, int x, int y){
    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
        printf("%d %d\n", x, y);
        if(x>=450 && x<=500){
            rotZ=-1;
            rot=true;
            printf("1\n");
        }else if(x>=0 && x<=50){
            rotZ=1;
            rot=true;
            printf("2\n");
        }else if(y>=450 && y<=500){
            rotX=-1;
            rot=true;
            printf("3\n");
        }else if(y>=0 && y<=50){
            rotX=1;
            rot=true;
            printf("4\n");
        }
    }else if(button==GLUT_LEFT_BUTTON && state==GLUT_UP){
        rot=false;
        rotX=rotZ=0;

    }
}
void key(unsigned char key, int x, int y){
    if(key=='+'){
        glScalef(1.2, 1.2, 1.2);
        scale/=1.2;
    }
     if(key=='-'){
        glScalef(0.8, 0.8, 0.8);
        scale/=0.8;
    }
     if(key=='w'){
        glTranslatef(0, -20*scale, 0);
    }
     if(key=='a'){
        glTranslatef(20*scale, 0, 0);
    }
     if(key=='s'){
        glTranslatef(0, 20*scale, 0);
    }
     if(key=='d'){
        glTranslatef(-20*scale, 0, 0);
    }
}
void animation(){
    display();
}
void menu(int id){
    if(id==1){
            glutDestroyWindow(win);
            mainDisplay();
    }
    if(id==2)exit(0);


}

void mainDisplay(){
    int mode = GLUT_RGB|GLUT_DOUBLE;
     glutInitDisplayMode( mode);
     glutInitWindowSize(500,500);
     win = glutCreateWindow("Visualization");
     init();
     //glutReshapeFunc(reshape);
     glutDisplayFunc(display);
     glutMouseFunc(mouse);
     glutKeyboardFunc(key);
     glutIdleFunc(animation);
     int menu_id = glutCreateMenu(menu);
     glutAddMenuEntry("Reset", 1);
     glutAddMenuEntry("Exit", 2);
     glutAttachMenu(GLUT_RIGHT_BUTTON);


     glutMainLoop();
}
void normalizeData(){
    for(int i=0; i<rows; i++){
        normalizedData[i][0] = (data[i][0])*100/(maxValue-minValue);
        normalizedData[i][1] = (data[i][1])*100/(maxValue-minValue);
        normalizedData[i][2] = (data[i][2])*100/(maxValue-minValue);
    }
}
int main() {

    char filename[20];
    FILE* fp;
    char buffer[255];
    do{
        printf("Enter file name: ");
        scanf("%s", filename);
        fp = fopen(filename, "r");
    }while(!fp);


    int j=0;

    while(fgets(buffer, 255, (FILE*) fp)) {
        char *ptr = strtok(buffer, ",");
        while(ptr != NULL)
        {
            //printf("%f ", atof(ptr));
            float value = atof(ptr);
            data[rows][j]=atof(ptr) ;
            if(value<minValue)minValue=value;
            if(value>maxValue)maxValue=value;
            j++;
            ptr = strtok(NULL, ",");
        }
        rows++;
        j=0;
    }
    fclose(fp);
    normalizeData();
    mainDisplay();


	return 0;
}


