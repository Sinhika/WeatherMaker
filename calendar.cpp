// $Id: calendar.cpp,v 1.5 2002/05/18 17:22:28 cyhiggin Exp $
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

/*----------------------------------------------------------------*/
/* class headers                                                  */
/*----------------------------------------------------------------*/
#include "macros.h"

#include "wthrmkr.h"
#include "calendar.h"

/*----------------------------------------------------------------*/
/* global variables                                               */
/*----------------------------------------------------------------*/

static const short earth_days_per_month[NUM_MONTHS] = 
{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static const short FR_extra_days_per_month[NUM_MONTHS] =
{1,0,0,1,0,0,1,0,1,0,1,0};

static const short Grey_extra_days_per_month[NUM_MONTHS] =
{0,0,7,0,0,7,0,0,7,0,0,7};

static const char* FR_extra_day_names[] = 
{ "Midwinter Day", "Greengrass Day", "Midsummer's Day", 
  "High Harvestide Day", "Feast of the Moon", "ShieldMeet"};

static const char* Grey_extra_day_names[] = 
{ "Growfest","Richfest","Brewfest","Needfest"};

static const short earth_change_day[4] = {22,22,22,22};
static const short FR_change_day[4] = {19,10,21,20};
static const short Grey_change_day[4] = {1,32,1,1};

static const char* norm_season_string[NUM_SEASONS] = 
{"Winter", "Spring", "Summer", "Fall"};

static const char *earth_month_string[NUM_MONTHS] = 
{"January", "February", "March",  "April", "May", "June", 
 "July", "August", "September", "October", "November", "December" };

static const char *FR_month_string[NUM_MONTHS] = 
{"Hammer", "Alturiak", "Ches", "Tarsakh", "Mirtul","Kythorn", 
 "Flamerule", "Eleasias", "Eleint", "Marpenoth", "Uktar", "Nightal" };

static const char *Grey_month_string[NUM_MONTHS] =
{"Fireseek", "Readying", "Coldeven", "Planting", "Flocktime",
 "Wealsun", "Reaping", "Goodmonth", "Harvester", "Patchwall",
 "Ready'reat", "Sunsebb" };

/*----------------------------------------------------------------*/
/* Methods - Calendar                                             */
/*----------------------------------------------------------------*/
Calendar::Calendar() 
{
   config = USE_EARTH;
   leap_method = EARTHLEAP;
   do_lunar_stuff = 0;
   set_calendar_config(USE_EARTH);
   ChangeYear(2002);
} // end

Calendar::Calendar(short yr, CALCONFIG cfg, short show_moon) 
{
   do_lunar_stuff = show_moon;
   config = cfg;
   set_calendar_config(cfg);
   ChangeYear(yr);
} // end

Calendar::~Calendar() 
{
   if (do_lunar_stuff && moons != NULL)
      delete [] moons;
   if (config == USE_FILE) {
        ; //TODO: remove allocated structures
   }
} // end

const char * Calendar::GetMonthString(short month)
{
   return (months[month].name);
}// end

short Calendar::GetMonthByDay(short day)
{
   short i;

   // check for obvious bad inputs
   if (day > days_per_year || day < 1)
      return -1;

   for (i=0; i<NUM_MONTHS; i++) {
      // day falls neatly into a month
      if (months[i].jday[1] >= day && months[i].jday[0] <= day)
         return i;
      // day is a trailing "special" day attached to previous month
      if (months[i].jday[0] > day)
         return i-1;
   } // end-for
   // is this a trailing day at end of year?
   if (months[NUM_MONTHS-1].jday[1] < day)
      return NUM_MONTHS-1;
   // nothing made sense. return error value
   return -1;
}// end

const char * Calendar::GetSeasonString(SEASON season)
{
   return (season_string[(int) season]);
}// end

SEASON Calendar::GetSeasonByDay(short day)
{
   // check for obvious bad inputs
   if (day > days_per_year || day < 1)
      return BADSEASON;

   if (day < season_divisions[D_SPRING_EQ])
      return WINTER;
   if (day >= season_divisions[D_SPRING_EQ]
       && day < season_divisions[D_SUMMER_SOL])
      return SPRING;
   if (day >= season_divisions[D_SUMMER_SOL]
       && day < season_divisions[D_FALL_EQ])
      return SUMMER;
   if (day >= season_divisions[D_FALL_EQ]
       && day < season_divisions[D_WINTER_SOL])
      return FALL;
   // remaining case: if (day >= season_divisions[D_WINTER_SOL])
   return WINTER;
} // end

int  Calendar::GetDayString(short day, char *day_string, int size_string)
{
   short mo, nth = 0, ndx;

   // check for obvious bad inputs
   if (day > days_per_year || day < 1 || day_string == NULL || size_string < 7)
      return -1;
   
   mo = GetMonthByDay(day);

   // "special" inter-month day?
   if (day > months[mo].jday[1]) {
      nth = day - months[mo].jday[1] - 1;
      if (nth < months[mo].trailing_extra_days) {

         // make sure no problems with name array
         if (extra_day_names == NULL) {
            day_string[0] = 0;
            return -2;
         }
         // check for special case of "festival" mini-month
         if (months[mo].extra_day_ndx[0] == months[mo].extra_day_ndx[1]
             && months[mo].trailing_extra_days > 1) {
            ndx = months[mo].extra_day_ndx[0];
            snprintf(day_string, size_string, "%s %hd",  
                     extra_day_names[ndx], nth+1);
         }
         // else each trailing day has its own name
         else {
            ndx = months[mo].extra_day_ndx[0]+nth;
            snprintf(day_string, size_string, "%s", extra_day_names[ndx]);
         }
      } 
      // named leapday?
      else if (nth == months[mo].trailing_extra_days 
               && months[mo].has_leap_day 
               && leap_method == SIMPLE_EXTRALEAP) {
         snprintf(day_string, size_string, "%s", leapday_name);
      }
   }
   // else ordinary day-of-month
   else {
      nth = day - months[mo].jday[0] + 1;
      snprintf(day_string, size_string, "%s %hd", 
               GetMonthString(mo), nth);
   }
   return 0;
} // end

short  Calendar::GetJDayByMonthDay( short mo, short day)
{
   short i;

   // check for obvious bad inputs
   if (mo < 0 || mo > (NUM_MONTHS-1))
      return -1;
   if (day < 0 
       || day > (months[mo].num_days
                 +months[mo].trailing_extra_days
                 +months[mo].has_leap_day))
      return -2;
   
   i = months[mo].jday[0] + day - 1;
   return i;
} // end

void Calendar::ChangeYear(short new_yr)
{
  year = new_yr;
  is_leap_year = check_leap_year();
  days_per_year = count_days_year(num_extra_days);
  set_season_divisions();
}

// returns number of years later.
short Calendar::FindMonthDayAfterN(short jday, short ndays, 
                                   short &new_mo, short &new_day)
{
   short temp_day,years, save_start_year;//, prev_year;

   // check for obvious bad inputs
   if (jday < 1 || jday > days_per_year || ndays < 1)
      return -1;

   temp_day = jday+ndays;
   years = 0;
   save_start_year = year;
   while (temp_day > 0) {
      // reduced to simple case: ndays later is in same year
      if (temp_day <= days_per_year) {
         new_mo = GetMonthByDay(temp_day);
         new_day = temp_day - months[new_mo].jday[0] + 1;
         break;
      } // end-if

      // not-so-simple; ndays is one or more years later
      // count off days for THIS year.
      temp_day -= days_per_year;

      // jump to next year
      years++;
      //prev_year = year;
      ChangeYear(year+1);
   } // end-while

   if (years > 0)
      ChangeYear(save_start_year);
   return years;
} // end

//*---------------------------------------------------------*//
//  Private Methods
//*---------------------------------------------------------*//

void Calendar::set_earth_calendar()
{
   int i;
   leap_method = EARTHLEAP;

   // set regular monthly data
   for (i=0; i<NUM_MONTHS; i++) {
      months[i].name = earth_month_string[i];
      months[i].num_days = earth_days_per_month[i];
      months[i].trailing_extra_days = 0;
      months[i].has_leap_day = 0;
      months[i].has_season_change_day = 0;
      months[i].extra_day_ndx[0] =  months[i].extra_day_ndx[1] = 0;
   }
   // note leap month
   months[1].has_leap_day = 1;
   // note equinoxes and solistices
   months[2].has_season_change_day = 1;
   months[5].has_season_change_day = 1;
   months[8].has_season_change_day = 1;
   months[11].has_season_change_day = 1;
   // earth calendar has no inter-monthly days.

    for (i=0; i<NUM_SEASONS; i++) {
      season_string[i] = norm_season_string[i];
      change_day_of_month[i] = earth_change_day[i];
   }

  extra_day_names = NULL;
  exact_year_length = 365.25;
  // set up lunar stuff, if needed
  if (do_lunar_stuff) {
     number_moons = 1;
     moons = new LunarData[1];
     strcpy(moons[0].name, "Moon");
     moons[0].known_full_moon.set_length_year(exact_year_length);
     moons[0].known_full_moon.set_year_day(2002, 86.0);
     moons[0].set_period(29.525);
  }
  else 
     moons = NULL;
} // end

void Calendar::set_FR_calendar()
{
   int i, last_ndx = 0;

   leap_period = 4;
   leap_method = SIMPLE_EXTRALEAP;
   for (i=0; i<NUM_MONTHS; i++) {
      months[i].name = FR_month_string[i];
      months[i].num_days = 30;
      months[i].trailing_extra_days = FR_extra_days_per_month[i];
      months[i].has_leap_day = 0;
      months[i].has_season_change_day = 0;
      if (months[i].trailing_extra_days > 0) {
         months[i].extra_day_ndx[0] = last_ndx;
         months[i].extra_day_ndx[1] = last_ndx+months[i].trailing_extra_days-1;
         last_ndx = months[i].extra_day_ndx[1]+1;
      }
      else
         months[i].extra_day_ndx[0] =  months[i].extra_day_ndx[1] = 0;
   } // end-for

   // note leap month
   months[6].has_leap_day = 1; 
   // special case -- leap day is a named extra trailing day in FR.
   leapday_name = FR_extra_day_names[5];

   // note equinoxes and solistices
   months[2].has_season_change_day = 1;
   months[5].has_season_change_day = 1;
   months[8].has_season_change_day = 1;
   months[11].has_season_change_day = 1;

   extra_day_names = FR_extra_day_names;

   for (i=0; i<NUM_SEASONS; i++) {
      season_string[i] = norm_season_string[i];
      change_day_of_month[i] = FR_change_day[i];
   }

  exact_year_length = 365.25;

  // set up lunar stuff, if needed
  if (do_lunar_stuff) {
     number_moons = 1;
     moons = new LunarData[1];
     strcpy(moons[0].name, "Selune");
     moons[0].known_full_moon.set_length_year(exact_year_length);
     moons[0].known_full_moon.set_year_day(1372, 1.0);
     moons[0].set_period(30.4375);
  }
  else 
     moons = NULL;
} // end

void Calendar::set_Greyhawk_calendar()
{
   int i, last_ndx = 0;
   leap_method =  NOLEAP;

   // set regular monthly data
   for (i=0; i<NUM_MONTHS; i++) {
      months[i].name = Grey_month_string[i];
      months[i].num_days = 28;
      months[i].trailing_extra_days = Grey_extra_days_per_month[i];
      months[i].has_leap_day = 0;
      months[i].has_season_change_day = 0;
      // extra days are a "fest", with same name
      if (months[i].trailing_extra_days > 0) {
         months[i].extra_day_ndx[1] = months[i].extra_day_ndx[0] = last_ndx;
         last_ndx++;
      }
      else
         months[i].extra_day_ndx[0] =  months[i].extra_day_ndx[1] = 0;
   }

   // note equinoxes and solistices
   months[1].has_season_change_day = 1;
   months[5].has_season_change_day = 1;
   months[9].has_season_change_day = 1;
   months[11].has_season_change_day = 1;

   extra_day_names = Grey_extra_day_names;

   for (i=0; i<NUM_SEASONS; i++) {
      season_string[i] = norm_season_string[i];
      change_day_of_month[i] = Grey_change_day[i];
   }

  exact_year_length = 364.0;

  // set up lunar stuff, if needed
  if (do_lunar_stuff) {
     number_moons = 2;
     moons = new LunarData[2];
     strcpy(moons[0].name, "Luna");
     moons[0].known_full_moon.set_length_year(exact_year_length);
     moons[0].known_full_moon.set_year_day(0, 3.0);
     moons[0].set_period(28.0);
     strcpy(moons[1].name, "Celene");
     moons[1].known_full_moon.set_length_year(exact_year_length);
     moons[1].known_full_moon.set_year_day(0, 85.0);
     moons[1].set_period(91.0);
  }
  else 
     moons = NULL;
} // 

short  Calendar::count_days_year(short &num_extra)
{
   int i;
   short total = 0;
   
   num_extra = 0;
   for (i=0; i<NUM_MONTHS; i++) {
      months[i].jday[0] = total+1;
      total += months[i].num_days;
      if ((leap_method == SIMPLELEAP
           || leap_method == EARTHLEAP)
          && is_leap_year && months[i].has_leap_day) {
         total++;
      }
      months[i].jday[1] = total;
      total += months[i].trailing_extra_days;
      num_extra += months[i].trailing_extra_days;
      if (leap_method == SIMPLE_EXTRALEAP
          && is_leap_year && months[i].has_leap_day) {
         total++;
         num_extra++;
      }
   }
   return total;
} // end

short Calendar::check_leap_year()
{
   switch(leap_method) {
   case EARTHLEAP:
      // if NOT divisible by 4, not a leap year
      if ((year % 4) != 0)
         return 0;
      // if divisible by 400, is a leap year.
      if ((year % 400) == 0)
         return 1;
      // if divisible by 100, not a leap year.
      if ((year % 100) == 0)
         return 0;
      // if otherwise still divisible by 4, leap year
      return 1;
      break;
   case SIMPLELEAP:
   case SIMPLE_EXTRALEAP:
      if ( (year % leap_period) == 0)
         return 1;
      else
         return 0;
      break;
   case NOLEAP:
      return 0;
   }
   return 0;
} // end

void Calendar::set_season_divisions()
{
   int i;
   short curr_change = D_SPRING_EQ;

   for (i=0; i<NUM_MONTHS && curr_change <= D_WINTER_SOL; i++) {
      if (months[i].has_season_change_day) {
         season_divisions[curr_change] 
            = months[i].jday[0] + change_day_of_month[curr_change] - 1;
         curr_change++;
      }
   } // end-if

} // end

void  Calendar::set_calendar_config(CALCONFIG c)
{
   switch (c) {
   case USE_EARTH:
      set_earth_calendar();
      break;
   case USE_FREALMS:
      set_FR_calendar();
      break;
   case USE_GREYHAWK:
      set_Greyhawk_calendar();
      break;
   case USE_FILE:
      // TODO
      break;
   }  // end-switch
} // end
