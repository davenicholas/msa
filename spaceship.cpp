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
 


#include "spaceship.h"


Spaceship::Spaceship() {
	this->LoadGLTextures();
	this->InitThrustParticles();
	this->Reset();
}

void Spaceship::Reset() {
	this->x_index = -3.5f;		
	this->y_index = 1.5f;
	this->x_rot = 0.0f;
	this->z_rot = 230.0f;
	this->motion_angle_x = 0.0f;
	this->motion_angle_y = 0.0f;
	
	this->particle_slowdown = 2.0f;
	
	this->is_alive = true;
	this->air = MAX_AIR;
	this->fuel = MAX_FUEL;
}

bool Spaceship::IsAlive() {
	return this->is_alive;
}

void Spaceship::InitThrustParticles() {
	
	GLfloat colors[12][3]={
		{1.0f,0.5f,0.5f},{1.0f,0.75f,0.5f},{1.0f,1.0f,0.5f},{0.75f,1.0f,0.5f},
		{0.5f,1.0f,0.5f},{0.5f,1.0f,0.75f},{0.5f,1.0f,1.0f},{0.5f,0.75f,1.0f},
		{0.5f,0.5f,1.0f},{0.75f,0.5f,1.0f},{1.0f,0.5f,1.0f},{1.0f,0.5f,0.75f}
	};
	
	for (int i=0;i<MAX_THRUST_PARTICLES;i++)
	{
		this->thrust_particles[i].active=true;	
		this->thrust_particles[i].life=0.2f;
		this->thrust_particles[i].fade=float(rand()%100)/1000.0f+0.003f;
		this->thrust_particles[i].r=colors[(i+1)/(MAX_THRUST_PARTICLES/12)][0];
		this->thrust_particles[i].g=colors[(i+1)/(MAX_THRUST_PARTICLES/12)][1];
		this->thrust_particles[i].b=colors[(i+1)/(MAX_THRUST_PARTICLES/12)][2];
		this->thrust_particles[i].x=0.0f;
		this->thrust_particles[i].y=0.0f;	
		this->thrust_particles[i].z=0.0f;
		this->thrust_particles[i].xi=x_index+float((rand()%60)-32.0f);
		this->thrust_particles[i].yi=y_index+float((rand()%60)-30.0f);
		this->thrust_particles[i].zi=float((rand()%60)-30.0f);
		this->thrust_particles[i].xg=0.0f;
		this->thrust_particles[i].yg=-0.8f;
		this->thrust_particles[i].zg=0.0f;
	}
	
}

Spaceship::~Spaceship() {
	glDeleteTextures(4, &texture[0]);
}

void Spaceship::LoadGLTextures() {

	SDL_Surface *surface_top;
	SDL_Surface *surface_middle;
	SDL_Surface *surface_bottom;
	SDL_Surface *surface_thrust;	
	 
	surface_top = SDL_LoadBMP("ship-top.bmp");
	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);			
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, surface_top->w, surface_top->h, GL_BGR, GL_UNSIGNED_BYTE, surface_top->pixels);
	
	SDL_FreeSurface(surface_top);
	
	surface_middle = SDL_LoadBMP("ship-center.bmp");
	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
	glGenTextures(1, &texture[1]);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);			
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, surface_middle->w, surface_middle->h, GL_BGR, GL_UNSIGNED_BYTE, surface_middle->pixels);
	
	SDL_FreeSurface(surface_middle);
	
	surface_bottom = SDL_LoadBMP("ship-bottom.bmp");
	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
	glGenTextures(1, &texture[2]);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);			
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, surface_bottom->w, surface_bottom->h, GL_BGR, GL_UNSIGNED_BYTE, surface_bottom->pixels);
		
	SDL_FreeSurface(surface_bottom);
	
	surface_thrust = SDL_LoadBMP("particle.bmp");
	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
	glGenTextures(1, &texture[3]);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);			
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, surface_thrust->w, surface_thrust->h, GL_BGR, GL_UNSIGNED_BYTE, surface_thrust->pixels);
		
	SDL_FreeSurface(surface_thrust);
}

