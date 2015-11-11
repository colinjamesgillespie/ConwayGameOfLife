#include <stdio.h>
#include <stdlib.h>
#include "math2Dlib.h"
#include <math.h>
#include <time.h>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

// ---------------------- FIELDS -----------------------


enum GAMESTATE {edit, run};
enum GAMESTATE GAME = edit;

enum SPEEDSTATE {verySlow, slow, fast, turbo};
enum SPEEDSTATE SPEED = fast;

const int speed0 = 1000; // very slow
const int speed1 = 500; // slow speed
const int speed2 = 90; // fast speed
const int speed3 = 10; // turbo

enum RUNSTATE {play, pause};
enum RUNSTATE RUN = pause;

const int xSize = 51;
const int ySize = 51;

const int xWindowSize = 600;
const int yWindowSize = 600;

// a value of 0 in the array means dead, a value of 1 means alive
int cellStateGrid[xSize][ySize];
int newCellStateGrid[xSize][ySize];

enum cellChangeState {off,dead,alive};
enum cellChangeState cellChange = off;

point2D p = point2D();

// ----------------- Grid --------------------------

void gridReset(){
	for (int i =0; i < xSize; i++){
		for (int j = 0; j < ySize; j++){
			cellStateGrid[i][j] = 0;
			newCellStateGrid[i][j] = 0;
		}
	}
}

void gridRandom(){
	for (int i =0; i < xSize; i++){
		for (int j = 0; j < ySize; j++){
			cellStateGrid[i][j] = rand() % 2;
		}
	}
}

// --------------STATE TRANSITION LOGIC ----------------

// counts how many alive cells are touching
// the selected cell. Can figure out how many
// dead cells by subtracting the alive cells from 8.
int aliveCounter(int x, int y){
	int aliveCount = 0;
	
	int xMin = x-1;
	int xMax = x+1;
	int yMin = y-1;
	int yMax = y+1;

	if (x == 0){
		xMin = x;
	}

	if (x == xSize - 1){
		xMax = x;
	}

	if (y == 0){
		yMin = y;
	}

	if (y == ySize -1){
		yMax = y;
	}

	for (int i = xMin; i <= xMax; i++){
		for (int j = yMin; j <= yMax; j++){
			aliveCount += cellStateGrid[i][j];
		}
	}
	if (cellStateGrid[x][y] == 1){
		aliveCount--;
	}

	return aliveCount;
}

void stateTrans(){
	for (int i =0; i < xSize; i++){
		for (int j = 0; j < ySize; j++){
			
			int alive = aliveCounter(i,j);
			int dead = 8 - alive;

			// under-population and overcrowding
			if (cellStateGrid[i][j] == 1){

				if (alive < 2 || alive > 3){
					newCellStateGrid[i][j] = 0;
				}

				if (alive == 2 || alive == 3){
					newCellStateGrid[i][j] = 1;
				}
			}

			// rebirth
			if (cellStateGrid[i][j] == 0){
				if (alive ==3){
					newCellStateGrid[i][j] = 1;
				}
				else {
					newCellStateGrid[i][j] = 0;
				}	
			}
		}
	}
	for (int i =0; i < xSize; i++){
		for (int j = 0; j < ySize; j++){
			cellStateGrid[i][j] = newCellStateGrid[i][j];
		}
	}
}

// ------------------- TEMPLATES -----------------------
// templates for testing 
void templates(int num){
	gridReset();
	switch (num){
		case 0:
			for (int i = 0; i < xSize; i++){
				cellStateGrid[xSize/2][i] = 1;
			}
			break;
		case 1:
			for (int i =0; i < xSize; i++){
				for (int j = 0; j < ySize; j++){
					if (i % 2 == 0 && j % 2 == 0){
						cellStateGrid[i][j] = 1;
					}	
				}
			}
			break;
		case 2:
			for (int i = 0; i < xSize; i++){
				for (int j = 0; j < ySize; j++){
					if (i % 2 != 0 && j % 2 != 0){
						cellStateGrid[i][j] = 1;
					}
					if (i % 2 == 0 && j % 2 == 0){
						cellStateGrid[i][j] = 1;
					}
				}
			}
			break;
		case 3:
			for (int i = 0; i < xSize; i++){
				for (int j = 0; j < ySize; j++){
					if (i == j){
						cellStateGrid[i][j] = 1;
					}
				}
			}
			break;
		case 4:
			for (int i = 0; i < xSize; i++){
				for (int j = 0; j < ySize; j++){
					if (i == j || i == ySize - j || i == ySize/2 || j == xSize/2 ){
						cellStateGrid[i][j] = 1;
					}
				}
			}
			break;
	}
}

