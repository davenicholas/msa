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
 


#ifdef WIN32										
#include <windows.h>								
#else												
#include <stdio.h>									
#include <stdlib.h>									
#include <stdarg.h>									
#endif		

#define APP_NAME "Marianas Space Adventure - Dave Nicholas" 
#define APP_VERSION	"0.1"
#define LOG_FILE "log.txt"

bool InitErrorLog(void);	
void CloseErrorLog(void);
int  Log(const char *, ...);