void Spaceship::Render() {

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	
	//Render ship

	glLoadIdentity();	
	glTranslatef(this->x_index, this->y_index, -5.0f);
		
	glRotatef(this->z_rot,0.0f,0.0f,1.0f);
	glRotatef(this->x_rot,0.0f,1.0f,0.0f);
	
	glScalef( 0.04f, 0.04f, 0.04f );

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	this->DrawThruster();
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	this->DrawCentrePod();
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	this->DrawNoseCone();

	//Render particle system

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	
	glLoadIdentity();	
		
	glTranslatef(this->x_index, this->y_index, -5.0f);
	
	glRotatef(this->z_rot, 0.0f, 0.0f, 1.0f);
	
	glScalef(0.05f, 0.05f, 0.05f);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);	
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	this->DrawThrustParticles();
	
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	
}

void Spaceship::DrawThruster() {
	for (int i=0; i<4; i++) {
		glRotatef(90.0f,0.0f,1.0f,0.0f);
		glBegin(GL_TRIANGLES);
			glTexCoord2f(0.5f, 1.0f); glVertex3f( 0.0f, 0.0f, 0.0f); //Top
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-2.0f, 1.0f); //Bottom Left
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-2.0f, 1.0f); //Bottom Right
		glEnd();
	}
}

void Spaceship::DrawCentrePod() {
	glBegin(GL_QUADS);
		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Top Left Of The Texture and Quad
		// Back Face
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Bottom Left Of The Texture and Quad
		// Top Face
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Top Right Of The Texture and Quad
		// Bottom Face
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
		// Right face
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
		// Left Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Top Left Of The Texture and Quad
	glEnd();
}

void Spaceship::DrawNoseCone() {
	for (int i=0; i<4; i++) {
		glRotatef(90.0f,0.0f,1.0f,0.0f);
		glBegin(GL_TRIANGLES);
			glTexCoord2f(0.5f, 1.0f); glVertex3f( 0.0f, 2.5f, 0.0f); //Top
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,0.5f, 1.0f); //Bottom Left	
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,0.5f, 1.0f); //Bottom Right
		glEnd();
	}
}

void Spaceship::DrawThrustParticles() {
	
	for (int i=0;i<MAX_THRUST_PARTICLES;i++) {
		if (this->thrust_particles[i].active) {
			float x=this->thrust_particles[i].x;	
			float y=this->thrust_particles[i].y;	
			float z=this->thrust_particles[i].z;	

			glColor4f(this->thrust_particles[i].r, this->thrust_particles[i].g, 
				this->thrust_particles[i].b, this->thrust_particles[i].life);
									
			
			glBegin(GL_TRIANGLE_STRIP);	
			    glTexCoord2d(1,1); glVertex3f(x+0.5f,y-2.3f,z);
				glTexCoord2d(0,1); glVertex3f(x-0.5f,y-2.3f,z); 
				glTexCoord2d(1,0); glVertex3f(x+0.5f,y-3.3f,z);
				glTexCoord2d(0,0); glVertex3f(x-0.5f,y-3.3f,z); 
			glEnd();

			this->thrust_particles[i].x+=this->thrust_particles[i].xi/(this->particle_slowdown*1000);
			this->thrust_particles[i].y+=this->thrust_particles[i].yi/(this->particle_slowdown*1000);
			this->thrust_particles[i].z+=this->thrust_particles[i].zi/(this->particle_slowdown*1000);

			this->thrust_particles[i].xi+=this->thrust_particles[i].xg;
			this->thrust_particles[i].yi+=this->thrust_particles[i].yg;
			this->thrust_particles[i].zi+=this->thrust_particles[i].zg;
			this->thrust_particles[i].life-=this->thrust_particles[i].fade;

			if (this->thrust_particles[i].life<0.0f) {
				this->thrust_particles[i].life=0.2f;
				this->thrust_particles[i].fade=float(rand()%100)/1000.0f+0.003f;
				this->thrust_particles[i].x=0.0f;
				this->thrust_particles[i].y=0.0f;	
				this->thrust_particles[i].z=0.0f;
				this->thrust_particles[i].xi=x_index+float((rand()%60)-32.0f);
				this->thrust_particles[i].yi=y_index+float((rand()%60)-30.0f);
				this->thrust_particles[i].zi=float((rand()%60)-30.0f);
				thrust_particles[i].yg=-0.8f;
			}
		}
	}
}

