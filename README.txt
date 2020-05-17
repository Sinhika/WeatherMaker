-- $Id$ --

WeatherMaker 1.3.5
Copyright (C) 2020 Cynthia Higginbotham

WeatherMaker is a command-line utility to generate weather reports for
role-playing games set in Earth-like worlds. It is not meant to be
either an accurate or a precise simulation of climate, but it should
give plausible results for the season, terrain and climate
requested. It is loosely based on tables published in the 1st edition
AD&D Wilderness Survival Guide; however, many features of my own have
been added. Many remain to be added; see TODO.txt for things I plan to
add as time allows.

-----------------------------
INSTALLING SOURCE & COMPILING
-----------------------------
Everybody:

0. Clone or download the latest source from GitHub 
(https://github.com/Sinhika/WeatherMaker) 

Linux/Unix:

1. You will need GNU g++ or the equivalent installed.

2. Change to the working directory and clean up any old object files
   or executables: 
   make clean

4. Build wthrmkr:
   make 

5. Copy the resulting wthrmkr executable to wherever you want it, like
   /usr/local/bin or /usr/games/bin.

Currently, WeatherMaker should compile on any system with g++ and a
working implementation of libc/glibc, including all the header files
(it requires getopt.h). I have tested it on Linux 4.15 and earlier with glibc.
It is single-threaded, so it should compile fine on older systems.

Windows/Cygwin:

If you're using the Cygwin environment, follow the Linux/Unix steps
above. It compiles fine. Run 'strip wthrmkr.exe' manually, if desired. 
Copy the  resulting wthrmkr.exe executable to someplace convenient. 
I tested it using gcc 2.95.2-6 under Cygwin 19.1001.8.

Windows/Cygwin-without-cygwin-DLL:

I am no longer supporting non-standard C++ compilers.

----------------------
INSTALLING EXECUTABLES
----------------------

Windows:

1. Download archive of latest Windows executables from my website or
other archive. Currently there isn't one, yet.

2. Unzip the archive; it should contain three files: wthrmkr.exe,
CHANGES.txt, and README.txt.

3. Read CHANGES.txt to see what has changed lately. Usage instructions
are in this file, README.txt.

4. Copy wthrmkr.exe somewhere convenient (in your path). Use.


-----
USAGE
-----

wthrmkr [options] <year>
wthrmkr -f <file> [-n <#days>] [-v] [-C] [-L] 
wthrmkr -V
wthrmkr -h

-c <climate>
--clime <climate>
-t <terrain>
--terrain <terrain>
        climate = A [arctic]         terrain = D [desert]
                  SA [subarctic]               F [forest]
                  T [temperate]                H [hills]
                  ST [subtropical]             M [mountains]
                  TR [tropical]                P [plains]
                                               S [seacoast]

-d <day>
--day <day>
        day = 1-31 [day of the month]

-m <month>
--month <month>
        month = 1-12 [month of year]

-n <#days>
--number-days <#days>
        #days = 1-9999 [# of days to report weather for]

-e <elevation>
--elevation <elevation>
        elevation = 0-30000 [elevation in feet]

<year>
        year = year according to campaign calendar

-f <file>
--prev-weather-file <file>
        file = weather file with STATUS line (overrides cmd-line args)

-C
--metric
        Display temperatures in Celsius and depths/speeds in metric

-L
--show-moon
        Show current phase of the moon

-v
--verbose
        Use verbose format

-F
--forgotten-realms
        Use Forgotten Realms calendar

-G
--greyhawk
        Use Greyhawk calendar

-V
--version
        Show version, license, and exit

-h
--help
        Show this help and exit

The basic idea:

You specify a starting year, and WeatherMaker generates 30 days of
weather starting January 1 for a temperate plains region.  What, you
wanted something else? That's easy.  Using command-line options, you
specify a climate (-c), a terrain-type (-t), which month (-m) you want
the reports to start, which day of the month (-d) you want the reports
to start, how many days you want the report to run for (-n), whether
you want temperatures, depths, rainfall, and windspeed displayed in
English or metric units (-C), whether you want to show the phases of
the moon(s) (-L), and whether you want the terse, one-line reports
(default) or something a little bit prettier and easier to read (-v).

Example of default output:

Year 1359, January 1 : high: 25 F low: 10 F wind: 10 mph S 
Moon: waxing crescent 
Year 1359, January 2 : high: 30 F low: 15 F No wind. 
Moon: waxing crescent 
Year 1359, January 3 : high: 18 F low: 10 F wind: 10 mph N 
Moon: waxing crescent 
STATUS 2 4 0 1359 0 4 500 5 20 10 0.000000 0 0 0 S

Example of verbose output:
--------------------------- Outdoors -------------------------------------
        Climate: Temperate              Terrain: Plains
        Year 1359, Hammer 1 to Year 1359, Hammer 3
        Elevation: 500          Season: Winter
--------------------------------------------------------------------------
Year 1359, Hammer 1 :
high: 25 F low: 10 F with a 15 mph wind out of the W. 
Selune is a full moon. 
--------------------------------------------------------------------------
Year 1359, Hammer 2 :
high: 30 F low: 15 F with a  5 mph wind out of the W. 
Selune is a full moon. 
0.5 inches of sleet fell.
--------------------------------------------------------------------------
Year 1359, Hammer 3 :
high: 25 F low: 10 F with a 15 mph wind out of the W. 
Selune is a full moon. 
--------------------------------------------------------------------------
STATUS 2 4 1 1359 0 4 500 6 20 15 0.000000 0 0 0 W


By default, WeatherMaker assumes that all desert and seacoast regions
are at sea level, that all plains and forests are at 500 feet elevation,
that all hills are 2000 feet above sea level, and that all mountains
are 4000 feet up.  If this doesn't quite fit your world, or at least the
region of the world you want weather reports on, there is an option to
specify elevation (-e). (Elevation affects temperature, if you were
wondering).

The "use old file" option (-f): imagine that you use this program, and
generate the weather your group is going to contend with for their
4-week wilderness journey. The output is saved in a file which you
refer to from time to time (that 2 feet of snow on the ground makes
the final attack on the enemy stronghold rather more interesting than
your players expected.)  Okay, the big fight is over, the PCs won and
looted the place and decide to head home, so you decide to generate
some more weather for the 4-week return trip.  Hmmm... what happened
to that cold snap and the 2 feet of snow on the ground? It just
vanished like it never was!

If you want the weather to pick up where you left off with it, just
provide your old weather file as input (-f). All other options except
for number of days (-n), English/metric (-C), show lunar phases (-L),
and verbose/not verbose (-v) will be ignored and read from the old
file instead, as will the current weather conditions for the day
before. Actually, all you need of the old weather file is the entire
line starting with the word "STATUS", and it will continue to generate
new weather based on the pre-existing conditions instead of starting
with seasonal defaults. If you're curious as to what all those numbers
are, I noted the output format for STATUS in the CHANGES.txt file when
I added this feature.

If you want to see the Forgotten Realms or Greyhawk month names in
your report instead of the usual English ones, there are options (-F
and -G) for that, too.

Please report bugs to cyhiggin@republicofnewhome.org

Finally, if you like WeatherMaker, but wish it did things a bit
differently, you have two options: (1) open an issue on GitHub
explaining what changes you would like made and why they are a good idea, 
(2) make the changes yourself, (3) or email me as per (1). 
Please read the GNU General Public License included in the file COPYING to find
out your responsibilities if you should decide to distribute your changed
version, however.

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
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA,
or visit their website at http:\www.fsf.org

