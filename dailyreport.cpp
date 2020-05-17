// $Id: dailyreport.cpp,v 1.10 2002/05/18 17:22:28 cyhiggin Exp $
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "macros.h"

/*----------------------------------------------------------------*/
/* class headers                                                  */
/*----------------------------------------------------------------*/
#include "dailyreport.h"

/*----------------------------------------------------------------*/
/* global variables                                               */
/*----------------------------------------------------------------*/
#include "tables.h"


/*----------------------------------------------------------------------------------*/
/* DailyReport methods                                                              */
/*----------------------------------------------------------------------------------*/
DailyReport::DailyReport(void) {
   strcpy(precip_type_str, "");
   strcpy(special_buffer, "");
   accumulation = 0.0;
   degree_char = 'F';
   strcpy(vel_unit_string,"mph");
   strcpy(depth_unit_string, "in.");
   strcpy(long_depth_unit_string, "inches");
   number_moons = 0;
   moons = NULL;
}

void DailyReport::SetMetric(short is_metric) {
   use_metric = is_metric;
   if (use_metric) {
      degree_char = 'C';
      strcpy(vel_unit_string, "kph");
      strcpy(depth_unit_string, "cm");
      strcpy(long_depth_unit_string, "centimeters");
   }
}

void DailyReport::Print(int compact) {
    char nl;
    char temp_string[80], temp_string2[80], moon_string[160];
    short output_temperature[2], output_wind_speed;
    double output_rainfall, output_accum;
    int i, nchar;

    moon_string[0] = 0;

    if (use_metric) {
       output_temperature[T_HIGH] = (short) convert2celsius(actual_temperature[T_HIGH]);
       output_temperature[T_LOW] = (short) convert2celsius(actual_temperature[T_LOW]);
       output_wind_speed = (short) convert2kph(actual_wind_speed);
       output_rainfall = convert2cm(rainfall);
       output_accum = convert2cm(accumulation);
    }
    else {
       output_temperature[T_HIGH] = actual_temperature[T_HIGH];
       output_temperature[T_LOW] = actual_temperature[T_LOW];
       output_wind_speed = actual_wind_speed;
       output_rainfall = rainfall;
       output_accum = accumulation;
    }

    nl = compact ? ' ' : '\n';
    if (compact) {
       if (output_wind_speed > 0)
	  sprintf(temp_string, "wind: %2hd %s %s ",
		  output_wind_speed, vel_unit_string, wind_dir);
       else 
	  strcpy(temp_string, "No wind. ");
       if (number_moons > 0 && moons != NULL) {
	  for (i=0; i<number_moons; i++) {
	     snprintf(temp_string2, 79, "%s: %s ", 
		      moons[i].name, moons[i].GetPhaseNameByJDate(*p_jdate));
	     temp_string2[79] = 0;
	     nchar = 159 - strlen(moon_string);
	     strncat(moon_string, temp_string2, nchar);
	     moon_string[159] = 0;
	  } // end-for
       } // end-if
    }
    else {
       if (output_wind_speed > 0)
	  sprintf(temp_string, "with a %2hd %s wind out of the %s. ",
		  output_wind_speed, vel_unit_string, wind_dir);
       else
	  strcpy(temp_string, "with not a breath of wind. ");
       if (number_moons > 0 && moons != NULL) {
	  for (i=0; i<number_moons; i++) {
	     snprintf(temp_string2, 79, "%s is a %s moon. ", 
		      moons[i].name, moons[i].GetPhaseNameByJDate(*p_jdate));
	     temp_string2[79] = 0;
	     nchar = 159 - strlen(moon_string);
	     strncat(moon_string, temp_string2, nchar);
	     moon_string[159] = 0;
	  } // end-for
       } // end-if
    }

    printf( "%chigh: %2hd %c low: %2hd %c %s\n",
	    nl, output_temperature[T_HIGH], degree_char,
	    output_temperature[T_LOW], degree_char,
	    temp_string);
    if (number_moons > 0 && moons != NULL)
       printf("%s\n", moon_string);

    if (output_accum > 0.0001) {
       if (compact)
	  printf("Snow depth: %3.1f %s ", output_accum, depth_unit_string);
       else
	  printf( "%3.1f %s of snow on the ground. ", 
		  output_accum, long_depth_unit_string );
    }
    if (output_rainfall > 0.0001) {
       if (compact)
	  printf("%3.1f %s %s",  output_rainfall, depth_unit_string, precip_type_str );
       else
	  printf( "%3.1f %s of %s fell.", 
		  output_rainfall, long_depth_unit_string, precip_type_str );
    }
   
    if (output_rainfall > 0.0001 || output_accum > 0.0001 )
        printf("\n");
    fputs(special_buffer, stdout);
    if (!compact) {
        printf("--------------------------------------------------------------------------\n");
    } /* endif */
} /* end */
