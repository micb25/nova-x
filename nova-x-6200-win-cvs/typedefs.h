/**
 * Type Definitions
 * This file contains type definitions, blah
 *
 * @version		1.0
 * @copyight	Copyright (C) 2002 - 2003 NovaX Development Team
 * @license		This program is free software; you can redistribute it and/or modify it under the terms of the
 *				GNU General Public License as published by the Free Software Foundation; either version 2 of the
 *				License, or (at your option) any later version. 
 *				This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 *				even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *				General Public License for more details. 
 *				You should have received a copy of the GNU General Public License along with this program;
 *				if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 *				MA 02111-1307, USA.
 *
 * @history		Date			Author			Changes
 *				2003/02/04		Tobias Philipp	Module/Project started.
 */

#ifndef _TYPEDEFS_H_
#define _TYPEDEFS_H_

#define pagetableentry unsigned long
#define NOVAX_API
#define null 0
#define true 1
#define false 0

// Colors

#define COLOR_BLACK         0
#define COLOR_NAVY          1
#define COLOR_GREEN         2
#define COLOR_TEAL          3
#define COLOR_RED           4
#define COLOR_PURPLE        5
#define COLOR_MAROON        6
#define COLOR_LIGHTGRAY     7
#define COLOR_DARKGRAY      8
#define COLOR_LIGHTPURPLE   9
#define COLOR_LIME          10
#define COLOR_BRIGHTTEAL    11
#define COLOR_LIGHTRED      12
#define COLOR_BRIGHTPURPLE  13
#define COLOR_YELLOW        14
#define COLOR_WHITE         15

typedef void NXvoid;
typedef short NXshort;
typedef unsigned short NXushort;
typedef int NXint;
typedef unsigned int NXuint;
typedef long NXlong;
typedef unsigned long dword;
typedef unsigned long NXulong;
typedef char NXchar;
typedef char* NXstring;
typedef int NXbool;
typedef unsigned short NXwstring;		/* wide string (corresponds to wchar_t)

//typedef long long NXsize;				 corresonds to size_t (64bit)

typedef NXvoid (*NXerrorhandler) (NXstring, NXstring, NXuint, NXstring);

typedef NXvoid (*NXwarninghandler) (NXstring, NXstring, NXuint, NXstring);

*/

/* Zeitstruktur für Nova-X */

typedef struct {
    unsigned char hours;
    unsigned char minutes;
    unsigned char seconds;
} t_time;

/* Datumsstruktur für Nova-X */

typedef struct {
    unsigned char day;
    unsigned char month;
    unsigned int year;
} t_date;

static t_date currentdate;
static t_time currenttime;

#endif
