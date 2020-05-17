// $Id: intermediatereport.cpp,v 1.6 2002/03/13 01:45:25 cyhiggin Exp $
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
#include "calendar.h"
#include "intermediatereport.h"

/*----------------------------------------------------------------*/
/* global variables                                               */
/*----------------------------------------------------------------*/
#include "tables.h"

/*----------------------------------------------------------------------------------*/
/* Methods - IntermediateReport                                                     */
/*----------------------------------------------------------------------------------*/
IntermediateReport::IntermediateReport(Outdoors *pOut, DailyReport *pD)
{
  int iStatus = 0;
    pOutdoor = pOut;
    pDaily = pD;
    pDaily->SetMetric(pOutdoor->show_metric);

    ChangeMonth();
    ChangeSeason();

    // Initialize from status_line, if available
    if (pOut->external_init_flag) {
      iStatus = parse_status_line(pOut->status_line);
    }

    /* temperature */
    if (!iStatus)
      temperature_step = ptemp_table[T_NORM];
    temperature_mod = ((pOut->default_elevation() - pOut->elevation)/1000) * 3;
    if (!iStatus)
        ground_temperature = (Code_Temperature[temperature_step][T_HIGH]
                          + Code_Temperature[temperature_step][T_LOW]) / 2
                         + temperature_mod;

    /* wind */
    pcurr_windstep = &(Step_Wind[5]);
    if (!iStatus)
        wind_speed = 0;
 
    /* rain */
    extra_precip = is_precip = 0;
    if (!iStatus)
        accumulation = 0.0;
    precipitation = NO_RAIN;
 
    /* other */
    if (!iStatus) {
        is_special = 0;
        duration_special = 0;
        special_step = 0;
    }
} /* end */

void IntermediateReport::ChangeSeason( void )
{
    short i, n;
    char *pstr, *tok=NULL;

    /* wind */
    pmax_wind = &(Table_MaxWind[pOutdoor->clime]);
    if (pOutdoor->clime == SUBARCTIC && pOutdoor->season == SUMMER)
        strncpy(p_prevailing_list, Table_Prevailing[NUM_CLIMATES], 20);
    else
        strncpy(p_prevailing_list, Table_Prevailing[pOutdoor->clime], 20);
    n = simple_dieroll(1,3,0);
    pstr = p_prevailing_list;
    for (i=0;i<n; i++) {
        tok = strtok(pstr, " ");
        if (!tok)
           break;
        pstr = NULL;
    } /* endfor */
    if (tok)
       memmove(p_prevailing_list, tok, strlen(tok)+1);

    /* rain */
    prain_table 
       = Table_Precipitation[pOutdoor->clime][pOutdoor->terrain][pOutdoor->season];
    normal_max_precip = max(prain_table[0], max(prain_table[1], prain_table[2]));
    precip_type = (pOutdoor->season == WINTER) ? SNOW : RAINFALL;

    /* other */
    //    pspec_table = Table_Special[pOutdoor->clime][pOutdoor->terrain][pOutdoor->season];
    // following necessary to work around some weird GCC 2.96 compiler quirk.
    int cdex = pOutdoor->clime;
    int tdex = pOutdoor->terrain;
    pspec_table = Table_Special[cdex][tdex][pOutdoor->season];
} // end

void IntermediateReport::ChangeMonth( void )
{
//    now handled in Outdoors::IterateDate()
//    pOutdoor->season = CALC_SEASON(pOutdoor->month[0]);
   
    /* temperature */
    ptemp_table 
       = Table_Temperature[pOutdoor->clime][pOutdoor->terrain][pOutdoor->month[0]];

    /* wind */
    // seasonal change, now handled in ChangeSeason()

    /* rain */
    // seasonal change, now handled in ChangeSeason()

    /* other */
    // seasonal change, now handled in ChangeSeason()
} /* end */


