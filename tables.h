// $Id: tables.h,v 1.6 2002/03/13 01:45:25 cyhiggin Exp $
/*
    Copyright (C) 2002 Cynthia Higginbotham
    This file is part of WeatherMaker.

    WeatherMaker is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    WeatherMaker is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with WeatherMaker; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
    or visit their website at http://www.fsf.org.
 */
/* tables.h - constant data tables */

#ifndef _TABLES_H
#define _TABLES_H

#include "wthrmkr.h"
#include "intermediatereport.h"

extern const short Table_Temperature[NUM_CLIMATES][NUM_TERRAINS][NUM_MONTHS][3];
extern const short Code_Temperature [26][2];
extern const short Step_Temperature [11];
extern WIND Step_Wind[11];
extern const char * Table_Prevailing[NUM_CLIMATES+1];
extern const short Table_MaxWind[NUM_CLIMATES];
extern const RAIN Table_Precipitation[NUM_CLIMATES][NUM_TERRAINS][NUM_SEASONS][3];
extern const char* precipitation_string[NUM_RAINTYPES];
extern const short Step_Special[2][11];
extern const SPECIALS Table_Special [NUM_CLIMATES][NUM_TERRAINS][NUM_SEASONS][4];
extern const char *extreme_precip_string[NUM_EXTREMES];
extern const char* special_string[NUM_SPECIALS];
extern const char* climate_string[NUM_CLIMATES];
extern const char* terrain_string[NUM_TERRAINS];
extern const short xtreme_temp_lookup[XTL_SIZE];
extern const XTREME_LOOKUP_ENTRY xtreme_type_lookup[2][XTL_SIZE][2];
#endif
