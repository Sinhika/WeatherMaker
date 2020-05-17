-- $Id: README.txt,v 1.6 2002/05/18 17:22:28 cyhiggin Exp $ --

WeatherMaker 1.3.4
Copyright (C) 2002 Cynthia Higginbotham

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

0. Download the latest source from my website. (See 
   http://www.republicofnewhome.org/lair/games/games3.html for current 
   version and changelog).

Linux/Unix:

1. Un-tar in a working directory:
   tar xvzf wthrmkr-1.3.4.tar.gz

2. Change to your working directory and clean up any old object files
   or executables: 
   make clean

3. Generate dependencies:
   make dep

4. Build wthrmkr:
   make wthrmkr

5. Copy the resulting wthrmkr executable to wherever you want it, like
   /usr/local/bin or /usr/games/bin.

Currently, WeatherMaker should compile on any system with gcc and a
working implementation of libc/glibc, including all the header files
(it requires getopt.h). I have only tested it on Linux 2.4 with glibc
2.2. It is single-threaded, so it should compile fine on older systems.

Optimized and stripped, wthrmkr weighs in at about 30 KB. Not bad in
these days of bloatware, eh?

Windows/Cygwin:

If you're using the Cygwin environment, follow the Linux/Unix steps
above. It compiles fine. Run 'strip wthrmkr.exe' manually, if desired. 
Copy the  resulting wthrmkr.exe executable to someplace convenient. 
I tested it using gcc 2.95.2-6 under Cygwin 19.1001.8.

Windows/Cygwin-without-cygwin-DLL:

If you want to generate a Windows executable using the Cygwin
environment that does NOT use the Cygwin DLLs, do the following:

1. Un-tar in a working directory:
   tar xvzf wthrmkr-1.3.4.tar.gz

2. Unzip wthrmkr_vs6.zip (described below) in your working directory.

3. Change to your working directory and clean up any old object files
   or executables: 
   make -f Makefile.nocygwin clean

4. Generate dependencies:
   make -f Makefile.nocygwin dep

5. Build wthrmkr.exe:
   make -f Makefile.nocygwin wthrmkr.exe

6. Copy the resulting wthrmkr.exe executable someplace convenient.

Normally, you wouldn't bother doing this; if you are running the Cygwin
environment, you have the Cygwin DLLs. I do this so I can distribute a
legally-compiled 64K Windows executable without distributing a 600K 
cygwin1.dll along with it.

Windows/Visual Studio 6:

1. Un-tar in your working directory. WinZip will unpack tar files.

2. There should be a ZIP file, wthrmkr_vs6.zip. Unzip that in your
   working directory, too. It contains the Visual Studio makefile
   (weather.mak), dependencies file (weather.dep), project file
   (weather.dsp), workspace file (weather.dsw), source files for my
   homebrew versions of a few STANDARD C functions that Microsoft was
   too lame to include (stupid_microsoft.cpp, stupid_microsoft.h), a
   makefile for using Cygwin gcc to generate non-Cygwin exes 
   (Makefile.nocygwin, not used by Visual Studio) and the GNU glibc 
   source files for the getopt() and getopt_long() functions (getopt.h, 
   getopt.c, getoptl.c), slightly hacked to compile under VS6.

3. From your working directory, build wthrmkr.exe:
   nmake /f weather.mak 

   A release version will be created in <your directory>/Release/.
   If you want the debug version (not optimized, has debugging symbols):
   nmake /f weather.mak CFG="weather - Win32 Debug"

   That will create wthrmkr.exe in <your directory>/Debug/.

   The workspace and project files are only needed if you want to hack
   around with the program in Visual Studio.  If you're doing that,
   you probably don't need me to tell you what to do with those
   files. :-)

4. Copy the resulting wthrmkr.exe executable somewhere convenient. I
   tested it using Visual Studio 6.0, SP6 on Windows NT 4.0, SP6a.

OS/2:

You should be able to hack the makefile to compile under OS/2, because
that's where this program started out in the first place. I don't have
it available to test with anymore, so you're on your own.

----------------------
INSTALLING EXECUTABLES
----------------------

Windows:

1. Download archive of latest Windows executables from my website or
other archive. Currently that is wthrmkr134-win32.zip.

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
	Climate: Temperate		Terrain: Plains
	Year 1359, Hammer 1 to Year 1359, Hammer 3
	Elevation: 500		Season: Winter
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
differently, you have two options: (1) send me e-mail explaining what
changes you would like made and why they are a good idea, or (2) make
the changes yourself. Please read the GNU General Public License
included in the file COPYING to find out your responsibilities if you
should decide to distribute your changed version, however.

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

