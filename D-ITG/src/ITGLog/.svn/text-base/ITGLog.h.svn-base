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






#define OFFSET_LOG_PORT 1000


#define SEED_LOG_PORT 9003


struct paramLogThread {
	int socket;					
};






extern ofstream out;


extern int flagTerm;


extern signaling signalingLog;


extern struct sockaddr_in senderLog;


extern int signalSocket;


#ifdef LINUX_OS
extern const char DEFAULT_LOG_FILE[];


#endif

#ifdef WIN32
extern const char DEFAULT_LOG_FILE[];

extern HANDLE mutexLog;

#endif




void printHelpAndExit();


void closeFileLog(ofstream *out);


void recvInfo(int signalingChannel,BYTE &protocol,char logFile[DIM_LOG_FILE]);


int findPortFree(int logSock);


char* allowedLogFile(char logFile[DIM_LOG_FILE]);


void *channelManager(void *param);



void terminate(int sign);



void reportErrorAndExit(const char *function, const char *program, const char *msg);


void createSignalingChannel();



void logInit();



void parserLog(int argc , char *argv[]);

