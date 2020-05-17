// $Id: tables.cpp,v 1.6 2002/03/13 01:45:25 cyhiggin Exp $
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
/* all this table stuff... */

#include "wthrmkr.h"
#include "intermediatereport.h"

/*----------------------------------------------------------------*/
/* global variables                                               */
/*----------------------------------------------------------------*/

const char* climate_string[NUM_CLIMATES]
            = { "Arctic", "Subarctic", "Temperate", "Subtropical", "Tropical" };
const char* terrain_string[NUM_TERRAINS]
    = { "Desert", "Forest", "Hills", "Mountains", "Plains", "Seacoast" };
const char* precipitation_string[NUM_RAINTYPES] = { "rain", "freezing rain", "sleet", "snow" };
const char* special_string[NUM_SPECIALS] = {"Cold Wave", "Hurricane", "Drought", "Gale",
                                      "Fog", "", "Tornado",
                                      "Extreme Precipitation", "Heat Wave" };
const char *extreme_precip_string[NUM_EXTREMES] = { "Hailstorm", "Ice Storm", "Thunderstorm",
                                              "Heavy Snowfall", "Extreme Precipitation"};


extern const short Code_Temperature [26][2] = {
   {-20, -40}, {-15,-30}, {-5,-20}, {0,-10}, {10,0}, {18,10},
   {25,10}, {30,15}, {35,15}, {40,20}, {50,30}, {60,40},
   {65,45}, {70,50}, {70,55}, {75,55}, {80,60}, {80,65},
   {85,65}, {85,70}, {90,70}, {90,75}, {95,75}, {100,80},
   {105,80}, {115,85}
}; /* end Code_Temperature */

extern const short Step_Temperature [11] = { -3, -2, -2, -1, -1, 0, 1, 1, 2, 2, 3 };

extern const short Step_Special[2][11] = { {0,0,0,1,1,1,-1,-1,-1,-1,-1 }, {-1,-1,-1,-1,-1,2,2,2,3,3,3 } };

WIND Step_Wind[11] = { 
    {45,PREVAILING}, {15,FROM_ARCTIC}, {10,FROM_ARCTIC},
    {15,PREVAILING}, {10,PREVAILING}, {0,NO_CHANGE},
    {-10, PREVAILING}, {-15, PREVAILING}, {-10,FROM_TROPIC},
    {-15,FROM_TROPIC}, {5,NO_CHANGE} 
};

const char * Table_Prevailing[NUM_CLIMATES+1] = {"E", "N NW", "N S W", "E NE", "E NE SE", "W SW" };
/* last is SUBARCTIC Summer prevailing wind */

extern const short Table_MaxWind[NUM_CLIMATES] = { 20, 30, 45, 45, 20 };

