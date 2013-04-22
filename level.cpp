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
 

#include "level.h"

Level::Level(int level_number) {
	this->current_level = level_number;
	this->LoadGLTextures();
	this->SetUpEnvironment();
	for(int i=0; i<LEVEL_MAX; ++i) {
		this->player_score[i].remaining_fuel = 0;
		this->player_score[i].remaining_air = 0;
	}
	this->LoadMenuBackground();
}

void Level::LoadGLTextures() {
	SDL_Surface * surface_pickup = NULL;
	
	surface_pickup = SDL_LoadBMP("pickup.bmp");
	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);			
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, surface_pickup->w, surface_pickup->h, GL_BGR, GL_UNSIGNED_BYTE, surface_pickup->pixels);
		
	SDL_FreeSurface(surface_pickup);

	SDL_Surface * surface_star = NULL;

	surface_star = SDL_LoadBMP("particle.bmp");
	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
	glGenTextures(1, &texture[1]);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);			
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, surface_star->w, surface_star->h, GL_BGR, GL_UNSIGNED_BYTE, surface_star->pixels);
		
	SDL_FreeSurface(surface_star);
}

Level::~Level() {
	this->ResetEnvironment();
	glDeleteTextures(2, &texture[0]);

	for (std::vector<MassBody*>::iterator it = this->menu_stella_bodies.begin(); it != this->menu_stella_bodies.end(); ++it) {
		delete (*it);
	}
	this->menu_stella_bodies.clear();
}

void Level::ResetEnvironment() {
	for (std::vector<MassBody*>::iterator it = this->stella_bodies.begin(); it != this->stella_bodies.end(); ++it) {
		delete (*it);
	}
	this->stella_bodies.clear();
	this->pickup_entities.clear();
	this->background_stars.clear();
}

void Level::SetLevel(int level_number) {
	this->current_level = level_number;
	this->SetUpEnvironment();
}

void Level::SetLevelScore(int fuel, int air) {
	this->player_score[this->current_level-1].remaining_fuel = fuel;
	this->player_score[this->current_level-1].remaining_air = air;
}

int Level::GetOverallScore() {
	int fuel_used = 0, air_used = 0;
	
	for(int i=0; i<LEVEL_MAX; ++i) {
		fuel_used += this->player_score[i].remaining_fuel;
		air_used += this->player_score[i].remaining_air;
	}
	return (fuel_used+air_used);
}

bool Level::NextLevel() {
	if(this->current_level < LEVEL_MAX) {
		this->current_level++;
		this->SetUpEnvironment();
		return true;
	} else {
		this->current_level = 1;
		this->SetUpEnvironment();
		return false;
	}
}

void Level::Reset() {
	this->SetUpEnvironment();
}

