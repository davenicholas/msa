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
 

#include "menu.h"

#include <sstream>

Menu::Menu() {
	this->LoadGLTextures();
	this->score = 0;
}

void Menu::LoadGLTextures() {
	SDL_Surface *surface_splash;
	
	surface_splash = SDL_LoadBMP("splash.bmp");
	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);			
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, surface_splash->w, surface_splash->h, GL_BGR, GL_UNSIGNED_BYTE, surface_splash->pixels);
		
	SDL_FreeSurface(surface_splash);
}

Menu::~Menu() {
	glDeleteTextures(1, &texture[0]);
}

void Menu::SetScore(int i) {
	this->score = i;
}

void Menu::Draw() {
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	
  	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -5.0f);	

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);  
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glScalef(2.2f, 2.2f, 1.0f);

	glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	
	glBegin(GL_TRIANGLE_STRIP);	
		glTexCoord2d(1, 1); glVertex3f(1.0f, 1.0f, 0.0f);
		glTexCoord2d(0, 1); glVertex3f(-1.0f, 1.0f, 0.0f); 
		glTexCoord2d(1, 0); glVertex3f(1.0f, -1.0f, 0.0f);
		glTexCoord2d(0, 0); glVertex3f(-1.0f, -1.0f, 0.0f); 
	glEnd();

	if(this->score > 0)
		this->DrawScore();

}

void Menu::DrawScore() {

	std::stringstream score_stream;
	score_stream << this->score;
	
	std::string score_str;
	score_str.append("You Scored: ")
		.append(score_stream.str());
	
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glLoadIdentity();
	glTranslatef(-0.18f, 0.17f, -1.0f);
	glColor3f(1.0f,0.0f,0.0f);
	glRasterPos2f(0, 0);
	Text::Instance()->PrintText(score_str.c_str());
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}