extern const short Table_Temperature[NUM_CLIMATES][NUM_TERRAINS][NUM_MONTHS][3] =
{
/* ARCTIC */ {
/* desert */   { {0,1,6}, {0,1,7}, {0,2,7}, {0,4,7}, {2,6,10}, {5,9,11},
                 {9,10,13}, {9,10,12}, {5,7,10}, {3,6,9}, {0,3,7}, {0,1,7} },
/* forest */   { {0,1,4}, {0,1,3}, {0,2,5}, {0,3,6}, {1,4,8}, {2,5,8},
                 {3,6,11}, {3,6,10}, {2,5,9}, {3,4,7}, {2,4,7}, {0,2,4} },
/* hills */    { {0,1,4}, {0,1,3}, {0,2,5}, {0,3,6}, {1,4,8}, {2,5,8},
                 {3,6,11}, {3,6,10}, {2,5,9}, {3,4,7}, {2,4,7}, {0,2,4} },
/* mountains */ { {0,1,4}, {0,1,3}, {0,2,5}, {0,3,6}, {1,4,8}, {2,5,8},
                 {3,6,11}, {3,6,10}, {2,5,9}, {3,4,7}, {2,4,7}, {0,2,4} },
/* plains */    { {0,1,6}, {0,1,7}, {0,2,7}, {0,4,7}, {2,6,10}, {5,9,11},
                 {9,10,13}, {9,10,12}, {5,7,10}, {3,6,9}, {0,3,7}, {0,1,7} },
/* seacoast */ { {0,2,7}, {0,1,7}, {0,2,7}, {0,4,9}, {2,6,10}, {5,9,13},
                 {9,10,15}, {9,10,13}, {4,8,11}, {2,6,9}, {0,4,7}, {0,2,8} } },
/* SUBARCTIC */ {
/* desert */   { {0,3,8}, {0,3,9}, {1,4,9}, {2,6,9}, {5,10,12}, {9,10,13},
                 {10,12,16}, {9,12,16}, {6,10,16}, {5,7,12}, {2,4,11}, {0,2,8} },
/* forest */   { {0,4,9}, {0,4,8}, {1,6,10}, {3,9,11}, {7,10,13}, {9,11,15},
                 {10,12,17}, {9,12,17}, {8,10,15}, {4,9,11}, {1,7,11}, {0,5,9} },
/* hills */    { {0,3,8}, {0,3,8}, {1,5,9}, {2,6,9}, {5,10,13}, {9,11,13},
                 {9,12,15}, {8,12,15}, {6,10,12}, {3,9,11}, {1,6,10}, {0,3,10} },
/* mountains */ { {0,2,8}, {0,3,8}, {0,5,9}, {1,7,11}, {4,9,12}, {8,10,12},
                  {9,11,13}, {8,12,14}, {6,10,12}, {3,9,11}, {1,5,10}, {0,3,9} },
/* plains */    { {0,3,8}, {0,3,9}, {1,4,9}, {2,6,9}, {5,10,12}, {9,11,12},
                  {10,12,16}, {9,12,16}, {6,10,16}, {5,7,12}, {2,4,11}, {0,2,8} },
/* seacoast */ { {1,5,10}, {1,6,10}, {2,7,10}, {3,9,10}, {7,10,13}, {9,11,17},
                 {10,12,17}, {10,12,17}, {9,11,15}, {4,10,12}, {2,7,11}, {0,5,10} }   },
/* TEMPERATE */ {
/* desert */   { {10,12,18}, {10,12,19}, {11,12,19}, {12,13,22}, {12,15,23}, {15,15,25},
                 {20,24,25}, {20,24,25}, {16,22,24}, {13,18,23}, {11,17,23}, {10,12,17} },
/* forest */   { {2,5,11}, {1,5,10}, {4,10,15}, {7,11,15}, {10,13,20}, {12,15,21},
                 {12,18,23}, {12,18,23}, {11,15,21}, {9,13,17}, {5,9,11}, {3,9,11} },
/* hills */    { {1,8,11}, {2,9,12}, {4,10,15}, {8,11,17}, {10,14,19}, {9,16,23},
                 {11,19,24}, {11,19,24}, {10,16,22}, {9,13,19}, {5,10,15}, {2,9,12} },
/* mountains */ { {0,8,11}, {1,8,12}, {2,9,13}, {4,10,16}, {5,11,18}, {10,14,21},
                  {11,16,23}, {10,16,22}, {8,13,20}, {4,11,18}, {2,10,17}, {1,9,11} },
/* plains */    { {1,7,12}, {2,7,13}, {3,10,16}, {6,11,18}, {10,13,20}, {11,15,23},
                  {12,18,24}, {12,18,24}, {10,15,22}, {6,11,18}, {4,10,16}, {2,9,12} },
/* seacoast */ { {4,9,12}, {5,10,13}, {8,10,16}, {10,11,18}, {10,12,19}, {11,14,22},
                 {12,15,23}, {12,15,22}, {11,14,22}, {10,11,19}, {6,10,15}, {4,10,12} }   },
/* SUBTROPIC */ {
/* desert */   { {11,15,19}, {11,15,20}, {12,18,22}, {13,19,23}, {14,19,23}, {17,21,24},
                 {19,23,25}, {19,23,25}, {17,20,24}, {15,20,24}, {13,18,23}, {12,15,22} },
/* forest */   { {11,19,20}, {11,19,21}, {12,19,21}, {13,19,22}, {13,19,23}, {15,20,23},
                 {15,20,23}, {15,20,23}, {14,20,22}, {13,20,22}, {12,18,21}, {11,18,21} },
/* hills */    { {10,13,16}, {10,14,17}, {11,16,20}, {12,18,21}, {13,19,22}, {14,20,23},
                 {15,20,23}, {15,20,24}, {13,18,23}, {12,16,21}, {11,15,18}, {10,13,17} },
/* mountains */ { {10,12,15}, {10,12,16}, {11,15,18}, {11,15,19}, {12,16,20}, {13,16,20},
                  {13,15,19}, {13,14,17}, {11,14,17}, {11,13,16}, {11,13,16}, {10,12,15} },
/* plains */    { {11,13,17}, {10,14,18}, {12,17,20}, {13,18,23}, {17,21,23}, {18,22,24},
                  {19,22,25}, {19,22,25}, {18,21,24}, {16,21,24}, {12,17,20}, {11,15,18} },
/* seacoast */ { {11,12,20}, {10,12,21}, {10,14,22}, {11,15,23}, {12,16,23}, {12,18,24},
                 {13,18,24}, {13,18,24}, {12,17,23}, {11,16,22}, {10,14,20}, {10,12,20} }   },
/* TROPIC */ {
/* desert */   { {15,17,20}, {15,18,19}, {15,19,22}, {15,20,23}, {16,21,24}, {19,22,25},
                 {21,24,25}, {21,24,25}, {20,23,24}, {20,21,23}, {17,20,22}, {16,18,21} },
/* forest */   { {12,20,22}, {11,20,22}, {12,20,22}, {14,20,23}, {14,20,23}, {16,20,23},
                 {16,20,23}, {16,20,23}, {16,20,23}, {15,20,23}, {14,20,22}, {12,20,22} },
/* hills */    { {12,15,17}, {13,15,19}, {13,17,19}, {15,17,20}, {15,19,23}, {16,20,24},
                 {18,22,25}, {18,22,25}, {18,21,24}, {16,19,22}, {14,17,20}, {13,16,19} },
/* mountains */ { {12,15,17}, {13,15,18}, {13,16,19}, {14,16,19}, {14,16,20}, {14,16,20},
                  {13,15,19}, {13,15,19}, {13,15,19}, {13,15,18}, {12,15,18}, {12,15,18} },
/* plains */    { {12,15,17}, {13,16,19}, {13,17,20}, {15,17,20}, {17,22,24}, {19,22,24},
                  {19,22,25}, {19,22,25}, {19,22,24}, {17,21,24}, {15,18,22}, {13,17,20} },
/* seacoast */ { {15,18,20}, {15,18,21}, {14,19,22}, {16,19,23}, {16,19,23}, {17,19,24},
                 {17,19,23}, {17,19,23}, {17,19,22}, {16,18,21}, {16,18,21}, {15,17,21} }   }
};  /* end Table_Temperature */

