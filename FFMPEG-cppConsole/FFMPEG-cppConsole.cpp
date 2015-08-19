// FFMPEG-cppConsole.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "SDL.h"  

int _tmain(int argc, _TCHAR* argv[])
{
	SDL_Surface *hello = NULL;
	SDL_Surface *screen = NULL;

	// Start SDL  
	SDL_Init(SDL_INIT_EVERYTHING); // 初始化SDL所有的子系统  

	// Set up screen  
	screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE); // 宽640，高480，32位每像素，在软件内存区创建screen  

	// Load image  
	hello = SDL_LoadBMP("e:\\1.bmp"); // 加载图片，注意设置好正确的图片保存的路径  

	// Apply image to screen  
	SDL_BlitSurface(hello, NULL, screen, NULL); // 将图片传到屏幕上  

	// Update screen  
	SDL_Flip(screen); // 刷新屏幕，否则将看不到图片而呈黑屏情况  

	// Pause  
	SDL_Delay(10000); // 延时10秒  

	// Free the loaded image      
	SDL_FreeSurface(hello); // 释放掉hello的内存，screen在SDL_Quit函数中会释放，故无须单独释放  

	// Quit SDL  
	SDL_Quit();

	return 0;
}