void IntermediateReport::GenerateWeather(void)
{
    short accum_factor = 0, avg_temp;
    
    rainfall=0.0; snowfall=0.0;
    special_set_ptype = 0;

    /* roll dice */
    die_roll = diceroll( 2, 6, 0, (short *) dice )-2;
    if (duration_special == 0) {
      special_step = 0;
      is_special = (die_roll == 0 || die_roll == 10);
    }

    /* INTERMEDIATE processing */

    /* temperature */
    yesterday_temp_step = temperature_step;
    temperature_step = max( temperature_step, ptemp_table[T_MIN]);
    temperature_step = min( temperature_step, ptemp_table[T_MAX]);

    temperature_step = temperature_step + Step_Temperature[die_roll];
    temperature_step = max( temperature_step, ptemp_table[T_MIN]);
    temperature_step = min( temperature_step, ptemp_table[T_MAX]);

    pDaily->actual_temperature[T_HIGH] 
       = Code_Temperature[temperature_step][T_HIGH] + temperature_mod;
    pDaily->actual_temperature[T_LOW] 
       = Code_Temperature[temperature_step][T_LOW] + temperature_mod;
 
    /* wind */
    wind_speed = min (max( wind_speed, 0), *pmax_wind);
    pcurr_windstep = &(Step_Wind[die_roll]);
    wind_speed += pcurr_windstep->step;
    wind_speed = min (max( wind_speed, 0), *pmax_wind);

    /* rain */
    is_precip = !(die_roll % 2);
    if (is_special )
        extra_precip = 0;
    else
        extra_precip += ((dice[0] == dice[1]) ? 1 : - extra_precip);
    if (is_precip) {
        if (dice[0] < dice[1])
           precipitation = prain_table[0];
        else if (dice[0] == dice[1])
           precipitation = prain_table[1];
        else
           precipitation = prain_table[2];

        /* check for heavy precipitation */
        if ( extra_precip == 2                      // TODO - check this!
             && simple_dieroll(1,2,0) == 1
             && precipitation < DOWNPOUR ) 
        {
            precipitation = (RAIN) ((int) precipitation + 1);
        } /* endif */

        /* how much fell and how much accumulated */
        rain2inches(precipitation, rainfall, snowfall);
    } /* endif */


    /* special */
    pDaily->special_buffer[0] = '\0';
    if (is_special) {
        if (duration_special == 0) {
            sp_dieroll = simple_dieroll(2,6,-2);
            if (die_roll == 0)
               special_step = Step_Special[0][sp_dieroll];
            else
               special_step = Step_Special[1][sp_dieroll];
        }
        if (special_step > -1) {
            describe_special( pDaily->special_buffer );
            if (duration_special)
               duration_special--;
        } else {
          special_step = 0;
          is_special = 0;
        } /* endif */
    } /* endif */

/* post-Special processing */
    /* temperature */
    avg_temp = (pDaily->actual_temperature[T_HIGH]
                + pDaily->actual_temperature[T_LOW] ) / 2;
    ground_temperature 
       = (short) ( ((double) ground_temperature * 0.9) + (0.1 * (double) avg_temp) );

    /* snow melt */
    accum_factor = 0;
    if (pDaily->actual_temperature[T_HIGH] > 60) {
        accum_factor = -5;
    } else if (pDaily->actual_temperature[T_HIGH] > 45) {
        accum_factor = -3;
    } else if (pDaily->actual_temperature[T_HIGH] > 32) {
        accum_factor = -1;
    } /* endif */

    if (ground_temperature > 60) {
        accum_factor += -5;
    } else if (ground_temperature > 45) {
        accum_factor += -3;
    } else if (ground_temperature > 32) {
        accum_factor += -1;
    } else {
        accum_factor += 1;
    } /* endif */
    accum_factor = min( 0, accum_factor);

    if (pDaily->actual_temperature[T_LOW] > 60) {
       accumulation += (3.0 * (float) accum_factor);
    } else if (pDaily->actual_temperature[T_LOW] > 45) {
       accumulation += (2.0 * (float) accum_factor);
    } else if (pDaily->actual_temperature[T_LOW] > 32) {
       accumulation += (float) accum_factor;
    } else if (pDaily->actual_temperature[T_LOW] > 20) {
       accumulation += (0.5 * (float) accum_factor);
    } else {
       accumulation += (0.1 * (float) accum_factor);
    } /* endif */
    accumulation = max( 0.0, accumulation);
    accum_factor = 0;

    /* wind */
    switch (pcurr_windstep->dir) {
    case FROM_TROPIC:
       strcpy( pDaily->wind_dir, "S");
       break;
    case FROM_ARCTIC:
       strcpy( pDaily->wind_dir, "N");
       break;
    case PREVAILING:
       strcpy( pDaily->wind_dir, p_prevailing_list);
       break;
    } /* endswitch */
    pDaily->actual_wind_speed = wind_speed;

    /* rain */
    if (is_precip) {
       if (!special_set_ptype) {
          if (pDaily->actual_temperature[T_LOW] >= 36) {
             precip_type = RAINFALL;
          } else if (pDaily->actual_temperature[T_HIGH] < 30) {
             precip_type = SNOW;
          } else {
             switch (simple_dieroll(1,3,0)) {
             case 1:
                precip_type = SNOW;
                break;
             case 2:
                precip_type = SLEET;
                break;
             case 3:
                precip_type = FREEZE_RAIN;
                break;
             } /* endswitch */
          } /* endif */
       }
       switch(precip_type) {
       case RAINFALL: accum_factor = -10; break;
       case SNOW: accum_factor = 1; break;
       case SLEET: accum_factor = 0; break;
       case FREEZE_RAIN: accum_factor = 0; break;
       };

       switch (precip_type) {
       case RAINFALL:
       case FREEZE_RAIN:
       case SLEET:
          accumulation += ((float) accum_factor * rainfall);
          accumulation = max( 0.0, accumulation);
          pDaily->rainfall = rainfall;
          break;
       case SNOW:
          accumulation += ((float) accum_factor * snowfall);
          pDaily->rainfall = snowfall;
          break;
       } /* endswitch */
       strcpy( pDaily->precip_type_str, precipitation_string[precip_type]);
    } else {  /* no precipitation */
       pDaily->rainfall = 0.0;
       pDaily->precip_type_str[0] = '\0';
    } /* endif */
    pDaily->accumulation = accumulation;

} /* end */