void Spaceship::SteerShip(GLfloat z) {
	this->z_rot += z;
	
	if(this->z_rot > 360.0f)
		this->z_rot = 0.0f;
	if(this->z_rot < 0.0f)
		this->z_rot = 360.0f;
	
}

void Spaceship::Update(std::vector<EnvironmentalAttribute> ev, Uint32 milliseconds) {
	
	this->air -= milliseconds;
	
	//check if air has run out
	if(this->air<=0)
		this->is_alive = false;
	
	//spin ship 
	this->x_rot += milliseconds/7;	
		
	for (std::vector<EnvironmentalAttribute>::iterator it = ev.begin(); it != ev.end(); ++it) {
			
		GLfloat x, y;
		
		if(this->x_index > (*it).x)
			x = this->x_index - (*it).x;
		else
			x = (*it).x - this->x_index;
			
		if(this->y_index > (*it).y)
			y = this->y_index - (*it).y;
		else
			y = (*it).y - this->y_index;
			
		GLfloat distance_from_ship = sqrt(x*x+y*y);
		
		//check if ship has hit planet
		
		if ((distance_from_ship - (*it).s) < 0)
			this->is_alive = false;
		
		//compute gravity of mass body
		
		GLfloat angle_from_ship = atan2(this->y_index - (*it).y, this->x_index - (*it).x) * 180 / PI;
		if(angle_from_ship<0)
			angle_from_ship = 360 + angle_from_ship;
		
		angle_from_ship+=180;
		if(angle_from_ship > 360)
			angle_from_ship = angle_from_ship - 360;
				
		if(distance_from_ship < 3.0f) {
			double time_correction = milliseconds / 11;
			
			if(time_correction < 1) time_correction = 1;
			
			double percentage_to_planet = (3-distance_from_ship)/0.03f;
			
			double velocity_factor_planet = (((percentage_to_planet * (*it).s) * time_correction) / 400000);
				
			GLfloat x_delta = cos( ( angle_from_ship *PI/180) );
			GLfloat y_delta = sin( ( angle_from_ship *PI/180) );
				
			this->motion_angle_x += x_delta * velocity_factor_planet;
				
			this->motion_angle_y += y_delta * velocity_factor_planet;
					
		}
				
	}
	
	this->x_index += (milliseconds/10) * this->motion_angle_x;
	this->y_index += (milliseconds/10) * this->motion_angle_y;
	
}

void Spaceship::Thrust() {
	
	GLfloat x_delta = cos( ( (this->z_rot + 90) *PI/180) );
	GLfloat y_delta = sin( ( (this->z_rot + 90) *PI/180) );
		
	this->motion_angle_x += x_delta / 5000;
		
	this->motion_angle_y += y_delta / 5000;
	
	this->fuel--;
	
	if(this->fuel<=0)
		this->is_alive = false;
		
	for (int i=0;i<MAX_THRUST_PARTICLES;i++)
	{
		thrust_particles[i].yg=-6.0f;
	}

}

int Spaceship::GetFuelAsPercentage() {
	GLfloat fuel_used = GLfloat(this->fuel)/MAX_FUEL
	return int(fuel_used * 100);
}

int Spaceship::GetAirAsPercentage() {
	GLfloat air_used = GLfloat(this->air)/MAX_AIR
	return int(air_used * 100);
}

GLfloat Spaceship::GetShipZAngle() {
	return this->z_rot;
}

void Spaceship::GetShipCoordinates(GLfloat& x, GLfloat& y) {
	x = this->x_index;
	y = this->y_index;
}
