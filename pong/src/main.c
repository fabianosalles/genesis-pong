/* 
  PONG FOR SEGA GENESIS 
  Fabiano Salles
*/

#include <genesis.h>

#define BALL_WIDTH 6
#define BALL_HEIGHT 6


static void reset();
static void update();
static void drawField();
void onJoyEvent(u16 joy, u16 changed, u16 state);


typedef struct {
	Vect2D_s16 speed;
	Vect2D_s16 position;
	Vect2D_s16 polygon[4];
} Ball;

typedef struct {
	Vect2D_s16 speed;
	Vect2D_s16 position;
	Vect2D_s16 polygon[4];
} Paddle;
	

typedef struct {
	u8 paused;
} Game;


static Ball ball = { 0 };
static Game game = { 0 };
static Paddle paddles[2];


static const Line field_bounds[4] = {
	{ {0,0}, {BMP_WIDTH, 0}, 0xFF },
	{ {BMP_WIDTH - 1,0}, {BMP_WIDTH - 1, BMP_HEIGHT}, 0xFF },
	{ {BMP_WIDTH, BMP_HEIGHT - 1}, {0, BMP_HEIGHT - 1}, 0xFF },
	{ {0,BMP_HEIGHT}, {0, 0}, 0xFF }
};

int main(void) {
	VDP_setScreenHeight224(); //320 x 224
	BMP_init(TRUE, PLAN_A, PAL0, FALSE);	
	JOY_setEventHandler(&onJoyEvent);
	reset();
	while (1) {
		BMP_waitFlipComplete();
		BMP_clear();
		update();
		drawField();	
		BMP_flip(TRUE);
	}
	return 0;
}

void onJoyEvent(u16 joy, u16 changed, u16 state) {

	if (state == 0) //released
	{
		switch (changed) {
		case BUTTON_START:
			game.paused = game.paused ? FALSE : TRUE;
			break;
		case BUTTON_A:

			break;
		case BUTTON_B:
			break;

		case BUTTON_UP:
			ball.speed.y += 1;
			break;

		case BUTTON_DOWN:
			ball.speed.y -= 1;
			break;

		case BUTTON_RIGHT:
			ball.speed.x += 1;
			break;		

		case BUTTON_LEFT:
			ball.speed.x -= 1;
			break;
		}
	}
}


static void translate2D(Vect2D_s16 *points, u16 num, Vect2D_s16 displacement) {
	for(u16 i=0; i<num; i++){
		points[i].x += displacement.x;
		points[i].y += displacement.y;
	}
}


static void drawField() {	
	BMP_drawLine((Line *)(&field_bounds[0]));
	BMP_drawLine((Line *)(&field_bounds[1]));
	BMP_drawLine((Line *)(&field_bounds[2]));
	BMP_drawLine((Line *)(&field_bounds[3]));		
	BMP_clearText(5, 1, 6);
	if (game.paused) {
		BMP_drawText("PAUSED", 5, 1);
	}
	else {		
		if (!BMP_isPolygonCulled(ball.polygon, 4)) {
			BMP_drawPolygon(ball.polygon, 4, 0xFF);			
		}
	}	

	BMP_showFPS(1);
}

static void updateBallPolygon() {
	// clockwise polygon vertexes 
	ball.polygon[0].x = ball.position.x;
	ball.polygon[0].y = ball.position.y;

	ball.polygon[1].x = ball.position.x + BALL_WIDTH;
	ball.polygon[1].y = ball.position.y;

	ball.polygon[2].x = ball.position.x + BALL_WIDTH;
	ball.polygon[2].y = ball.position.y + BALL_HEIGHT;

	ball.polygon[3].x = ball.position.x;
	ball.polygon[3].y = ball.position.y + BALL_HEIGHT;
}

static void updateBall() {
	ball.position.x += ball.speed.x;
	ball.position.y += ball.speed.y;

	if ((ball.position.y < (BALL_HEIGHT-1)) || (ball.position.y > BMP_HEIGHT - BALL_HEIGHT))
		ball.speed.y = -ball.speed.y;
	
	if ((ball.position.x < (BALL_WIDTH-1)) || (ball.position.x > BMP_WIDTH - BALL_WIDTH))
		ball.speed.x = -ball.speed.x;
}

static void reset() {
	ball.position.x = (BMP_WIDTH  >> 1) - (BALL_WIDTH >> 1);
	ball.position.y = (BMP_HEIGHT >> 1) - (BALL_HEIGHT >> 1);
	ball.speed.x = 4;
	ball.speed.y = 4;
	updateBallPolygon();	
}

static void update() {
	if (!game.paused) {
		updateBall();
		updateBallPolygon();
	}
}