// -----------------DISPLAY HELPERS--------------------

void setupGridLines(){
	glColor3f(0.4,0.4,0.4);

	glBegin(GL_LINES);
		for (int i = 0; i < xSize;i++){
			glVertex2i(i,0);
			glVertex2i(i,ySize);
		}
		for (int i = 0; i < ySize; i++){
			glVertex2i(0,i);
			glVertex2i(xSize,i);
		}

	glEnd();
}

// if colourValue == 0, cell is dead, if colourValue == 1
// cell is alive
void fillSpace(int cellState,int x, int y){

	y = ySize -y -1;

	glBegin(GL_POLYGON);
		if (cellState == 0){
			glColor3f(0.0,1.0,1.0);
			glVertex2i(x,y);
			glVertex2i(x,y+1);
			glVertex2i(x+1,y+1);
			glVertex2i(x+1,y);
			cellStateGrid[x][ySize -y - 1] = 0;
		}
		else {
			glColor3f(0.1,0.1,0.44);
			glVertex2i(x,y);
			glVertex2i(x,y+1);
			glVertex2i(x+1,y+1);
			glVertex2i(x+1,y);
			cellStateGrid[x][ySize -y - 1] = 1;
	}
	glEnd();
}

// Initializes the grid to the current values inside 
// cellStateGrid array
void initGrid(){
	for (int i =0; i < xSize; i++){
		for (int j = 0; j < ySize; j++){
			if (cellStateGrid[i][j] == 0){
				fillSpace(0,i,j);
			}
			else {
				fillSpace(1,i,j);
			}

		}
	}
}


// -------------------- DISPLAY-------------------------
void display(void)
 {

	glClear(GL_COLOR_BUFFER_BIT);
	initGrid();
	if (GAME == edit){

		if (cellChange == alive){
			fillSpace(1,p.x,p.y);
			cellChange = off;
		}
		if (cellChange == dead) {
			fillSpace(0,p.x, p.y);
			cellChange = off;
		}

		if (cellChange == off){
		}

	}

	if (GAME == run){
		if (RUN == play){
			stateTrans();
		}
	}
	

	setupGridLines();
 	glFlush();
}

// ---------------------KEYBOARD --------------------

void keyboard(unsigned char key, int x, int y){
	switch (key){
		case 27:
			exit(0);
			break;

		case 32:
			
			if (RUN == play){
				RUN = pause;
				GAME = edit;
			}
			else {
				RUN = play;
				GAME = run;
			}
			break;
		case 'c':
		case 'C':
			GAME = edit;
			gridReset();
			break;
		case 'r':
		case 'R':
			RUN = pause;
			GAME = edit;
			gridRandom();
			break;
	}
	
}

void special(int key, int x, int y){

	switch (key){
		case GLUT_KEY_UP:
			SPEED = turbo;
			break;
		case GLUT_KEY_DOWN:
			SPEED = verySlow;
			break;
		case GLUT_KEY_LEFT:
			SPEED = slow;
			break;
		case GLUT_KEY_RIGHT:
			SPEED = fast;
			break;
	}
	
}


// -------------------- SCREEN --------------------

void reshape(int w,int h){

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1,1,1,-1);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0,0,w,h);
	
}	

void timerRedisplay(int value){
	int speed;

	switch (SPEED){
		case verySlow:
			speed = speed0;
			break;
		case slow:
			speed = speed1;
			break;
		case fast:
			speed = speed2;
			break;
		case turbo:
			speed = speed3;
			break;
	}
	
	glutTimerFunc(speed, timerRedisplay, 0);
	glutPostRedisplay();
}

// ----------------------- MOUSE ---------------------

void stateEditClick(int x,int y){

	if (cellStateGrid[x][y] == 1){
		cellChange = dead;
	}
	if (cellStateGrid[x][y] == 0){
		cellChange = alive;
	}

}

void mouse(int btn,int state, int x, int y){
	if (state == GLUT_DOWN && btn == GLUT_LEFT_BUTTON){

		if (GAME == edit){
			p.x = (x*xSize)/xWindowSize; p.y = ySize - (y*ySize)/yWindowSize -1;
			stateEditClick(p.x,p.y);
		}
				
	}
}

void motion(int x, int y){
	
}

void passive(int x, int y){
	 // DON'T NEED any passive mouse input
}


// --------------------MENUS -----------------------------

