/**********************************************************************************/
/* WTHRMKR.H                                                                      */
// $Id: wthrmkr.h,v 1.9 2002/03/29 12:20:19 cyhiggin Exp $
/**********************************************************************************/
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
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
    02111-1307 USA or visit their website at http://www.fsf.org.  */
#ifndef _WTHRMKR_H
#define _WTHRMKR_H

#define NUM_CLIMATES    5
#define NUM_TERRAINS    6
#define NUM_MONTHS      12
#define NUM_SEASONS     4
#define NUM_RAINTYPES   4
#define NUM_SPECIALS    9
#define NUM_EXTREMES    5
#define NUM_LUNARPHASES 8

#define XTL_SIZE 7

#define T_HIGH          0
#define T_LOW           1
#define T_MIN           0
#define T_NORM          1
#define T_MAX           2

#define D_SPRING_EQ     0
#define D_SUMMER_SOL    1
#define D_FALL_EQ       2
#define D_WINTER_SOL    3

enum _CLIMATE    { ARCTIC=0, SUBARCTIC, TEMPERATE, SUBTROPIC, TROPIC };
enum _TERRAIN    { DESERT=0, FOREST, HILLS, MOUNTAINS, PLAINS, SEACOAST };
enum _SEASON     { WINTER=0, SPRING, SUMMER, FALL, BADSEASON };
enum _RAIN       { NO_RAIN=0, TRACE, LIGHT, MODERATE, HEAVY, DOWNPOUR };
enum _RAINTYPE   { RAINFALL=0, FREEZE_RAIN, SLEET, SNOW };
enum _SPECIALS   { AA=0, CC, DD, GG, MM, SS, TT, XX, ZZ };
enum _EXTREMES {HSTORM=0, ISTORM, TSTORM, SSTORM, NOSTORM};

enum _LEAPMTHD {EARTHLEAP=0, SIMPLELEAP, SIMPLE_EXTRALEAP, NOLEAP};
enum _CALCONFIG {USE_EARTH=0, USE_FREALMS, USE_GREYHAWK, USE_FILE};
enum _LUNARPHASE {FULLMOON=0, WANE_GIBB, WANE_HALF, WANE_CRESCENT,
		  NEWMOON, WAX_CRESCENT, WAX_HALF, WAX_GIBB};

typedef enum _CLIMATE   CLIMATE;
typedef enum _TERRAIN   TERRAIN;
typedef enum _SEASON    SEASON;
typedef enum _RAIN      RAIN;
typedef enum _RAINTYPE  RAINTYPE;
typedef enum _SPECIALS  SPECIALS;
typedef enum _EXTREMES  EXTREMES;
typedef enum _LEAPMTHD  LEAPMETHOD;
typedef enum _CALCONFIG CALCONFIG;
typedef enum _LUNARPHASE LUNARPHASE;

#if 0
#define IS_WINTER(x)    ((x) >= 0 && (x) < 3)
#define IS_SPRING(x)    ((x) >= 3 && (x) < 6)
#define IS_SUMMER(x)    ((x) >= 6 && (x) < 9)
#define IS_FALL(x)      ((x) >= 9 && (x) < 12)
#endif

// #define CALC_SEASON(x)  ((SEASON)((x)/3))
#define convert2celsius(x) (((double)(x)-32.0)/1.8)
#define convert2kph(x)  ((double)(x)*1.61)
#define convert2cm(x)   ((double)(x)*2.54)

/*---- wind direction table codes */
#define FROM_TROPIC      3
#define FROM_ARCTIC     2
#define PREVAILING      1
#define NO_CHANGE       0

extern short diceroll( short num,short type,short plus, short *dice_array);
extern short simple_dieroll( short num, short type, short plus);

#endif
