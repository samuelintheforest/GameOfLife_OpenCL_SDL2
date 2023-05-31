#pragma once

#include <SDL.h>
#include <iostream>

class DrawBoard
{
public:
	static unsigned char s_PenColor[4];
	static unsigned char s_BackColor[4];
private:
	static int s_CallingCounter;
	static SDL_Window* s_SDLWindow;
	static SDL_Renderer* s_SDLRenderer;
	static SDL_Rect s_Container;

public:
	static void SetupDrawing();
	static void StartDrawing();
	static void StopDrawing();
	static void ReleaseDrawing();
	static void SetPenColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	static void SetBackColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	inline static void FillRect(int x, int y, int w, int h)
	{
		s_Container = { x, y, w, h };
		SDL_RenderFillRect(DrawBoard::s_SDLRenderer, &s_Container);
	}
	static bool PollEvents();
private:
	
};

