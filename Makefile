#   $Id: Makefile,v 1.14 2002/05/18 17:22:28 cyhiggin Exp $
#   Copyright (C) 2002 Cynthia Higginbotham
#   This file is part of WeatherMaker.
#
#   WeatherMaker is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
#   WeatherMaker is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with WeatherMaker; if not, write to the Free Software
#   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#   or visit their website at http://www.fsf.org.
#
# Hand-converted from IBM OS/2 Makefile.
# dependecies created using makedepend

SHELL := /bin/bash
DEPDIR := ./.deps

OBJS := special.o wthrmkr.o outdoors.o \
	intermediatereport.o dailyreport.o calendar.o lunar.o tables.o 

SRCS := $(OBJS:.o=.cpp) calendar.h dailyreport.h intermediatereport.h  \
	macros.h outdoors.h tables.h wthrmkr.h

CXX := g++
CXXFLAGS = -m64 -ggdb -std=gnu++98 -fno-strict-aliasing -Wall -Wextra
#CXXFLAGS = -Wall -O2

INCLUDE := -I.

LIBS =

all:	wthrmkr

wthrmkr: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)
	#strip wthrmkr

# generate dependency files & compile all in one step
df = $(DEPDIR)/$(*F)
%.o : %.cpp
	$(CXX) -MMD $(CXXFLAGS) -c $< $(INCLUDE) 
	@cp $*.d $(df).P; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
                -e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $(df).P; \
	rm -f $*.d

-include $(OBJS:%.o=$(DEPDIR)/%.P)

.PHONY: clean

clean:
	rm -f $(OBJS) wthrmkr *~ *.bak $(DEPDIR)/*.P