extern const RAIN Table_Precipitation[NUM_CLIMATES][NUM_TERRAINS][NUM_SEASONS][3] = 
{
/* ARCTIC */ {
/* desert */    { {NO_RAIN, TRACE, NO_RAIN}, {NO_RAIN, TRACE, NO_RAIN},
                  {NO_RAIN, TRACE, NO_RAIN}, {NO_RAIN, TRACE, NO_RAIN}     },
/* forest */     { {NO_RAIN, NO_RAIN, TRACE}, {NO_RAIN, NO_RAIN, TRACE},
                  {NO_RAIN, NO_RAIN, TRACE}, {NO_RAIN, NO_RAIN, TRACE}      },
/* hills */     { {NO_RAIN, NO_RAIN, TRACE}, {NO_RAIN, NO_RAIN, TRACE},
                  {NO_RAIN, NO_RAIN, TRACE}, {NO_RAIN, NO_RAIN, TRACE}      },
/* mountains */ { {NO_RAIN, NO_RAIN, TRACE}, {NO_RAIN, NO_RAIN, TRACE},
                  {NO_RAIN, NO_RAIN, TRACE}, {NO_RAIN, NO_RAIN, TRACE}      },
/* plains   */  { {NO_RAIN, TRACE, TRACE}, {NO_RAIN, LIGHT, TRACE},
                  {NO_RAIN, MODERATE, LIGHT}, {NO_RAIN, TRACE, TRACE}      },
/* seacoast */  { {NO_RAIN, TRACE, NO_RAIN}, {NO_RAIN, LIGHT, TRACE},
                  {NO_RAIN, LIGHT, TRACE}, {NO_RAIN, LIGHT, TRACE}          }   },
/* SUBARCTIC */ {
/* desert */    { {NO_RAIN, TRACE, NO_RAIN}, {NO_RAIN, TRACE, NO_RAIN},
                  {NO_RAIN, TRACE, NO_RAIN}, {NO_RAIN, TRACE, NO_RAIN}     },
/* forest */    { {NO_RAIN, LIGHT, TRACE}, {TRACE, MODERATE, LIGHT},
                  {TRACE, MODERATE, LIGHT}, {TRACE, LIGHT, TRACE}       },
/* hills */     { {NO_RAIN, TRACE, TRACE}, {NO_RAIN, LIGHT, TRACE},
                  {TRACE, LIGHT, LIGHT}, {NO_RAIN, LIGHT, TRACE}        },
/* mountains */ { {NO_RAIN, TRACE, TRACE}, {TRACE, MODERATE, LIGHT},
                  {TRACE, LIGHT, TRACE}, {NO_RAIN, LIGHT, TRACE}        },
/* plains */    { {NO_RAIN, LIGHT, TRACE},{TRACE, MODERATE, LIGHT},
                  {TRACE, MODERATE, LIGHT}, {TRACE, LIGHT, TRACE}       },
/* seacoast */  { {NO_RAIN, TRACE, TRACE}, {NO_RAIN, LIGHT, TRACE},
                  {TRACE, MODERATE, LIGHT}, {NO_RAIN, LIGHT, TRACE}     } },
/* TEMPERATE */ {
/* desert */    { {NO_RAIN, TRACE, NO_RAIN}, {NO_RAIN, TRACE, NO_RAIN},
                  {NO_RAIN, TRACE, NO_RAIN}, {NO_RAIN, NO_RAIN, NO_RAIN}     },
/* forest */    { {TRACE, MODERATE, LIGHT}, {LIGHT, HEAVY, MODERATE},
                  {LIGHT, MODERATE, MODERATE}, {LIGHT, MODERATE, MODERATE}   },
/* hills */     { {TRACE, MODERATE, LIGHT}, {LIGHT, HEAVY, MODERATE},
                  {LIGHT, HEAVY, MODERATE}, {TRACE, MODERATE, LIGHT}    },
/* mountains */ { {NO_RAIN, MODERATE, LIGHT}, {NO_RAIN, MODERATE, LIGHT},
                  {NO_RAIN, LIGHT, TRACE}, {NO_RAIN, MODERATE, LIGHT}   },
/* plains */    { {NO_RAIN, LIGHT, TRACE}, {LIGHT, HEAVY, MODERATE},
                  {LIGHT, HEAVY, MODERATE}, {TRACE, MODERATE, LIGHT}     },
/* seacoast */  { {LIGHT, HEAVY, MODERATE}, {TRACE, MODERATE, LIGHT},
                  {NO_RAIN, LIGHT, TRACE}, {LIGHT, HEAVY, MODERATE}      }      },
/* SUBTROPIC */ {
/* desert */    { {NO_RAIN, TRACE, NO_RAIN}, {NO_RAIN, TRACE, NO_RAIN},
                  {NO_RAIN, NO_RAIN, NO_RAIN}, {NO_RAIN, TRACE, NO_RAIN}     },
/* forest */    { {MODERATE, HEAVY, MODERATE}, {MODERATE, DOWNPOUR, HEAVY},
                  {MODERATE, DOWNPOUR, HEAVY}, {MODERATE, HEAVY, HEAVY}      },
/* hills */     {  {TRACE, LIGHT, LIGHT}, {LIGHT,HEAVY, MODERATE},
                    { LIGHT, HEAVY, MODERATE}, {TRACE, MODERATE, LIGHT}      },
/* mountains */ {  {TRACE, MODERATE, LIGHT}, {LIGHT, HEAVY, MODERATE},
                   {LIGHT, MODERATE, MODERATE}, {TRACE, MODERATE, LIGHT}     },
/* plains */    { {NO_RAIN, LIGHT, TRACE}, {TRACE, HEAVY, LIGHT},
                  {TRACE, MODERATE, LIGHT}, {NO_RAIN, LIGHT, TRACE}      },
/* seacoast */  { {TRACE, MODERATE, LIGHT}, {LIGHT, HEAVY, MODERATE},
                  {LIGHT, DOWNPOUR, MODERATE}, {NO_RAIN, LIGHT, TRACE}   }      },
/* TROPIC */ {
/* desert */    { {NO_RAIN, TRACE, NO_RAIN}, {NO_RAIN, LIGHT, TRACE},
                  {NO_RAIN, TRACE, NO_RAIN}, {NO_RAIN, TRACE, NO_RAIN}      },
/* forest */    { {MODERATE, DOWNPOUR, HEAVY}, {MODERATE, DOWNPOUR, HEAVY},
                  {MODERATE, DOWNPOUR, HEAVY}, {MODERATE, DOWNPOUR, HEAVY}   },
/* hills */     { {NO_RAIN, TRACE, NO_RAIN}, {NO_RAIN, LIGHT, TRACE},
                  {TRACE, MODERATE, LIGHT}, {NO_RAIN, TRACE, NO_RAIN}    },
/* mountains */ { {TRACE, MODERATE, LIGHT}, {MODERATE, HEAVY, HEAVY},
                  {TRACE, MODERATE, LIGHT}, {MODERATE, HEAVY, HEAVY}        },
/* plains */    { {NO_RAIN, TRACE, NO_RAIN}, {LIGHT, HEAVY, MODERATE},
                  {MODERATE, HEAVY, MODERATE}, {LIGHT, HEAVY, MODERATE}      },
/* seacoast */  { {NO_RAIN, TRACE, NO_RAIN}, {LIGHT, DOWNPOUR, MODERATE},
                  {HEAVY, DOWNPOUR, DOWNPOUR}, {NO_RAIN, LIGHT, TRACE}       }      }
}; /* end Table_Precipitation */

