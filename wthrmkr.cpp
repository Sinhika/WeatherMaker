// $Id: wthrmkr.cpp,v 1.12 2002/05/18 17:22:29 cyhiggin Exp $
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
#include <time.h>
#include "macros.h"
#include "wthrmkr.h"

/*----------------------------------------------------------------*/
/* class headers                                                  */
/*----------------------------------------------------------------*/
#include "calendar.h"
#include "outdoors.h"
#include "intermediatereport.h"
#include "dailyreport.h"

/*----------------------------------------------------------------*/
/* global variables                                               */
/*----------------------------------------------------------------*/

#include "tables.h"


const char version_string[] = "WeatherMaker 1.3.4";
const char copyright_string[] = "Copyright (C) 2002 Cynthia Higginbotham";
const char license_string[] = 
"This program is free software; you can redistribute it and/or modify\n"
"it under the terms of the GNU General Public License as published by\n"
"the Free Software Foundation; either version 2 of the License, or\n"
"(at your option) any later version.\n\n"
"This program is distributed in the hope that it will be useful,\n"
"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
"GNU General Public License for more details.\n\n"
"You should have received a copy of the GNU General Public License\n"
"along with this program; if not, write to the Free Software\n"
"Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA,\n"
"or visit their website at http:\\www.fsf.org\n";

/*-------------------------------------------------------------------------*/
/* local functions                                                         */
/*-------------------------------------------------------------------------*/
void UsageMsg(short v_only);

/*-------------------------------------------------------------------------*/
/* Main Program                                                            */
/*-------------------------------------------------------------------------*/

int main (int argc, char *argv[] )
{
    Outdoors Here(argc, argv);
    DailyReport Weather;
    short mo;
    SEASON s;

    if (!Here.IsParsed()) {
        UsageMsg(Here.version_only);
    } else {
       printf("%s %s\n\n", version_string, copyright_string);
        if (Here.IsDump())
            Here.Dump();
    /* initialize random number generator */
        srand( (unsigned int) time(NULL) );

	// point daily report object at lunar data, if needed.
	if (Here.show_moon) {
	   Weather.number_moons = Here.pCal->number_moons;
	   Weather.moons = Here.pCal->moons;
	   Weather.p_jdate = &Here.curr_jdate;
	}
        IntermediateReport WorkArea(&Here, &Weather);
        mo = Here.month[0];
	s = Here.season;
        do {
	   if (Here.month[0] != mo) {
                WorkArea.ChangeMonth();
	   }
	   if (Here.season != s) {
	       WorkArea.ChangeSeason();
		if (Here.IsDump())
		   Here.Dump(0);
	   }
	       
            mo = Here.month[0];
	    s = Here.season;
            WorkArea.GenerateWeather();
            Here.PrintDate();
            Weather.Print( !Here.IsDump());
        } while ( Here.IterateDate() ); /* enddo */
	WorkArea.PrintStatus();
   } /* endif */
	return 0;
} /* end main */


/*-------------------------------------------------------------------------*/
/* UsageMsg - print usage message.                                      */
/*---------------------------------------------------------------------*/
void UsageMsg(short v_only)
{
   puts(version_string);
   if (v_only) {
      puts(copyright_string);
      puts(license_string);
      return;
   }
   puts("Usage:");
   puts( "wthrmkr [options] <year>");
   puts( "wthrmkr -f <file> [-n <#days>] [-v] [-C] [-L] ");
   puts( "wthrmkr -V");
   puts( "wthrmkr -h\n");
   puts("-c <climate>");
   puts("--clime <climate>");
   puts("-t <terrain>");
   puts("--terrain <terrain>");
   puts( "\tclimate = A [arctic]         terrain = D [desert]");
   puts( "\t          SA [subarctic]               F [forest]");
   puts( "\t          T [temperate]                H [hills]");
   puts( "\t          ST [subtropical]             M [mountains]");
   puts( "\t          TR [tropical]                P [plains]");
   puts( "\t                                       S [seacoast]\n");
   puts( "-d <day>");
   puts( "--day <day>");
   puts( "\tday = 1-31 [day of the month]\n");
   puts( "-m <month>");
   puts( "--month <month>");
   puts("\tmonth = 1-12 [month of year]\n");
   puts("-n <#days>");
   puts("--number-days <#days>");
   puts("\t#days = 1-9999 [# of days to report weather for]\n");
   puts("-e <elevation>");
   puts("--elevation <elevation>");
   puts("\televation = 0-30000 [elevation in feet]\n");
   puts("<year>");
   puts("\tyear = year according to campaign calendar\n");
   puts("-f <file>");
   puts("--prev-weather-file <file>");
   puts("\tfile = weather file with STATUS line (overrides cmd-line args)\n");
   puts("-C");
   puts("--metric");
   puts("\tDisplay temperatures in Celsius and depths/speeds in metric\n");
   puts("-L");
   puts("--show-moon");
   puts("\tShow current phase of the moon\n");
   puts("-v");
   puts("--verbose");
   puts("\tUse verbose format\n");
   puts("-F");
   puts("--forgotten-realms");
   puts("\tUse Forgotten Realms calendar\n");
   puts("-G");
   puts("--greyhawk");
   puts("\tUse Greyhawk calendar\n");
   puts("-V");
   puts("--version");
   puts("\tShow version, license, and exit\n");
   puts("-h");
   puts("--help");
   puts("\tShow this help and exit\n");
   puts("\tReport bugs to cyhiggin@republicofnewhome.org");
} /* end */

/*--------------------------------------------------------------------------*/
/* diceroll - generate a dice roll, and save individual rolls in an array.  */
/*--------------------------------------------------------------------------*/
short diceroll( short num, short type, short plus, short *dice_array)
{
    register int result,i;
    short *pdice;

    for (i=0,result=0,pdice=dice_array; i<num; i++, pdice++) {
        // *pdice = (short) (((double) type * rand())/RAND_MAX) + 1;
	*pdice = (short) ((long) rand() % (long) type) +1;
        result += (*pdice);
    }
    return (result + plus);
} /* end */

short simple_dieroll( short num, short type, short plus)
{
    register int result,i;

    for (i=0,result=0; i<num; i++) {
	result += (short) ((long) rand() % (long) type)+1;
    }
    return (result + plus);
} /* end */

