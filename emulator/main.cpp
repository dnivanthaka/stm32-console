#include <stdio.h>
#include "SDL2/SDL.h"


#define KEYPAD_UP(val)    (!(val & (1 << 0)))
#define KEYPAD_DOWN(val)  (!(val & (1 << 1)))
#define KEYPAD_LEFT(val)  (!(val & (1 << 2)))
#define KEYPAD_RIGHT(val) (!(val & (1 << 3)))
#define KEYPAD_A(val)     (!(val & (1 << 4)))
#define KEYPAD_B(val)     (!(val & (1 << 5)))
#define KEYPAD_SELECT(val) (!(val & (1 << 7)))
#define KEYPAD_START(val)  (!(val & (1 << 6)))

const int SCREEN_WIDTH = 128 * 5;
const int SCREEN_HEIGHT = 64 * 5;


const int VIEWPORT_WIDTH = 128;
const int VIEWPORT_MID_WIDTH = VIEWPORT_WIDTH / 2;
const int VIEWPORT_HEIGHT = 64;
const int VIEWPORT_MID_HEIGHT = VIEWPORT_HEIGHT / 2;


const static int FRAME_RATE = 60;
const static int TICKS_PER_FRAME = 1000 / FRAME_RATE;

static uint8_t keystate = 0xff;

typedef struct coord_t {
    int x, y;
} coord_t;

typedef struct entity_t {
    uint8_t w, h;
    int x_vel, y_vel;
    coord_t pos, prev1, prev2;
} entity_t;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
SDL_Renderer *m_pRenderer;
	
//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

bool game_running = true;

uint8_t pixels[1024] = {1};

void rcc_vsync_wait(uint16_t count){
    for(uint16_t i=0;i<count;i++){
        SDL_Delay(TICKS_PER_FRAME);
    }
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow( "LCD Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface( gWindow );
            m_pRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            SDL_SetRenderDrawColor( m_pRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

		}
	}

	return success;
}

