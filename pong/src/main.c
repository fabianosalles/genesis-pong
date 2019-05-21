/* 
  PONG FOR SEGA GENESIS 
  Fabiano Salles
*/

#include <genesis.h>

static void update();
static void drawField();
void onJoyEvent(u16 joy, u16 changed, u16 state);


typedef struct {
	Vect2D_s16 polygon[4];
	Vect2D_s16 speed;
} Ball;

static Ball ball = {
	{
		{00, 00},
		{05, 00},
		{05, 05},
		{00, 05}
	},
	{0,0}	
};

typedef struct {
	u8 paused;
} Game;

static Game game = {0};


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
	switch (changed) {
	case BUTTON_START:
		if (state == 0) //released
			game.paused = game.paused ? FALSE : TRUE;
		break;
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

	if (game.paused) {
		BMP_drawText("PAUSED", 5, 1);
	}
	else {
		BMP_clearText(5, 1, 6);
		if (!BMP_isPolygonCulled(ball.polygon, 4)) {
			BMP_drawPolygon(ball.polygon, 4, 0xFF);
			translate2D(ball.polygon, 4, ball.speed);
		}
	}	
}



static void update() {

}
