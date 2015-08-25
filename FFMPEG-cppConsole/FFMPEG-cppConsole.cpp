// FFMPEG-cppConsole.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "sdl/sdl.h"
#include "myFFMPEGLib.cpp"
#include "main1.cpp"

int _tmain(int argc, _TCHAR* argv[])
{
	SDL_Surface *hello = NULL;
	SDL_Surface *screen = NULL;

	// Start SDL  
	SDL_Init(SDL_INIT_EVERYTHING); // 初始化SDL所有的子系统  

	// Set up screen  
	screen = SDL_SetVideoMode(1000, 1000, 32, SDL_SWSURFACE); // 宽640，高480，32位每像素，在软件内存区创建screen  
	doSomthing(screen);
	
	return 0;
}

