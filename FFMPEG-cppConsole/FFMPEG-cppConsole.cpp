// FFMPEG-cppConsole.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "SDL.h"  

int _tmain(int argc, _TCHAR* argv[])
{
	SDL_Surface *hello = NULL;
	SDL_Surface *screen = NULL;

	// Start SDL  
	SDL_Init(SDL_INIT_EVERYTHING); // ��ʼ��SDL���е���ϵͳ  

	// Set up screen  
	screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE); // ��640����480��32λÿ���أ�������ڴ�������screen  

	// Load image  
	hello = SDL_LoadBMP("e:\\1.bmp"); // ����ͼƬ��ע�����ú���ȷ��ͼƬ�����·��  

	// Apply image to screen  
	SDL_BlitSurface(hello, NULL, screen, NULL); // ��ͼƬ������Ļ��  

	// Update screen  
	SDL_Flip(screen); // ˢ����Ļ�����򽫿�����ͼƬ���ʺ������  

	// Pause  
	SDL_Delay(10000); // ��ʱ10��  

	// Free the loaded image      
	SDL_FreeSurface(hello); // �ͷŵ�hello���ڴ棬screen��SDL_Quit�����л��ͷţ������뵥���ͷ�  

	// Quit SDL  
	SDL_Quit();

	return 0;
}

