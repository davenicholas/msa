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
 

#ifndef _LEVEL_H_
#define _LEVEL_H_

#include <GL/gl.h>
#include <GL/glu.h>

#include <SDL.h>

#include "massbody.h"
#include "planet.h"
#include "gas_planet.h"
#include "green_planet.h"
#include "moon.h"

#include <vector>

#define LEVEL_MAX 12

struct PickupEntity {
	PickupEntity(GLfloat x_index, GLfloat y_index, GLfloat z_angle) { 
		this->x=x_index; 
		this->y=y_index, 
		this->z_rot=z_angle;
		this->is_active = true;
	}
	GLfloat x, y, z_rot;
	bool is_active;
};

struct Star {
	Star(GLfloat x_index, GLfloat y_index, GLfloat star_size,
	    	GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) { 
		this->x=x_index; 
		this->y=y_index, 
		this->size=star_size;
		this->r=red;
		this->g=green;
		this->b=blue;
		this->a=alpha;
	}
	GLfloat x, y, size,	r, g, b, a;
};

struct Score {
	int remaining_air;
	int remaining_fuel;
};

class Level {

	public:
		Level(int level_number = 1);
		~Level();
		void SetLevel(int level_number = 1);
		bool NextLevel();
		void Reset();
		bool IsLevelComplete();
		void Render();
		void LoadMenuBackground();
		void RenderMenuBackground();
		void RenderStars();
		void RenderStellaBodies();
		void RenderMenuStellaBodies();
		void Update(Uint32 milliseconds);
		std::vector<EnvironmentalAttribute> GetEnvironmentalAttributes();
		void SetLevelScore(int fuel, int air);
		void PickUpCheck(GLfloat& x, GLfloat& y);
		int GetOverallScore();

	private:
		void LoadGLTextures();
		void SetUpEnvironment();
		void ResetEnvironment();
		void RenderPickUps();
		std::vector<MassBody*> stella_bodies;
		std::vector<MassBody*> menu_stella_bodies;
		std::vector<PickupEntity> pickup_entities;
		std::vector<Star> background_stars;
		int current_level;
		GLuint texture[2];
		bool has_active_pickups;
		Score player_score[LEVEL_MAX];
};

#endif
