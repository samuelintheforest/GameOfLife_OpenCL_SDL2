#include "DrawBoard.hpp"

unsigned char DrawBoard::s_PenColor[4] = {0, 0, 0, 0};
unsigned char DrawBoard::s_BackColor[4] = { 0, 0, 0, 0 };
int DrawBoard::s_CallingCounter = 0;
SDL_Renderer* DrawBoard::s_SDLRenderer = nullptr;
SDL_Window* DrawBoard::s_SDLWindow = nullptr;
SDL_Rect DrawBoard::s_Container = { 0, 0, 0, 0 };

void DrawBoard::SetupDrawing()
{
	if (s_CallingCounter == 0)
	{
		s_CallingCounter++;

		/*
		##### Setup SDL2 #####
		*/

		// 1. Init SDL2
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			std::cout << SDL_GetError() << std::endl;
			exit(-1);
		}

		// 2. Create Window
		DrawBoard::s_SDLWindow = SDL_CreateWindow("Életjáték", 0, 0, 100, 100, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_SHOWN);
		if (DrawBoard::s_SDLWindow == nullptr)
		{
			std::cout << SDL_GetError() << std::endl;
			exit(-1);
		}

		// 3. Create rendering context for the window
		DrawBoard::s_SDLRenderer = SDL_CreateRenderer(DrawBoard::s_SDLWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (DrawBoard::s_SDLRenderer == nullptr)
		{
			std::cout << SDL_GetError() << std::endl;
			exit(-1);
		}


	}
}

void DrawBoard::ReleaseDrawing()
{
	SDL_DestroyRenderer(DrawBoard::s_SDLRenderer);
	SDL_DestroyWindow(DrawBoard::s_SDLWindow);

	SDL_Quit();
}

void DrawBoard::StartDrawing()
{
	// Clear the screen with a basic color
	SDL_SetRenderDrawColor(DrawBoard::s_SDLRenderer, DrawBoard::s_BackColor[0], DrawBoard::s_BackColor[1], DrawBoard::s_BackColor[2], DrawBoard::s_BackColor[3]);
	SDL_RenderClear(DrawBoard::s_SDLRenderer);
	// Set pen color
	SDL_SetRenderDrawColor(DrawBoard::s_SDLRenderer, DrawBoard::s_PenColor[0], DrawBoard::s_PenColor[1], DrawBoard::s_PenColor[2], DrawBoard::s_PenColor[3]);
}

void DrawBoard::StopDrawing()
{
	// DO Something
	SDL_RenderPresent(DrawBoard::s_SDLRenderer);
}

bool DrawBoard::PollEvents()
{
	//Event handler
	SDL_Event e;
	//Handle events on queue
	while (SDL_PollEvent(&e) != 0)
	{
		//User requests quit
		if (e.type == SDL_QUIT || (e.key.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
		{
			return true;
		}


	}

	return false;
}

void DrawBoard::SetBackColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	s_BackColor[0] = r;
	s_BackColor[1] = g;
	s_BackColor[2] = b;
	s_BackColor[3] = a;
	SDL_SetRenderDrawColor(DrawBoard::s_SDLRenderer, DrawBoard::s_BackColor[0], DrawBoard::s_BackColor[1], DrawBoard::s_BackColor[2], DrawBoard::s_BackColor[3]);
}

void DrawBoard::SetPenColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	s_PenColor[0] = r;
	s_PenColor[1] = g;
	s_PenColor[2] = b;
	s_PenColor[3] = a;
	SDL_SetRenderDrawColor(DrawBoard::s_SDLRenderer, DrawBoard::s_PenColor[0], DrawBoard::s_PenColor[1], DrawBoard::s_PenColor[2], DrawBoard::s_PenColor[3]);
}
