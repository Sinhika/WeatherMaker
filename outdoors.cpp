// $Id: outdoors.cpp,v 1.10 2002/05/18 17:22:29 cyhiggin Exp $
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
#include <strings.h>

#include "macros.h"
#include <getopt.h>
#include <errno.h>


/*----------------------------------------------------------------*/
/* class headers                                                  */
/*----------------------------------------------------------------*/
#include "outdoors.h"
#include "calendar.h"

/*----------------------------------------------------------------*/
/* global variables                                               */
/*----------------------------------------------------------------*/
#include "tables.h"

/*----------------------------------------------------------------*/
/* Methods - Outdoors                                             */
/*----------------------------------------------------------------*/

Outdoors::Outdoors( int argc, char *argv[] )
{
    init_defaults();
    Parse( argc, argv);
} /* end */

Outdoors::Outdoors( void )
{
    init_defaults();
    pCal = new Calendar();
} /* end */

Outdoors::~Outdoors(void)
{
  if (status_line)
    delete [] status_line;
} // end

void Outdoors::init_defaults( void )
{
    clime = TEMPERATE;
    terrain = PLAINS;
    month[0] = 0;
    month[1] = 1;
    day[0] = 1;
    day[1] = 31;
    year[0] = 0;
    year[1] = 0;
    curr_jday = start_jday = 1;
    num_days = 31;
    elevation = default_elevation();
    pCal = NULL;
    season = WINTER;
    good_parse = 0;
    version_only = 0;
    DUMP_FLAG = 0;
    external_init_flag = 0;
    status_line = NULL;
    calendar_cfg = USE_EARTH;
    show_moon = show_metric = 0;
} /* end */

int Outdoors::default_elevation(void)
{
   int elev;

   switch (terrain) {
   case DESERT:
   case SEACOAST:
      elev = 0;
      break;
   case PLAINS:
   case FOREST:
      elev = 500;
      break;
   case HILLS:
      elev = 2000;
      break;
   case MOUNTAINS:
      elev = 4000;
      break;
   default:
      elev = 0;
   } /* endswitch */
   return elev;
} /* end */

short Outdoors::Parse( int argc, char *argv[])
{
   static struct option long_options[] =
   {
      {"clime",1,0,'c'},
      {"terrain",1,0,'t'},
      {"day",1,0,'d'},
      {"month",1,0,'m'},
      {"elevation",1,0,'e'},
      {"number-days",1,0,'n'},
      {"prev-weather-file", 1, 0, 'f'},
      {"verbose",0,0,'v'},
      {"forgotten-realms",0,0,'F'},
      {"metric",0,0,'C'},
      {"greyhawk",0,0,'G'},
      {"show-moon",0,0,'L'},
      {"version",0,0,'V'},
      {"help",0,0,'h'},
      {0,0,0,0}
   };

   int c, option_index = 0;
   short parsed_elev = 0;

    good_parse = 1;
    while ( (c = getopt_long(argc,argv,
                             "c:t:d:n:e:f:m:vVFGhCL",
                             long_options, &option_index)) 
            != -1)
    {
       switch (c) {
       case 'c':
          if (strcasecmp("A",optarg)==0)
             clime = ARCTIC;
          else if (strcasecmp("SA",optarg)==0)
             clime = SUBARCTIC;
          else if (strcasecmp("T",optarg)==0)
             clime = TEMPERATE;
          else if (strcasecmp("ST",optarg)==0)
             clime = SUBTROPIC;
          else if (strcasecmp("TR",optarg)==0)
             clime = TROPIC;
          else
             good_parse = 0;
          break;
       case 't':
          if (strcasecmp("D",optarg)==0)
             terrain = DESERT;
          else if (strcasecmp("F",optarg)==0)
             terrain = FOREST;
          else if (strcasecmp("H",optarg)==0)
             terrain = HILLS;
          else if (strcasecmp("M",optarg)==0)
             terrain = MOUNTAINS;
          else if (strcasecmp("P",optarg)==0)
             terrain = PLAINS;
          else if (strcasecmp("S",optarg)==0)
             terrain = SEACOAST;
          else
             good_parse = 0;
          break;
       case 'd':
          if ((day[0] = atoi(optarg)) == 0)
             good_parse = 0;
          // don't decrement day[0] by 1; it *should* be 1-based.
          break;
       case 'm':
          if ((month[0] = atoi(optarg)) == 0)
             good_parse = 0;
          else
             month[0]--;
          break;
       case 'n':
          if ((num_days = atoi(optarg)) == 0 || num_days < 0)
             good_parse = 0;
          else {
             // knock off one day; all internal algorithms calculate
             // Nth day *AFTER* start date
             // num_days--;
             ;
          }
          break;
       case 'e':
          if ((elevation = atoi(optarg)) == 0)
             good_parse = 0;
          else
             parsed_elev = 1;
          break;
       case 'f':
         if (get_status_line(optarg) == 0)
           external_init_flag = 1;
         else {
           external_init_flag = 0;
           good_parse = 0;
         }
         break;
       case 'v':
          DUMP_FLAG = 1;
          break;
       case 'V':           // display version & license
         version_only = 1;
         good_parse = 0;
         break;
       case 'h':           // display help
          good_parse = 0;
          break;
       case 'F':
          calendar_cfg = USE_FREALMS;
          break;
       case 'G':
          calendar_cfg = USE_GREYHAWK;
          break;
       case 'C':
          show_metric = 1;
          break;
       case 'L':
          show_moon = 1;
          break;
       case '?':
          good_parse = 0;
          break;
       case 0:      // long option
          break;
       } /* endswitch */
    } /* endwhile */

    if (good_parse == 0 || (external_init_flag == 0 && optind == argc))
       good_parse = 0;
    else if (external_init_flag == 0 && (year[0] = atoi(argv[optind]))==0) {
       good_parse = 0;
    } else {
       if (external_init_flag) {
          // get calendar_cfg, clime, terrain, start date, 
          // and elevation from status_line;
          if (!parse_status_line()) {
             good_parse = 0;
             return good_parse;
          }
          parsed_elev = 1;
       }
       year[1] = year[0];
       pCal = new Calendar(year[0], calendar_cfg, show_moon);

       start_jday = pCal->GetJDayByMonthDay(month[0],day[0]);
       // input day,month was bad
       if (start_jday < 1) {
          printf("Bad month or day (%hd, %hd) for year %hd\n", 
                 month[0]+1, day[0], year[0]);
          good_parse = 0;
          return good_parse;
       }

       curr_jday = start_jday;
       curr_jdate.set_length_year(pCal->GetDayLength());
       curr_jdate.set_year_day(year[0], (double) curr_jday);
       year[1] += pCal->FindMonthDayAfterN( start_jday, num_days-1, 
                                           month[1], day[1]);

       if (!parsed_elev) 
          elevation = default_elevation();
       season = pCal->GetSeasonByDay(start_jday);
       good_parse = 1;
    } /* endif */
    return good_parse;
} /* end */

