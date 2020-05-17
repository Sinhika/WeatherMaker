/* ------------------------------------------------- */
// dailyreport.h - class DailyReport
// $Id: dailyreport.h,v 1.5 2002/03/21 23:51:28 cyhiggin Exp $
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

#ifndef _DAILYREPORT_H
#define _DAILYREPORT_H

#include "calendar.h"

class DailyReport {
    public:
    /* temperature */
        short actual_temperature[2];
    /* wind */
        short actual_wind_speed;
        char wind_dir[10];
    /* rain */
        float rainfall;
        float accumulation;
        char precip_type_str[20];
    /* special */
        char special_buffer[400];
	char depth_unit_string[4];
	char vel_unit_string[4];
    /* lunar */
	short number_moons;
	LunarData *moons;     // allocate #moons-size array.
	JDate *p_jdate;      // current JDate
  private:
	short use_metric;
	char degree_char;
	char long_depth_unit_string[15];
  public:
	DailyReport(void);
        void Print(int compact=0);
	void SetMetric(short is_metric);
    }; /* end class */

#endif