void mainMenu(int value){
	switch (value){

		case 0:
			GAME = run;
			RUN = play;
			break;
		case 1:
			GAME = edit;
			RUN = pause;
			break;
		case 5:
			GAME = edit;
			gridReset();
			break;
		case 6:
			RUN = pause;
			GAME = edit;
			gridRandom();
			break;
		case 7:
			exit(0);
			break;
	}
}

void speedMenu(int value){
	switch (value){

		case 0:
			SPEED = verySlow;
			break;
		case 1:
			SPEED = slow;
			break;
		case 2:
			SPEED = fast;
			break;
		case 3:
			SPEED = turbo;
			break;
	}
}

void templatesMenu(int value){
	switch (value){

		case 0:
			RUN = pause;
			GAME = edit;
			templates(0);
			break;
		case 1:
			RUN = pause;
			GAME = edit;
			templates(1);
			break;
		case 2:
			RUN = pause;
			GAME = edit;
			templates(2);
			break;
		case 3:
			RUN = pause;
			GAME = edit;
			templates(3);
			break;
		case 4:
			RUN = pause;
			GAME = edit;
			templates(4);
			break;
	}
}

void initMenu(){

	int mainID = glutCreateMenu(mainMenu);
	int speedID = glutCreateMenu(speedMenu);
	int templatesID = glutCreateMenu(templatesMenu);

	glutSetMenu(mainID);
	glutAddMenuEntry("PLAY (SPACE)", 0);
	glutAddMenuEntry("PAUSE/EDIT (SPACE)", 1);
	glutAddSubMenu("SPEED (ARROW-KEYS)", speedID);
	glutAddSubMenu("TEMPLATES", templatesID);
	glutAddMenuEntry("CLEAR (C)", 5);
	glutAddMenuEntry("RANDOM (R)", 6);
	glutAddMenuEntry("QUIT (ESC)", 7);

	glutSetMenu(speedID);
	glutAddMenuEntry("VERY SLOW (DOWN)",0);
	glutAddMenuEntry("SLOW (LEFT)",1);
	glutAddMenuEntry("FAST (RIGHT)",2);
	glutAddMenuEntry("TURBO (UP)",3);

	glutSetMenu(templatesID);
	glutAddMenuEntry("VERTICAL LINE",0);
	glutAddMenuEntry("INDIVIDUAL",1);
	glutAddMenuEntry("CHECKERBOARD",2);
	glutAddMenuEntry("DIAGONAL LINE",3);
	glutAddMenuEntry("MULTIPLE LINES",4);

	glutSetMenu(mainID);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}

// ------------------- CALLBACKS -----------------------

void glutCallbacks(){
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(passive);

}

//---------------------------MAIN-------------------------//

int main(int argc, char** argv){

	// creates a seed in which the randomizor uses
	srand(time(NULL));

	// sets up the cellStateGrid double array to
	// have all values initially equal to zero (dead)
	gridReset(); 

	glutInit(&argc, argv);		
	glutInitWindowSize(xWindowSize,yWindowSize);
	glutCreateWindow("Game of Life");	
	
	gluOrtho2D(0,xSize,0,ySize);

	glutCallbacks();

	initMenu();
	timerRedisplay(0);

	printf("\n----------CONTROLS----------\n\n--KEYBOARD--\n\nESC: Quit the program.\n\nC: Clears the grid.\n\nSPACE: Plays program if paused, pauses program and enters edit mode if playing.\n\nR: Randomizes the grid, Enters PAUSE/EDIT mode.\n\nArrow Keys: Adjusts speed\n\tDown - Slowest speed\n\tLeft - Slow speed\n\tRight - Fast speed\n\tUp - Fastest speed\n\n--MOUSE--\n\nLeft Click: Highlight/Unhighlight cell.\n\nRight Click: Enter Menu.\n\n--INFO--\n\nConway's Game of Life rules:\n\n1. Any live cell with fewer than two live neighbours dies, as if caused by underpopulation.\n2. Any live cell with more than three live neighbours dies, as if by overpopulation.\n3. Any live cell with two or three live neighbours lives on to the next generation.\n4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.\n\nLight blue cells are dead.\nDark blue cells are alive\n\nGame starts in PAUSE/EDIT mode.\nGrid can not be edited while in PLAY mode.\n\nGrid is 51x51 cells.\n\n--TEMPLATES--\n\nThe templates option in the menu will give some starting sequences to observe, if wanted.\n\n");

	glutMainLoop();				

	return(0);					
}