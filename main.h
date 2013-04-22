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
 
#ifndef _MAIN_H_
#define _MAIN_H_


#ifdef WIN32												
#include <windows.h>										
#else														
#include <stdio.h>									
#endif														
#include <SDL.h>											
#include "error.h"

#define SCREEN_W 1024										
#define SCREEN_H 600									
#define SCREEN_BPP 16										


#include "sound.h"
#include "msa.h"

typedef struct
{
	bool visible;
	bool mouse_focus;
	bool keyboard_focus;	
}
	AppStatus;

extern AppStatus application_status;

int main(int, char **);										

bool InitTimers(Uint32 *);
bool CreateWindowGL(SDL_Surface *, int, int, int, Uint32);
SDL_Surface *SetUpIcon(const char *);								

void ReshapeGL(int, int);									
void ToggleFullscreen(void);								
void TerminateApplication(void);


#endif
