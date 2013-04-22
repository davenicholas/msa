/* Mariana's Space Adventure
 * Copyright (C) Dave Nicholas 2010 <dave@davenicholas.me.uk>
 * 
 * Mariana's Space Adventure is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Mariana's Space Adventure is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 

#ifdef WIN32
#include <windows.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>	
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>

#include "main.h"

#ifdef WIN32
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLu32.lib")
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "SDL.lib")	
#endif	

										
AppStatus application_status;														
SDL_Surface *screen;


bool InitTimers(Uint32 *C) {
	*C = SDL_GetTicks();													

	return true;														
}

void TerminateApplication(void)	{
	static SDL_Event event;													

	event.type = SDL_QUIT;													

	if(SDL_PushEvent(&event) == -1)	{
		Log("SDL_QUIT event can't be pushed: %s\n", SDL_GetError() );		
		exit(1);													
	}

	return;															
}

void ToggleFullscreen(void)	{
	SDL_Surface * surface;														

	surface = SDL_GetVideoSurface();												

	if(!surface || (SDL_WM_ToggleFullScreen(surface)!=1)) {
		Log("Unable to toggle fullscreen: %s\n", SDL_GetError() );			
	}
	
	return;															
}

void ReshapeGL(int width, int height) {
	glViewport(0,0,(GLsizei)(width),(GLsizei)(height));				
	glMatrixMode(GL_PROJECTION);											
	glLoadIdentity();													

	gluPerspective(50.0f,(GLfloat)(width)/(GLfloat)(height),1.0f,100.0f);	
	glMatrixMode(GL_MODELVIEW);												
	glLoadIdentity();													

	return;															
}

bool CreateWindowGL(int w, int h, int b, Uint32 f) {
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );								
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );							
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );							

	if(!(screen = SDL_SetVideoMode(w, h, b, f))) {
		return false;													
	}

	SDL_FillRect(screen, NULL, SDL_MapRGBA(screen->format,0,0,0,0));								

	ReshapeGL(SCREEN_W, SCREEN_H);											

	return true;														
}

int main(int argc, char **argv)	{
	SDL_Event event;													
	Uint8 * keys;													
	Uint32 Vflags;													
	Uint32 tickCount;												
	Uint32 lastCount;
	bool is_program_looping;
	Msa * msa = new Msa();
		
	screen = NULL;														
	keys = NULL;														
	Vflags = SDL_HWSURFACE|SDL_OPENGLBLIT;																	

	InitErrorLog();														

	if(SDL_Init(SDL_INIT_VIDEO)<0) {
		Log("Unable to open SDL: %s\n", SDL_GetError() );					
		exit(1);													
	}

	atexit(SDL_Quit);													

#if defined FULLSCREEN_ASK													

	if(MessageBox(HWND_DESKTOP, "Want to use fullscreen mode?",				
		"Fullscreen?", MB_YESNO|MB_ICONQUESTION) == IDYES) {
		Vflags|=SDL_FULLSCREEN;												
	}

#elif defined FULLSCREEN													

	Vflags|=SDL_FULLSCREEN;													

#endif																
																

	SetUpIcon("Icon.bmp");												

	if(!CreateWindowGL(SCREEN_W, SCREEN_H, SCREEN_BPP, Vflags))	{
		Log("Unable to open screen surface: %s\n", SDL_GetError() );		
		exit(1);													
	}

	SDL_WM_SetCaption(APP_NAME, NULL);										

	if(!InitTimers(&lastCount))	{
		Log("Can't init the timers: %s\n", SDL_GetError() );				
		exit(1);													
	}

	if(!msa->InitGL()) {
		Log("Can't init GL: %s\n", SDL_GetError() );						
		exit(1);													
	}

	if(!msa->Initialize()) {
		Log("App init failed: %s\n", SDL_GetError() );						
		exit(1);													
	}

	is_program_looping = true;												

	while(is_program_looping)	{
		if(SDL_PollEvent(&event)) {
			switch(event.type) {
				
			case SDL_QUIT: {
					is_program_looping = false;								
					break;											
				}

			case SDL_VIDEORESIZE: {	
					ReshapeGL(event.resize.w, event.resize.h);						
					break;											
				}

			case SDL_ACTIVEEVENT: {
					if(event.active.state & SDL_APPACTIVE) {
						if(event.active.gain) {
							application_status.visible = true;						
						} else {
							application_status.visible = false;						
						}
					}
					
					if(event.active.state & SDL_APPMOUSEFOCUS) {
						if(event.active.gain) {
							application_status.mouse_focus = true;						
						} else {
							application_status.mouse_focus = false;					
						}
					}

					if(event.active.state & SDL_APPINPUTFOCUS) {
						if(event.active.gain) {
							application_status.keyboard_focus = true;					
						} else {
							application_status.keyboard_focus = false;				
						}
					}
					
					break;											
				}

				case SDL_KEYDOWN: {
					keys = SDL_GetKeyState(NULL);
					break;											
				}

			}
		} else {
			if(!application_status.visible) {
				SDL_WaitEvent(NULL);										
			} else {

				tickCount = SDL_GetTicks();									
				msa->Update(tickCount-lastCount, keys);
				
				msa->Draw();
							
				lastCount = tickCount;										
				
				SDL_GL_SwapBuffers();	
			}
		}
	}

	msa->Deinitialize();													
																
	CloseErrorLog();													
	exit(0);														

	return 0;														
}

SDL_Surface *SetUpIcon(const char *file) {
	SDL_Surface * icon;													
	Uint8 * pixels, * mask;												
	int i, mlen;													
	if( (icon = SDL_LoadBMP(file)) == NULL)	{
		Log("Unable to load icon: %s\n", SDL_GetError());
		return NULL;
	}

	if((icon->w%8) != 0) {
		Log("Icon width must be a multiple of 8\n" );
		SDL_FreeSurface(icon);
		return NULL;
	}

	if(!icon->format->palette) {
		Log("Icon file must have a palette.\n" );
		SDL_FreeSurface(icon);
		return NULL;
	}

	SDL_SetColorKey(icon, SDL_SRCCOLORKEY, *((Uint8 *)icon->pixels));		

	pixels =(Uint8 *)icon->pixels;											
	mlen   = icon->w*icon->h;												

	if(!(mask = (Uint8 *)malloc(mlen/8))) {
		Log("malloc() failed while loading icon: %s\n", SDL_GetError() );
		SDL_FreeSurface(icon);
		return NULL;
	}

	memset(mask, 0, mlen/8);												

	for(i=0;i<mlen;) {
		if(pixels[i] != *pixels) {
			mask[i/8] |= 0x01;											
		}

		++i;

		if((i%8)!=0) {
			mask[i/8] <<= 1;
		}
	}

	SDL_WM_SetIcon(icon, mask);												

	return icon;														
}
