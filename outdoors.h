/* ------------------------------------------------- */
// outdoors.h - class Outdoors
// $Id: outdoors.h,v 1.8 2002/03/21 23:51:28 cyhiggin Exp $
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
#ifndef _OUTDOORS_H
#define _OUTDOORS_H

#include "wthrmkr.h"
#include "calendar.h"

class Outdoors {
    public:
    /* input items */
        CLIMATE clime;
        TERRAIN terrain;
        short month[2];
        short day[2];
        short year[2];
        long elevation;
        short num_days;
        short start_jday;         // of starting year
        short curr_jday;         // of current year
        short external_init_flag;
        char *status_line;
        short version_only;
        short show_metric;
        short show_moon;
    /* derived items */
        SEASON season;
        Calendar *pCal;
        CALCONFIG calendar_cfg;
        JDate curr_jdate;       // used by lunar functions
    private:
        short good_parse;
        short DUMP_FLAG;
        void init_defaults( void );
        int get_status_line( char *fname );
        int parse_status_line(void);
    public:
        Outdoors( int argc, char *argv[] );
        Outdoors( void );
        ~Outdoors(void);
        short IsParsed(void) { return good_parse; };
        short Parse( int argc, char *argv[] );
        void Dump(short do_dateline=1);
        void PrintDate(void);
        short IsDump(void) {return DUMP_FLAG;};
        int IterateDate(void);
        int default_elevation( void );
        int LastDay(void) {return (num_days <= 0); };
    };  /* end class */

#endif
