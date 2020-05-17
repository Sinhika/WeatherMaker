// $Id: calendar.h,v 1.2 2002/03/21 23:51:28 cyhiggin Exp $
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

#ifndef _CALENDAR_H
#define _CALENDAR_H

#include "wthrmkr.h"

class JDate {
  protected:
   short year;    // year
   double fyear;  // year & day, decimalized
   double fday;   // day of year
   double length_year;  // EXACT length of year in days
  public:
   JDate();
   JDate( short yr, double d, double l);
   short get_year(void) { return year; };
   double get_fday(void) { return fday; };
   double get_fyear(void) { return fyear; };
   void set_year_day( short yr, double d);
   double get_length_year(void) {return length_year;};
   void set_length_year( double l);
   double Delta(JDate date1) {           // delta in days
      return (length_year * (fyear - date1.get_fyear())); 
   }; 
};

class LunarData {
  public:
   JDate known_full_moon;
   short phase_lookup_tbl[NUM_LUNARPHASES];
   char name[40];
   const char **phase_names;
  protected:
   double period;        // period (length of lunar month in days)
   LUNARPHASE get_phase( short frac_p_days);
   const char *get_phase_name( LUNARPHASE phase) { 
      return phase_names[(int) phase];
   };
  public:
   LunarData();
   void set_period(double p);
   double get_period(void) { return period;};
   const char *GetPhaseNameByJDate(JDate curr_jdate);
};

class MonthData {
  public:
   const char *name;
   short num_days;             // "normal" number of days per month
   short jday[2];              // julian day, 1st & last of month
   short trailing_extra_days;  // count of inter-monthly days following the month
   short extra_day_ndx[2];     // index into extra_day_names, 1st-to-last.
   short has_leap_day;         // has a leap day added to end on leap years?
   short has_season_change_day; // an equinox or solistice falls in this month
}; // end class

class Calendar {
  public:
   short year;
   const char *season_string[NUM_SEASONS];
   short number_moons;
   LunarData *moons;     // allocate #moons-size array.

  protected:
   CALCONFIG config;
   short days_per_year;
   MonthData months[NUM_MONTHS];
   double exact_year_length;
   // leap year stuff
   LEAPMETHOD leap_method;
   short is_leap_year;
   short leap_period;
   const char *leapday_name;
   // "extra" day stuff
   const char **extra_day_names;
   short num_extra_days;
   // seasonal stuff
   short season_divisions[NUM_SEASONS];  // 1-based Julian days of equinoxes & solistices
   short change_day_of_month[NUM_SEASONS]; // day of month of each equinox & solistice.
   // lunar phase stuff
   short do_lunar_stuff;
  protected:
   void set_earth_calendar();
   void set_FR_calendar();
   void set_Greyhawk_calendar();
   void set_calendar_config(CALCONFIG c);
   short count_days_year(short &num_extra);
   short check_leap_year();
   void set_season_divisions();

  public:
   Calendar();
   Calendar(short yr, CALCONFIG cfg=USE_EARTH, short show_moon=0);
   ~Calendar();
   const char *GetMonthString(short month);
   short GetMonthByDay(short jday);
   const char *GetSeasonString(SEASON season);
   SEASON GetSeasonByDay(short jday);
   int GetDayString(short jday, char *day_string, int size_string);
   short GetJDayByMonthDay( short mo, short day);
   void ChangeYear(short new_yr);
   short FindMonthDayAfterN(short jday, short ndays, 
			    short &new_mo, short &new_day);
   double GetDayLength() {return exact_year_length;};
}; // end class

#endif
