/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, standard IO, and strings
#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include <iostream>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window
bool init();

//Frees media and shuts down SDL
void close();

//Draws scene
void drawScene(int row, int col, SDL_Surface* stamp);

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

void drawScene(int row, int col, SDL_Surface* pStamp)
{

    int *pPixels = (int *)gScreenSurface->pixels;
    int width = gScreenSurface->pitch;
    int height = SCREEN_HEIGHT;
    
    //Draw background
    
    for (int i=0; i<width*height/4; i++)
    {
        *pPixels=0x00990022;
        pPixels += 1;
    }
    
    //Draw postage stamp
    
    int *pStampPixels = (int *)pStamp->pixels;
    int stampWidth = pStamp->pitch/4;
    int stampHeight = pStamp->h;
    
    std::cout << stampWidth << "\n";
    std::cout << stampHeight << "\n";
    
    for (int i = 0; i < stampHeight; i++)
    {
        for (int j = 0; j < stampWidth; j++)
        {
            pPixels[width*i+j] = 0x00009922;
            
        }
    }
    
    //pStampPixels[stampWidth*i+j]
    
}

SDL_Surface* loadSurface( std::string path )
{
    //The final optimized image
    SDL_Surface* optimizedSurface = NULL;
    
    //Load image at specified path
    SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
    }
    else
    {
        //Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, NULL );
        if( optimizedSurface == NULL )
        {
            printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    
    return optimizedSurface;
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
        SDL_Surface* pPostageStamp = loadSurface("/Users/emilyabrash/Documents/SDL_Test/05_optimized_surface_loading_and_soft_stretching/OptimizedSurfacesStretching/OptimizedSurfacesStretching/catface.bmp");
    
        int stampHeight = pPostageStamp->h;
        
        //Main loop flag
        bool quit = false;
        int row = 0;
        bool countdown = true;

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
            else if (row == SCREEN_HEIGHT-stampHeight-1 && countdown == true)
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
            
            drawScene(0, 0, pPostageStamp);
        
            //Update the surface
            SDL_UpdateWindowSurface( gWindow );
            SDL_Delay(5);
        }
        
        SDL_FreeSurface(pPostageStamp);
        pPostageStamp = NULL;
	}

	//Free resources and close SDL
	close();

	return 0;
}