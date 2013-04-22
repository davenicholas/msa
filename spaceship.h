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
 


#ifndef _SPACESHIP_H_
#define _SPACESHIP_H_

#ifdef WIN32
#include <windows.h>
#else
#include <stdio.h>
#include <stdlib.h>	
#endif	

#include <cmath>

#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <SDL.h>
#include <vector>
#include "error.h"
#include "massbody.h"

#define PI 3.14159265
#define	MAX_THRUST_PARTICLES 1000
#define MAX_FUEL 600;
#define MAX_AIR 50000;

typedef struct	
{
	bool	active;	
	float	life;
	float	fade;
	float	r;	
	float	g;	
	float	b;	
	float	x;
	float	y;
	float	z;
	float	xi;
	float	yi;
	float	zi;
	float	xg;
	float	yg;
	float	zg;	
}
thrust_particle;

class Spaceship { 
	
	public:
		Spaceship();
		~Spaceship();
		void Render();
		void SteerShip(GLfloat z);
		void Thrust();
		void Update(std::vector<EnvironmentalAttribute> ev, Uint32 milliseconds);
		bool IsAlive();
		void Reset();
		int GetFuelAsPercentage();
		int GetAirAsPercentage();
		void GetShipCoordinates(GLfloat& x, GLfloat& y);
		GLfloat GetShipZAngle();
		
	private:
		void LoadGLTextures();
		void InitThrustParticles();
		void DrawThruster();
		void DrawCentrePod();
		void DrawNoseCone();
		void DrawThrustParticles();
		GLuint texture[4];
		GLfloat x_index;
		GLfloat y_index;
		GLfloat x_rot;
		GLfloat z_rot;
		
		GLfloat motion_angle_x;
		GLfloat motion_angle_y;
		
		GLfloat particle_slowdown;
		thrust_particle thrust_particles[MAX_THRUST_PARTICLES];
		
		bool is_alive;
		int air;
		int fuel;
};



#endif