extern const SPECIALS Table_Special [NUM_CLIMATES][NUM_TERRAINS][NUM_SEASONS][4] = 
{
/* ARCTIC */ {
/* desert */    { {GG,AA,DD,DD}, {GG, AA, DD, DD}, {GG, AA, DD, DD}, {GG,AA,DD,DD} },
/* forest */    { {GG,AA,DD,DD}, {GG, AA, DD, DD}, {GG, AA, DD, DD}, {GG,AA,DD,DD} },
/* hills */     { {GG,AA,DD,DD}, {GG,AA,DD,MM}, {GG,AA,DD,ZZ}, {GG,AA,DD,DD} },
/* mountains */ { {GG,AA,DD,DD}, {GG,AA,DD,MM}, {GG,AA,MM,ZZ}, {GG,AA,DD,MM} },
/* plains */    { {GG,AA,DD,DD}, {AA,GG,DD,MM}, {XX,AA,DD,ZZ}, {GG,AA,DD,ZZ} },
/* seacoast */  { {GG,AA,DD,DD}, {AA,GG,MM,ZZ}, {GG,AA,MM,ZZ}, {GG,AA,MM,ZZ} }  },
/* SUBARCTIC */ {
/* desert */    { {GG,AA,DD,DD}, {GG,AA,DD,DD}, {AA,SS,DD,DD}, {GG,AA,DD,DD} },
/* forest */    { {GG,AA,DD,ZZ}, {AA,XX,DD,ZZ}, {XX,AA,ZZ,DD}, {AA,XX,MM,DD} },
/* hills */     { {GG,AA,MM,DD}, {AA,GG,DD,ZZ}, {AA,XX,ZZ,DD}, {GG,AA,MM,DD} },
/* mountains */ { {GG,AA,DD,MM}, {AA,XX,MM,ZZ}, {AA,XX,ZZ,DD}, {GG,AA,MM,DD} },
/* plains */    { {GG,AA,MM,DD}, {AA,XX,DD,ZZ}, {ZZ,AA,ZZ,DD}, {GG,AA,DD,ZZ} },
/* seacoast */  { {GG,AA,MM,ZZ}, {GG,AA,MM,ZZ}, {XX,AA,ZZ,MM}, {AA,GG,MM,ZZ} }  },
/* TEMPERATE */ {
/* desert */    { {AA,SS,DD,ZZ}, {GG,SS,DD,ZZ}, {GG,SS,DD,ZZ}, {SS,GG,DD,ZZ} },
/* forest */    { {AA,XX,MM,ZZ}, {GG,XX,MM,ZZ}, {GG,XX,MM,ZZ}, {AA,XX,MM,ZZ} },
/* hills */     { {AA,XX,MM,ZZ}, {AA,XX,ZZ,TT}, {AA,XX,DD,ZZ}, {AA,XX,DD,ZZ} },
/* mountains */ { {AA,XX,MM,DD}, {AA,XX,MM,ZZ}, {AA,XX,DD,ZZ}, {XX,AA,DD,ZZ} },
/* plains */    { {AA,GG,DD,ZZ}, {AA,XX,TT,ZZ}, {XX,GG,DD,ZZ}, {XX,AA,DD,ZZ} },
/* seacoast */  { {AA,XX,MM,ZZ}, {CC,XX,MM,ZZ}, {CC,XX,MM,ZZ}, {AA,XX,MM,ZZ} }  },
/* SUBTROPIC */ {
/* desert */    { {AA,SS,DD,ZZ}, {GG,SS,DD,ZZ}, {GG,SS,DD,ZZ}, {GG,SS,DD,ZZ} },
/* forest */    { {XX,AA,MM,ZZ}, {XX,GG,ZZ,MM}, {XX,XX,ZZ,MM}, {AA,XX,MM,ZZ} },
/* hills */     { {AA,XX,DD,ZZ}, {AA,XX,DD,TT}, {XX,GG,DD,ZZ}, {AA,XX,ZZ,MM} },
/* mountains */ { {AA,XX,DD,ZZ}, {AA,XX,ZZ,MM}, {XX,GG,ZZ,DD}, {XX,AA,ZZ,MM} },
/* plains */    { {AA,XX,DD,ZZ}, {XX,GG,ZZ,TT}, {XX,GG,DD,ZZ}, {GG,AA,DD,ZZ} },
/* seacoast */  { {AA,XX,DD,ZZ}, {CC,XX,MM,ZZ}, {CC,XX,MM,ZZ}, {CC,XX,DD,ZZ} }  },
/* TROPIC */    {
/* desert */    { {AA,SS,DD,ZZ}, {GG,SS,DD,ZZ}, {GG,SS,DD,ZZ}, {GG,SS,DD,ZZ} },
/* forest */    { {XX,XX,MM,ZZ}, {XX,XX,ZZ,MM}, {XX,XX,ZZ,MM}, {XX,XX,MM,ZZ} },
/* hills */     { {AA,GG,DD,ZZ}, {GG,XX,DD,ZZ}, {GG,XX,DD,ZZ}, {GG,GG,ZZ,DD} },
/* mountains */ { {AA,XX,MM,ZZ}, {AA,XX,MM,ZZ}, {XX,GG,ZZ,MM}, {AA,GG,MM,ZZ} },
/* plains */    { {GG,XX,DD,ZZ}, {GG,XX,DD,ZZ}, {SS,XX,ZZ,DD}, {GG,XX,ZZ,DD} },
/* seacoast */  { {XX,AA,DD,ZZ}, {AA,XX,MM,ZZ}, {CC,XX,MM,ZZ}, {XX,AA,ZZ,MM} } }
};

