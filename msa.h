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
 

#ifndef _MSA_H_
#define _MSA_H_

#ifdef WIN32
#include <windows.h>
#else
#include <stdio.h>
#include <stdlib.h>	
#endif

#include <iomanip>

#include <vector>
#include <string>

#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#include <SDL.h>

#include "main.h"
#include "massbody.h"
#include "spaceship.h"
#include "level.h"
#include "menu.h"

#include "text.h"

#define PI 3.14159265

const GLfloat MAX_X = 4.0f;
const GLfloat MAX_Y = 2.3f;


class Msa {
	
	public:

		bool Initialize(void);
		
		bool InitGL();
		void Deinitialize(void);
		void Update(Uint32, Uint8 *);
		void Draw();
		
	private:
	
		void DrawShip();
		void Draw3D();
		void DrawEnvironment();
		void DrawBackground();
		void DrawText();
		void glPrint( const char *fmt, ... );
		void IsShipOffScreen(GLfloat& x, GLfloat& y);
		void RenderOffScreenShip(GLfloat& x, GLfloat& y);
		void LoadGLTextures();
		void ShowMainMenu();
	
		Spaceship * spaceship;
		Level * level_environment;
		Menu * menu;
		std::vector<EnvironmentalAttribute> environment_attributes;
		GLuint texture[1];
		GLuint thrust_timer_control;
		GLuint escape_key_timer_control;
		GLuint level_skip_timer_control;

		bool game_running;
		bool show_menu;
};

#endif