void Level::SetUpEnvironment() {
	
	this->ResetEnvironment();

	this->has_active_pickups = true;

	for (int i=0; i<27; i++) {
		this->background_stars.push_back(
		    Star(
			    (((-4.0f)-4.0f)*((float)rand()/RAND_MAX))+4.0f, 
			    (((-2.3f)-2.3f)*((float)rand()/RAND_MAX))+2.3f,
			    ((0.065f-0.0051f)*((float)rand()/RAND_MAX))+0.005f,
			    ((1.0f-0.1f)*((float)rand()/RAND_MAX))+0.1f,
			    ((1.0f-0.1f)*((float)rand()/RAND_MAX))+0.1f,
			    ((1.0f-0.1f)*((float)rand()/RAND_MAX))+0.1f,
			    ((0.5f-0.01f)*((float)rand()/RAND_MAX))+0.01f)
		    );
	}

	switch(this->current_level) {
		case 1:
			this->pickup_entities.push_back(PickupEntity(-2.0f, -1.5f, 0.1f));
			this->pickup_entities.push_back(PickupEntity(0.0f, 0.5f, 200.0f));		
			this->stella_bodies.push_back(new Planet(-1.5f, -1.0f, 0.2f));
			this->stella_bodies.push_back(new GreenPlanet(2.0f, 0.0f, 0.1f));
			this->stella_bodies.push_back(new GasPlanet(3.0f, 1.0f, 0.6f));		
			break;
		case 2:
			this->pickup_entities.push_back(PickupEntity(0.7f, 1.0f, 100.0f));
			this->pickup_entities.push_back(PickupEntity(0.0f, 0.0f, 200.0f));	
			this->pickup_entities.push_back(PickupEntity(-0.5f, -0.8f, 300.0f));	
			this->stella_bodies.push_back(new GasPlanet(0.8f, -0.7f, 0.6f));
			this->stella_bodies.push_back(new Moon(-0.5f, 0.6f, 0.3f));
			break;
		case 3:
			this->pickup_entities.push_back(PickupEntity(-1.0f, 2.0f, 100.0f));
			this->pickup_entities.push_back(PickupEntity(2.0f, 1.0f, 200.0f));	
			this->pickup_entities.push_back(PickupEntity(1.0f, -1.0f, 300.0f));	
			this->stella_bodies.push_back(new GreenPlanet(0.0f, 0.0f, 0.9f));
			break;
		case 4:
			this->pickup_entities.push_back(PickupEntity(0.0f, 1.0f, 0.1f));
			this->pickup_entities.push_back(PickupEntity(0.5f, 0.0f, 100.0f));
			this->pickup_entities.push_back(PickupEntity(-0.5f, 0.0f, 200.0f));		
			this->stella_bodies.push_back(new Planet(-1.5f, 0.0f, 0.4f));
			this->stella_bodies.push_back(new Moon(2.0f, 0.0f, 0.2f));		
			break;
		case 5:
			this->pickup_entities.push_back(PickupEntity(2.8f, -1.3f, 100.0f));
			this->pickup_entities.push_back(PickupEntity(0.8f, 0.8f, 200.0f));
			this->pickup_entities.push_back(PickupEntity(-0.5f, -0.5f, 200.0f));
			this->stella_bodies.push_back(new GasPlanet(2.0f, -0.8f, 0.6f));
			this->stella_bodies.push_back(new GreenPlanet(-0.5f, 0.6f, 0.3f));
			break;
		case 6:
			this->pickup_entities.push_back(PickupEntity(-2.0f, 0.0f, 100.0f));
			this->pickup_entities.push_back(PickupEntity(0.0f, 0.0f, 200.0f));	
			this->pickup_entities.push_back(PickupEntity(2.0f, 0.0f, 300.0f));	
			this->stella_bodies.push_back(new Planet(1.0f, 0.0f, 0.4f));
			this->stella_bodies.push_back(new Moon(-1.0f, 0.0f, 0.3f));
			break;
		case 7:
			this->pickup_entities.push_back(PickupEntity(0.0f, 1.5f, 100.0f));
			this->pickup_entities.push_back(PickupEntity(0.0f, 0.0f, 200.0f));	
			this->pickup_entities.push_back(PickupEntity(0.0f, -1.5f, 300.0f));	
			this->stella_bodies.push_back(new GasPlanet(0.0f, 0.5f, 0.3f));
			this->stella_bodies.push_back(new GreenPlanet(0.0f, -0.5f, 0.3f));		
			break;
		case 8:
			this->pickup_entities.push_back(PickupEntity(-2.0f, 0.0f, 100.0f));
			this->pickup_entities.push_back(PickupEntity(0.0f, 0.0f, 200.0f));	
			this->pickup_entities.push_back(PickupEntity(0.0f, 1.5f, 100.0f));	
			this->pickup_entities.push_back(PickupEntity(0.0f, -1.5f, 300.0f));
			this->stella_bodies.push_back(new Planet(2.0f, 0.0f, 0.4f));
			this->stella_bodies.push_back(new Moon(-1.0f, 0.0f, 0.3f));
			this->stella_bodies.push_back(new GasPlanet(0.0f, 0.5f, 0.3f));
			this->stella_bodies.push_back(new GreenPlanet(0.0f, -0.5f, 0.3f));	
			break;
		case 9:
			this->pickup_entities.push_back(PickupEntity(-1.3f, 2.0f, 100.0f));
			this->pickup_entities.push_back(PickupEntity(0.0f, 1.7f, 0.0f));
			this->pickup_entities.push_back(PickupEntity(1.3f, 1.0f, 200.0f));
			this->pickup_entities.push_back(PickupEntity(2.0f, 0.0f, 200.0f));
			this->pickup_entities.push_back(PickupEntity(1.3f, -1.3f, 300.0f));	
			this->stella_bodies.push_back(new GreenPlanet(0.0f, 0.0f, 1.0f));
			break;
		case 10:
			this->pickup_entities.push_back(PickupEntity(-2.0f, 1.0f, 0.1f));
			this->pickup_entities.push_back(PickupEntity(0.0f, -1.0f, 200.0f));
			this->pickup_entities.push_back(PickupEntity(0.0f, 0.0f, 200.0f));
			this->pickup_entities.push_back(PickupEntity(2.0f, 1.0f, 300.0f));
			this->stella_bodies.push_back(new Planet(-2.0f, -1.0f, 0.4f));
			this->stella_bodies.push_back(new Moon(0.0f, 1.0f, 0.3f));
			this->stella_bodies.push_back(new GreenPlanet(2.0f, -1.0f, 0.4f));
			break;
		case 11:
			this->pickup_entities.push_back(PickupEntity(-0.8f, 0.5f, 100.0f));
			this->pickup_entities.push_back(PickupEntity(-1.8f, -0.7f, 200.0f));	
			this->pickup_entities.push_back(PickupEntity(3.2f, 0.6f, 300.0f));	
			this->stella_bodies.push_back(new GasPlanet(-1.3f, 0.9f, 0.4f));
			this->stella_bodies.push_back(new GreenPlanet(-2.0f, -1.3f, 0.4f));
			this->stella_bodies.push_back(new Moon(2.0f, 1.0f, 0.9f));
			break;
		case 12:
			this->pickup_entities.push_back(PickupEntity(-1.0f, 2.0f, 100.0f));
			this->pickup_entities.push_back(PickupEntity(0.0f, 1.5f, 0.0f));
			this->pickup_entities.push_back(PickupEntity(1.3f, 1.0f, 200.0f));
			this->pickup_entities.push_back(PickupEntity(2.0f, 0.0f, 200.0f));
			this->pickup_entities.push_back(PickupEntity(1.8f, 1.5f, 200.0f));
			this->pickup_entities.push_back(PickupEntity(2.5f, 1.3f, 200.0f));
			this->pickup_entities.push_back(PickupEntity(1.3f, -1.3f, 300.0f));
			this->pickup_entities.push_back(PickupEntity(1.0f, -1.0f, 300.0f));
			this->stella_bodies.push_back(new GasPlanet(-1.0f, -0.5f, 1.0f));
			break;
	}
}

