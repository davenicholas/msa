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

#include "text.h"

Text * Text::p_instance = NULL; 

Text::Text() {
	this->InitText();
}

Text * Text::Instance() {
   if (!p_instance)
      p_instance = new Text();

   return p_instance;
}


void Text::InitText() {
	Display *dpy;
	XFontStruct *fontInfo;
	base_text = glGenLists( 96 );
	dpy = XOpenDisplay( NULL );
	fontInfo = XLoadQueryFont( dpy, "-adobe-helvetica-medium-r-normal--12-*-*-*-p-*-iso8859-1" );
	if ( fontInfo == NULL ) 
		fontInfo = XLoadQueryFont( dpy, "fixed" );
	glXUseXFont( fontInfo->fid, 32, 96, base_text );
	XFreeFont( dpy, fontInfo );
	XCloseDisplay( dpy );
}

void Text::PrintText( const char *fmt, ... )
{
	char text[256]; 
	va_list ap;

	va_start(ap, fmt);
		vsprintf(text, fmt, ap);
	va_end(ap);

	glPushAttrib(GL_LIST_BIT);
	glListBase(base_text - 32);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();
}

