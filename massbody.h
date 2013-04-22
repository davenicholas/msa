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
 


#ifndef _MASSBODY_H_
#define _MASSBODY_H_

#include <GL/gl.h>

struct EnvironmentalAttribute {
	GLfloat x, y, s;
};

class MassBody {
	
	public:		
		virtual void Render() = 0;
		virtual EnvironmentalAttribute GetLocationAndDensity() {
			EnvironmentalAttribute ev;
			ev.x = this->x_index;
			ev.y = this->y_index;
			ev.s = this->size;
			return ev;
		}
		
	protected:
		GLfloat density;
		GLfloat size;
		GLfloat x_index;
		GLfloat y_index;
		GLfloat	zrot;
		GLfloat	yrot;
		GLfloat zspeed;	
		GLfloat yspeed;
		
		virtual void LoadGLTextures() = 0;
};

#endif
