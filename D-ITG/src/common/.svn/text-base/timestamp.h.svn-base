/*
 *   Component of the D-ITG v2.8.0-Beta platform (http://www.grid.unina.it/software/ITG)
 *
 *   Copyright    : (C) 2004-2010 by Stefano Avallone, Alessio Botta, Alberto Dainotti
 *                                     Walter de Donato, Antonio Pescape' (PI)
 *                                     of the COMICS (COMputer for Interaction and 
 *                                     CommunicationS) Group, Dipartimento di Informatica
 *                                     e Sistemistica of the University of Napoli "Federico II".
 *   email        : stavallo@unina.it , a.botta@unina.it , alberto@unina.it
 *                  walter.dedonato@unina.it , pescape@unina.it
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 * 
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 * 		     
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *				     
 *   For commercial use please refer to D-ITG Professional.
 */



#ifdef WIN32
#include <windows.h>
#endif

typedef double Real;

#ifdef WIN32
#define UPDATE_TICKER(a,b,c,d,e) updateTicker(a,b,c,d,e)


#define GET_TIME_OF_DAY(a,b,c,d,e,f,g)	gettimeofday(a,b,c,d,e,f,g)


#define TSTART(a,b,c,d,e,f)	tstart(a,b,c,d,e,f)


#define RETRIEVE_SEC(a)		a
#endif

#ifdef LINUX_OS
#define UPDATE_TICKER(a,b,c,d,e) updateTicker(a)

#define GET_TIME_OF_DAY(a,b,c,d,e,f,g)	gettimeofday(a,NULL)



#define TSTART(a,b,c,d,e,f)	


#define RETRIEVE_SEC(a)		a%86400L

#endif

struct TTicker {
	Real count;
	struct timeval lastTime;
};

#ifdef WIN32

extern LARGE_INTEGER freq;
#endif

#ifdef WIN32

int tstart(LARGE_INTEGER & _tstart, unsigned long &secs, unsigned long &msecs,
    int &first, BYTE meter, int flag);


int gettimeofday(struct timeval *thisTime, LARGE_INTEGER & _tend, LARGE_INTEGER & _tstart,
    unsigned long &secs, unsigned long &msecs, BYTE meter, int flag);


void updateTicker(struct TTicker *Ticker, LARGE_INTEGER & _tend, LARGE_INTEGER & _tprec,
    LARGE_INTEGER & _tstart, int &first);
#endif


void setSeedRandom();

#ifdef LINUX_OS

void updateTicker(struct TTicker *Ticker);
#endif
