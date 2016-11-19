/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, standard IO, and strings
#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window
bool init();

//Frees media and shuts down SDL
void close();

//Draws scene
void drawScene(int offset);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
	
//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//Current displayed image
SDL_Surface* gStretchedSurface = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface( gWindow );
		}
	}
    
    drawScene(0);

	return success;
}


void close()
{
	//Free loaded image
	SDL_FreeSurface( gStretchedSurface );
	gStretchedSurface = NULL;

	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

void drawScene(int offset)
{
    int *pPixels = (int *)gScreenSurface->pixels;
    
    //Draw background
    
    for (int i=0; i<76800*4; i++)
    {
        *pPixels=0x00990022;
        pPixels += 1;
    }
    
    //Draw cat head outline
    
    pPixels = (int *)gScreenSurface->pixels;
    
    int width = gScreenSurface->pitch/4;
    
    
    pPixels[offset*width+2] = 0x00ffcc66;
    pPixels[offset*width+6] = 0x00ffcc66;
    
    for (int i = 1; i < 5; i++)
    {
        for (int j = 1; j < 8; j++)
        {
            pPixels[offset*width+width*i+j] = 0x00ffcc66;
        }
    }
    
    //Draw eyes
    
    pPixels[offset*width+width*2+3] = 0x0099ccff;
    pPixels[offset*width+width*2+5] = 0x0099ccff;
    
    //Draw nose
    
    pPixels[offset*width+width*3+4] = 0x00ffcccc;
    
    //Draw whiskers
    
    for (int i = 0; i < 9; i++)
    {
        if (i < 3 or i > 5)
        {
            pPixels[offset*width+width*3+i] = 0x00e6e6e6;
        }
    }
    
}



int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
        //Main loop flag
        bool quit = false;
        int row = 0;
        bool countdown = true;
        int catHeight = 5;

        //Event handler
        SDL_Event e;

        //While application is running
        while( !quit )
        {
            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                if( e.type == SDL_QUIT )
                {
                    quit = true;
                }
            }
            
            if (row == 0 && countdown == false)
            {
                countdown = true;
            }
            else if (row == SCREEN_HEIGHT-catHeight-1 && countdown == true)
            {
                countdown = false;
            }
                
            if (countdown)
            {
                row++;
            }
            else
            {
                row--;
            }
            
            drawScene(row);
        
            //Update the surface
            SDL_UpdateWindowSurface( gWindow );
            SDL_Delay(50);
        }
	}

	//Free resources and close SDL
	close();

	return 0;
}