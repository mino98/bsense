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
 *
 *   Version modified for use in the BSense project, Giacomo Bernardi <g.bernardi@sms.ed.ac.uk>
 */




#include "../common/ITG.h"
#include "../common/debug.h"
#include "../common/thread.h"
#include "../common/pipes.h"
#include "ITGRecv.h"
#include "data.h"

#ifdef WIN32
#include <math.h>
#endif

#ifdef LINUX_OS
#include <sys/uio.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#ifdef BSD
#include <sys/wait.h>
#else
#include <wait.h>
#endif
#endif


struct addrinfo *globaleLogHost;


int globaleLogSock = 0;


int globaleLogSockSignaling = 0;


int defaultLogPortSignaling = DEFAULT_LOG_PORT_SIGNALING;


int defaultPortSignaling = DEFAULT_PORT_SIGNALING;


int sockSignaling;

 
int flagTerm = NO_TERMINATE;


int logCheck = 0;


int logRemote = 0;


ofstream out;


char logFile[DIM_LOG_FILE];


char* ifaceBind = NULL;


BYTE l4ProtoLog = DEFAULT_PROTOCOL_TX_LOG;


manageLogFile memLogFile[MAX_NUM_THREAD];


bool setPriority = false;


char nameProgram[] = "ITGRecv";


#ifdef WIN32
const char DEFAULT_LOG_FILE[] = "ITGRecv.log";

HANDLE mutexLog;

HANDLE mutexLogRem;

int userId = 0;
#endif

#ifdef LINUX_OS
const char DEFAULT_LOG_FILE[] = "/tmp/ITGRecv.log";

uid_t userId;

pthread_mutex_t mutexLog;

pthread_mutex_t mutexLogRem;
#endif

#ifdef SCTP
int sctpSessionCount = 0;
sctpSession sctpSessions[MAX_NUM_THREAD];
pthread_mutex_t mutexSctp = PTHREAD_MUTEX_INITIALIZER;
#endif

pthread_t waitStopKeyTid;

bool passiveMode=false;				
struct addrinfo *passiveSender;		
unsigned short numHolePkt=1;        

char * SigInterface = NULL;			

struct addrinfo *globalBindAddr = NULL;         

void* waitStopKey(void* s)
{
#ifdef WIN32
	Sleep(1000);
#endif
	PRINTD(1, "Waiting for the stop key\n");
	while (getchar() != 'C') {}
	printf("Terminated by request\n");
	terminate(0);
	return NULL;
}


void reportErrorAndExit(const char *function, const char *program, const char *msg)
{
	
	if (flagTerm == NO_TERMINATE) {
		printf("\n** ERROR_TERMINATE **\n");
		printf("Function %s aborted caused by %s \n", function, program);
		printf("** %s ** \n", msg);
		
		terminate(SIGTERM);
	}
	
	else if (flagTerm == ERROR_TERMINATE) {
		printf("\n** ERROR_TERMINATE IN TERMINATE **\n");
		printf("Function %s aborted caused by %s \n", function, program);
		printf("** %s ** \n", msg);
		printf("Finish with error in terminate!\n");
		exit(1);
	}
	
	else
		
		sleep(INFINITE);
}



void terminate(int sign)
{
	if (flagTerm == NO_TERMINATE) {
		
		flagTerm = TERMINATE;
		PRINTD(1,"** Terminate function ***\n");
		
		if (logCheck == 1)
			out.close();
		
		else if (logRemote == 1) {
			signaling signalingLog;
			signalingLog.stop = true;
			if (sendto(globaleLogSockSignaling, (char *) &signalingLog,
				sizeof(signalingLog), 0, globaleLogHost->ai_addr,
				globaleLogHost->ai_addrlen) < 0) {
				
				flagTerm = ERROR_TERMINATE;
				reportErrorAndExit("terminate", "sendto",
				    "Cannot send to LogServer info about stopping signaling");
			}
			PRINTD(1,"Signaling Log STOP sent to Log Server \n");
			if (closeSock(globaleLogSock) < 0) {
				
				flagTerm = ERROR_TERMINATE;
				reportErrorAndExit("terminate", "closeSock",
				    "Cannot close socket logSock");
			}
			if (closeSock(globaleLogSockSignaling) < 0) {
				
				flagTerm = ERROR_TERMINATE;
				reportErrorAndExit("terminate", "closeSock",
				    "Cannot close socket logSockSignaling");
			}
		}
		if (MUTEX_THREAD_RELEASE(mutexLog) < 0) {
			
			flagTerm = ERROR_TERMINATE;
			reportErrorAndExit("terminate", "MUTEX_THREAD_RELEASE",
			    "Cannot release Log Mutex");
		}
		if ( MUTEX_THREAD_RELEASE(mutexLogRem) < 0) {
			
			flagTerm = ERROR_TERMINATE;
			reportErrorAndExit("terminate","MUTEX_THREAD_RELEASE","Cannot release Remote Log Mutex ");
		}
#ifdef WIN32
		
		if (WSACleanup() != 0) {
			
			flagTerm = ERROR_TERMINATE;
			reportErrorAndExit("terminate", "WSACleanup", "Cannot clean WSA");
		}
#endif
		


		
		if (sign == SIGINT)
			printf("Finish with CTRL-C! \n");
		
		else if (sign == SIGTERM)
			printf("Finish requested caused by errors! \n");
	}
	PRINTD(1, "Terminating thread waitStopKey and exiting\n");
	terminateThread(waitStopKeyTid);
	
	if (sign == SIGTERM)
		exit(1);
	else
		exit(0);
}



char *allowedLogFile(char logFile[DIM_LOG_FILE])
{
	int i = 0;
	
	bool find = true;
	
	while ((i < MAX_NUM_THREAD) && (find == true)) {
		if (strcmp(memLogFile[i].logFile, logFile) == 0) {
			
			find = false;
			
			memLogFile[i].num++;
			return NULL;
		} else
			i++;
	}
	i = 0;
	
	while (memLogFile[i].num != -1)
		i++;
	
	memLogFile[i].out.open(logFile, ios::out | ios::binary | ios::trunc);
	if (!memLogFile[i].out.is_open()) {
		
		char *tail =
		    (char *) malloc(sizeof("Error into open this file : ") + sizeof(logFile));
		if (tail == NULL)
			reportErrorAndExit("allowedLogFile", "malloc",
			    "Insifficient memory available");
		
		sprintf(tail, "Error into open this file : %s", logFile);
		reportErrorAndExit("allowedLogFile", "open", tail);
	}
	
	memLogFile[i].num = 1;
	strcpy(memLogFile[i].logFile, logFile);
	
	return (char *) &memLogFile[i].out;
}



void closeFileLog(ofstream * out)
{
	int i = 0;
	
	while (out != (ofstream *) & memLogFile[i].out)
		i++;
	
	memLogFile[i].num--;
	if (memLogFile[i].num == 0) {
		
		memLogFile[i].num = -1;
		strcpy(memLogFile[i].logFile, " ");
		
		(*out).close();
	}
}



int sendAck(int signaling, BYTE typeMessage)
{
	
	char msg;
	
	putValue(&msg, (void *) &typeMessage, sizeof(typeMessage));

	
	if (send(signaling, (char *) &msg, sizeof(msg), 0) < 0)
		return -1;
	else
		return 0;
}



int sendAckFlow(int signaling, BYTE typeMessage, int flowId, uint16_t openedPort)
{
	
	char msg[sizeof(BYTE) + sizeof(int) + sizeof(uint16_t)];
	
	char *next;
	int sizeMessag = 0;
	int net_flowId = htonl(flowId); 
	uint16_t net_openedPort = htons(openedPort);

	
	next = putValue(&msg, (void *) &typeMessage, sizeof(typeMessage));
	sizeMessag += sizeof(typeMessage);

	next = putValue(next, (void *) &net_flowId, sizeof(net_flowId));
	sizeMessag += sizeof(net_flowId);

	if (typeMessage == TSP_ACK_SEND_FLOW){
		next = putValue(next, (void *) &net_openedPort, sizeof(net_openedPort));
		sizeMessag += sizeof(net_openedPort);
	}

	
	if (send(signaling, (char *) &msg, sizeMessag, 0) < 0)
		return -1;
	else
		return 0;
}



