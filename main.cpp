#include <GL\freeglut.h>
#include <time.h>

#define WIDTH 70  //width of the field
#define HEIGHT 30  //height of the field

class game{
public:
	int delay;
	float scale;
	int WinScale;
	int WinPosX;
	int WinPosY;
	int StartPosX;
	int StartPosY;
	int StartSize;
	game(){
	StartSize = 3;  // size os the snake in blocks
	StartPosX = (int)(WIDTH/2);  //coordinates of snake head
	StartPosY = (int)(HEIGHT/2);
	delay = 100;  //delay
	scale = 1; 
	WinScale = 5;
	WinPosX = 200;  //placement of window on the screen
	WinPosY = 100;
	}
	void init();
}settings;  //contains all settings

class block {  //cannot exceed the maximum blocks of a field
public:
	int x;
	int y;
	block(){
	x = -1;
	y = -1;
	}
void draw();  //draws
}blocks[WIDTH*HEIGHT];

class food{
public:
	int x;
	int y;
	food(){
	x = -1;
	y = -1;
	}
	void draw_food();  //draws 
	void spawn();  //spawns
}food; 

class snake{
public:
	int direction; //direction of the snake
	int size;
	bool turn;  //cant turn more then once in 1 turn
	snake(){
	turn = false;
	size = settings.StartSize;
	direction = GLUT_KEY_UP;
	}
	void DrawSnake();  //draws a snake
	void move();
	void cut();  
	void eat();
}player;

void food::draw_food(){  //draws a block through 4 dots
glColor3f(1,1,1);  //RGB color white
glVertex2f(x*settings.scale,y*settings.scale);
glVertex2f(x*settings.scale + settings.scale,y*settings.scale);
glVertex2f(x*settings.scale + settings.scale,y*settings.scale + settings.scale);
glVertex2f(x*settings.scale,y*settings.scale + settings.scale);
}

void block::draw(){  //draws a block through 4 dots
glColor3f(1,1,1);  //RGB color white
glVertex2f(x*settings.scale,y*settings.scale);
glVertex2f(x*settings.scale+settings.scale,y*settings.scale);
glVertex2f(x*settings.scale+settings.scale,y*settings.scale+settings.scale);
glVertex2f(x*settings.scale,y*settings.scale+settings.scale);
}

void snake::move(){
	for(int i = size; i > 0; i--){  //the drawing of the snake from tail to head
		blocks[i].x = blocks[i-1].x;
		blocks[i].y = blocks[i-1].y;
	}
	switch (direction){  //switches the direction of the movement
	case GLUT_KEY_UP :
		blocks[0].y++;
		break;
	case GLUT_KEY_DOWN :
		blocks[0].y--;
		break;
	case GLUT_KEY_LEFT :
		blocks[0].x--;
		break;
	case GLUT_KEY_RIGHT :
		blocks[0].x++;
		break;
	}
	if(blocks[0].x < 0)blocks[0].x += WIDTH;
	if(blocks[0].x >= WIDTH)blocks[0].x -= WIDTH;
	if(blocks[0].y < 0)blocks[0].y += HEIGHT;
	if(blocks[0].y >= HEIGHT)blocks[0].y -= HEIGHT;
}

void snake::DrawSnake(){  //draws a snake with extra block
	for(int i = 0; i < size; i++){
		blocks[i].draw();
	}
}

void game::init(){  //positioning
blocks[0].x = StartPosX;
blocks[0].y = StartPosY;
}

void snake::eat(){
	if((blocks[0].x == food.x)&&(blocks[0].y == food.y)){  //checks if the coordinates of the head are equeal to foods
	size++;  //if yes , increase in size
	Beep(523,500); //beeps when it eats food
	food.spawn(); //and spawn food
	}
}

void snake::cut(){  //snake eats its own tale
	for(int i = 3; i < size; i++){  
		if((blocks[0].x == blocks[i].x)&&(blocks[0].y == blocks[i].y)){
			size = i;
			break;
		}
	}
}

void food::spawn(){  //spawns food strictly in the field and cant be on the snake it self
	x = (rand()%WIDTH);
	y = (rand()%HEIGHT);
	for(int i = 0; i < player.size; i++){
		if((x == blocks[i].x)&&(y == blocks[i].y))spawn();
	}
}

void keyboard(int key, int x,int y){  //pressing only 1 key is allowed and doesnt allow to go backwards
	switch(key){
	case GLUT_KEY_UP :
		if((player.direction != GLUT_KEY_DOWN)&&(player.turn))player.direction = GLUT_KEY_UP;
		player.turn = false;
		break;
	case GLUT_KEY_DOWN :
		if((player.direction != GLUT_KEY_UP)&&(player.turn))player.direction = GLUT_KEY_DOWN;
		player.turn = false;
		break;
	case GLUT_KEY_LEFT :
		if((player.direction != GLUT_KEY_RIGHT)&&(player.turn))player.direction = GLUT_KEY_LEFT;
		player.turn = false;
		break;
	case GLUT_KEY_RIGHT :
		if((player.direction != GLUT_KEY_LEFT)&&(player.turn))player.direction = GLUT_KEY_RIGHT;
		player.turn = false;
		break;
	}
}

void Timer (int value){
	player.turn = true;  //every cicle allows snake to make a turn
	player.move();
	player.cut();
	player.eat();
	glutPostRedisplay();
	glutTimerFunc(settings.delay,Timer,0);  
}

void draw(){
	glClear(GL_COLOR_BUFFER_BIT);  //clears the window
	glBegin(GL_QUADS);  //every 4 dots is a square
	food.draw_food();  //draws food
	player.DrawSnake();  //draws snake
	glEnd();  //ends session with squares
	glutSwapBuffers();  //allows us to see the snake after moving
}

int main (int argc, char ** argv){  
	settings.init();  //initial position of the snakes head
	food.spawn();  //spawns food
	srand(time(NULL));  //generates random number
	glutInit(&argc, argv);   //makes windows
	glutInitDisplayMode(GLUT_RGB);  //modification of the screen
	glutInitWindowSize(WIDTH*settings.WinScale, HEIGHT*settings.WinScale);  // width of the window
	glutInitWindowPosition(settings.WinPosX, settings.WinPosY);  // placement of the window
	glutCreateWindow("GCU SNAKE");   //creates a window
	glutDisplayFunc(draw);   //whats going to be drawn on the screen
	glutTimerFunc(settings.delay,Timer,0); // delay
	glutSpecialFunc(keyboard);  //keyboards settings
	glMatrixMode(GL_PROJECTION); //generates pictures 
	glLoadIdentity();  //clears pictures
	glOrtho(0,WIDTH,0,HEIGHT,-1,1);  // minimum and maximum X and Y
	glutMainLoop();  //cicle
	PlaySound("Jungle Sounds.mp3");
		return(0);
}