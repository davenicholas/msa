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
 

#include "msa.h"

#ifdef WIN32
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "GLu32.lib")
#pragma comment(lib, "SDLmain.lib")	
#pragma comment(lib, "SDL.lib")
#endif			

#include <sstream>

bool Msa::InitGL() {
	
	glClearColor(0.0f,0.0f,0.0f,0.5f);
	glClearDepth(1.0f);	
	
	glDepthFunc(GL_LEQUAL);	
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glShadeModel(GL_SMOOTH);	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glColorMaterial(GL_FRONT, GL_EMISSION);

	GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat position[] = { -2.0f, -2.0f, 1.0f, 1.0f };
	 
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	this->LoadGLTextures();
	
	this->spaceship = new Spaceship();
	this->level_environment = new Level(1);
	this->environment_attributes = this->level_environment->GetEnvironmentalAttributes();

	this->menu = new Menu();

	InitMusic();
	PlayMenuBackgroundMusic();

	return true;
}

void Msa::LoadGLTextures() {
	
	SDL_Surface *surface_ship_indicator;
	
	surface_ship_indicator = SDL_LoadBMP("ship_indicator.bmp");
	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);			
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, surface_ship_indicator->w, surface_ship_indicator->h, GL_BGR, GL_UNSIGNED_BYTE, surface_ship_indicator->pixels);
		
	SDL_FreeSurface(surface_ship_indicator);
}


bool Msa::Initialize(void) {
	application_status.visible = true;	
	application_status.mouse_focus = false;	
	application_status.keyboard_focus = true;

	this->game_running = false;
	this->show_menu = true;
	
	this->thrust_timer_control = 0;
	this->level_skip_timer_control = 0;
	this->escape_key_timer_control = 0;
		
	return true;												
}


void Msa::Deinitialize(void) {
	delete this->spaceship;
	delete this->level_environment;
	delete this->menu;
	
	glDeleteTextures(1, &texture[0]);
	
	CloseAudio();
}

void Msa::Update(Uint32 milliseconds, Uint8 * keys) {
	if(keys && keys[SDLK_F1]) {
		ToggleFullscreen();	
	}	
	if(keys && this->game_running) {
		if(keys && (keys[SDLK_p] || keys[SDLK_ESCAPE])) {
			this->ShowMainMenu();
			this->escape_key_timer_control = 0;
		}
		if(keys[SDLK_RIGHT]) {
			this->spaceship->SteerShip(-3.0f);
		}
		if(keys[SDLK_LEFT]) {
			this->spaceship->SteerShip(3.0f);
		}
		if(keys[SDLK_SPACE] || keys[SDLK_UP]) {
			if(this->thrust_timer_control > 30) {
				this->spaceship->Thrust();
				this->thrust_timer_control = 0;
			}
		}
		if(keys[SDLK_r]) {
			this->level_environment->Reset();
			this->spaceship->Reset();
			this->thrust_timer_control = 0;
		}
		if(keys[SDLK_n] && (this->thrust_timer_control > 200)) {
			if(!this->level_environment->NextLevel()) {
				//all levels are finished
				this->ShowMainMenu();
			}
			this->environment_attributes = this->level_environment->GetEnvironmentalAttributes();
			this->spaceship->Reset();
			this->thrust_timer_control = 0;
			this->level_skip_timer_control = 0;
		}	
	}

	if(keys && this->show_menu) {
		if(keys[SDLK_ESCAPE]) {
			if(this->escape_key_timer_control > 200) {
				TerminateApplication();
				
			}
		}
		if(keys[SDLK_SPACE]) {
			this->show_menu	= false;
			this->game_running = true;
			CloseAudio();
			InitMusic();
			PlayBackgroundMusic();
		}
	}

	if(this->game_running) {
		this->thrust_timer_control += milliseconds;
		this->level_skip_timer_control += milliseconds;

		if(this->level_environment->IsLevelComplete()) {
			this->level_environment->SetLevelScore(this->spaceship->GetFuelAsPercentage(), 
				this->spaceship->GetAirAsPercentage());
			if(!this->level_environment->NextLevel()) {
				this->ShowMainMenu();
				return;
			}
			this->environment_attributes = this->level_environment->GetEnvironmentalAttributes();
			this->spaceship->Reset();
		}

		this->spaceship->Update(this->environment_attributes, milliseconds);
		this->level_environment->Update(milliseconds);
	
		if(!this->spaceship->IsAlive()) {
			this->level_environment->Reset();
			this->spaceship->Reset();
		}
	} else {
		this->escape_key_timer_control += milliseconds;
	}
	
	
	return;	
}

