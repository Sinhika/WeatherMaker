/*-------------------------------------------------------------------------*/
/* $Id: special.cpp,v 1.8 2002/03/09 13:22:40 cyhiggin Exp $ */
/* special.cpp - methods of class IntermediateReport for special cases */
/*-------------------------------------------------------------------------*/
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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "macros.h"
#include "wthrmkr.h"
#include "intermediatereport.h"
#include "tables.h"
#include <assert.h>

/*-------------------------------------------------------------------------*/
/* describe_special - list description of special weather conditions.     */
/*------------------------------------------------------------------------*/
void IntermediateReport::describe_special( char *buffer)
{
    SPECIALS sp;

    sp = pspec_table[special_step];
    sprintf( buffer, "%s", special_string[sp] );

    switch (sp) {
    case AA:                                    /* Cold Wave */
        process_cold_wave(buffer);
       break;
    case CC:                                            /* Hurricane */
        process_hurricane(buffer);
       break;
    case DD:                                            /* Drought */
        process_drought(buffer);
       break;
    case GG:                                            /* Gale */
        process_gale(buffer);
       break;
    case MM:                                           /* Fog */
        process_fog(buffer);
       break;
    case SS:                         /* Sandstorm/Blowing Snow */
       process_sandstorm(buffer);
       break;
    case TT:                                           /* Tornado */
        process_tornado(buffer);
       break;
    case XX:                                  /* Extreme Precipitation */
        process_extreme_precip(buffer);
       break;
    case ZZ:                                            /* Heat Wave */
        process_heat_wave(buffer);
       break;
    } /* endswitch */
} /* end */

//-----------------------------------------------------
// process_cold_wave - implemented per book.
//-----------------------------------------------------

void IntermediateReport::process_cold_wave( char *buffer)
{
    if (!duration_special) {
        duration_special = simple_dieroll(1,6,2);
        strcat( buffer, " begins...\n");
    } else
        strcat( buffer, " continues...\n");
    temperature_step -= (4 + Step_Temperature[die_roll]);
    temperature_step  = max( temperature_step, ptemp_table[T_MIN]-1);
    if (temperature_step < 0) {
        pDaily->actual_temperature[T_HIGH] = -30 + temperature_mod;
        pDaily->actual_temperature[T_LOW] = -50 + temperature_mod;
        duration_special = min(duration_special, 3);
    } else {
        pDaily->actual_temperature[T_HIGH] 
            = Code_Temperature[temperature_step][T_HIGH]
            + temperature_mod;
        pDaily->actual_temperature[T_LOW] 
            = Code_Temperature[temperature_step][T_LOW]
            + temperature_mod;
    } /* endif */
}

//-----------------------------------------------------
// process_heat_wave - implemented as per book.
//-----------------------------------------------------
void IntermediateReport::process_heat_wave( char *buffer)
{
    if (!duration_special) {
        duration_special = simple_dieroll(1,6,2);
        strcat( buffer, " begins...\n");
    } else
        strcat( buffer, " continues...\n");
    temperature_step += (4 + Step_Temperature[die_roll]);
    temperature_step  = min( temperature_step, ptemp_table[T_MAX]+1);
    if (temperature_step >=26 ) {
        pDaily->actual_temperature[T_HIGH] = 125 + temperature_mod;
        pDaily->actual_temperature[T_LOW] = 95 + temperature_mod;
        duration_special = min(duration_special, 3);
    } else {
        pDaily->actual_temperature[T_HIGH] 
            = Code_Temperature[temperature_step][T_HIGH]
            + temperature_mod;
        pDaily->actual_temperature[T_LOW] 
            = Code_Temperature[temperature_step][T_LOW]
            + temperature_mod;
    } /* endif */
}

