// $Id: lunar.cpp,v 1.4 2002/05/06 23:17:07 cyhiggin Exp $
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

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

/*----------------------------------------------------------------*/
/* class headers                                                  */
/*----------------------------------------------------------------*/
#include "wthrmkr.h"
#include "macros.h"
#include "calendar.h"

/*----------------------------------------------------------------*/
/* global variables                                               */
/*----------------------------------------------------------------*/
static const char *normal_phase_names[] =
{ "full", "waning gibbous", "waning half", "waning crescent",
  "new", "waxing crescent", "waxing half", "waxing gibbous" };

/*----------------------------------------------------------------*/
/* Methods - JDate                                                */
/*----------------------------------------------------------------*/
JDate::JDate(void)
{
   year = 0;
   length_year = 1;
   fday = 1.0;
   fyear = 1.0;
} // end

JDate::JDate( short yr, double d, double l)
{
   length_year = l;
   set_year_day(yr, d);
} // end

void JDate::set_year_day( short yr, double d)
{
   // assume length_year already set
   year = yr;
   fday = d;
   fyear = (double) year + fday/length_year;
} // end

void JDate:: set_length_year( double l)
{
   length_year = l;
} // end

/*----------------------------------------------------------------*/
/* Methods - LunarData                                            */
/*----------------------------------------------------------------*/
LunarData::LunarData(void)
{
   phase_names = normal_phase_names;
} // end

void LunarData::set_period(double p)
{
   int l_quarter, l_total, iperiod, t, r_bits;
   short p_days[NUM_LUNARPHASES];
   
   period = p;
   l_quarter = (int) rint(period/10.0);
   iperiod = (int) rint(period);
   l_total = l_quarter * 4;
   r_bits = (int) ((period - (double) l_total)/4.0);

   l_total = r_bits * 4 + l_total;

   // figure adjustment to r_bits
   t = iperiod - l_total;
   r_bits = r_bits + (t/4);
   p_days[1] = p_days[3] = p_days[5] = p_days[7] = r_bits;
   p_days[0] = p_days[2] = p_days[4] = p_days[6] = l_quarter;

   // deal with the odd day here and there
   switch (abs(t)%4) {
   case 1:
      if (t>0)
         p_days[0]++;
      else
         p_days[7]--;
      break;
   case 3:
      if (t>0)
         p_days[0]++;
      else
         p_days[7]--;
      // yes, fall thru to next case.
   case 2:
      if (t>0) {
         p_days[1]++; 
         p_days[7]++;
      }
      else {
         p_days[1]--; 
         p_days[7]--;
      }
      break;
   }; // end-switch

   // set up phase_lookup_tbl
   phase_lookup_tbl[0] = max(1,p_days[0]-1);
   for (int i=0; i<NUM_LUNARPHASES; i++) {
      if (i>0)
         phase_lookup_tbl[i] = phase_lookup_tbl[i-1]+p_days[i];
   }
} // end

LUNARPHASE  LunarData::get_phase( short frac_p_days)
{
   int i;

   for (i=0; i<NUM_LUNARPHASES; i++) {
      if (phase_lookup_tbl[i] >= frac_p_days)
         return ((LUNARPHASE) i);
   } // end for
   return WAX_GIBB;
} // end

const char * LunarData::GetPhaseNameByJDate(JDate curr_jdate)
{
   double delta, factor, adjust, new_fyear, rem_days, fdummy;
   LUNARPHASE ph;

   // find number of days since a known full moon
   delta = curr_jdate.Delta(known_full_moon);

   // our known full moon falls AFTER the given date. Adjust back.
   if (delta < 0.0) {
      // how many multiples of period are we behind?
      factor = ceil(fabs(delta) / period);
      // period is what fraction of a year?
      adjust = period / known_full_moon.get_length_year();
      // subtract adjustment
      new_fyear = known_full_moon.get_fyear() - (factor * adjust);
      // re-calculate delta
      delta = curr_jdate.get_length_year() 
         * (curr_jdate.get_fyear() - new_fyear);
   } // end-if

   // how many periods since full?
   delta /= period;
   // get fraction of a period...
   rem_days = modf(delta, &fdummy);
   // convert it back to days
   rem_days *= period;

   ph = get_phase((short) rem_days);
   return (get_phase_name(ph));

} // end