void IntermediateReport::PrintStatus(void)
{
  printf("STATUS %hd %hd %hd %hd %hd %hd %ld %hd %hd %hd %f %hd %hd %hd %s\n", 
         (short) pOutdoor->clime, (short) pOutdoor->terrain,
         (short) pOutdoor->calendar_cfg, pOutdoor->year[0],
         pOutdoor->month[0], pOutdoor->day[0], pOutdoor->elevation,
         temperature_step, ground_temperature, 
         wind_speed, (double) accumulation, 
         is_special, duration_special, special_step,
         p_prevailing_list);
 
} /* end */

int  IntermediateReport::parse_status_line(char *sline)
{
    int igot;

    igot = sscanf(sline, "STATUS %*d %*d %*d %*d %*d %*d %*d"
                  " %hd %hd %hd %f %hd %hd %hd %2[NWSE]",
                  &temperature_step, &ground_temperature, &wind_speed,
                  &accumulation, &is_special, &duration_special, &special_step,
                  p_prevailing_list);

    // bad parse?
    if (igot != 8) {
        printf("Bad status line(2): %s\n", sline);
    }

    // everything okay
    return (igot == 8);
} /* end */

void IntermediateReport::rain2inches(RAIN precip, double &rainfall, double &snowfall)
{
   /* how much fell and how much accumulated */
   switch (precip) {
   case NO_RAIN:
      rainfall = 0.0; snowfall = 0.0;
      break;
   case TRACE:
      rainfall = 0.125; snowfall = 0.5;
      break;
   case LIGHT:
      rainfall = 0.5; snowfall = 1.0;
      break;
   case MODERATE:
      rainfall = 0.75; snowfall = 2.0;
      break;
   case HEAVY:
      rainfall = 1.5; snowfall = 4.0;
      break;
   case DOWNPOUR:
      rainfall = 3.0; snowfall = 8.0;
      break;
   } /* endswitch */
} // end