//-----------------------------------------------------
// process_hurricane - implemented as per book
// - hurricanes do not happen +-5 deg. of Equator.
//-----------------------------------------------------
void IntermediateReport::process_hurricane( char *buffer)
{
   int i;

   if (!duration_special) {
// - day 1, on seacoast, severe tstorm. process_x_thunderstorm.
      duration_special = 6; 
      strcpy( buffer, "Approaching hurricane. Severe Thunderstorms.");
      process_x_thunderstorm(buffer);
   } else if (duration_special > 2) {
       strcat( buffer, " continues...");
// - wind velocity from 80-180mph (1d6*20+60).
      wind_speed = simple_dieroll(1,6,0)*20+60;
// - torrential rain, 2x DOWNPOUR precip for 1-2 days.
      precip_type = RAINFALL;
      special_set_ptype = 1;
      precipitation = DOWNPOUR;
      rain2inches(precipitation, rainfall, snowfall);
      rainfall *= ((double) simple_dieroll(1,4,2));
      is_precip = 1;
// - next 3 days, do not use day-to-day changes. Temperature
//   remains constant. rain & wind as above.
      temperature_step = yesterday_temp_step;
      pDaily->actual_temperature[T_HIGH] 
         = Code_Temperature[temperature_step][T_HIGH] + temperature_mod;
      pDaily->actual_temperature[T_LOW] 
         = Code_Temperature[temperature_step][T_LOW] + temperature_mod;
   }
   else {
      strcpy(buffer,"After the hurricane...");
// - 2 days following, wind <= 10mph from prevailing.
      wind_speed = simple_dieroll(1,10,0);
      pcurr_windstep->dir = PREVAILING;
// - 2 days following, no precip.
      is_precip = 0;
      precipitation = NO_RAIN;
      rain2inches(precipitation, rainfall, snowfall);
      special_set_ptype = 1;
// - 2 days following, temp varies only one step from storm day temp
//   (2d6: 6-, 1 step down; 7=no change; 8+ = 1 step up).
      i = simple_dieroll(2,6,0);
      if (i < 7) {
         temperature_step = yesterday_temp_step - 1;
         temperature_step = max( temperature_step, ptemp_table[T_MIN]);
         temperature_step = min( temperature_step, ptemp_table[T_MAX]);
      }
      else if (i==7) {
         temperature_step = yesterday_temp_step;
      }
      else if (i>7) {
         temperature_step = yesterday_temp_step + 1;
         temperature_step = max( temperature_step, ptemp_table[T_MIN]);
         temperature_step = min( temperature_step, ptemp_table[T_MAX]);
      }
      pDaily->actual_temperature[T_HIGH] 
         = Code_Temperature[temperature_step][T_HIGH] + temperature_mod;
      pDaily->actual_temperature[T_LOW] 
         = Code_Temperature[temperature_step][T_LOW] + temperature_mod;
   } // end-if
   strcat(buffer, "\n");
}

//-----------------------------------------------------
// process_drought - implemented as per book
//-----------------------------------------------------
void IntermediateReport::process_drought( char *buffer)
{
   static short precip_flag;

    if (!duration_special) {
        strcat( buffer, " begins...\n");
        // get duration 2d6+2 days
        duration_special = simple_dieroll(2,6,2);
        // 1st day, no precip.
        days_since_special_started = 0;
        rainfall = 0.0; snowfall = 0.0;
        precipitation = NO_RAIN;
        precip_flag = 0;
    } else {
       strcat( buffer, " continues...\n");
//    - TRACE precip becomes NONE
       if (precipitation <= TRACE)
          precipitation = NO_RAIN;
//    - In areas where LIGHT or MODERATE precip is normal max,
//      no greater than TRACE occurs, no more than 1 day in 5.
       else if (normal_max_precip <= MODERATE 
                && precip_flag == 0) {
          precipitation = TRACE;
          precip_flag = 1;
       }
//    - In areas where HEAVY+ is normal max, reduce all precip
//      results 2 steps.
       else if (normal_max_precip >= HEAVY) {
          precipitation = (RAIN) ( (int) precipitation - 2);
       }

       rain2inches(precipitation, rainfall, snowfall);
        
    }
    days_since_special_started++;
    if (days_since_special_started % 5 == 0)
       precip_flag = 0;

}

//-----------------------------------------------------
// process_gale - implemented as per book.
//-----------------------------------------------------
void IntermediateReport::process_gale( char *buffer)
{
    duration_special = 1;
    if (wind_speed > 20) {
        wind_speed = 50;
        if (pOutdoor->show_metric)
           strcat(buffer, ". Up to 115kph gusts");
        else
           strcat(buffer, ". Up to 70mph gusts");
    } else {
        wind_speed = 30;
        if (pOutdoor->show_metric)
           strcat(buffer, ". Up to 80kph gusts");
        else
           strcat(buffer, ". Up to 50mph gusts");
    }
    if (pOutdoor->terrain == DESERT || (accumulation > 0.0001)) {
        strcat(buffer, ", causing a ");
        process_sandstorm(buffer);
    }
    strcat(buffer, ".\n");
}

