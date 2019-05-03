#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <bits/stdc++.h>

using namespace std;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 667;


struct LTexture
{
		LTexture();

		~LTexture();

		bool loadFromFile( std::string path );

		void free();

		void render( double x, double y );

		int getWidth();
		int getHeight();

		SDL_Texture* mTexture;

		int mWidth;
		int mHeight;
};

bool init();

bool loadMedia();

void close();

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

LTexture gPlaneTexture, newPlaneTexture;
LTexture gBackgroundTexture;


LTexture::LTexture()
{
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	free();
}

bool LTexture::loadFromFile( std::string path )
{
	free();

	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		cout<< "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError();
	}
	else
	{
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0xFF, 0xFF, 0xFF ) );

        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			cout<< "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError();
		}
		else
		{
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		SDL_FreeSurface( loadedSurface );
	}

	mTexture = newTexture;
	return mTexture != NULL;
}

void LTexture::free()
{
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::render( double x, double y )
{
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	SDL_RenderCopy( gRenderer, mTexture, NULL, &renderQuad );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

bool init()
{
	bool success = true;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		cout<< "SDL could not initialize! SDL Error: %s\n", SDL_GetError();
		success = false;
	}
	else
	{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			cout<< "Warning: Linear texture filtering not enabled!";
		}

		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			cout<< "Window could not be created! SDL Error: %s\n", SDL_GetError();
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				cout<< "Renderer could not be created! SDL Error: %s\n", SDL_GetError();
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					cout<< "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError();
					success = false;
				}
			}
		}
	}

	return success;
}


struct Plane
{
		static const int PLANE_WIDTH = 20;
		static const int PLANE_HEIGHT = 20;

		static const int DOT_VEL = 2;

		Plane();
		Plane(int posx, int posy, double vX, double vY)
		{
		    mPosX = posx;
		    mPosY = posy;
		    mVelX = vX;
		    mVelY = vY;
		}

		void handleEvent( SDL_Event& e );

		void move();

		void render();

        void render2();

        int get_PosX();

        int get_PosY();

        bool check_crash(Plane B);

        double mPosX,mPosY;

		double mVelX, mVelY;
};


Plane::Plane()
{
    mPosX = 0;
    mPosY = 300;

    mVelX = 0;
    mVelY = 0;
}

void Plane::handleEvent( SDL_Event& e )
{
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY -= DOT_VEL; break;
            case SDLK_DOWN: mVelY += DOT_VEL; break;
            case SDLK_LEFT: mVelX -= DOT_VEL; break;
            case SDLK_RIGHT: mVelX += DOT_VEL; break;
        }
    }
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY += DOT_VEL; break;
            case SDLK_DOWN: mVelY -= DOT_VEL; break;
            case SDLK_LEFT: mVelX += DOT_VEL; break;
            case SDLK_RIGHT: mVelX -= DOT_VEL; break;
        }
    }
}

void Plane::move()
{
    mPosX += 0.7 * mVelX;

    if( ( mPosX < 0 ) || ( mPosX + PLANE_WIDTH > SCREEN_WIDTH ) )
    {
        mPosX -= 0.7 * mVelX;
    }

    mPosY += 0.7 * mVelY;

    if( ( mPosY < 0 ) || ( mPosY + PLANE_HEIGHT > SCREEN_HEIGHT ) )
    {
        mPosY -= 0.7 * mVelY;
    }
}

int Plane::get_PosX()
{
   return mPosX;
}

int Plane::get_PosY()
{
   return mPosY;
}

bool Plane::check_crash(Plane B)
{
    if (abs(mPosX - B.get_PosX()) <= 100 && abs(mPosY - B.get_PosY()) <= 100)
        return 1;
    return 0;
}

void Plane::render()
{
	gPlaneTexture.render( mPosX, mPosY );
}

void Plane::render2()
{
	newPlaneTexture.render( mPosX, mPosY );
}

bool loadMedia()
{
	bool success = true;

	if( !gPlaneTexture.loadFromFile( "plane1.png" ) )
	{
		cout<< "Failed to load plane1 texture image!\n";
		success = false;
	}
    if( !newPlaneTexture.loadFromFile( "plane2.png" ) )
	{
		cout<< "Failed to load plane1 texture image!\n";
		success = false;
	}
	if( !gBackgroundTexture.loadFromFile( "background.jpg" ) )
	{
		cout<< "Failed to load background texture image!\n";
		success = false;
	}

	return success;
}

void close()
{
	gPlaneTexture.free();
	gBackgroundTexture.free();

	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	IMG_Quit();
	SDL_Quit();
}

void init_plane(Plane* plane)
{
    for(int i = 0; i < 5; i++)
        plane[i] = Plane(950, rand() % 6 * 111 + rand() % 111, -1 - (rand() % 10)/20.0, 0);
}

void _move_plane(Plane* plane)
{
    for(int i = 0; i < 5; i++){
        plane[i].move();
        if (plane[i].get_PosX() == 0)
                if (rand() % 271 == 0) plane[i] = Plane(950 - rand() % 100, rand() % 6 * 111 + rand() % 100, -1 - (rand() % 10)/20.0, 0);
            plane[i].render2();
    }
}

bool check(Plane P, Plane* plane)
{
    for(int i = 0; i < 5; i++)
        if (P.check_crash(plane[i])) return 1;
    return 0;
}

int main( int argc, char* args[] )
{
	if( !init() )
	{
		cout<< "Failed to initialize!\n";
	}
	else
	{
		if( !loadMedia() )
		{
			cout<< "Failed to load media!\n";
		}
		else
		{
            int start = SDL_GetTicks();
		    Plane Plane, plane[5];

			bool quit = false;

			SDL_Event e;

			init_plane(plane);

			while( !quit )
			{

				while( SDL_PollEvent( &e ) != 0 )
				{

					if( e.type == SDL_QUIT )
					{
						quit = true;

					}
					Plane.handleEvent(e);
				}

                Plane.move();
				SDL_RenderClear( gRenderer );

				gBackgroundTexture.render( 0, 0 );

				 _move_plane(plane);

				Plane.render();

				SDL_RenderPresent( gRenderer );

                if (check(Plane, plane))
                    quit = 1;

			}
			int recent = SDL_GetTicks();
            cout << "Time: " << (recent - start)/60000 << " : " << (recent - start)/1000 << '\n';
		}
	}

	close();

	return 0;
}
