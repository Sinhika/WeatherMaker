/* ------------------------------------------------- */
// $Id: intermediatereport.h,v 1.6 2002/03/13 01:45:25 cyhiggin Exp $
// intermediatereport.h - class IntermediateReport
/* ------------------------------------------------- */
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
#ifndef _INTERMEDIATEREPORT_H
#define _INTERMEDIATEREPORT_H

#include "wthrmkr.h"
#include "dailyreport.h"
#include "outdoors.h"

typedef struct {
   short step;
   unsigned short dir;
} WIND;

typedef struct {
   EXTREMES xtreme_type;
   short low_roll, high_roll;
} XTREME_LOOKUP_ENTRY;

class IntermediateReport {
    /* stuff */
        Outdoors *pOutdoor;
        DailyReport *pDaily;
        short dice[2], 
              die_roll;
        short sp_dieroll;
    /* temperature */
        short temperature_step;
        short temperature_mod;
        short ground_temperature;
        const short *ptemp_table;
    /* wind */
        short wind_speed;
        WIND *pcurr_windstep;
        char p_prevailing_list[21];
        const short *pmax_wind;
    /* rain */
        short extra_precip,
              is_precip;
        float accumulation;
        RAIN precipitation;
        const RAIN *prain_table;
        RAINTYPE precip_type;
        double rainfall;
        double snowfall;
    /* special */
        short is_special;
        short duration_special;
        short special_step;
        short special_set_ptype;
        const SPECIALS *pspec_table;
        short days_since_special_started;
        RAIN normal_max_precip;
        short yesterday_temp_step;
    public:
        IntermediateReport( Outdoors *pOut, DailyReport *pD);
        void ChangeMonth( void );
        void ChangeSeason( void );
        void GenerateWeather(void);
        void PrintStatus(void);
        void describe_special( char *buffer);
    private:
        int parse_status_line(char *sline);
        void process_cold_wave(char *buffer);
        void process_heat_wave(char *buffer);
        void process_hurricane(char *buffer);
        void process_drought(char *buffer);
        void process_gale(char *buffer);
        void process_fog(char *buffer);
        void process_sandstorm(char *buffer);
        void process_tornado(char *buffer);
        void process_extreme_precip(char *buffer);
        void process_x_thunderstorm(char *buffer);
        void process_x_hailstorm(char *buffer);
        void process_x_icestorm(char *buffer);
        void process_x_blizzard(char *buffer);
        void rain2inches(RAIN precip, double &rainfall, double &snowfall);
    }; /* end class */

#endif
