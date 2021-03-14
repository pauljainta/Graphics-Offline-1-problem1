#include<stdio.h>
#include<stdlib.h>
#include<math.h>


#include <windows.h>
#include <GL/glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
double wall_side;
double wall_y;
double sphere_radius,small_sphere_radius,barrel_height;
int color;

double whole_part_rotate,whole_part_except_leftHem_rotate;




struct point
{
	double x,y,z;
};


struct point pos,u,l,r;



struct point cross_gun(struct point p1,struct point p2)
{
    struct point cross_P;

    cross_P.x = p1.y * p2.z - p1.z * p2.y;
    cross_P.y = p1.z * p2.x - p1.x * p2.z;
    cross_P.z = p1.x * p2.y - p1.y * p2.x;

    return cross_P;


}


double dotGun(struct point A, struct point B)
{

    double product = 0;
    product = product + (A.x*B.x)+(A.y*B.y)+(A.z*B.z);
    return product;
}

void RotateLR(int side)
{
     double ldot=dotGun(u,l);
     struct point lcross=cross_gun(u,l);

     double rdot=dotGun(u,r);
     struct point rcross=cross_gun(u,r);

    l.x=l.x*cos(side*angle)+lcross.x*sin(side*angle)+(u.x)*ldot*(1-cos(side*angle));
    l.y=l.y*cos(side*angle)+lcross.y*sin(side*angle)+(u.y)*ldot*(1-cos(side*angle));
    l.z=l.z*cos(side*angle)+lcross.z*sin(side*angle)+(u.z)*ldot*(1-cos(side*angle));

    r.x=r.x*cos(side*angle)+rcross.x*sin(side*angle)+(u.x)*rdot*(1-cos(side*angle));
    r.y=r.y*cos(side*angle)+rcross.y*sin(side*angle)+(u.y)*rdot*(1-cos(side*angle));
    r.z=r.z*cos(side*angle)+rcross.z*sin(side*angle)+(u.z)*rdot*(1-cos(side*angle));



}


void RotateLU(int side)
{
     double ldot=dotGun(r,l);
     struct point lcross=cross_gun(r,l);

     double udot=dotGun(r,u);
     struct point ucross=cross_gun(r,u);

    l.x=l.x*cos(side*angle)+lcross.x*sin(side*angle)+(r.x)*ldot*(1-cos(side*angle));
    l.y=l.y*cos(side*angle)+lcross.y*sin(side*angle)+(r.y)*ldot*(1-cos(side*angle));
    l.z=l.z*cos(side*angle)+lcross.z*sin(side*angle)+(r.z)*ldot*(1-cos(side*angle));

    u.x=u.x*cos(side*angle)+ucross.x*sin(side*angle)+(r.x)*udot*(1-cos(side*angle));
    u.y=u.y*cos(side*angle)+ucross.y*sin(side*angle)+(r.y)*udot*(1-cos(side*angle));
    u.z=u.z*cos(side*angle)+ucross.z*sin(side*angle)+(r.z)*udot*(1-cos(side*angle));


}

void animate(){
//	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}


void RotateRU(int side)
{
     double rdot=dotGun(l,r);
     struct point rcross=cross_gun(l,r);

     double udot=dotGun(l,u);
     struct point ucross=cross_gun(l,u);

    r.x=r.x*cos(side*angle)+rcross.x*sin(side*angle)+(l.x)*rdot*(1-cos(side*angle));
    r.y=r.y*cos(side*angle)+rcross.y*sin(side*angle)+(l.y)*rdot*(1-cos(side*angle));
    r.z=r.z*cos(side*angle)+rcross.z*sin(side*angle)+(l.z)*rdot*(1-cos(side*angle));

    u.x=u.x*cos(side*angle)+ucross.x*sin(side*angle)+(l.x)*udot*(1-cos(side*angle));
    u.y=u.y*cos(side*angle)+ucross.y*sin(side*angle)+(l.y)*udot*(1-cos(side*angle));
    u.z=u.z*cos(side*angle)+ucross.z*sin(side*angle)+(l.z)*udot*(1-cos(side*angle));




}



void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawWall(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, wall_y,a);
		glVertex3f( a,wall_y,-a);
		glVertex3f(-a,wall_y,-a);
		glVertex3f(-a,wall_y,a);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    //glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=-(radius*cos(((double)i/(double)segments)*pi/2))+radius*2;
        points[i].y=(radius*sin(((double)i/(double)segments)*pi/2))+barrel_height;
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawLastShape(double radius , int segments)
{


    double rotation_angle = 360/segments;

    //glTranslatef(circle_x , circle_y , circle_z);
    drawCircle(radius , 30);
    //glTranslatef(-50 , circle_y , circle_z);

    int i;
    int color = 1;
    double angle_rotating = 0.2;
    for(i=0 ; i<segments ; i++)
    {
        color = 1 - color;
        double current_angle = 0;
        glColor3f(color , color , color);
        while(current_angle <= rotation_angle)
        {
            glRotatef(angle_rotating , 0 , 1 , 0);
            drawCircle(radius , 30);

            current_angle += 0.2;
        }
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();

    }
}


void drawLeftHemisphere(double radius,int slices,int stacks)
{
    struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}

	//glRotatef(90,1,0,0);
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{

		for(j=0;j<slices;j++)
		{

		    glColor3f(color,color,color);
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);

			}glEnd();

			if(color==0) color=1;
			else color=0;
		}

	}

}

