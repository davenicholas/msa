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
 

#include "error.h"									

static FILE * ErrorLog;								

bool InitErrorLog(void)								
{
	if(!(ErrorLog = fopen(LOG_FILE, "w"))) {
		perror("Can't init Logfile!\n" );			
		exit(1);									
	}

	Log("%s V%s -- Log Init...\n\n", APP_NAME, APP_VERSION);						

	return true;									
}

void CloseErrorLog(void)							
{
	Log("\n-- Closing Log...\n");					
	if(ErrorLog) {
		fclose(ErrorLog);							
	}
	return;											
}

int Log(const char * szFormat, ...)						
{
	va_list Arg;									

	va_start(Arg,szFormat);							
	if(ErrorLog) {
		vfprintf(ErrorLog, szFormat, Arg);			
		fflush(ErrorLog);							
	}
	va_end(Arg);									
	return 0;										
}