void *signalManager(void *param)
{
	
	struct addrinfo logHost;

	
	int logSock = 0;

	
	int logSockSignaling = 0;

	
	paramThread *para;

	
	para = (paramThread *) param;
	
	pthread_t hThr[MAX_NUM_THREAD];
	
#ifdef LINUX_OS
	fd_set activeSet;
	
	timeval timeOutFile;
	
#endif
	memChannel flowIdNum[MAX_NUM_THREAD];
	
	paramThread paraThread[MAX_NUM_THREAD];
	
	BYTE type;
	
	int numFlow = 0;
	
	int newSockSignaling = para->socket;
	
	char buffer[1];
	
	bool uscita = false;
	int numDiscovery = 0;
	
	char *fileDescriptor = (char *) &out;
	char nameFileLog[DIM_LOG_FILE];
	BYTE protocolLog;
#ifdef LINUX_OS
	int fd, maxfd;
	int rPipe[2];
#endif
#ifdef WIN32
	HANDLE rPipe[3];
	HANDLE fd, namedPipe;
	HANDLE events[2];
	DWORD available = 0;
	DWORD waited;
	unsigned long pending;
#endif

	int size = 0;

	
	for (int i = 0; i < MAX_NUM_THREAD; i++) {
		flowIdNum[i].flowId = -1;
		hThr[i] = 0;
		paraThread[i].flowId = 0;
		paraThread[i].iface = ifaceBind;
		paraThread[i].count = 0;
		paraThread[i].socket = 0;
		paraThread[i].socketClose = 0;
		paraThread[i].preambleSize = StandardMinPayloadSize;
		paraThread[i].payloadLogType = PL_STANDARD;
		paraThread[i].addressInfos = NULL;
	}

	
	if (createNewPipe(rPipe) < 0) {
		printf("Error in signal to create a new pipe \n");
		fflush(stdout);
	}

	
	do {
		if (recv(newSockSignaling, (char *) &buffer, sizeof(BYTE), 0) < 0)
			reportErrorAndExit("signalManager", "recv - connect",
			    "Cannot receive newSockSignaling data");
		
	} while (*(BYTE *) buffer != TSP_CONNECT);
	PRINTD(1,"Signal Manager : Received TSP_CONNECT(1) message\n");

	
	if (sendAck(newSockSignaling, TSP_ACK_CONNECT) < 0)
		reportErrorAndExit("signalManager", "sendAck", "Cannot send connect ack");
	PRINTD(1,"Signal Manager : Sent TSP_ACK_CONNECT(2) message\n");



	fd = rPipe[0];

#ifdef LINUX_OS
	
	maxfd = MAX(fd, newSockSignaling);		
#endif

#ifdef WIN32
	
	events[0] = WSACreateEvent();
	WSAEventSelect(newSockSignaling, events[0], FD_READ);
	
	events[1] = rPipe[1];
	namedPipe = rPipe[2];
#endif
	if (logRemote == 1) {
		logSockSignaling = globaleLogSockSignaling;
		logSock = globaleLogSock;
		copia(globaleLogHost, logHost);
	}

	
	while (1) {
		
		type = 0;
#ifdef LINUX_OS
		
		FD_ZERO(&activeSet);

		
		FD_SET(fd, &activeSet);
		FD_SET((unsigned int) newSockSignaling, &activeSet);
		
		timeOutFile.tv_sec = TIME_OUT;
		timeOutFile.tv_usec = 0;
		
		if (select(FD_SETSIZE, &activeSet, NULL, NULL, &timeOutFile) < 0)
			reportErrorAndExit("signalManager", "select - type",
			    "Invalid file descriptor");

		
		if (FD_ISSET(fd, &activeSet)) {
#endif

#ifdef WIN32
		waited = WaitForMultipleObjects(2, (const HANDLE *) events, FALSE, TIME_OUT * 1000);
		ResetEvent(events[0]);
		available = 0;
		
		if (PeekNamedPipe(namedPipe, NULL, 0, NULL, &available, NULL) == 0) {
			reportErrorAndExit("signalManager", "PeekNamedPipe",
			    "Error in peek named Pipe");
		}
		PRINTD(1,"available: %d\n", (int) available);
		while (available > 0) {
#endif
			pipeParser(newSockSignaling, numFlow, rPipe, flowIdNum, paraThread, hThr);
#ifdef WIN32
			
			if (PeekNamedPipe(namedPipe, NULL, 0, NULL, &available, NULL) == 0) {
				reportErrorAndExit("signalManager", "PeekNamedPipee",
				    "Error in peek named Pipe");
			}	
#endif
		}	

#ifdef LINUX_OS
		
		else if (FD_ISSET(newSockSignaling, &activeSet)) {
#endif
#ifdef WIN32
		pending = 0;
		ioctlsocket(newSockSignaling, FIONREAD, &pending);
		PRINTD(1,"pending - out: %lu\n", pending);
		while ((pending > 0) && (uscita != true)) {
#endif
			numDiscovery = 0;
			type=0xFF;	
			
			size = recv(newSockSignaling, (char *) &type, sizeof(type), 0);

			if (size <= 0){
				printf("\nSender has shut down the connection gracefully or sender down\n");
				type = TSP_SENDER_DOWN;
				uscita = true;
			}

			if (type == TSP_SEND_NAME_LOG) {
				recvNameLog(nameFileLog, newSockSignaling);
				if (logCheck != 1) {
					fileDescriptor = allowedLogFile(nameFileLog);	
					if (fileDescriptor == NULL) {
						if (sendAck(newSockSignaling, TSP_ERR_MSG_4) < 0)
							reportErrorAndExit("pipeParser", "",
							    "Cannot send TSP_ERR_MSG_4 message to sender");
							PRINTD(1,"Signal manager : sent TSP_ERR_MSG_4(22) message\n");
					}	
					else {
						if (sendAck(newSockSignaling,
							TSP_ACK_SEND_NAME_LOG) < 0)
							reportErrorAndExit("recvNameLog",
							    "sendAck - type = TSP_ACK_SEND_NAME_LOG(18)",
							    "Cannot send Ack on newSockSignaling");
						PRINTD(1,"Signal manager : sent TSP_ACK_SEND_FLOW_LOG(18) message\n");
						logCheck = 2;
					}	
				}	
				else {
					if (sendAck(newSockSignaling,
						TSP_ACK_SEND_NAME_LOG) < 0)
						reportErrorAndExit("recvNameLog",
						    "sendAck - type = TSP_ACK_SEND_NAME_LOG(18)",
						    "Cannot send Ack on newSockSignaling");
					PRINTD(1,"Signal manager : sent TSP_ACK_SEND_FLOW_LOG(18) message\n");
					fileDescriptor = (char *) &out;
				}
			} else if (type == TSP_SEND_FLOW_LOG) {
				recvFlowLog(newSockSignaling, logHost, protocolLog,
				    nameFileLog);
				if (logRemote != 1) {	
					createRemoteLogFile(logHost, nameFileLog,
					    protocolLog, logSockSignaling, logSock);
					logRemote = 2;
				} else {	
					logSockSignaling = globaleLogSockSignaling;
					logSock = globaleLogSock;
					copia(globaleLogHost, logHost);
				}
				if (sendAck(newSockSignaling, TSP_ACK_SEND_FLOW_LOG) < 0)
					reportErrorAndExit("sendFlowLog",
					    "sendAck - type = TSP_ACK_SEND_FLOW_LOG(12)",
					    "Cannot send Ack on newSockSignaling");
				PRINTD(1,"Signal manager : sent TSP_ACK_SEND_FLOW_LOG(12) message\n");
			} else if ((type == TSP_RELEASE) || (type == TSP_CLOSED_ERR)
			    || (type == TSP_SENDER_DOWN) || (type == TSP_SEND_FLOW)
			    || (type == TSP_CLOSED_FLOW) || (type == TSP_ERR_MSG_1)
			    || (type == TSP_ACK_SEND_FLOW)) {
				if (typeParser(type, numFlow, newSockSignaling, flowIdNum,
					paraThread, hThr, rPipe, fileDescriptor, logSock,
					logSockSignaling, &logHost) == -1) {
					uscita = true;
				}	
			}else if ((type > TSP_ERR_MSG_5) && (size > 0)){
				reportErrorAndExit("signalManager","unknown message received",programName);
			}
#ifdef WIN32
			ioctlsocket(newSockSignaling, FIONREAD, &pending);
			PRINTD(1,"pending -in : %lu\n", pending);
#endif
		}	

#ifdef LINUX_OS
		else {
#endif

#ifdef WIN32
		if (waited == WAIT_TIMEOUT) {
#endif
			int err;

			
			type = TSP_DISCOVERY;
			
#ifdef LINUX_OS
			err = send(newSockSignaling, (char *) &type, sizeof(type),  MSG_DONTWAIT);
#endif
#ifdef WIN32
			int timeo=100000;
            		if(setsockopt(newSockSignaling,SOL_SOCKET,SO_SNDTIMEO,(char*) &timeo,sizeof(timeo))<0)
               			PRINTD(1,"Error in setting timeout for sending\n");
            		err = send(newSockSignaling, (char *) &type, sizeof(type), 0);
            		timeo=0;
            		if(setsockopt(newSockSignaling,SOL_SOCKET,SO_SNDTIMEO,(char*) &timeo,sizeof(timeo))<0)
               			PRINTD(1,"Error in setting timeout for sending\n");
#endif
			if (err < 0 || err == EAGAIN)
				reportErrorAndExit("signalManager", "recv - discovery",
				    "Cannot receive data on newSockSignaling");
			PRINTDS(1,"Signal manager : Sent TSP_DISCOVERY message\n");
			numDiscovery++;
			if (numDiscovery == 2) {
				PRINTD(1,"Signal manager : Sender is down\n");
				type = TSP_SENDER_DOWN;
				uscita = true;
			}
		}
		if (uscita == true)
			break;

		
	}

	
	if (type != TSP_SENDER_DOWN)
		
		if (sendAck(newSockSignaling, TSP_ACK_RELEASE) < 0)
			reportErrorAndExit("signalManager", "sendAck", "Cannot send ack release");
	PRINTD(1,"Signal Manager : sent TSP_ACK_RELEASE(7) message\n");

	if (logRemote == 2) {
		
		signaling signalingLog;
		signalingLog.stop = true;

		if (logHost.ai_family == PF_INET)
			((struct sockaddr_in *) (logHost.ai_addr))->sin_port =
			    htons(defaultLogPortSignaling);
		else if (logHost.ai_family == PF_INET6)
			((struct sockaddr_in6 *) (logHost.ai_addr))->sin6_port =
			    htons(defaultLogPortSignaling);

		if (sendto(logSockSignaling, (char *) &signalingLog,
			sizeof(signalingLog), 0, logHost.ai_addr,
			logHost.ai_addrlen) < 0)
			reportErrorAndExit("typeParser", "send stop to LOG_SERVER",
			    "Cannot send stop logServer");
		PRINTD(1,"Sent infos to LogServer\n");

		if (closeSock(logSock) < 0) {
			reportErrorAndExit("signalManager", "closeSock",
			    "Cannot close socket logSock after sending ack release");
			
		}
		if (closeSock(logSockSignaling) < 0) {
			reportErrorAndExit("signalManager", "closeSock",
			    "Cannot close socket logSockSignaling after sending ack release");
			
		}
	}

	
	
	if (closeSock(newSockSignaling) < 0)
		reportErrorAndExit("signalManager", "closeSock",
		    "Cannot close socket newSockSignaling after sending ack release");
	
	if (closePipe(rPipe) < 0)
		reportErrorAndExit("signalManager", "closePipe",
		    "Cannot close pipe ");
	PRINTD(1,"Closed Signaling Channel\n");

	
	PRINTD(2,"Signal Manager : Garbage collector - start\n");
	for (int i = 0; i < MAX_NUM_THREAD; i++) {
		terminateThread(hThr[i]);
		closeSock(paraThread[i].socketClose);
		if (paraThread[i].addressInfos != NULL)
			free(paraThread[i].addressInfos);
	}
	PRINTD(2,"Signal Manager : Garbage collector - end\n");

	if(passiveMode==false){ 
		
		exitThread();
	}

	return 0;
} 



void printHelpAndExit()
{
	printf(" Name\n");
	printf("     ITGRecv - Receiver component of D-ITG platform\n\n");
	printf(" Synopsis\n");
#ifdef LINUX_OS
	printf("     ./ITGRecv   [-i <interface>] [-Si <interface>]\n");
#endif
#ifdef WIN32
	printf("     ITGRecv.exe [-P]\n");
#endif
	printf("                 [-l [logfile]] [-Sp <port>]\n"
	       "                 [-L [a:<address>] [p:<port>] [P:<protocol>]]\n"
           "                 [-H <ITGSend_address> [-n <num_pkt>]]\n"
           "                 [-a <bind_address>]\n"
	       " Options\n");
#ifdef WIN32
	printf("     -P                       Enable thread high priority\n");
#endif
#ifdef LINUX_OS
	printf("     -i <interface>           Bind to a specific network interface\n");
	printf("     -Si <interface>          Bind to specified interface (for signaling channel)\n");
#endif
	printf("     -Sp <port>               Signaling port number\n");
	printf("                              (Default: %d)\n",
		DEFAULT_PORT_SIGNALING);
	printf("     -l [logfile]             Generate a log file\n");
	printf("                              (Default : %s)\n",
		DEFAULT_LOG_FILE);
	printf("     -L                       Enable remote logging to a running ITGLog server\n"
	       "        [a:<address>]         ITGLog server address\n"
	       "                              (Default: %s)\n", DEFAULT_LOG_IP);
	printf("        [p:<port>]            ITGLog server signaling port\n"
	       "                              (Default: %d)\n", DEFAULT_LOG_PORT_SIGNALING);
	printf("        [P:<protocol>]        ITGLog server data transport protocol\n");
	printf("                              (Default: %s)\n", invFindL4Proto(DEFAULT_PROTOCOL_TX_LOG));

	printf("     -H <ITGSend_address>]    Passive Mode\n");
	printf("     -n <num_pkt>             Number of outgoing packets\n");
	printf("     -a <bind_address>        Force address for the data flow\n");

	printf(" \n For more information consult the D-ITG manual\n");
	exit(1);
}



void recvInit()
{
#ifdef WIN32
	if (InitializeWinsock(MAKEWORD(1, 1)) != 1) {
		flagTerm = ERROR_TERMINATE;
		reportErrorAndExit("recvInit", "InitializeWinsock", "Cannot initialize WinSocket");
	}
#endif
	
	userId = USER_ID();
	
	if (MUTEX_THREAD_INIT(mutexLog) < 0)
		reportErrorAndExit("recvInit", "CreateMutex", "Cannot initialize Mutex");
	if ( MUTEX_THREAD_INIT(mutexLogRem) < 0)
		reportErrorAndExit("recvInit","CreateMutex","Cannot initialize Mutex for remote log");
	
	strcpy(logFile, DEFAULT_LOG_FILE);
	
	l4ProtoLog = DEFAULT_PROTOCOL_TX_LOG;
	
	if (getaddrinfo(DEFAULT_LOG_IP, NULL, NULL, &globaleLogHost) != 0)
		reportErrorAndExit("recvInit", "getaddrinfo",
		    "Cannot set the default IP log address");
	
	SET_PORT(globaleLogHost, htons(DEFAULT_LOG_PORT));
#ifdef WIN32
	
	LARGE_INTEGER temp;
	if (QueryPerformanceFrequency(&temp) == 0)
		reportErrorAndExit("recvInit", "QueryPerformaceFrequency",
		    "Cannot use system's high performace counter");
	if (QueryPerformanceCounter(&temp) == 0)
		reportErrorAndExit("recvInit", "QueryPerformaceCounter",
		    "Cannot use system's high performace counter");
	PRINTD(1,"High performance system counter PRESENT\n");
#endif
}



void createSignalingLogChannel(struct addrinfo logHost, char logFile[DIM_LOG_FILE],
    BYTE protocolLog, int &logSockSignaling)
{
	
	signaling signal;
	
	char buffer[1];

	
	logSockSignaling = socket(logHost.ai_family, SOCK_STREAM, 0);
	if (logSockSignaling < 0)
		reportErrorAndExit("createSignalingLogChannel", "socket",
		    "Cannot create socket logSockSignaling");
	
	if (logHost.ai_family == PF_INET)
		((struct sockaddr_in *) (logHost.ai_addr))->sin_port =
		    htons(defaultLogPortSignaling);
	else if (logHost.ai_family == PF_INET6)
		((struct sockaddr_in6 *) (logHost.ai_addr))->sin6_port =
		    htons(defaultLogPortSignaling);

	
	signal.protocol = protocolLog;
	
	strcpy(signal.logFile, logFile);
	
	if (connect(logSockSignaling, logHost.ai_addr, logHost.ai_addrlen) < 0)
		reportErrorAndExit("createSignalingLogChannel", "connect",
		    "Error into connect - Start first LogServer!");
	
	do {
		if (recv(logSockSignaling, (char *) &buffer, sizeof(BYTE), 0) < 0)
			reportErrorAndExit("createSignalingLogChannel", "recv",
			    "Cannot receive logSockSignaling1 data");
		
	} while (*(BYTE *) buffer != LOG_CONNECT);
	PRINTD(1,"Log Signal Manager : Received LOG_CONNECT Message\n");
	
	if (send(logSockSignaling, (char *) &signal, sizeof(signal), 0) < 0)
		reportErrorAndExit("createSignalingLogChannel", "send",
		    "Cannot send infos to LogServer");
	PRINTD(1,"Log Signal Manager : Sent to LogServer infos for creating signaling channel\n");
}


void createRemoteLogFile(struct addrinfo &logHost, char logFile[DIM_LOG_FILE],
    BYTE protocolLog, int &logSockSignaling, int &logSock)
{
	
	int sizeMsg = sizeof(int);
	
	char buffer[sizeMsg];

	

	createSignalingLogChannel(logHost, logFile, protocolLog, logSockSignaling);

	

	switch (protocolLog) {
	case L4_PROTO_UDP:
		
		logSock = socket(logHost.ai_family, SOCK_DGRAM, 0);
		if (logSock < 0)
			reportErrorAndExit("createRemoteLogFile",
			    "socket DATAGRAM", "Cannot create socket logSock");
		break;
	case L4_PROTO_TCP:
		
		logSock = socket(logHost.ai_family, SOCK_STREAM, 0);
		if (logSock < 0)
			reportErrorAndExit("createRemoteLogFile",
			    "socket STREAM", "Cannot create socket logSock");
		break;
	default:
		break;
	}
	
	if (recv(logSockSignaling, (char *) buffer, sizeof(buffer), 0) < 0)
		reportErrorAndExit("createRemoteLogFile", "recv",
		    "Cannot receive logSockSignaling2 data");
	unsigned int port = ntohl(*(unsigned int *) buffer); 
	
	
	PRINTD(1,"Port received for log : %d\n",port);
	
	
	if (logHost.ai_family == PF_INET){
		((struct sockaddr_in*)(logHost.ai_addr))->sin_port=htons(port);
	} else if (logHost.ai_family == PF_INET6)
		((struct sockaddr_in6*)(logHost.ai_addr))->sin6_port=htons(port);

	
	if (connect(logSock, logHost.ai_addr, logHost.ai_addrlen) < 0)
		reportErrorAndExit("createRemoteLogFile", "connect",
		    "Error into connect with logsender!");
}



void recvFlowLog(int newSockSignaling, struct addrinfo &logHost, BYTE & protocolLog, char logFile[])
{
	

	
	int sizeMsg =
	    2 * sizeof(int) + sizeof(in_addr_t) + sizeof(in_port_t) + sizeof(BYTE) + DIM_LOG_FILE;
	
	char buffer[sizeMsg];
	
	char *ptrIPVer = buffer;
	
	char *ptrIP = ptrIPVer + sizeof(int);
	
	char *ptrPort = ptrIP + sizeof(in_addr_t);
	
	char *logProtocol = ptrPort + sizeof(in_port_t);
	
	char *fileName = logProtocol + sizeof(BYTE);


	
	

	
	if (recv(newSockSignaling, (char *) buffer, sizeof(buffer), 0) < 0)
		reportErrorAndExit("recvFlowLog", "recv - type = 12",
		    "Cannot receive data on newSockSignaling");

	
	*((int *) ptrIPVer) = ntohl(*((int *) ptrIPVer));
	logHost.ai_family = *((int *) ptrIPVer);
	
	if (*((int *) ptrIPVer) == PF_INET)
		ptrPort = ptrIP + sizeof(in_addr_t);
	else if (*((int *) ptrIPVer) == PF_INET6)
		ptrPort = ptrIP + 4 * sizeof(in_addr_t);
	
	logProtocol = ptrPort + sizeof(in_port_t);
	
	fileName = logProtocol + sizeof(BYTE);
	
	protocolLog = *(BYTE *) logProtocol;

	
	if (logHost.ai_family == PF_INET) {
		
		logHost.ai_addrlen = sizeof(struct sockaddr_in);
		
		logHost.ai_addr = (struct sockaddr *) malloc(sizeof(struct sockaddr_in));
		((struct sockaddr_in *) logHost.ai_addr)->sin_family = AF_INET;
		((struct sockaddr_in *) logHost.ai_addr)->sin_addr.s_addr = *(in_addr_t *) ptrIP;
		
		((struct sockaddr_in *) logHost.ai_addr)->sin_port = *(in_port_t *) ptrPort;
		
	} else if (logHost.ai_family == PF_INET6) {
		
		logHost.ai_addrlen = sizeof(struct sockaddr_in6);
		
		logHost.ai_addr = (struct sockaddr *) malloc(sizeof(struct sockaddr_in6));
		((struct sockaddr_in6 *) logHost.ai_addr)->sin6_family = AF_INET6;
		memcpy(((struct sockaddr_in6 *) logHost.ai_addr)->sin6_addr.
		    s6_addr32, ptrIP, 4 * sizeof(in_addr_t));
		
		((struct sockaddr_in6 *) logHost.ai_addr)->sin6_port = *(in_port_t *) ptrPort;
	}
	
	strcpy(logFile, fileName);
}



void recvNameLog(char nameFile[DIM_LOG_FILE], int newSockSignaling)
{
	

	
	int sizeMsg = DIM_LOG_FILE;
	
	char buffer[sizeMsg];

	int size_r = 0;

	
	if ((size_r = recv(newSockSignaling, (char *) buffer, sizeof(buffer), 0)) < 0)
		reportErrorAndExit("sendName", "recv - type = 12",
		    "Cannot receive data on newSockSignaling");
	strncpy(nameFile, buffer, size_r);
}


#ifdef LINUX_OS
void pipeParser(int newSockSignaling, int &numFlow, int rPipe[],
    memChannel flowIdNum[], paramThread paraThread[], pthread_t hThr[])
#endif
#ifdef WIN32
void pipeParser(int newSockSignaling, int &numFlow, HANDLE rPipe[],
    memChannel flowIdNum[], paramThread paraThread[], pthread_t hThr[])
#endif
{
	struct pipeMsg msg;
	uint16_t port = 0;
	
	if (recvPipeMsg(rPipe, &msg) < 0) {
		reportErrorAndExit("pipeParser", "", "Cannot receive message into pipe");
	}
	switch (msg.code) {
	case MSG_FT_ERR1:
		PRINTD(1,"Error into bind - flow %d \n", msg.flowId);
		
		if (sendAckFlow(newSockSignaling, TSP_ERR_MSG_2, msg.flowId) < 0)
			reportErrorAndExit("pipeParser", "",
			    "Cannot send TSP_ERR_MSG_2 message to sender");
		int k;
		k = 0;
		
		while ((msg.flowId != flowIdNum[k].flowId) && (k < MAX_NUM_THREAD)) {
			k++;
		}

		if (k < MAX_NUM_THREAD) {
			
			if (terminateThread(hThr[k]) < 0)
				reportErrorAndExit("pipeParser", "", "Cannot terminate thread");

			
			free(paraThread[k].addressInfos);
			paraThread[k].addressInfos = NULL;
			
			flowIdNum[k].flowId = -1;
			
			numFlow--;
		}
		break;
	case MSG_FT_ERR2:
		PRINTD(1,"Error: you must be root to generate ICMP traffic - %d \n", msg.flowId);
		
		if (sendAckFlow(newSockSignaling, TSP_ERR_MSG_3, msg.flowId) < 0)
			reportErrorAndExit("pipeParser", "",
			    "Cannot send TSP_ERR_MSG_3 message to sender");
		int j;
		j = 0;
		
		while ((msg.flowId != flowIdNum[j].flowId)  && (j < MAX_NUM_THREAD)) {
			j++;
		}

		if (j < MAX_NUM_THREAD){
			
			if (terminateThread(hThr[j]) < 0)
				reportErrorAndExit("pipeParser", "", "Cannot terminate thread");

			
			free(paraThread[j].addressInfos);
			paraThread[j].addressInfos = NULL;
			
			flowIdNum[j].flowId = -1;
			
			numFlow--;
		}
		break;
	
	case MSG_FT_ERR_SOCK:

		int t;
		t = 0;
		
		while ((msg.flowId != flowIdNum[t].flowId) && (t < MAX_NUM_THREAD)) {
			t++;
		}

		
		if (t < MAX_NUM_THREAD){
			PRINTD(1,"Error on receiving data from flow socket - flow %d \n", msg.flowId);
			
			if (sendAckFlow(newSockSignaling, TSP_ERR_MSG_5, msg.flowId) < 0)
				reportErrorAndExit("pipeParser", "",
						"Cannot send TSP_ERR_MSG_5 message to sender");

			
			if (terminateThread(hThr[t]) < 0)
				reportErrorAndExit("pipeParser", "", "Cannot terminate thread");

			
			free(paraThread[t].addressInfos);
			paraThread[t].addressInfos = NULL;
			
			flowIdNum[t].flowId = -1;
			
			numFlow--;
		}
		break;
	
	case MSG_FT_OK:
		PRINTD(1,"signal manager received MSG_FT_OK - %d \n", msg.flowId);

		t = 0;
		port = 0;
		
		while ((msg.flowId != flowIdNum[t].flowId) && (t < MAX_NUM_THREAD)) {
			t++;
		}

		
		if (t < MAX_NUM_THREAD){

			GET_PORT((&(paraThread[t].destHost)),port);

			port = ntohs(port);

			
			if (sendAckFlow(newSockSignaling, TSP_ACK_SEND_FLOW, msg.flowId,port) == -1)
				reportErrorAndExit("pipeParser", "", "Cannot send TSP_ACK_SEND_FLOW message to sender");

			PRINTD(1,"TCP Pkt Receiver : Sent TSP_ACK_SEND_FLOW(5) message with port number = %u\n",port);
		}
		break;
	default:
		printf("undefined message received from signal manager\n");
		fflush(stdout);
		exit(-1);
		break;
	}
}



#ifdef WIN32
int typeParser(BYTE type, int &numFlow, int newSockSignaling,
    memChannel flowIdNum[], paramThread paraThread[], pthread_t hThr[],
    HANDLE rPipe[], char *fileLog, int logSock, int logSockSignaling, struct addrinfo *logHost)
#endif
#ifdef LINUX_OS
int typeParser(BYTE type, int &numFlow, int newSockSignaling,
    memChannel flowIdNum[], paramThread paraThread[], pthread_t hThr[],
    int rPipe[], char *fileLog, int logSock, int logSockSignaling, struct addrinfo *logHost)
#endif
{
	if (type == TSP_SEND_FLOW) {
		PRINTD(1,"Signal Manager : Received TSP_SEND_FLOW(3) message\n");
		
		int flowPosition;
		
		for (flowPosition = 0; flowPosition < MAX_NUM_THREAD; flowPosition++) {
			if (flowIdNum[flowPosition].flowId == -1) {
				break;
			}
		}
		PRINTD(1,"flowPosition for TSP_SEND_FLOW = %d\n", flowPosition);
		
		if (flowPosition < MAX_NUM_THREAD) {
			
			int sizeMsg6 = 4 * sizeof(in_addr_t);
			int sizeMsg4 = sizeof(in_addr_t);
			int sizeMsg = 3;

			
			char buffer[sizeMsg + sizeMsg6];

			
			BYTE *ptrProtocol = (BYTE *) buffer;

			
			BYTE *ptrUpProtocol = ptrProtocol + sizeof(BYTE);

			
			BYTE *ptrMeter = ptrUpProtocol + sizeof(BYTE);

			
			int ptrIPVer;

			
			int ptrFlowId;


			
			int ptrPLType;

			
			char ptrSerialReceiver[DIM_NAME_SERIAL_INTERFACE];

			
			in_port_t ptrPort;

			
			in_addr_t *ptrIP = (in_addr_t *)malloc(4 * sizeof(in_addr_t));

			
			uint32_t ptrScope;

			uint16_t portForPssv;			
#ifdef DEBUG
			char trash[INET6_ADDRSTRLEN+1];	
#endif
			BYTE srcAddrSpecify=0x00;		


			
			
			if (recv(newSockSignaling, buffer, sizeMsg, 0) < 0)
				reportErrorAndExit("typeParser", "recv - type = TSP_SEND_FLOW(3",
				    "Cannot receive data on newSockSignaling");
			
			if (recv(newSockSignaling,(char *)&ptrIPVer, sizeof(int), 0) < 0)
				reportErrorAndExit("typeParser","recv - type = TSP_SEND_FLOW(3",
					"Cannot receive data on newSockSignaling");
			
			if (recv(newSockSignaling,(char *)&ptrFlowId, sizeof(int), 0) < 0)
				reportErrorAndExit("typeParser","recv - type = TSP_SEND_FLOW(3",
					"Cannot receive data on newSockSignaling");
			
			if (recv(newSockSignaling,(char *)&ptrPLType, sizeof(int), 0) < 0)
				reportErrorAndExit("typeParser","recv - type = TSP_SEND_FLOW(3",
					"Cannot receive data on newSockSignaling");
			
			if (recv(newSockSignaling,ptrSerialReceiver, DIM_NAME_SERIAL_INTERFACE, 0) < 0)
				reportErrorAndExit("typeParser","recv - type = TSP_SEND_FLOW(3",
					"Cannot receive data on newSockSignaling");

			
			
			
			

			if(passiveMode == true){
				if (recv(newSockSignaling,(char *)&portForPssv, sizeof(portForPssv), 0) < 0)
					reportErrorAndExit("typeParser","recv - type = TSP_SEND_FLOW(3",
							"Cannot receive data on newSockSignaling");

				paraThread[flowPosition].portForPssv=ntohs(portForPssv);

				if (recv(newSockSignaling,(char *)&srcAddrSpecify, sizeof(srcAddrSpecify), 0) < 0)
									reportErrorAndExit("typeParser","recv - type = TSP_SEND_FLOW(3",
											"Cannot receive data on newSockSignaling");

				PRINTD(1,"Signal Manager : srcAddrSpecify : %d\n",srcAddrSpecify);
			}
			

			
			if (recv(newSockSignaling,(char *)&ptrPort, sizeof(in_port_t), 0) < 0)
				reportErrorAndExit("typeParser","recv - type = TSP_SEND_FLOW(3",
					"Cannot receive data on newSockSignaling");
			
			ptrIPVer = ntohl(ptrIPVer);
            ptrFlowId = ntohl(ptrFlowId);
			

#ifdef DEBUG
			printf("ptrIPVer: %d\n", ptrIPVer);
			for (int pp = 0;pp < sizeMsg; pp++) {
				printf("%02X ", buffer[pp]);
			}
			printf("\n");
#endif
			if (ptrIPVer == PF_INET) {
				
				if (recv(newSockSignaling, (char *)ptrIP, sizeMsg4, 0) < 0)
					reportErrorAndExit("typeParser", "recv - type = TSP_SEND_FLOW(3",
					    "Cannot receive IPv4 address on newSockSignaling");
			} else if (ptrIPVer == PF_INET6) {
				
				if (recv(newSockSignaling, (char *)ptrIP, sizeMsg6, 0) < 0) {
					reportErrorAndExit("typeParser", "recv - type = TSP_SEND_FLOW(3",
					    "Cannot receive IPv6 address on newSockSignaling");
				}
				
				if(recv(newSockSignaling, (char *)&ptrScope, sizeof(uint32_t), 0) < 0) {
					reportErrorAndExit("typeParser", "recv - type = TSP_SEND_FLOW(3",
					"Cannot receive IPv6 address scope on newSockSignaling");
				}
#ifndef NOIPV6
				if (IN6_IS_ADDR_LINKLOCAL(ptrIP) && ptrScope == 0)
					printf("** WARNING **: IPv6 address scope id not specified by the sender\n");
#endif

			}
			PRINTD(1,"Buffer with TYPE = TSP_SEND_FLOW(3) received\n");

			paraThread[flowPosition].destHost.ai_family = ptrIPVer;

			
			
			if (!((srcAddrSpecify == 0x00) && (passiveMode == true))){

				
				if (paraThread[flowPosition].destHost.ai_family == PF_INET) {
					
					paraThread[flowPosition].destHost.
						ai_addrlen = sizeof(struct sockaddr_in);
					
					paraThread[flowPosition].destHost.ai_addr =
						(struct sockaddr *) malloc(sizeof(struct sockaddr_in));
					
					((struct sockaddr_in *)
						paraThread[flowPosition].destHost.
						ai_addr)->sin_family = AF_INET;
					
					((struct sockaddr_in *)
						paraThread[flowPosition].destHost.
						ai_addr)->sin_addr.s_addr = *ptrIP;
					
					((struct sockaddr_in *)
						paraThread[flowPosition].destHost.
						ai_addr)->sin_port = ptrPort;
				
				} else if (paraThread[flowPosition].destHost.ai_family == PF_INET6) {
					
					paraThread[flowPosition].destHost.ai_addrlen = sizeof(struct sockaddr_in6);
					
					paraThread[flowPosition].destHost.ai_addr =
						(struct sockaddr *) malloc(sizeof(struct sockaddr_in6));
					
					((struct sockaddr_in6 *)
						paraThread[flowPosition].destHost.ai_addr)->sin6_family = AF_INET6;
					
					memcpy(((struct sockaddr_in6 *)
						paraThread[flowPosition].destHost.ai_addr)->sin6_addr.s6_addr32,
						ptrIP, 4 * sizeof(in_addr_t));
					((struct sockaddr_in6 *)paraThread[flowPosition].destHost.ai_addr)->sin6_flowinfo = 0;
					
					((struct sockaddr_in6 *)paraThread[flowPosition].destHost.ai_addr)->sin6_scope_id = ptrScope;

					
					((struct sockaddr_in6 *)
						paraThread[flowPosition].destHost.
						ai_addr)->sin6_port = ptrPort;
				}

			}else{
				
				

#ifdef DEBUG
				getnameinfo(passiveSender->ai_addr,passiveSender->ai_addrlen,trash,passiveSender->ai_addrlen,NULL,0,NI_NUMERICHOST);
				PRINTD(1,"Signal Manager : take %s as Sender address\n",trash);
#endif

				paraThread[flowPosition].destHost.ai_family = passiveSender->ai_family;
				paraThread[flowPosition].destHost.ai_addrlen= passiveSender->ai_addrlen;

				if (passiveSender->ai_family == PF_INET){
					
					paraThread[flowPosition].destHost.ai_addr =	(struct sockaddr *) malloc(sizeof(struct sockaddr_in));
					memcpy(paraThread[flowPosition].destHost.ai_addr,passiveSender->ai_addr,passiveSender->ai_addrlen);
					((struct sockaddr_in *)	paraThread[flowPosition].destHost.ai_addr)->sin_port = ptrPort;

				}else if (passiveSender->ai_family == PF_INET6){
					
					paraThread[flowPosition].destHost.ai_addr =	(struct sockaddr *) malloc(sizeof(struct sockaddr_in6));
					memcpy(((struct sockaddr_in6 *)paraThread[flowPosition].destHost.ai_addr)->sin6_addr.s6_addr32,
						   ((struct sockaddr_in6 *)passiveSender->ai_addr)->sin6_addr.s6_addr32, 4 * sizeof(in_addr_t));
					((struct sockaddr_in6 *)paraThread[flowPosition].destHost.ai_addr)->sin6_family = AF_INET6;
					((struct sockaddr_in6 *)paraThread[flowPosition].destHost.ai_addr)->sin6_flowinfo = 0;
					((struct sockaddr_in6 *)paraThread[flowPosition].destHost.ai_addr)->sin6_scope_id =
						((struct sockaddr_in6 *)passiveSender->ai_addr)->sin6_scope_id;
					((struct sockaddr_in6 *)paraThread[flowPosition].destHost.ai_addr)->sin6_port = ptrPort;

				}
			}
#if DEBUG > 2
			memset(trash,0,INET6_ADDRSTRLEN+1);
			getnameinfo(paraThread[flowPosition].destHost.ai_addr,paraThread[flowPosition].destHost.ai_addrlen,trash,paraThread[flowPosition].destHost.ai_addrlen,NULL,0,NI_NUMERICHOST);
			PRINTD(1,"Signal Manager : IP used %s\n",trash);
#endif




			paraThread[flowPosition].rPipe[0] = rPipe[0];
			paraThread[flowPosition].rPipe[1] = rPipe[1];

			
			flowIdNum[flowPosition].flowId = ptrFlowId;

			
			flowIdNum[flowPosition].l4Proto = *ptrProtocol;
			PRINTD(1,"Signal Manager : Level 4 Protocol Received : %s\n",
				l4Protocols[(unsigned int)(flowIdNum[flowPosition].l4Proto)]);

			

			
			paraThread[flowPosition].l7Proto = *ptrUpProtocol;
			PRINTD(1,"Signal Manager : Level 7 Protocol Received : %s\n",
				l7Protocols[(unsigned int)(paraThread[flowPosition].l7Proto)]);

			
			paraThread[flowPosition].flowId = ptrFlowId;
			
			paraThread[flowPosition].meter = *ptrMeter;

			
			paraThread[flowPosition].payloadLogType = ptrPLType;

			if (paraThread[flowPosition].payloadLogType == PL_STANDARD)
				paraThread[flowPosition].preambleSize = StandardMinPayloadSize;
			else if (paraThread[flowPosition].payloadLogType == PL_SHORT)
				paraThread[flowPosition].preambleSize = ShortMinPayloadSize;
			else
				paraThread[flowPosition].preambleSize = NoneMinPayloadSize;

			if (flowIdNum[flowPosition].l4Proto != L4_PROTO_UDP)
				paraThread[flowPosition].preambleSize = paraThread[flowPosition].preambleSize + sizeof(int);
			PRINTD(1,"Protocol Type %d \n", flowIdNum[flowPosition].l4Proto);
			PRINTD(1,"Payload Type %d; PayloadSize %d \n", paraThread[flowPosition].payloadLogType,
				paraThread[flowPosition].preambleSize);

			
			strncpy(paraThread[flowPosition].serial, ptrSerialReceiver, DIM_NAME_SERIAL_INTERFACE);
			PRINTD(1,"Signal Manager : SERIAL: %s\n", ptrSerialReceiver);

#ifdef DEBUG
			getnameinfo(paraThread[flowPosition].destHost.ai_addr,paraThread[flowPosition].destHost.ai_addrlen,trash,paraThread[flowPosition].destHost.ai_addrlen,NULL,0,NI_NUMERICHOST);
#endif
			if(passiveMode==false){			
				PRINTD(1,"Signal Manager : PORT: %hu\n", ntohs(ptrPort));
				PRINTD(1,"Signal Manager : IP: %s%%%d\n",trash,	ptrIPVer == PF_INET6 ? ptrScope : 0);
			
			}else{
				PRINTD(1,"Signal Manager : Receiver PORT: %hu\n", paraThread[flowPosition].portForPssv);
				PRINTD(1,"Signal Manager : Sender PORT: %hu\n", ntohs(ptrPort));
				PRINTD(1,"Signal Manager : Sender IP: %s%%%d\n",trash,ptrIPVer == PF_INET6 ? ptrScope : 0);
			}
			

			
			paraThread[flowPosition].fileLog = fileLog;

			
			paraThread[flowPosition].logSock = logSock;

			
			paraThread[flowPosition].logHost = logHost;
			PRINTD(1,"Signal Manager : Meter Received : %s\n",
				meters[(unsigned int)(paraThread[flowPosition].meter)]);

			
			if((passiveMode == true)){
				
				if (sendAckFlow(newSockSignaling, TSP_ACK_SEND_FLOW, flowPosition+1) == -1)
					reportErrorAndExit("typeParser", "",
							"Cannot send TSP_ACK_SEND_FLOW message to sender");

				PRINTD(1,"Signal Manager : Sent TSP_ACK_SEND_FLOW(5) message\n");
				return 0;
			 }
			

			
			
			
			if (globalBindAddr != NULL){

				
				GET_PORT((&(paraThread[flowPosition].destHost)),portForPssv);
				
				free(paraThread[flowPosition].destHost.ai_addr);
#ifdef DEBUG
				getnameinfo(globalBindAddr->ai_addr,globalBindAddr->ai_addrlen,trash,INET6_ADDRSTRLEN,NULL,0,NI_NUMERICHOST);
				PRINTD(1,"Signal Manager : Forced address for data channel : %s\n",trash);
#endif

				paraThread[flowPosition].destHost.ai_family = globalBindAddr->ai_family;
				paraThread[flowPosition].destHost.ai_addrlen= globalBindAddr->ai_addrlen;

				if (globalBindAddr->ai_family == PF_INET){
					
					paraThread[flowPosition].destHost.ai_addr =     (struct sockaddr *) malloc(sizeof(struct sockaddr_in));
					memcpy(paraThread[flowPosition].destHost.ai_addr,globalBindAddr->ai_addr,globalBindAddr->ai_addrlen);

				}else if (globalBindAddr->ai_family == PF_INET6){
					
					paraThread[flowPosition].destHost.ai_addr =     (struct sockaddr *) malloc(sizeof(struct sockaddr_in6));
					memcpy(((struct sockaddr_in6 *)paraThread[flowPosition].destHost.ai_addr)->sin6_addr.s6_addr32,
							((struct sockaddr_in6 *)globalBindAddr->ai_addr)->sin6_addr.s6_addr32, 4 * sizeof(in_addr_t));
					((struct sockaddr_in6 *)paraThread[flowPosition].destHost.ai_addr)->sin6_family = AF_INET6;
					((struct sockaddr_in6 *)paraThread[flowPosition].destHost.ai_addr)->sin6_flowinfo = 0;
					((struct sockaddr_in6 *)paraThread[flowPosition].destHost.ai_addr)->sin6_scope_id =
							((struct sockaddr_in6 *)globalBindAddr->ai_addr)->sin6_scope_id;
				}
				SET_PORT((&(paraThread[flowPosition].destHost)),portForPssv);
			}
			


			

			switch (flowIdNum[flowPosition].l4Proto) {
			case L4_PROTO_UDP:
				if (CREATE_THREAD(&paraThread[flowPosition],
					udpSock, NULL, hThr[flowPosition]) < 0)
					reportErrorAndExit("typeParser",
					    "CREATE_THREAD - type = TSP_SEND_FLOW(3",
					    "Cannot create thread for udpSock");
				PRINTD(1,"Return value CREATE_THREAD udpSock hThr[flowPosition] : %d \n",
				    (int) hThr[flowPosition]);
				break;
			case L4_PROTO_TCP:
				if (CREATE_THREAD(&paraThread[flowPosition],
					tcpSock, NULL, hThr[flowPosition]) < 0)
					reportErrorAndExit("typeParser",
					    "CREATE_THREAD - type = TSP_SEND_FLOW(3",
					    "Cannot create thread for tcpSock");
				PRINTD(1,"Return value CREATE_THREAD tcpSock hThr[flowPosition] : %d \n",
				    (int) hThr[flowPosition]);
				break;
#ifdef SCTP
			case L4_PROTO_SCTP:
				if (CREATE_THREAD(&paraThread[flowPosition],
					sctpSock, NULL, hThr[flowPosition]) < 0)
					reportErrorAndExit("typeParser",
					    "CREATE_THREAD - type = TSP_SEND_FLOW(3",
					    "Cannot create thread for sctpSock");
				PRINTD(1,"Return value CREATE_THREAD sctpSock hThr[flowPosition] : %d \n",
				    (int) hThr[flowPosition]);
 				break;
#endif
#ifdef DCCP
			case L4_PROTO_DCCP :
				if ( CREATE_THREAD(&paraThread[flowPosition], dccpSock, NULL, hThr[flowPosition]) < 0)
					reportErrorAndExit("typeParser",
						"CREATE_THREAD - type = TSP_SEND_FLOW(3",
						"Cannot create thread for dccpSock");
				PRINTD(1,"Return value CREATE_THREAD dccpSock hThr[flowPosition] : %d \n",(int)hThr[flowPosition]);
				break;

#endif
			case L4_PROTO_ICMP:
				if (CREATE_THREAD(&paraThread[flowPosition],
					icmpSock, NULL, hThr[flowPosition]) < 0)
					reportErrorAndExit("typeParser",
					    "CREATE_THREAD - type = TSP_SEND_FLOW(3",
					    "Cannot create thread for icmpSock");
				PRINTD(1,"Return value CREATE_THREAD icmpSock hThr[flowPosition] : %d \n",
				    (int) hThr[flowPosition]);
				break;
			default:
				break;
			}
#ifdef WIN32
			if (setPriority)
				
				if (SetThreadPriority(hThr[flowPosition], THREAD_PRIORITY_TIME_CRITICAL) == 0) {
					printf ("Error - Impossible set priority for thread - %d \n", (int) GetLastError());
					fflush(stdout);
				}
#endif

			
			numFlow++;
		} else
			reportErrorAndExit("typeParser", "flowPosition", "Too many thread");
		
	} else if (type == TSP_CLOSED_FLOW) {

		PRINTD(1,"Signal Manager : Received TSP_CLOSED_FLOW(4) message\n");
		

		
		int sizeMsg = sizeof(int);

		
		char buffer[sizeMsg];

		char *ptrFlowId = buffer;
		
		if (recv(newSockSignaling, (char *) buffer, sizeof(buffer), 0) <= 0)
			reportErrorAndExit("typeParser",
			    "recv - type = TSP_CLOSED_FLOW(4)", "Cannot receive data");
		PRINTD(1,"Buffer with TYPE = TSP_CLOSED_FLOW(4) received\n");
		*(unsigned int *) ptrFlowId = ntohl(*(unsigned int *) ptrFlowId);  

		int k = 0;
		
		while ((*(int *) ptrFlowId != flowIdNum[k].flowId) && (k < MAX_NUM_THREAD)) {
			k++;
		}

		if (k < MAX_NUM_THREAD){

#ifdef DEBUG									
			if ((logCheck!=0) || (logRemote!=0))	
				sleep(3);							
#endif											

			
			if ((logCheck == 1) || (logCheck == 2))
				
				flushBuffer((ofstream *) paraThread[k].fileLog,
						paraThread[k].addressInfos, paraThread[k].count);
			
			else if ((logRemote == 1) || (logRemote == 2)) {
				int size = 0;
				MUTEX_THREAD_LOCK(mutexLogRem);
				if ((size =
						sendto(logSock,
								(char *) paraThread[k].addressInfos,
								paraThread[k].count *
								sizeof(struct info), 0,
								logHost->ai_addr, logHost->ai_addrlen)) < 0)
					reportErrorAndExit("typeParser",
							"sent infos to LOG_SERVER", "Cannot send infos to logServer");
				MUTEX_THREAD_UNLOCK(mutexLogRem);
				PRINTD(1,"size send  %d \n", size);
				PRINTD(1,"TCP Pkt Receiver : Sent infos to LogServer\n");
			}
			
			if (logCheck == 2)
				closeFileLog((ofstream *) paraThread[k].fileLog);


			if (flowIdNum[k].l4Proto == L4_PROTO_ICMP) {
				PRINTD(1,"Finish ICMP packets!\n");
			}
			else {
				if (flowIdNum[k].l4Proto == L4_PROTO_TCP)
					PRINTD(1,"Finish TCP packets!\n");
#ifdef SCTP
				if (flowIdNum[k].l4Proto == L4_PROTO_SCTP)
					PRINTD(1,"Finish SCTP packets!\n");
#endif
				if (flowIdNum[k].l4Proto == L4_PROTO_UDP)
					PRINTD(1,"Finish UDP packets!\n");

				
				in_port_t tmpPort = 0;
				if (passiveMode==false){											
					
					GET_PORT((&(paraThread[k].destHost)), tmpPort);
					printf("Finish on port: %d\n", ntohs(tmpPort));
				}else{																
					printf("Finish on port: %d\n", paraThread[k].portForPssv);		
				}																	
				fflush(stdout);
#ifdef SCTP
				if (flowIdNum[k].l4Proto == L4_PROTO_SCTP) {
					int sctpId;

					MUTEX_THREAD_LOCK(mutexSctp);
					
					for (sctpId = 0; sctpId < sctpSessionCount; sctpId++) {
						if (sctpSessions[sctpId].parsedStreams > 0 && sctpSessions[sctpId].port == tmpPort)
							break;
					}
					sctpSessions[sctpId].parsedStreams = 0;
					sctpSessions[sctpId].sock = 0;
					sctpSessionCount--;
					MUTEX_THREAD_UNLOCK(mutexSctp);
				}
#endif
			}

			sleep(1);


			
			if (terminateThread(hThr[k]) < 0)
				reportErrorAndExit("typeParser",
						"terminateThread - type = 4", "Cannot terminate thread hThr[k]");
			
			if (closeSock(paraThread[k].socketClose) < 0)
				reportErrorAndExit("typeParser",
						"closeSock - type = 4", "Cannot close socket socketClose");

			
			free(paraThread[k].addressInfos);
			paraThread[k].addressInfos = NULL;
			
			flowIdNum[k].flowId = -1;
			
			numFlow--;

			
			if (sendAckFlow(newSockSignaling, TSP_ACK_CLOSED_FLOW, *(int *) ptrFlowId) < 0)
				reportErrorAndExit("typeParser",
						" - type = TSP_ACK_CLOSED_FLOW(6)",
						"Cannot send ack closed flow id on newSockSignaling");
			PRINTD(1,"Signal Manager : Sent TSP_ACK_CLOSED_FLOW(6) message\n");
			PRINTD(1,"Closed Flow %d \n", *ptrFlowId);
		}
		
	} else if (type == TSP_CLOSED_ERR) {

		PRINTD(1,"Signal Manager : Received TSP_CLOSED_ERR(17) message\n");
		

		
		int sizeMsg = sizeof(int);

		
		char buffer[sizeMsg];

		char *ptrFlowId = buffer;

		
		if (recv(newSockSignaling, (char *) buffer, sizeof(buffer), 0) <= 0)
			reportErrorAndExit("typeParser",
			    "recv - type = TSP_CLOSED_FLOW(17)", "Cannot receive data");
		PRINTD(1,"Buffer with TYPE = TSP_CLOSED_ERR(17) received\n");
		
		*(int *) ptrFlowId = ntohl(*(int *) ptrFlowId);  
		int k = 0;
		
		
		while ((*(int *) ptrFlowId != flowIdNum[k].flowId) && (k < MAX_NUM_THREAD)) {
			k++;
		}

		if (k < MAX_NUM_THREAD){

			
			if (terminateThread(hThr[k]) < 0)
				reportErrorAndExit("typeParser",
						"terminateThread - type = 4", "Cannot terminate thread hThr[k]");
			
			if (closeSock(paraThread[k].socketClose) < 0)
				reportErrorAndExit("typeParser",
						"closeSock - type = 4", "Cannot close socket socketClose");

			
			if (logCheck == 2)
				closeFileLog((ofstream *) paraThread[k].fileLog);


			
			free(paraThread[k].addressInfos);
			paraThread[k].addressInfos = NULL;
			
			flowIdNum[k].flowId = -1;
			
			numFlow--;
			PRINTD(1,"Closed Flow %d \n", *ptrFlowId);
		}
		
	} else if (type == TSP_RELEASE) {
		PRINTD(1,"Signal Manager : Received TSP_RELEASE(11) message\n");
		return -1;

		
	} else if (type == TSP_ERR_MSG_1) {
		PRINTD(1,"Signal Manager : Received TSP_ERR_MSG_1(14) message : authentication not successful\n");
		return -1;
		
	} else if (type == TSP_SENDER_DOWN) {
		PRINTD(1,"Signal Manager : Received TSP_SENDER_DOWN(21) message\n");
		return -1;
	}
	
	else if (type == TSP_ACK_SEND_FLOW){
		
		int sizeMsg = sizeof(int);

		
		char buffer[sizeMsg];

		char *ptrFlowId = buffer;
		
		if (recv(newSockSignaling, (char *) buffer, sizeof(buffer), 0) <= 0)
			reportErrorAndExit("typeParser",
				"recv - type = TSP_ACK_SEND_FLOW(5)", "Cannot receive data");
		PRINTD(1,"Buffer with TYPE = TSP_ACK_SEND_FLOW(5) received\n");
		*(int *) ptrFlowId = ntohl(*(int *) ptrFlowId);  
		int k = 0;
		
		while ((*(int *) ptrFlowId != flowIdNum[k].flowId) && (k < MAX_NUM_THREAD)) {
			k++;
		}

		if (k < MAX_NUM_THREAD) {
			switch (flowIdNum[k].l4Proto) {
			case L4_PROTO_TCP:
				PRINTD(1,"Signal Manager : Received TSP_ACK_SEND_FLOW(5) message\n");
				if (CREATE_THREAD(&paraThread[k],tcpSock, NULL, hThr[k]) < 0)
					reportErrorAndExit("typeParser","CREATE_THREAD - type = TSP_SEND_FLOW(3","Cannot create thread for tcpSock");
				PRINTD(1,"Return value CREATE_THREAD tcpSock hThr[k] : %d \n",(int) hThr[k]);
				break;

			case L4_PROTO_UDP:
				PRINTD(1,"Signal Manager : Received TSP_ACK_SEND_FLOW(5) message\n");
				if (CREATE_THREAD(&paraThread[k],udpSock, NULL, hThr[k]) < 0)
					reportErrorAndExit("typeParser","CREATE_THREAD - type = TSP_SEND_FLOW(3","Cannot create thread for udpSock");
				PRINTD(1,"Return value CREATE_THREAD udpSock hThr[k] : %d \n",(int) hThr[k]);
				break;
			}
		}
	}
	
	return 0;
}


void copia(const struct addrinfo *src, struct addrinfo &dst)
{
	
	if (src->ai_family == PF_INET) {
		
		dst.ai_family = src->ai_family;
		dst.ai_addrlen = sizeof(struct sockaddr_in);
		
		dst.ai_addr = (struct sockaddr *) malloc(sizeof(struct sockaddr_in));
		((struct sockaddr_in *) dst.ai_addr)->sin_family = AF_INET;
		((struct sockaddr_in *) dst.ai_addr)->sin_addr.s_addr =
		    ((struct sockaddr_in *) src->ai_addr)->sin_addr.s_addr;
		
		((struct sockaddr_in *) dst.ai_addr)->sin_port =
		    ((struct sockaddr_in *) src->ai_addr)->sin_port;
	}

}



#define NEXT_OPT argc--;argv++
void parserRecv(int argc, char *argv[])
{
	int tmp=0;

	
	while (argc > 0) {
		
		if (argv[0][0] != '-') {
			printf("Unknow option : %s\n\n", argv[0]);
			printHelpAndExit();
		}

		
		switch (argv[0][1]) {
		case 'h': 
			printHelpAndExit();
			break;
#ifdef LINUX_OS
		case 'i': 
			if (argc < 2) {
				printf("Missing parameter : option %s\n\n", argv[0]);
				printHelpAndExit();
			}
			ifaceBind = (char*) malloc(strlen(argv[1]) + 1);
			strcpy(ifaceBind, argv[1]);
			NEXT_OPT;
			break;
#endif
		case 'l': 
			logCheck = 1;

			
			if (argc > 1 && argv[1][0] != '-') {
				
				strcpy(logFile, argv[1]);
				NEXT_OPT;
			} else {
				
				strcpy(logFile, DEFAULT_LOG_FILE);
			}
			break;
		case 'L': 
			printf("Option '-L' is disabled in this version, part of the BSense project\n");
			exit(-1);
#ifdef WIN32
		case 'P': 
			if (SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS) == 0) {
				printf ("Error - Impossible set priority class - %d \n", (int) GetLastError());
				fflush(stdout);
			}
			PRINTD(1," Enabled thread priority \n");
			setPriority = true;
			break;
#endif
		case 'S': 
			switch (argv[0][2]) {
			case 'p':
				if (argc <= 1 || argv[1][0] == '-') {
					printf("Missing parameter : option %s\n\n", argv[0]);
					printHelpAndExit();
				}
				defaultPortSignaling = atoi(argv[1]);
				PRINTD(1,"\nPort to listen: %d\n",defaultPortSignaling);
				NEXT_OPT;
				break;
#ifdef LINUX_OS
			case 'i':
				if (argc < 2)
					reportErrorAndExit("main", "general parser", "Invalid interface");
				SigInterface = (char*)malloc(strlen(argv[1]) + 1);
				strcpy(SigInterface, argv[1]);
				PRINTD(1,"\nSignaling channel bind to interface: %s\n",SigInterface);
				NEXT_OPT;
				break;
#endif
			default:
				printf("Unknow option : %s\n\n", argv[0]);
				printHelpAndExit();
			} 
			break;
		
		case 'H':
			PRINTD(1," Enabled passive mode \n");
			passiveMode = true;
			freeaddrinfo(passiveSender);
			if ((argc < 2) || getaddrinfo(argv[1], NULL, NULL, &passiveSender)){
				printf("Invalid ITGSend address or protocol \n");
				printHelpAndExit();
			}
			NEXT_OPT;
			break;
		case 'n':
			if (argc < 2)
				reportErrorAndExit("main","general parser","Invalid number of \"Hole Punching\" packets");
			tmp=atoi(argv[1]);
			if (tmp < 0)
				reportErrorAndExit("main","general parser","Invalid number of \"Hole Punching\" packets");
			numHolePkt=tmp;
			PRINTD(1,"\nNumbers of \"Hole Punching\" packet: %d",numHolePkt);
			NEXT_OPT;
			break;
		case 'a':
			PRINTD(1," Enabled global address \n");
			freeaddrinfo(globalBindAddr);
			if ((argc < 2) || getaddrinfo(argv[1], NULL, NULL, &globalBindAddr))
				reportErrorAndExit("main","general parser","Invalid address for \"-a\" option");
			NEXT_OPT;
			break;
		
		default:
			printf("Unknow option : %s\n\n", argv[0]);
			printHelpAndExit();
		} 

		
		NEXT_OPT;
	} 
}



int main(int argc, char *argv[])
{
	
	paramThread para[MAX_NUM_THREAD];

	
	pthread_t hThr[MAX_NUM_THREAD];

	if (argc > 1){
		if (strcmp(argv[1],"-gui") == 0){
			
			createThread((void*)"wait", &waitStopKey, NULL, waitStopKeyTid);
			argc--;
			argv++;
		}
	}

	
	int newSockSignaling = 0;
	
	struct addrinfo *sockAddress = 0;
	
	signal(SIGINT, terminate);

	
	for (int i = 0; i < MAX_NUM_THREAD; i++) {
		hThr[i] = 0;
		para[i].flowId = 0;
		para[i].count = 0;
		para[i].socket = 0;
		para[i].socketClose = 0;
		memLogFile[i].num = -1;
		strcpy(memLogFile[i].logFile, " ");
#ifdef SCTP
		sctpSessions[i].parsedStreams = 0;
#endif
	}

	
	recvInit();

	
	argv++;
	
	argc--;

	
	parserRecv(argc, argv);

	
	if ((logCheck) && (logRemote))
		logCheck = 0;

	
	if ((logCheck) && (logRemote == 0)) {
		
		out.open(logFile, ios::out | ios::binary | ios::trunc);
		if (!out) {
			
			char *tail = (char *)
			    malloc(sizeof("Error into open this file : ") + sizeof(logFile));
			if (tail == NULL)
				reportErrorAndExit("main", "malloc3",
				    "Insufficient memory available");
			
			sprintf(tail, "Error into open this file : %s", logFile);
			reportErrorAndExit("main", "open", tail);
		}		
	}			
	else if (logRemote)
		
		createRemoteLogFile((*globaleLogHost), logFile, l4ProtoLog,
		    globaleLogSockSignaling, globaleLogSock);



	printf("Press Ctrl-C to terminate\n");
	fflush(stdout);

	if(passiveMode==false){         

		

#ifdef LINUX_OS
		
#ifdef BSD
		
		if (getaddrinfo("0.0.0.0", NULL, NULL, &sockAddress) ||
#else
		if (getaddrinfo("::", NULL, NULL, &sockAddress) ||
#endif
			((sockSignaling = socket(sockAddress->ai_family, SOCK_STREAM, 0)) < 0)) {
			
			if (sockAddress)
				freeaddrinfo(sockAddress);
			if (getaddrinfo("0.0.0.0", NULL, NULL, &sockAddress) ||
				((sockSignaling = socket(sockAddress->ai_family, SOCK_STREAM, 0)) < 0)) {
				reportErrorAndExit("main", "socket",
					"Cannot create a socket for signaling");
			}
		}
#endif

#ifdef WIN32
		
#ifdef IPv6RECV
		if (getaddrinfo("::", NULL, NULL, &sockAddress) ||
			
#else
				if (getaddrinfo("0.0.0.0", NULL, NULL, &sockAddress) ||
#endif
			((sockSignaling = socket(sockAddress->ai_family, SOCK_STREAM, 0)) < 0)) {
			reportErrorAndExit("main", "socket", "Cannot create a socket for signaling");
		}
#endif

		int reuse = 1;
		int optlen = sizeof(reuse);
		if (setsockopt(sockSignaling, SOL_SOCKET, SO_REUSEADDR, (char *) &reuse, optlen) < 0) {
			reportErrorAndExit("main", "socket", "Error setting socket option");
		}

		
		SET_PORT(sockAddress, htons(defaultPortSignaling));
		
		if (bind(sockSignaling, sockAddress->ai_addr, sockAddress->ai_addrlen)) {
			
			char *tail = (char *) malloc(sizeof("Cannot bind a socket on port ")
				+ sizeof(defaultPortSignaling) + sizeof(" for signaling"));
			if (tail == NULL)
				reportErrorAndExit("main", "malloc4 - bind",
					"Insufficient memory available");
			
			sprintf(tail,
				"Cannot bind a socket on port %d for signaling", defaultPortSignaling);
			reportErrorAndExit("main", "general parser", tail);
		}

		
#if defined LINUX_OS && ! defined BSD
		
		if(SigInterface != NULL){
			printf("\nBinding to device %s for signaling channel\n",SigInterface);
			if (setsockopt(sockSignaling, SOL_SOCKET, SO_BINDTODEVICE, SigInterface, strlen(SigInterface)) < 0) {
				printf("** WARNING ** Cannot bind to device %s (hint: you must be root)\n", SigInterface);
				fflush(stdout);
			}
		}
#endif
		

		if (listen(sockSignaling, SOMAXCONN) < 0) {
			
			char *tail = (char *)
				malloc(sizeof("Cannot listen for signaling connections on port ")
				+ sizeof(defaultPortSignaling));
			if (tail == NULL)
				reportErrorAndExit("main", "malloc5 - listen",
					"Insufficient memory available");
			
			sprintf(tail,
				"Cannot listen for signaling connections on port %d", defaultPortSignaling);
			reportErrorAndExit("main", "general parser", tail);
		}

		int i = 0;

		

		while (1) {
			PRINTD(1,"Before the accept()\n");
			if ((newSockSignaling = accept(sockSignaling, NULL, NULL)) < 0) {
				reportErrorAndExit("main", "accept",
					"Connection fault on port for signaling");
			}
#ifdef WIN32
#ifdef IPv6RECV
			printf("*** New Socket IPv6 created for signaling ***\n");
#else
			printf("*** New Socket IPv4 created for signaling ***\n");
#endif
			fflush(stdout);
#endif
			
			para[i].socket = newSockSignaling;

			if (CREATE_THREAD(&para[i], signalManager, NULL, hThr[i]) < 0)
				reportErrorAndExit("main", "createThread", "Cannot create thread");
			PRINTD(1,"Return value CREATE_THREAD signalManager hThr[i] : %d \n", (int) hThr[i]);
			i++;
		}
	
	}else{
	// Passive mode:

		sockSignaling = socket(passiveSender->ai_family, SOCK_STREAM, 0);

		if (sockSignaling < 0)
			reportErrorAndExit("main","socket","Socket error(Passive Mode)");

		
#if defined LINUX_OS && ! defined BSD
		int dontRoute = 1;
		
		if(SigInterface != NULL){
			printf("\nBinding to device %s for signaling channel\n",SigInterface);
			if (setsockopt(sockSignaling, SOL_SOCKET, SO_BINDTODEVICE, SigInterface, strlen(SigInterface)) < 0) {
				printf("** WARNING ** Cannot bind to device %s (hint: you must be root)\n", SigInterface);
				fflush(stdout);
			}
			else if (setsockopt(sockSignaling, SOL_SOCKET, SO_DONTROUTE, &dontRoute, sizeof(int)) < 0) {
				printf("** WARNING ** Cannot set don't route (hint: you must be root)\n");
				fflush(stdout);
			}
		}
#endif
		

		
		SET_PORT(passiveSender, htons(defaultPortSignaling));
		
		if (connect(sockSignaling, passiveSender->ai_addr, passiveSender->ai_addrlen) < 0) {
			PRINTD(1,"\nError:%s\n",strerror(errno));
			reportErrorAndExit("main", "connect","Connection fault for signaling (Passive Mode)");
		}
		
		para[0].socket = sockSignaling;

		signalManager(&para[0]);
		
		terminate(0);
	}
	return 0;
} 