//  ---------------------------------------------------
//  Temperature        Trop/subTrop        Temp/SA/Arc.
//  ---------------------------------------------------
//  <= -1                 na            S(1-3) none(4-6)
//  0-9                   na            S(1-5) none(6)
//  10-29                 na            S(1-5) I(6)*
//  30-35               H(1-6)          S(1-4) H(5) I(6)
//  36-49               L(1-5) H(6)     L(1-4) H(5-6)
//  50-74               L(1-6)          L(1-5) H(6)
//  >= 75               L(1-6)          L(1-6)
//  ---------------------------------------------------
//  H = hailstorm I = ice/sleet storm L = thunderstorm
//  S = severe snowstorm   *I only if t >= 25, otherwise none.
//  ---------------------------------------------------
extern const short xtreme_temp_lookup[XTL_SIZE] = {-1, 9, 29, 35, 49, 74, 999};

extern const XTREME_LOOKUP_ENTRY xtreme_type_lookup[2][XTL_SIZE][2] = {
   /* TROPICAL,SUBTROPICAL */
   { { {NOSTORM,0,99},{NOSTORM,0,0} },
     { {NOSTORM,0,99},{NOSTORM,0,0} },
     { {NOSTORM,0,99},{NOSTORM,0,0} },
     { {HSTORM,1,6}, {NOSTORM,0,0} },
     { {TSTORM,1,5}, {HSTORM,6,6} },
     { {TSTORM,1,6}, {NOSTORM,0,0} },
     { {TSTORM,1,6}, {NOSTORM,0,0} }
   },
   /* TEMPERATE,SUBARCTIC,ARCTIC */
   { { {SSTORM,1,3}, {NOSTORM,4,6} },
     { {SSTORM,1,5}, {NOSTORM,6,6} },
     { {SSTORM,1,5}, {ISTORM,6,6} },
     { {SSTORM,1,4}, {ISTORM,5,6} },
     { {TSTORM,1,4}, {HSTORM,5,6} },
     { {TSTORM,1,5}, {HSTORM,6,6} },
     { {TSTORM,1,6}, {NOSTORM,0,0} }
   }
};