void Msa::ShowMainMenu() {
	this->game_running = false;
	this->show_menu = true;
	CloseAudio();
	InitMusic();
	PlayMenuBackgroundMusic();
	this->menu->SetScore(this->level_environment->GetOverallScore());
}

void Msa::Draw() {

	glClearDepth(1);
    glClearColor (0.0,0.0,0.0,10.0);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(this->show_menu) {
		this->DrawBackground();
		this->menu->Draw();
		this->level_environment->RenderMenuStellaBodies();
	}
	
	if(this->game_running) {
		GLfloat x, y;
		this->spaceship->GetShipCoordinates(x, y);
		
		this->level_environment->PickUpCheck(x, y);

		this->DrawBackground();
		this->DrawShip();
		this->DrawEnvironment();	
		this->IsShipOffScreen(x, y);
		this->DrawText();
	}
}

void Msa::IsShipOffScreen(GLfloat& x, GLfloat& y) {
	GLfloat off_screen_ship_x, off_screen_ship_y;
	off_screen_ship_x = off_screen_ship_y = 0.0f;
	
	//check if ship is offscreen
	if((x > MAX_X || x < -MAX_X) || (y > MAX_Y || y < -MAX_Y)) {
		this->RenderOffScreenShip(x, y);
	}
}

void Msa::RenderOffScreenShip(GLfloat& x, GLfloat& y) {

	GLfloat x_index, y_index;
	
	x_index = x;
	y_index = y;
	
	if(x >= MAX_X - 0.2f)
		x_index = MAX_X - 0.2f;
	if(x <= -MAX_X + 0.2)
		x_index = -MAX_X + 0.2f;
	if(y >= MAX_Y  - 0.2f)
		y_index = MAX_Y - 0.2f;
	if(y <= -MAX_Y + 0.2f)
		y_index = -MAX_Y + 0.2f;

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);    
   
    glLoadIdentity();
	glTranslatef( x_index, y_index, -5.0f );
		
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );   
	
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	
	glRotatef(this->spaceship->GetShipZAngle(),0.0f,0.0f,1.0f);	

	glBegin(GL_TRIANGLE_STRIP);	
		glTexCoord2d(1, 1); glVertex3f(0.2f, 0.2f, 0.0f);
		glTexCoord2d(0, 1); glVertex3f(-0.2f, 0.2f, 0.0f); 
		glTexCoord2d(1, 0); glVertex3f(0.2f, -0.2f, 0.0f);
		glTexCoord2d(0, 0); glVertex3f(-0.2f, -0.2f, 0.0f); 
	glEnd();
	
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	
	std::stringstream distance_off_screen;
	distance_off_screen << "x: " << int((x - x_index)*10) 
		<< "  y: " << int((y - y_index)*10);
	
	std::string off_screen;
	off_screen.append("<<   ")
		.append(distance_off_screen.str())
		.append("   >>");
	
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glLoadIdentity();
	glTranslatef(-0.50f, 0.42f, -1.0f);
	glColor3f(1.0f,1.0f,1.0f);
	glRasterPos2f(0, 0);
	Text::Instance()->PrintText(off_screen.c_str());
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

}

void Msa::DrawText() {
	
	std::stringstream fuel_percentage;
	fuel_percentage << this->spaceship->GetFuelAsPercentage();
	
	std::stringstream air_percentage;
	air_percentage << this->spaceship->GetAirAsPercentage();
	
	std::string game_hud_info;
	game_hud_info.append("Fuel: ")
		.append(fuel_percentage.str())
		.append("%%")
		.append("   Air: ")
		.append(air_percentage.str())
		.append("%%");
	
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glLoadIdentity();
	glTranslatef( -0.75f, 0.42f, -1.0f );
	glColor3f(1.0f,1.0f,1.0f);
	glRasterPos2f(0.0f,0.0f);
	Text::Instance()->PrintText(game_hud_info.c_str());
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}

void Msa::DrawShip() {
	this->spaceship->Render();
}

void Msa::DrawEnvironment() {
	this->level_environment->Render();
}

void Msa::DrawBackground() { 
	this->level_environment->RenderStars();
}