void drawRightHemisphere(double radius,int slices,int stacks)
{
    color=0;

    struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//glRotatef(right_sphere_rotate,0,0,1);
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{

		for(j=0;j<slices;j++)
		{

		    glColor3f(color,color,color);
			glBegin(GL_QUADS);{

                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();

			if(color==0) color=1;
			else color=0;
		}

	}


}

void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}


void drawBarrel(double radius,double height,int slices)
{

    //double shade;
    struct point points[100];

    for(int i=0;i<=slices;i++)
    {
        points[i].x=radius*cos(((double)i/(double)slices)*2*pi);
        points[i].z=radius*sin(((double)i/(double)slices)*2*pi);
    }



     color = 0;
    for(int i=0;i<slices;i++)
    {
        glColor3f(color , color , color);

        glBegin(GL_QUADS);
        {
			glVertex3f(points[i].x,0+radius,points[i].z);
			glVertex3f(points[i+1].x,0+radius,points[i+1].z);
			glVertex3f(points[i+1].x,height+radius,points[i+1].z);
			glVertex3f(points[i].x,height+radius,points[i].z);
        }
        glEnd();
        if(color==0) color=1;
        else color=0;
    }



}



void drawSS()
{
   /* glColor3f(1,0,0);
    drawSquare(20);

    glRotatef(angle,0,0,1);
    glTranslatef(110,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawSquare(15);

    glPushMatrix();
    {
        glRotatef(angle,0,0,1);
        glTranslatef(60,0,0);
        glRotatef(2*angle,0,0,1);
        glColor3f(0,0,1);
        drawSquare(10);
    }
    glPopMatrix();

    glRotatef(3*angle,0,0,1);
    glTranslatef(40,0,0);
    glRotatef(4*angle,0,0,1);
    glColor3f(1,1,0);
    drawSquare(5);
    */
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
		    RotateLR(1);
            break;
        case '2':
            RotateLR(-1);
			break;
        case '3':
            RotateLU(1);
            break;
		case '4':
		    RotateLU(-1);
			break;
		case '5':
		    RotateRU(1);
			break;
		case '6':
		    RotateRU(-1);
			break;
		case 'a':
//            right_sphere_rotate+=1;
            break;
        case 'q':
            if(whole_part_rotate!=45)
            whole_part_rotate+=1;
            break;
        case 'w':
            if(whole_part_rotate!=-45)
            whole_part_rotate-=1;
            break;
         case 'e':
            if(whole_part_except_leftHem_rotate!=45)
            whole_part_except_leftHem_rotate+=1;
            break;
        case 'r':
            if(whole_part_except_leftHem_rotate!=-45)
            whole_part_except_leftHem_rotate-=1;
            break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			//cameraHeight -= 3.0;

			pos.x-=l.x;
			pos.y-=l.y;
			pos.z-=l.z;
			break;
		case GLUT_KEY_UP:// up arrow key
			//cameraHeight += 3.0;
			pos.x+=l.x;
			pos.y+=l.y;
			pos.z+=l.z;
			break;

		case GLUT_KEY_RIGHT:
			//cameraAngle += 0.03;
			pos.x+=r.x;
			pos.y+=r.y;
			pos.z+=r.z;
			break;
		case GLUT_KEY_LEFT:
			//cameraAngle -= 0.03;
			pos.x-=r.x;
			pos.y-=r.y;
			pos.z-=r.z;
			break;

		case GLUT_KEY_PAGE_UP:
		    pos.x+=u.x;
			pos.y+=u.y;
			pos.z+=u.z;
			break;

		case GLUT_KEY_PAGE_DOWN:
		    pos.x-=u.x;
			pos.y-=u.y;
			pos.z-=u.z;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(pos.x, pos.y, pos.z, pos.x + l.x, pos.y + l.y, pos.z + l.z, u.x, u.y, u.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();
	drawWall(wall_side);
	glRotatef(whole_part_rotate , 0 , 0 , 1);

	 glPushMatrix();
    {
        glRotatef(90,1,0,0);
        drawLeftHemisphere(sphere_radius,80,80);

    }
    glPopMatrix();

    glRotatef(whole_part_except_leftHem_rotate , 1 , 0 , 0);

    glPushMatrix();
    {
        glRotatef(90,1,0,0);
        drawRightHemisphere(sphere_radius,80,80);
    }
    glPopMatrix();



     glPushMatrix();
    {
        glTranslatef(0,sphere_radius,0);

        glRotatef(90,1,0,0);
        glTranslatef(0,0,-small_sphere_radius);
        drawLeftHemisphere(small_sphere_radius,80,80);

    }
    glPopMatrix();

     glPushMatrix();
    {

        glTranslatef(0,sphere_radius,0);
        drawBarrel(small_sphere_radius,barrel_height,40);
    }
    glPopMatrix();

     glPushMatrix();
    {

        glTranslatef(0,sphere_radius+small_sphere_radius,0);
        drawLastShape(small_sphere_radius,40);
    }
    glPopMatrix();







	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}



void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0.02;

	wall_side=200.0;
	wall_y=200.0;
	sphere_radius=40.0;
	small_sphere_radius=10.0;
	barrel_height=100.0;
	color=1;
//	right_sphere_rotate=90;
	whole_part_rotate=0;


	whole_part_except_leftHem_rotate=0;


	pos.x=100;
    pos.y=100;
    pos.z=0;

    u.x=0;
    u.y=0;
    u.z=1;

    l.x=(-1)/sqrt(2);
    l.y=(-1)/sqrt(2);
    l.z=0;

    r.x=(-1)/sqrt(2);
    r.y=(1)/sqrt(2);
    r.z=0;


	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);
	glutIdleFunc(animate);	//display callback function
             		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