//-----------------------------------------------------
// process_fog - implemented as per book.
//-----------------------------------------------------
void IntermediateReport::process_fog( char *buffer)
{
   char fbuf[80];
   short fweight;

// - roll d6: sunrise (1-4), or sunset (5-6). lasts <roll> hours
   short fwhen = simple_dieroll(1,6,0);
   strcpy(fbuf, (fwhen <=4) ? "sunrise" : "sunset" );

// - fog is HEAVY on seacoast or near body of water; MODERATE elsewhere.
   fweight = (pOutdoor->terrain == SEACOAST) ? 2 : 1;
   sprintf(buffer, "%s fog for %hd hours starting at %s.\n",
           (fweight == 1) ? "Moderate" : "Heavy", fwhen, fbuf);
    duration_special = 1;
}

//-----------------------------------------------------
// process_sandstorm - implemented as per book.
//-----------------------------------------------------
void IntermediateReport::process_sandstorm( char *buffer)
{
   short sduration;
   char sbuf[30], svbuf[30], sbuf2[80];

// - if winds < gale-force, lasts d4+1 hours.
   svbuf[0] = 0;
   if (wind_speed < 35) {
      sduration = simple_dieroll(1,4,1);
      strcpy(sbuf, "Duststorm");
   }
   else if (wind_speed < 50) {
      sduration = simple_dieroll(1,4,1);
      strcpy(sbuf, "Sandstorm");
   }
// - else if winds >= gale-force, is "Severe" and lasts d6+1 hours.
   else {
      sduration = simple_dieroll(1,6,1);
      strcpy(svbuf, "Severe ");
      strcpy(sbuf, "sandstorm");
   }

// - no precip.
   is_precip = 0;

// - desert = sandstorm/duststorm; arctic/subarctic = blowing snow.
// actually, if there's snow on the ground, it's blowing snow.
   if (accumulation > 0.0001)
      strcpy(sbuf, "Storm of blowing snow");

// - poor visibility.
   sprintf( sbuf2, "%s%s for %hd hours.\n", 
            svbuf, sbuf, sduration);
   strcat(buffer, sbuf2);
   duration_special = 1;
} // end

//-----------------------------------------------------
// process_tornado - implemented.
// - temperature goes 2 steps higher.
// - strong winds, HEAVY precipitation 1d3 hours beforehand.
// - during tornado, temp drops 3 steps.
// - afterwards, rises to normal level for day.
//-----------------------------------------------------
void IntermediateReport::process_tornado( char *buffer)
{
// we don't care about hour-to-hour temperature.
// just process all that as a thunderstorm & print tornado string.
   process_x_thunderstorm(buffer);
    duration_special = 1;
    strcat(buffer, "\n");
}

//-----------------------------------------------------
// process_extreme_precip - implemented as per book
//-----------------------------------------------------
void IntermediateReport::process_extreme_precip( char *buffer)
{
   short result, tdx, which_col;
   EXTREMES our_extreme;

// - roll on extreme precipitation type table
   result = simple_dieroll(1,6,0);
   for (tdx=0; tdx<XTL_SIZE; tdx++)
      if (pDaily->actual_temperature[T_HIGH] < xtreme_temp_lookup[tdx])
         break;
   assert(tdx < XTL_SIZE);

   which_col =  (pOutdoor->clime >= SUBTROPIC) ? 0 : 1;

   if (result <= xtreme_type_lookup[which_col][tdx][0].high_roll)
      our_extreme =  xtreme_type_lookup[which_col][tdx][0].xtreme_type;
   else
      our_extreme =  xtreme_type_lookup[which_col][tdx][1].xtreme_type;

   // *I only if t >= 25, otherwise none.
   if (our_extreme == ISTORM && pDaily->actual_temperature[T_HIGH] < 25)
      our_extreme = NOSTORM;

   strcpy(buffer, extreme_precip_string[our_extreme]);
   duration_special = 1;

   switch(our_extreme) {
   case HSTORM:
      process_x_hailstorm(buffer);
      break;
   case TSTORM:
      process_x_thunderstorm(buffer);
      break;
   case SSTORM:
      process_x_blizzard(buffer);
      break;
   case ISTORM:
      process_x_icestorm(buffer);
      break;
   case NOSTORM:  // Extreme Precipitation should still mean something!
      if (precipitation < DOWNPOUR) {
         precipitation = (RAIN) ((int) precipitation + 1);
         rain2inches(precipitation, rainfall, snowfall);
      }
      else {  // just double it
         rainfall *= 2.0;
         snowfall *= 2.0;
      }
      break;
   };

   strcat(buffer, ".\n");
}

