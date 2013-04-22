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
 


#include "gas_planet.h"

GasPlanet::GasPlanet(GLfloat x, GLfloat y, GLfloat s) {
	
	this->x_index = x;
	this->y_index = y;
	this->size = s;
	
	this->quadratic=gluNewQuadric();
	gluQuadricNormals(this->quadratic, GLU_SMOOTH);
	gluQuadricTexture(this->quadratic, GL_TRUE);
	this->LoadGLTextures();
	this->zrot = ((360.0f-0.1f)*((float)rand()/RAND_MAX))+0.1f;
	this->yrot = ((360.0f-0.1f)*((float)rand()/RAND_MAX))+0.1f;
	this->zspeed = ((0.5f-0.1f)*((float)rand()/RAND_MAX))+0.1f;
	this->yspeed = ((0.5f-0.1f)*((float)rand()/RAND_MAX))+0.1f;
}

GasPlanet::~GasPlanet() {
	gluDeleteQuadric(this->quadratic);
	glDeleteTextures(1, &texture[0]);
}

void GasPlanet::LoadGLTextures() {
	SDL_Surface *surface_purple;
	
	surface_purple = SDL_LoadBMP("purple.bmp");
	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    
 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, surface_purple->w, surface_purple->h, GL_BGR, GL_UNSIGNED_BYTE, surface_purple->pixels);
	
	SDL_FreeSurface(surface_purple);
}

void GasPlanet::Render() {
	glLoadIdentity();	
	glTranslatef(this->x_index, this->y_index, -5.0f);

	glRotatef(this->zrot,0.0f,0.0f,1.0f);	
	glRotatef(this->yrot,0.0f,1.0f,0.0f);	

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	gluSphere(this->quadratic, this->size, 20, 20);
	
	this->zrot+=this->zspeed;
	this->yrot+=this->yspeed;
}

