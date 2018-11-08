#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <ctype.h>
#include <math.h>
#include<GL/glut.h>
//#include <quadTree.h>
#define KEY_ESC 27
#include "Rtree.h"
#include <string>
#include "fstream"
#define PI 3.1415926535897932384626433832795
typecor radio=30;
double px,py;

//vector <point> area;


//Crear quad tree


//dibuja un simple

//qt test (2,300,300);
Rtree test(5);
void displayGizmo()
{
	glBegin(GL_LINES);
	glColor3d(255,0,0);
	glVertex2d(-300, 0);
	glVertex2d(300, 0);
	glColor3d(0, 255, 0);
	glVertex2d(0, -300);
	glVertex2d(0, 300);
	glEnd();
	glBegin(GL_POINTS);
	glColor3d(255,67,0);
	glVertex2d(150, 150);
	glColor3d(255,67,0);
	glVertex2d(150, 180.8798);
	glEnd();
}

void drawrtree (node** p)
{
    if ((*p)->data!=0)
    {
        glBegin(GL_POINTS);
        glColor3d(255,67,0);
        glVertex2d((*p)->data->first, (*p)->data->second);
        glEnd();
    return;
    }
    //typecor dis=(*p)->cmax.first-(*p)->cmin.first;
    //dis/=2;
    typecor miniX,miniY,maxiX,maxiY;
    miniX=(*p)->cmin.first;
    miniY=(*p)->cmin.second;
    maxiX=(*p)->cmax.first;
    maxiY=(*p)->cmax.second;
    glBegin(GL_LINES);
    glColor3d(255,0,0);
	glVertex2d(miniX,miniY);
	glVertex2d(maxiX,miniY);
	glColor3d(255,0, 0);
	glVertex2d(maxiX,miniY);
	glVertex2d(maxiX,maxiY);
	glColor3d(255,0,0);
	glVertex2d(miniX,maxiY);
	glVertex2d(maxiX,maxiY);
	glColor3d(255,0, 0);
	glVertex2d(miniX,miniY);
	glVertex2d(miniX,maxiY);
	glEnd();
	for (int i=0;i<(*p)->children.size();i++){
        drawrtree(&((*p)->children[i]));
       // cout<<"I:"<<i<<endl;
	}
}

void DrawCircle(double cx, double cy, double r, int num_segments) {
    glBegin(GL_LINE_LOOP);
    double x,y,theta;
    glColor3d(255,255,255);
    //cout<<"RADIOO"<<radio<<endl;
    for (int ii = 0; ii < num_segments; ii++)   {
        theta = 2.0f * 3.1415926f * double(ii) / double(num_segments);//get the current angle
        x = r * cosf(theta);//calculate the x component
        y = r * sinf(theta);//calculate the y component
        //cout<<"["<<x+cx<<","<<y+cy<<"]\t";
        glVertex2d(x + cx, y + cy);//output vertex
       // cout<<x+cx<<" "<<y+cy<<endl;
    }

    //cout << endl;
    glEnd();
}

void OnMouseClick(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
      test.rtinsert(make_pair((x-300)*1.0,(300-y)*1.0));
    //convertir x,y
	//insertar un nuevo punto en el quadtree
  }
}

void OnMouseMotion(int x, int y)
{
     //opcional
	 //hacer algo x,z cuando se mueve el mouse
    px = x-300;
    py = 300-y;
//    area.resize(0);
//    test.circle(test.root,area,make_pair(px*1.0,py*1.0),radio);
}



void idle(){ // AGREGAR ESTA FUNCION
	glutPostRedisplay();
}

//funcion llamada a cada imagen
void glPaint(void) {

	//El fondo de la escena al color initial
	glClear(GL_COLOR_BUFFER_BIT); //CAMBIO
	glLoadIdentity();
	glOrtho(-300.0f,  300.0f,-300.0f, 300.0f, -1.0f, 1.0f);

	//dibujar quadTree (qt->draw())

	//dibuja el gizmo

	drawrtree(&test.root);///displayGizmo();
//displayGizmo();
	//DrawCircle(px,py,radio,100);
   /* glBegin(GL_POINTS);
	glPointSize(3);
	glColor3d(0,255,0);
	for(int i=0;i<area.size();i++)
    {
        glVertex2d(area[i].first,area[i].second);
        //cout<<"punto: "<<area[i].first<<" , "<<area[i].second<<endl;
    }
	glEnd();*/
	//doble buffer, mantener esta instruccion al fin de la funcion
	glutSwapBuffers();
}

//
//inicializacion de OpenGL
//
void init_GL(void) {
	//Color del fondo de la escena
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro

	//modo projeccion
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

//en el caso que la ventana cambie de tamaño
GLvoid window_redraw(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


}

GLvoid window_key(unsigned char key, int x, int y) {
	switch (key) {
	case KEY_ESC:
		exit(0);
		break;

	default:
		break;
	}

}
//
//el programa principal
//
int main(int argc, char** argv) {

	//Inicializacion de la GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600); //tamaño de la ventana
	glutInitWindowPosition(100, 100); //posicion de la ventana
	glutCreateWindow("TP2 bis OpenGL : Bresenham"); //titulo de la ventana
	ifstream input;
	string num,num2;
	double n,m;

	input.open("C:/Users/Adriana/Documents/quadtree/tt.txt",ios::in);
	if(!input.is_open())
        cout<<"no se pudo abrir archivo";
	char c;
	while (input.get(c))
    {

        if (isdigit(c))
            num+=c;
        else if(c=='.' || c=='-')
        {
                char temp=c;
                input.get(c);
                if (isdigit(c)){
                    num+=temp;
                    num+=c;
                }
        }
        else if (c==',')
        {
            if(num.size()>0 && num2.size()==0)
            {
                num2=num;
                num="";
            }
            else if (num.size()>0 && num2.size()>0)
            {
                n=stod(num2);
                n=((n/-87.67370828)-1)*40000;
                m=stod(num);
                m=((m/41.84307539)-1)*40000;
                //cout<<n<<","<<m<<endl;
                test.rtinsert(make_pair(n,m));
                num=num2="";
            }

        }
    }

  /*  point w=make_pair(35,70);
    test.rtinsert(w);
    node**p=&(test.root);
    cout<<"res: "<<test.findpointleaf(p,make_pair(45,20));*/

   // test.root->set_rec();
    //cout<<test.root->cmin.first<<","<<test.root->cmin.second<<" "<<test.root->cmax.first<<","<<test.root->cmax.second<<endl;
	init_GL(); //funcion de inicializacion de OpenGL


	glutDisplayFunc(glPaint);
	glutReshapeFunc(&window_redraw);
	// Callback del teclado
	glutKeyboardFunc(&window_key);
	glutMouseFunc(&OnMouseClick);
	glutMotionFunc(&OnMouseMotion);
	glutIdleFunc(&idle);


	//qt = new quadTree();
	glutMainLoop(); //bucle de rendering
	//no escribir nada abajo de mainloop
	return 0;
}