//-----------------------------------------------------
// process_x_thunderstorm - implemented as per book
//-----------------------------------------------------
void IntermediateReport::process_x_thunderstorm( char *buffer)
{
   double t_rain = 0.0;

// - tstorm drops .75-1.0 inches rain.
   t_rain = ((double) simple_dieroll(1,6,0)) * 0.05 + 0.70;
// - if LIGHT or MODERATE precip, tstorm replaces sched. precip.
   if (precipitation <= MODERATE)
      rainfall = t_rain;
// - else if HEAVY+ precip, tstorm begins sched. precip & adds to it.
   else
      rainfall += t_rain;
   precip_type = RAINFALL;
   special_set_ptype = 1;
}

//-----------------------------------------------------
// process_x_hailstorm - implemented as per book
//-----------------------------------------------------
void IntermediateReport::process_x_hailstorm( char *buffer)
{
   char hbuf[80];
// - lasts 1d6+4 minutes.
   short hduration = simple_dieroll(1,6,4);
// - .25 - 3.0 in diameter hailstones.
   double hdiameter = ((double) simple_dieroll(1,12,0)) * 0.25;
   double out_hdiameter;

   if (pOutdoor->show_metric)
      out_hdiameter = convert2cm(hdiameter);
   else 
      out_hdiameter = hdiameter;

   sprintf(hbuf, " %0.1f %s hailstones for %hd min", 
           out_hdiameter, pDaily->depth_unit_string, hduration);
   strcat(buffer, hbuf);
 // - hail accompanies a tstorm (process_x_thunderstorm).
   process_x_thunderstorm(buffer);
// - hail accumlates/melts as snow. (figure size of stone as depth).
   accumulation += hdiameter;

   precip_type = RAINFALL;
   special_set_ptype = 1;
}

//-----------------------------------------------------
// process_x_icestorm - implemented as per book
//-----------------------------------------------------
void IntermediateReport::process_x_icestorm( char *buffer)
{
   char ibuf[80];
// - ice coating .5-.75 thick
   double ithick = ((double) simple_dieroll(1,6,0)) * 0.05 + 0.45;
   double out_ithick;

   accumulation += ithick;
   if (pOutdoor->show_metric)
      out_ithick = convert2cm(ithick);
   else
      out_ithick = ithick;

   sprintf(ibuf, " coated everything with %0.1f %s of ice", 
           out_ithick, pDaily->depth_unit_string);
   strcat(buffer, ibuf);
   precip_type = FREEZE_RAIN;
   special_set_ptype = 1;
}

//-----------------------------------------------------
// process_x_blizzard - implemented as per book
//-----------------------------------------------------
void IntermediateReport::process_x_blizzard( char *buffer)
{
   double bfell;
   char bbuf[80];

// - full wind velocity at beginning, tapers off after 1-2 hours
//   to moderate breeze (15-20mph) at best.
   short bwind = simple_dieroll(1,6,14);
   short out_bwind;
   if (pOutdoor->show_metric)
      out_bwind = (short) convert2kph(bwind);
   else
      out_bwind = bwind;

// - if normal max precip is MODERATE, d6+4 inches of snow.
   if (normal_max_precip <= MODERATE) 
      bfell = ((double) simple_dieroll(1,6,4));
// - else if norm. max is HEAVY+, d6+6 inches of snow.
   else 
      bfell = ((double) simple_dieroll(1,6,6));
   snowfall += bfell;
   precip_type = SNOW;
   special_set_ptype = 1;

// - comes down at 1 inch/hour until accumlation is reached.
   sprintf(bbuf, " for %hd hours.", (short) bfell);
   strcat(buffer, bbuf);
   if (bwind < wind_speed)
      sprintf(bbuf, " Wind drops to %hd %s after 1-2 hours", 
             out_bwind, pDaily->vel_unit_string);
   strcat(buffer, bbuf);
}