void Outdoors::Dump( short do_dateline )
{

    printf("\n--------------------------- Outdoors -------------------------------------\n");
    printf("\tClimate: %s\t\tTerrain: %s\n", 
           climate_string[clime], terrain_string[terrain]);

    if (do_dateline) {
       char buf[40], buf2[40];
       short jday;

       jday = pCal->GetJDayByMonthDay(month[0], day[0]);
       pCal->GetDayString(jday, buf, 40);
       pCal->ChangeYear(year[1]);
       jday = pCal->GetJDayByMonthDay(month[1], day[1]);
       pCal->GetDayString(jday, buf2, 40);
       pCal->ChangeYear(year[0]);

       printf("\tYear %hd, %s to Year %hd, %s\n", 
              year[0], buf, year[1], buf2);
    }
    printf("\tElevation: %ld\t\tSeason: %s\n", 
           elevation, pCal->GetSeasonString(season));
    printf("--------------------------------------------------------------------------\n");
            
} /* end */

void Outdoors::PrintDate(void)
{
   char buf[40];

   pCal->GetDayString(curr_jday, buf, 40);
   printf("Year %1hd, %s :", year[0], buf);
} /* end */


int Outdoors::IterateDate(void)
{
   short mo, yr, d;

      //  redone using pCal
   yr = pCal->FindMonthDayAfterN(curr_jday, 1, mo, d);
   if (yr) {
      year[0]++;
      pCal->ChangeYear(year[0]);
   }
   month[0] = mo;
   day[0] = d;
   curr_jday = pCal->GetJDayByMonthDay(mo,d);
   curr_jdate.set_year_day(year[0], (double) curr_jday);
   season = pCal->GetSeasonByDay(curr_jday);
   num_days--;

   if (LastDay()) 
      return 0;
 
   return 1;
} /* end */

int  Outdoors::get_status_line( char *fname )
{
   FILE *fp;
   short found_it = 0;
   char instring[80], *p;

  // open file
   if ( (fp = fopen(fname, "r")) == NULL) {
      // an error message might be nice here...
      printf("%s: %s\n", strerror(errno), fname);
      return 1;
   }

   // loop thru file until we find STATUS line.
   do {
      p = fgets(instring, 78, fp);
      if (p) {
         found_it = !strncmp(instring, "STATUS ", 7);
      }
   } while (!found_it && ! feof(fp) );

   // if found, copy to status_line
   if (found_it) {
      status_line = new char[80];
      strncpy( status_line, instring, 78);
   }

   // close file
   fclose(fp);

   // didn't find it in file
   if (!found_it) {
      printf("STATUS line not found in %s\n", fname);
      return 2;
   }

  // everything worked.
  return 0;
} /* end */

int  Outdoors::parse_status_line( void )
{
  int igot;
  short cval, tval, cfgval;

  igot = sscanf(status_line, "STATUS %hd %hd %hd %hd %hd %hd %ld",
                &cval, &tval, &cfgval, &year[0], &month[0], &day[0], 
                &elevation);
  clime = (CLIMATE) cval;
  terrain = (TERRAIN) tval;
  calendar_cfg = (CALCONFIG) cfgval;

  // error message if failed.
  if (igot != 7)
     printf("Bad status_line: %s\n", status_line);

  return (igot == 7);
} /* end */