void close()
{
	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

void putpixel(int x, int y, int color){
    if( x >= VIEWPORT_WIDTH || x < 0 || y >= VIEWPORT_HEIGHT || y < 0){
        return;
    }

    if(color == 1){
        SDL_SetRenderDrawColor(m_pRenderer, 60, 65, 44, 0xff);
    }else{
        SDL_SetRenderDrawColor( m_pRenderer, 168, 198, 78, 0xFF );
    }

    for(int i=1;i<=4;i++){
        for(int j=1;j<=4;j++){
            SDL_RenderDrawPoint(m_pRenderer, i + (x * 5), j + (y * 5));
        }
    }
}

void draw_grid(){
    SDL_SetRenderDrawColor( m_pRenderer, 168, 198, 78, 0xFF );

	//Clear screen
	SDL_RenderClear( m_pRenderer );

    SDL_SetRenderDrawColor(m_pRenderer, 0xff, 0xff, 0xff, 0xff);
                
    for (int i = 0; i < SCREEN_HEIGHT; i = i + 5){
        for (int j = 0; j < SCREEN_WIDTH; j++)
            SDL_RenderDrawPoint(m_pRenderer, j, i);
    }

    for (int i = 0; i < SCREEN_WIDTH; i = i + 5){
        for (int j = 0; j < SCREEN_HEIGHT; j++)
            SDL_RenderDrawPoint(m_pRenderer, i, j);
    }
}

void draw_entity(entity_t *entity, uint8_t color){
    for(uint8_t i=0;i<entity->h;i++){
        for(uint8_t j=0;j<entity->w;j++){
            putpixel(entity->pos.x + j, entity->pos.y + i, color);
        }
    }
}

uint8_t keypad_getstate(){
    return keystate;
}

int main( int argc, char* args[] )
{
    SDL_Event e;
    uint32_t fpsTimer, capTimer;
    uint16_t frame = 0;

    entity_t paddle;
    paddle.w = 13;
    paddle.h = 2;
    paddle.pos.x = 64 - 6; //mid width
    paddle.pos.y = 64 - 4;
    paddle.prev1.x = paddle.pos.x;
    paddle.prev1.y = paddle.pos.y;
    paddle.prev2.x = paddle.pos.x;
    paddle.prev2.y = paddle.pos.y;
    paddle.x_vel = 0;
    paddle.y_vel = 0;

    entity_t ball;
    ball.w = 4;
    ball.h = 4;
    //ball.pos.x = SCREEN_WIDTH_MID - 1;
    ball.pos.x = rand() % 128;
    ball.pos.y = 0;
    ball.prev1.x = ball.pos.x;
    ball.prev1.y = ball.pos.y;
    ball.prev2.x = ball.pos.x;
    ball.prev2.y = ball.pos.y;
    ball.x_vel = 2;
    ball.y_vel = 2;

    entity_t old_paddle, old_ball;

    old_paddle = paddle;
    old_ball = ball;

    draw_entity(&paddle, 1);

	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
        //const unsigned int texWidth = SCREEN_WIDTH;
        //const unsigned int texHeight = SCREEN_HEIGHT;

        draw_grid();

		while(game_running){
            capTimer = SDL_GetTicks();
            //Handle events on queue
				if( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						game_running = false;
					}else if(e.type == SDL_KEYDOWN){
                        if(e.key.keysym.scancode == SDL_SCANCODE_LEFT){
                             //SDL_Log("LEFT DOWN");
                             keystate &= ~(1 << 2);
                        }else if(e.key.keysym.scancode == SDL_SCANCODE_RIGHT){
                             //SDL_Log("RIGHT DOWN");
                             keystate &= ~(1 << 3);
                        }else if(e.key.keysym.scancode == SDL_SCANCODE_UP){
                             //SDL_Log("UP DOWN");
                             keystate &= ~1;
                        }else if(e.key.keysym.scancode == SDL_SCANCODE_DOWN){   
                            //SDL_Log("DOWN DOWN");
                            keystate &= ~(1 << 1);
                        }
                    }else if(e.type == SDL_KEYUP){
                        if(e.key.keysym.scancode == SDL_SCANCODE_LEFT){
                             //SDL_Log("LEFT UP");
                             keystate |= 1 << 2;
                        }else if(e.key.keysym.scancode == SDL_SCANCODE_RIGHT){
                             //SDL_Log("RIGHT UP");
                             keystate |= 1 << 3;
                        }else if(e.key.keysym.scancode == SDL_SCANCODE_UP){
                            keystate |= 1;
                        }else if(e.key.keysym.scancode == SDL_SCANCODE_DOWN){
                            keystate |= 1 << 1;
                        }
                    }else{
                        keystate = 0xff;
                    }
				}

                paddle.x_vel = 0;
                paddle.y_vel = 0;

                uint8_t tmp = keypad_getstate();
                //printf("%X\n",tmp);

                draw_grid();

                if(KEYPAD_UP(tmp)) {
                    //gpio_out(GPIOA, 0, 1);
                    //y_vel = -1;
                }else if(KEYPAD_DOWN(tmp)){
                    //gpio_out(GPIOA, 1, 1);
                    //y_vel = 1;
                }else if(KEYPAD_LEFT(tmp)){
                    //gpio_out(GPIOA, 0, 1);
                    //x_vel = -1;
                    paddle.x_vel = -4;
                }else if(KEYPAD_RIGHT(tmp)){
                    //gpio_out(GPIOA, 1, 1);
                    //x_vel = 1;
                    paddle.x_vel = 4;
                }else if(KEYPAD_A(tmp)){
                    //gpio_out(GPIOA, 0, 1);
                }else if(KEYPAD_B(tmp)){
                    //gpio_out(GPIOA, 1, 1);
                }else if(KEYPAD_SELECT(tmp)){
                    //gpio_out(GPIOA, 0, 1);
                }else if(KEYPAD_START(tmp)){
                    //gpio_out(GPIOA, 1, 1);
                }else{
                    //gpio_out(GPIOA, 0, 0);
                    //gpio_out(GPIOA, 1, 0);
                    //x_vel = 0;
                    //y_vel = 0;
                }

                if(paddle.pos.x + paddle.x_vel + paddle.w >= VIEWPORT_WIDTH){
                    //x_vel = -1;
                    paddle.x_vel = 0;
                    paddle.pos.x = VIEWPORT_WIDTH - paddle.w;
                }else if(paddle.pos.x + paddle.x_vel < 0){
                    //x_vel = 1;
                    paddle.x_vel = 0;
                    paddle.pos.x = 0;
                }
                
                //SDL_SetRenderDrawColor(m_pRenderer, 60, 65, 44, 0xff);

                //x, y
                /*for(int x=0;x<128;x++){
                    for(int y=0;y<32;y++){
                        putpixel(x, y, 1);
                    }
                }*/
                if(ball.pos.x + ball.w > 128){
                    //x_vel = -1;
                    ball.x_vel = -2;
                    //ball.pos.x = SCREEN_WIDTH;
                }
                if(ball.pos.x + ball.x_vel < 0){
                    //x_vel = 1;
                    ball.x_vel = 2;
                    //ball.pos.x = -1;
                }
                //if(ball.pos.y + ball.y_vel + ball.h > SCREEN_HEIGHT){
                if(ball.pos.y + ball.h > 64){
                    //x_vel = -1;
                    ball.y_vel = -2;
                    //ball.pos.y = SCREEN_HEIGHT;
                }
                if(ball.pos.y + ball.y_vel < 0){
                    //x_vel = 1;
                    ball.y_vel = 2;
                    //ball.pos.y = -1;
                }


                if(frame % 4 == 0){
                    ball.prev2.x = ball.prev1.x;
                    ball.prev2.y = ball.prev1.y;

                    ball.prev1.x = ball.pos.x;
                    ball.prev1.y = ball.pos.y;

                    ball.pos.x += ball.x_vel;
                    ball.pos.y += ball.y_vel;

                    //remove last object
                    draw_entity(&old_ball, 0);
                    
                    draw_entity(&ball, 1);
                    
                    old_ball = ball;
                    //delay_ms(25);
                    //rcc_vsync_wait(2);
                }

                if(frame % 1 == 0){
                        paddle.prev2.x = paddle.prev1.x;
                        paddle.prev2.y = paddle.prev1.y;

                        paddle.prev1.x = paddle.pos.x;
                        paddle.prev1.y = paddle.pos.y;

                        paddle.pos.x += paddle.x_vel;
                        paddle.pos.y += paddle.y_vel;
                            //paddle.pos.x += paddle.x_vel;
                            //paddle.pos.y += paddle.y_vel;

                        //st7565r_putpixel(x, y, 1);
                            draw_entity(&old_paddle, 0);
                            draw_entity(&paddle, 1);
                            old_paddle = paddle;
                            //rcc_vsync_wait(4);
                            //frame = 0;
                            //continue;
                 }

				//Update screen
				SDL_RenderPresent( m_pRenderer );
                frame++;

            int frameTicks = SDL_GetTicks() - capTimer;
            if(frameTicks < TICKS_PER_FRAME){
                
                //TODO Write a better frame rate handler, this is not the proper way
                SDL_Delay(TICKS_PER_FRAME - frameTicks);
            }

        }
	}

	//Free resources and close SDL
	close();

	return 0;
}
