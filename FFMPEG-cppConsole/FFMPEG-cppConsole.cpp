// FFMPEG-cppConsole.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "sdl/sdl.h"
#include "myFFMPEGLib.cpp"

int _tmain(int argc, _TCHAR* argv[])
{
	SDL_Surface *hello = NULL;
	SDL_Surface *screen = NULL;

	// Start SDL  
	SDL_Init(SDL_INIT_EVERYTHING); // ��ʼ��SDL���е���ϵͳ  

	// Set up screen  
	screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE); // ��640����480��32λÿ���أ�������ڴ�������screen  
	doSomthing(screen);
	
	return 0;
}