bool Level::IsLevelComplete() {
	return (!this->has_active_pickups);
}	

void Level::Render() {
	this->RenderStellaBodies();
	this->RenderPickUps();
}

void Level::RenderStellaBodies() {
	for (std::vector<MassBody*>::iterator it = this->stella_bodies.begin(); it != this->stella_bodies.end(); ++it) {
		(*it)->Render();
	}
}

void Level::LoadMenuBackground() {
	this->menu_stella_bodies.push_back(new GreenPlanet(-3.5f, 2.2f, 1.1f));
	this->menu_stella_bodies.push_back(new GasPlanet(3.0f, -1.2f, 0.7f));		
}

void Level::RenderMenuStellaBodies() {
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	
	for (std::vector<MassBody*>::iterator it = this->menu_stella_bodies.begin(); it != this->menu_stella_bodies.end(); ++it) {
		(*it)->Render();
	}
}

void Level::RenderPickUps() {
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);    

	this->has_active_pickups = false;
	
	for (std::vector<PickupEntity>::iterator it = this->pickup_entities.begin(); it != this->pickup_entities.end(); ++it) {
		if((*it).is_active) {
			glLoadIdentity();
			glTranslatef((*it).x, (*it).y, -5.0f);
				
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);   
			
			glBindTexture(GL_TEXTURE_2D, texture[0]);
			
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			
			glRotatef((*it).z_rot, 0.0f, 0.0f, 1.0f);	

			glBegin(GL_TRIANGLE_STRIP);	
				glTexCoord2d(1, 1); glVertex3f(0.1f, 0.1f, 0.0f);
				glTexCoord2d(0, 1); glVertex3f(-0.1f, 0.1f, 0.0f); 
				glTexCoord2d(1, 0); glVertex3f(0.1f, -0.1f, 0.0f);
				glTexCoord2d(0, 0); glVertex3f(-0.1f, -0.1f, 0.0f); 
			glEnd();

			this->has_active_pickups = true;
		}
	}
	
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void Level::RenderStars() {

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	for (std::vector<Star>::iterator it = this->background_stars.begin(); it != this->background_stars.end(); ++it) {

		glLoadIdentity();

		glTranslatef((*it).x, (*it).y, -6.0f);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);  
		glBindTexture(GL_TEXTURE_2D, texture[1]);

		glColor4f((*it).r, (*it).g, (*it).b, (*it).a);
			
		glBegin(GL_TRIANGLE_STRIP);	
				glTexCoord2d(1, 1); glVertex3f((*it).size, (*it).size, 0.0f);
				glTexCoord2d(0, 1); glVertex3f(-(*it).size, (*it).size, 0.0f); 
				glTexCoord2d(1, 0); glVertex3f((*it).size, -(*it).size, 0.0f);
				glTexCoord2d(0, 0); glVertex3f(-(*it).size, -(*it).size, 0.0f); 
		glEnd();
		
	}
	
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void Level::PickUpCheck(GLfloat& x, GLfloat& y) {
	for (std::vector<PickupEntity>::iterator it = this->pickup_entities.begin(); it != this->pickup_entities.end(); ++it) {
		if((*it).is_active) {
			GLfloat diff_x = x - (*it).x;
			GLfloat diff_y = y - (*it).y;
			if((diff_x > -0.2f && diff_x < 0.2f) && (diff_y > -0.2f && diff_y < 0.2f))
				(*it).is_active = false;
		}
	}	
}

void Level::Update(Uint32 milliseconds) {
	for (std::vector<PickupEntity>::iterator it = this->pickup_entities.begin(); it != this->pickup_entities.end(); ++it) {
		(*it).z_rot += milliseconds/7;
	}
}

std::vector<EnvironmentalAttribute> Level::GetEnvironmentalAttributes() {
	std::vector<EnvironmentalAttribute> ev;
	for (std::vector<MassBody*>::iterator it = this->stella_bodies.begin(); it != this->stella_bodies.end(); ++it) {
		ev.push_back((*it)->GetLocationAndDensity());
	}
	return ev;
}

