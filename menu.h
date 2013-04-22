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
 

#ifndef _MENU_H_
#define _MENU_H_

#ifdef WIN32
#include <windows.h>
#else
#include <stdio.h>
#include <stdlib.h>	
#endif

#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#include <SDL.h>

#include "text.h"

class Menu {
	
	public:
		
		Menu();
		~Menu();
		void Draw();
		void SetScore(int i);
		
	private:
		void LoadGLTextures();
		void DrawScore();
		GLuint texture[1];
		int score;
};

#endif
