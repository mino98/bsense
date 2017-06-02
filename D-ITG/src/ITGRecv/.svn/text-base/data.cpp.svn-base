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




#include "../common/ITG.h"
#include "../common/debug.h"
#include "../common/serial.h"
#include "../common/pipes.h"
#include "../common/thread.h"
#include "../common/timestamp.h"
#include "ITGRecv.h"
#include "data.h"

#ifdef LINUX_OS
#include <netinet/ip.h>
#endif

#ifdef SCTP
extern int sctpSessionCount;
extern sctpSession sctpSessions[];
extern pthread_mutex_t mutexSctp;
#endif




void *icmpSock(void *param)
{
#ifdef WIN32
	
	int first = 1;
#endif
   	
	int dimheader = 0;

	
   	HANDLE hComm = 0;

	
	paramThread *paraThread;
	
	paraThread = (paramThread *) param;
	
	int sock = 0;
	
	unsigned char buffer[MAX_PAYLOAD_SIZE + 32];
	
	unsigned char payload[MAX_PAYLOAD_SIZE];
	
	struct info *infos = (struct info *) malloc(DIM * sizeof(info));
	
	struct addrinfo SrcAddress;

	
	int size_r = 0;
	
	struct timeval RcvTime;
	#ifdef WIN32
	
	struct addrinfo* sockAddress;

	

	
	LARGE_INTEGER _tstart, _tend;
	

	
	unsigned long secs = 0, msecs = 0;
#endif

	
	
	char *ptrSeqNum = (char *) payload + sizeof(uint32_t);                  
	
	
	char *ptrTimeSec = ptrSeqNum + sizeof(uint32_t);						
	
	
	char *ptrTimeUsec = ptrTimeSec + sizeof(uint32_t);						

	
	paraThread->addressInfos = infos;
	
	paraThread->count = 0;

	if (strcmp(paraThread->serial,"noSerial")!=0) {
		hComm = serialUp(paraThread->serial);
        if (hComm == INVALID_HANDLE_VALUE)
             printf("Error opening interface %s \n", paraThread->serial);
    }

	
	sock = socket(paraThread->destHost.ai_family, SOCK_RAW,
		(paraThread->destHost.ai_family == AF_INET) ? IPPROTO_ICMP : IPPROTO_ICMPV6);
	if (sock < 0) {
		
		struct pipeMsg msg;
		printf("icmpSock - error into create socket");

		msg.code = MSG_FT_ERR2;
		msg.flowId = paraThread->flowId;
		if (sendPipeMsg(paraThread->rPipe, &msg) < 0) {
			printf(" sending msg error");
		}
		sleep(INFINITE);
	}

#ifdef WIN32

#ifdef IPv6RECV
	if ( getaddrinfo("::", NULL, NULL, &sockAddress) != 0)
		reportErrorAndExit("icmpSock","getaddrinfo IPv6","Cannot set the default IP log address");

#else
	if ( getaddrinfo("0.0.0.0", NULL, NULL, &sockAddress) != 0)
		reportErrorAndExit("icmpSock","getaddrinfo IPv4","Cannot set the default IP log address");
#endif
	
	if ( bind(sock, sockAddress->ai_addr, sockAddress->ai_addrlen))
		reportErrorAndExit("icmpSock","bind","Cannot bind a socket on port");
#endif
#if defined LINUX_OS && ! defined BSD
	
	if (paraThread->iface) {
		printf("Binding to device %s\n",paraThread->iface);
		if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, paraThread->iface, strlen(paraThread->iface)) < 0) {
			printf("** WARNING ** Cannot bind to device %s (hint: you must be root)\n", paraThread->iface);
			fflush(stdout);
		}
	}
#endif
	
	
	struct pipeMsg msg;
	msg.code = MSG_FT_OK;
	msg.flowId = paraThread->flowId;
	if (sendPipeMsg(paraThread->rPipe, &msg) < 0) {
		printf("error into sending msg to signal manager");
	}

	fprintf(stderr, "Listening ICMP traffic \n");
	
	paraThread->socketClose = sock;
	
	SrcAddress.ai_family = paraThread->destHost.ai_family;
	
	if (SrcAddress.ai_family == PF_INET) {
		
		SrcAddress.ai_addr = (struct sockaddr *) malloc(sizeof(struct sockaddr_in));
		
		SrcAddress.ai_addrlen = sizeof(struct sockaddr_in);
		
		dimheader = sizeof(icmp) + sizeof(iphdr);

	
	} else if (SrcAddress.ai_family == PF_INET6) {
		
		SrcAddress.ai_addr = (struct sockaddr *) malloc(sizeof(struct sockaddr_in6));
		
		SrcAddress.ai_addrlen = sizeof(struct sockaddr_in6);
		
		dimheader = sizeof(icmpv6);
	}

	int firstpacket = 1;
	char HelpSrcAddress[INET6_ADDRSTRLEN];
	char HelpDstAddress[INET6_ADDRSTRLEN];
	int tmpPort_SrcPort=0;
	int tmpPort_DstPort=0;
	
	TSTART(_tstart, secs, msecs, first, 0, RECEIVER);
	
	while (1) {
		
		size_r =
		    recvfrom(sock, (char *) buffer, MAX_PAYLOAD_SIZE + dimheader, 0,
		    SrcAddress.ai_addr, (socklen_t *) & SrcAddress.ai_addrlen);

			PRINTD(2,"ICMP Pkt Receiver : Received RAW Packet :%d\n", size_r);

		if (size_r < 0)
			reportErrorAndExit("icmpSock","recvfrom","Cannot receive RAW packets");
		
		char *ptr = (char *) buffer;

		
		ptr = ptr + dimheader;

		
		size_r = size_r - dimheader;

		
		memcpy(&payload, ptr, size_r);

	        if (hComm != INVALID_HANDLE_VALUE){
        	      DTR_Disable(hComm);
              	      DTR_Enable(hComm);
        	}

		
		GET_TIME_OF_DAY(&RcvTime, _tend, _tstart, secs, msecs, 0, RECEIVER);

		

		
		if ((logCheck != 0) || (logRemote != 0)) {
			if (firstpacket == 1) {
				getInfo(&SrcAddress, tmpPort_SrcPort, HelpSrcAddress);
				getInfo(&paraThread->destHost, tmpPort_DstPort, HelpDstAddress);
				firstpacket = 0;
			}

			if (logCheck != 0){
				
				
				int net_TimeSec = ntohl(*(uint32_t *) ptrTimeSec); 
				int net_TimeUsec = ntohl(*(uint32_t *) ptrTimeUsec); 

				if ( paraThread ->payloadLogType == PL_STANDARD ) 
					writeInBufferStandard(&infos[paraThread->count],*(unsigned int *) payload,*(unsigned int *) ptrSeqNum, HelpSrcAddress, HelpDstAddress, tmpPort_SrcPort, tmpPort_DstPort, net_TimeSec, RcvTime.tv_sec % 86400L, net_TimeUsec, RcvTime.tv_usec, size_r);
				else if ( paraThread ->payloadLogType == PL_SHORT ) 
					writeInBufferShort(&infos[paraThread->count],*(unsigned int *) payload,*(unsigned int *) ptrSeqNum, HelpSrcAddress, HelpDstAddress, tmpPort_SrcPort, tmpPort_DstPort, RcvTime.tv_sec % 86400L, RcvTime.tv_usec, size_r);
				else 
					writeInBufferNone(&infos[paraThread->count],*(unsigned int *) payload, HelpSrcAddress, HelpDstAddress, tmpPort_SrcPort, tmpPort_DstPort,RcvTime.tv_sec % 86400L, RcvTime.tv_usec, size_r);

				
				
				
				
				
			}
			
			
			
			if ( logRemote != 0){
				if ( paraThread ->payloadLogType == PL_STANDARD ) 
					writeInBufferStandard(&infos[paraThread->count],*(unsigned int *) payload,*(unsigned int *) ptrSeqNum, HelpSrcAddress, HelpDstAddress, tmpPort_SrcPort, tmpPort_DstPort, 0, 0, 0,0,htonl(size_r));
				else if ( paraThread ->payloadLogType == PL_SHORT ) 
					writeInBufferShort(&infos[paraThread->count],*(unsigned int *) payload,*(unsigned int *) ptrSeqNum, HelpSrcAddress, HelpDstAddress, tmpPort_SrcPort, tmpPort_DstPort, 0, 0, htonl(size_r));
				else 
					writeInBufferNone(&infos[paraThread->count],*(unsigned int *) payload, HelpSrcAddress, HelpDstAddress, tmpPort_SrcPort, tmpPort_DstPort, 0, 0, htonl(size_r));

				infos[paraThread->count].flowId = htonl(infos[paraThread->count].flowId); 
				infos[paraThread->count].seqNum = htonl(infos[paraThread->count].seqNum); 
				infos[paraThread->count].srcPort = htonl(infos[paraThread->count].srcPort);
                infos[paraThread->count].destPort = htonl(infos[paraThread->count].destPort);
                infos[paraThread->count].txTime1 = *(unsigned int*) ptrTimeSec;
                infos[paraThread->count].txTime2 = *(unsigned int*) ptrTimeUsec;
                infos[paraThread->count].txTime3 = 0;
                infos[paraThread->count].rxTime1 = htonl(RcvTime.tv_sec % 86400L);
                infos[paraThread->count].rxTime2 = htonl(RcvTime.tv_usec);
                infos[paraThread->count].rxTime3 = 0;
			}

			
			paraThread->count++;
			
			if (paraThread->count == DIM) {
			

			if (logCheck != 0)
				flushBuffer((ofstream *) paraThread->fileLog, infos , DIM);
			
			else  if (logRemote != 0) {
					MUTEX_THREAD_LOCK(mutexLogRem);
					if ( sendto(paraThread->logSock, (char *) infos,
							DIM * sizeof(struct info), 0, paraThread->logHost->ai_addr,
							paraThread->logHost->ai_addrlen) < 0)
						reportErrorAndExit("icmpSock","sendto","Cannot send log infos to LogServer");
					MUTEX_THREAD_UNLOCK(mutexLogRem);
					PRINTD(1,"ICMP Pkt Receiver : Sent Infos to LogServer\n");
				}
				
				paraThread->count = 0;
			}	
		}	
	}	
	return NULL;	
}	





void *udpSock(void *param)
{
#ifdef WIN32
	
	int first = 1;
#endif
	
	HANDLE hComm = 0;

	
	paramThread *paraThread;

	
	paraThread = (paramThread *) param;
	
	int sock = 0;
	
	unsigned char payload[MAX_PAYLOAD_SIZE];
	
	struct info *infos = (struct info *) malloc(DIM * sizeof(info));
	
	struct addrinfo SrcAddress;
	
	in_port_t tmpPort = 0;
	
	int size_r = 0;
	
	
	unsigned char *ptrSeqNum = payload + sizeof(uint32_t);			
	
	
	unsigned char *ptrTimeSec = ptrSeqNum + sizeof(uint32_t);		
	
	
	unsigned char *ptrTimeUsec = ptrTimeSec + sizeof(uint32_t);		
	
	struct timeval RcvTime;
#ifdef WIN32
	

	
	LARGE_INTEGER _tstart, _tend;
	

	
	unsigned long secs = 0, msecs = 0;
#endif
	
	paraThread->addressInfos = infos;
	
	paraThread->count = 0;

	if (strcmp(paraThread->serial,"noSerial")!=0) {
		hComm = serialUp(paraThread->serial);
        if (hComm == INVALID_HANDLE_VALUE)
             printf("Error opening interface %s \n", paraThread->serial);
 	}

	
	sock = socket(paraThread->destHost.ai_family, SOCK_DGRAM, 0);
	if (sock < 0)
		reportErrorAndExit("udpSock","socket","Cannot create a DATAGRAM socket on port");


	if (passiveMode==false){			

		if (bind(sock, paraThread->destHost.ai_addr, paraThread->destHost.ai_addrlen) < 0) {
			  
			printf("Error into bind function!\n");
			struct pipeMsg msg;
			msg.code = MSG_FT_ERR1;
			msg.flowId = paraThread->flowId;
			if (sendPipeMsg(paraThread->rPipe, &msg) < 0) {
				printf(" sending msg error");
			}
			sleep(INFINITE);
		} else {
			
#ifdef WIN32
			int len=paraThread->destHost.ai_addrlen;
			getsockname(sock,paraThread->destHost.ai_addr,&len);
			paraThread->destHost.ai_addrlen=len;
#else
			getsockname(sock,paraThread->destHost.ai_addr, &(paraThread->destHost.ai_addrlen));
#endif

			
			GET_PORT((&(paraThread->destHost)), tmpPort);
			fprintf(stderr, "Listening on port: %d\n",ntohs(tmpPort));
			fflush(stderr);

			
			struct pipeMsg msg;
			msg.code = MSG_FT_OK;
			msg.flowId = paraThread->flowId;
			if (sendPipeMsg(paraThread->rPipe, &msg) < 0) {
				printf("error sending msg to signal manager");
			}
		}

	}
	
	else{
		struct addrinfo *addrForListen=NULL;

#if (defined WIN32 && defined IPv6RECV) || defined LINUX_OS || defined BSD
		if (paraThread->destHost.ai_family == PF_INET6){
					getaddrinfo("::",NULL,NULL,&addrForListen);
		} else
#endif
		{
			getaddrinfo("0.0.0.0",NULL,NULL,&addrForListen);
		}

		
		SET_PORT((addrForListen), htons((paraThread->portForPssv)));

		
		if (bind(sock, addrForListen->ai_addr, addrForListen->ai_addrlen) < 0) {
			
			struct pipeMsg msg;

			msg.code = MSG_FT_ERR1;
			msg.flowId = paraThread->flowId;
			if (sendPipeMsg(paraThread->rPipe, &msg) < 0) {
				printf(" sending msg error");
			}
			sleep(INFINITE); 
		}
		else {
			
			GET_PORT(( addrForListen), tmpPort);
			printf("Listening on port: %d\n",ntohs(tmpPort));
			fflush(stdout);
			
			for (int x=0;x<numHolePkt;x++)
				if (sendto(sock,"hello", sizeof("hello"), 0,  paraThread->destHost.ai_addr,paraThread->destHost.ai_addrlen) < 0){
					reportErrorAndExit("udpSock","sendto","Cannot sendto (Passive Mode --> Hole punching)");
				}
		}

		
		if(connect(sock,paraThread->destHost.ai_addr,paraThread->destHost.ai_addrlen) < 0)
			reportErrorAndExit("udpSock","connect","Cannot connect (Passive Mode)");

		freeaddrinfo(addrForListen);
	}
	

#if defined LINUX_OS && ! defined BSD
       
	if (paraThread->iface) {
		printf("Binding to device %s\n",paraThread->iface);
		if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, paraThread->iface, strlen(paraThread->iface)) < 0) {
			printf("** WARNING ** Cannot bind to device %s (hint: you must be root)\n", paraThread->iface);
			fflush(stdout);
		}
	}
#endif



	
	paraThread->socketClose = sock;
	
	SrcAddress.ai_family = paraThread->destHost.ai_family;
	
	if (SrcAddress.ai_family == PF_INET) {
		
		SrcAddress.ai_addr = (struct sockaddr *) malloc(sizeof(struct sockaddr_in));
		
		SrcAddress.ai_addrlen = sizeof(struct sockaddr_in);
		
	} else if (SrcAddress.ai_family == PF_INET6) {
		
		SrcAddress.ai_addr = (struct sockaddr *) malloc(sizeof(struct sockaddr_in6));
		
		SrcAddress.ai_addrlen = sizeof(struct sockaddr_in6);
	}



	int firstpacket = 1;
	char HelpSrcAddress[INET6_ADDRSTRLEN];
	char HelpDstAddress[INET6_ADDRSTRLEN];
	int tmpPort_SrcPort=0;
	int tmpPort_DstPort=0;

	
	
	

	
	TSTART(_tstart, secs, msecs, first, 0, RECEIVER);

	
	
	
	while (1){
		if (passiveMode == false){														
			
			size_r =recvfrom(sock, (char *) payload, MAX_PAYLOAD_SIZE, 0, SrcAddress.ai_addr,(socklen_t *) & SrcAddress.ai_addrlen);
		}else{
					
			size_r = recvfrom(sock, (char *) payload, MAX_PAYLOAD_SIZE, 0, NULL,NULL);	
		}																				
		PRINTD(2,"UDP Pkt Receiver : Received DATAGRAM packet\n");

		if (size_r < 0){
			PRINTD(1,"\nError:%s\n",strerror(errno));
// Quick&dirty workaround for ITGRecv on OSX: only for UDP traffic, it terminates 
// with this error after the last packet has been read. Instead, we just exit cleanly.
#ifndef OSX
			reportErrorAndExit("udpSock","recvfrom","Cannot receive UDP packets");
#else
			exit(0);
#endif
		}

		if (hComm != INVALID_HANDLE_VALUE){
			DTR_Disable(hComm);
			DTR_Enable(hComm);
		}

		
		GET_TIME_OF_DAY(&RcvTime, _tend, _tstart, secs, msecs, 0, RECEIVER);
		

		
		if ((logCheck!=0) || (logRemote != 0)){  
			if (firstpacket == 1) {
				if(passiveMode==false){
					getInfo(&SrcAddress, tmpPort_SrcPort, HelpSrcAddress);
					getInfo(&paraThread->destHost, tmpPort_DstPort, HelpDstAddress);
				}else{
					
#ifdef WIN32
					int len=SrcAddress.ai_addrlen;										
					getsockname(sock,SrcAddress.ai_addr,&len);							
					SrcAddress.ai_addrlen=len;											
#else
					getsockname(sock,SrcAddress.ai_addr,&SrcAddress.ai_addrlen);		
#endif
					getInfo(&paraThread->destHost, tmpPort_SrcPort, HelpSrcAddress);	
					getInfo(&SrcAddress, tmpPort_DstPort, HelpDstAddress);				
				}
				firstpacket = 0;
			}
			
			if (logCheck != 0){
				
				
				int net_TimeSec = ntohl(*(uint32_t *) ptrTimeSec); 
				int net_TimeUsec = ntohl(*(uint32_t *) ptrTimeUsec); 

				if ( paraThread ->payloadLogType == PL_STANDARD ) 
					writeInBufferStandard(&infos[paraThread->count],*(unsigned int *) payload,*(unsigned int *) ptrSeqNum, HelpSrcAddress, HelpDstAddress, tmpPort_SrcPort, tmpPort_DstPort, net_TimeSec, RcvTime.tv_sec % 86400L, net_TimeUsec, RcvTime.tv_usec, size_r);
				else if ( paraThread ->payloadLogType == PL_SHORT ) 
					writeInBufferShort(&infos[paraThread->count],*(unsigned int *) payload,*(unsigned int *) ptrSeqNum, HelpSrcAddress, HelpDstAddress, tmpPort_SrcPort, tmpPort_DstPort, RcvTime.tv_sec % 86400L, RcvTime.tv_usec, size_r);
				else 
					writeInBufferNone(&infos[paraThread->count],*(unsigned int *) payload, HelpSrcAddress, HelpDstAddress, tmpPort_SrcPort, tmpPort_DstPort,RcvTime.tv_sec % 86400L, RcvTime.tv_usec, size_r);

				
				
				
				
				
			}
			
			
			
			if ( logRemote != 0){
				if ( paraThread ->payloadLogType == PL_STANDARD ) 
					writeInBufferStandard(&infos[paraThread->count],*(unsigned int *) payload,*(unsigned int *) ptrSeqNum, HelpSrcAddress, HelpDstAddress, tmpPort_SrcPort, tmpPort_DstPort, 0, 0, 0,0,htonl(size_r));
				else if ( paraThread ->payloadLogType == PL_SHORT ) 
					writeInBufferShort(&infos[paraThread->count],*(unsigned int *) payload,*(unsigned int *) ptrSeqNum, HelpSrcAddress, HelpDstAddress, tmpPort_SrcPort, tmpPort_DstPort, 0, 0, htonl(size_r));
				else 
					writeInBufferNone(&infos[paraThread->count],*(unsigned int *) payload, HelpSrcAddress, HelpDstAddress, tmpPort_SrcPort, tmpPort_DstPort, 0, 0, htonl(size_r));

				infos[paraThread->count].flowId = htonl(infos[paraThread->count].flowId); 
				infos[paraThread->count].seqNum = htonl(infos[paraThread->count].seqNum); 
				infos[paraThread->count].srcPort = htonl(infos[paraThread->count].srcPort);
				infos[paraThread->count].destPort = htonl(infos[paraThread->count].destPort);
				infos[paraThread->count].txTime1 = *(unsigned int*) ptrTimeSec;
				infos[paraThread->count].txTime2 = *(unsigned int*) ptrTimeUsec;
				infos[paraThread->count].txTime3 = 0;
				infos[paraThread->count].rxTime1 = htonl(RcvTime.tv_sec % 86400L);
				infos[paraThread->count].rxTime2 = htonl(RcvTime.tv_usec);
				infos[paraThread->count].rxTime3 = 0;
			}
			
			
			paraThread->count++;

			
			if (paraThread->count == DIM) {
				
				if (logCheck !=0 )
					flushBuffer((ofstream *) paraThread->fileLog, infos , DIM);
				
				else if (logRemote != 0) {
					MUTEX_THREAD_LOCK(mutexLogRem);
					if ( sendto(paraThread->logSock, (char *) infos,
							DIM * sizeof(struct info), 0, paraThread->logHost->ai_addr,
							paraThread->logHost->ai_addrlen) < 0)
						reportErrorAndExit("udpSock","sendto","Cannot send log infos to LogServer");
					MUTEX_THREAD_UNLOCK(mutexLogRem);
					PRINTD(1,"UDP Pkt Receiver : Sent Infos to LogServer\n");
				}
				
				paraThread->count = 0;
			}	
		}	
		
		if (paraThread->meter == METER_RTTM) {
			if(passiveMode==false){																				
				if ( sendto(sock, (char *) payload, size_r, 0, SrcAddress.ai_addr,SrcAddress.ai_addrlen) < 0)
					reportErrorAndExit("udpSock","sendto","Cannot send back payload for rttm");
			}else{																								
				if ( sendto(sock, (char *) payload, size_r, 0, NULL, 0) < 0)									
					reportErrorAndExit("udpSock","sendto","Cannot send back payload for rttm (Passive Mode)");	
			}																									
			PRINTD(2,"UDP Pkt Receiver : Sent RTTM message\n");
		}	
	}	
	return NULL;	
}	







void *tcpSock(void *param)
{
#ifdef WIN32
	
	int first = 1;
#endif
    
    HANDLE hComm = 0;

	
	paramThread *paraThread;
	
	paraThread = (paramThread *) param;


	
	int sock = 0;

	
	unsigned char payload[MAX_PAYLOAD_SIZE];
	
	struct info *infos = (struct info *) malloc(DIM * sizeof(info));
	
	struct addrinfo SrcAddress;
	
	in_port_t tmpPort = 0;
	
	int newSock = 0;
    
	int size = 0;
	
	
	unsigned char *ptrSeqNum = payload + sizeof(uint32_t);			
	
	
	unsigned char *ptrTimeSec = ptrSeqNum + sizeof(uint32_t);		
	
	
	unsigned char *ptrTimeUsec = ptrTimeSec + sizeof(uint32_t);		
	
	struct timeval RcvTime;
#ifdef WIN32
    

    
	LARGE_INTEGER _tstart, _tend;
	

	
	unsigned long secs = 0, msecs = 0;
#endif
	
	paraThread->addressInfos = infos;
	
	paraThread->count = 0;

	if (strcmp(paraThread->serial,"noSerial")!=0) {
		hComm = serialUp(paraThread->serial);
        if (hComm == INVALID_HANDLE_VALUE)
             printf("Error opening interface %s \n", paraThread->serial);
    }

	
	sock = socket(paraThread->destHost.ai_family, SOCK_STREAM, 0);
	if (sock < 0)
		reportErrorAndExit("tcpSock","socket","Cannot create a STREAM socket on port");

	if (passiveMode == false){ 	

		
		if (bind(sock, paraThread->destHost.ai_addr, paraThread->destHost.ai_addrlen) < 0) {
			
			struct pipeMsg msg;

			msg.code = MSG_FT_ERR1;
			msg.flowId = paraThread->flowId;
			if (sendPipeMsg(paraThread->rPipe, &msg) < 0) {
				printf(" sending msg error");
			}
			sleep(INFINITE); 
		}

	}

#if defined LINUX_OS && ! defined BSD
	
	if (paraThread->iface) {
		printf("Binding to device %s\n",paraThread->iface);
		if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, paraThread->iface, strlen(paraThread->iface)) < 0) {
			printf("** WARNING ** Cannot bind to device %s (hint: you must be root)\n", paraThread->iface);
			fflush(stdout);
		}
	}
#endif

	if (passiveMode == false){ 	

		
		if (listen(sock, SOMAXCONN) < 0)
				reportErrorAndExit("tcpSock","listen","Cannot listen on a port");
		else {
			
#ifdef WIN32
			int len=paraThread->destHost.ai_addrlen;
			getsockname(sock,paraThread->destHost.ai_addr,&len);
			paraThread->destHost.ai_addrlen=len;
#else
			getsockname(sock,paraThread->destHost.ai_addr, &(paraThread->destHost.ai_addrlen));
#endif
			
			GET_PORT((&(paraThread->destHost)), tmpPort);
			printf("Listening on port: %d\n",ntohs(tmpPort));
			fflush(stdout);

			
			struct pipeMsg msg;
			msg.code = MSG_FT_OK;
			msg.flowId = paraThread->flowId;
			if (sendPipeMsg(paraThread->rPipe, &msg) < 0) {
				printf("error into sending msg to signal manager");
			}
		}

	}

	
	SrcAddress.ai_family = paraThread->destHost.ai_family;
	
	if (SrcAddress.ai_family == PF_INET) {
		
		SrcAddress.ai_addr = (struct sockaddr *) malloc(sizeof(struct sockaddr_in));
		
		SrcAddress.ai_addrlen = sizeof(struct sockaddr_in);
	
	} else if (SrcAddress.ai_family == PF_INET6) {
		
		SrcAddress.ai_addr = (struct sockaddr *) malloc(sizeof(struct sockaddr_in6));
		
		SrcAddress.ai_addrlen = sizeof(struct sockaddr_in6);
	}

	if (passiveMode == false){ 	

		

		if ((newSock =
			accept(sock, SrcAddress.ai_addr, (socklen_t *) & SrcAddress.ai_addrlen)) < 0)
				reportErrorAndExit("tcpSock","accept","Cannot accept connection");
		
		if ( closeSock(sock) == -1)
			reportErrorAndExit("tcpSock","closeSock","Cannot close socket sock");

		
		paraThread->socketClose = newSock;

	
	}else{
		struct addrinfo *SrcAddrForConnect=NULL;

#if (defined WIN32 && defined IPv6RECV) || defined LINUX_OS || defined BSD
		if (paraThread->destHost.ai_family == PF_INET6){
			getaddrinfo("::",NULL,NULL,&SrcAddrForConnect);
		} else
#endif
		{
			getaddrinfo("0.0.0.0",NULL,NULL,&SrcAddrForConnect);
		}

		
		SET_PORT((SrcAddrForConnect), htons((paraThread->portForPssv)));

		
		if (bind(sock, SrcAddrForConnect->ai_addr, SrcAddrForConnect->ai_addrlen) < 0) {
			
			struct pipeMsg msg;

			msg.code = MSG_FT_ERR1;
			msg.flowId = paraThread->flowId;
			if (sendPipeMsg(paraThread->rPipe, &msg) < 0) {
				printf(" sending msg error");
			}
			sleep(INFINITE); 
		}

		
		if(connect(sock,paraThread->destHost.ai_addr,paraThread->destHost.ai_addrlen) < 0)
			reportErrorAndExit("tcpSock","connect","Cannot connect (Passive Mode)");
		PRINTD(1,"TCP Pkt Receiver : Connection establishes (Passive Mode)\n");

		freeaddrinfo(SrcAddrForConnect);

		
		newSock=sock;

		
		paraThread->socketClose=sock;

		
#ifdef WIN32
		int len=SrcAddress.ai_addrlen;										
		getsockname(sock,SrcAddress.ai_addr,&len);							
		SrcAddress.ai_addrlen=len;											
#else
		getsockname(sock,SrcAddress.ai_addr,&SrcAddress.ai_addrlen);		
#endif
	}
	

    int firstpacket =1;
	char HelpSrcAddress[INET6_ADDRSTRLEN];
	char HelpDstAddress[INET6_ADDRSTRLEN];
    int tmpPort_SrcPort=0;
    int tmpPort_DstPort=0;

	
	TSTART(_tstart, secs, msecs, first, 0, RECEIVER);
	
	while (1) {
	  
		PRINTD(2, "parathread->preambleSize = %d \n",paraThread->preambleSize);
       size=TCPrecvPacket((unsigned char*)payload,newSock,paraThread->preambleSize,paraThread->payloadLogType);

       
       if (size <= 0){
    	   PRINTD(1,"TCP Pkt Receiver : TCPrecvPacket() = %d\n",size);
    	   
    	   if (size < 0){
    		   
    		   struct pipeMsg msg;

        	   if(passiveMode==false){
        		   GET_PORT((&(paraThread->destHost)), tmpPort_DstPort);
        	   }else{
        		   GET_PORT((&SrcAddress), tmpPort_DstPort);
        	   }
        	   printf("Error on port: %d\n", ntohs(tmpPort_DstPort));
        	   printf("Finish on port: %d\n\n", ntohs(tmpPort_DstPort));
        	   fflush(stdout);

    		   msg.code = MSG_FT_ERR_SOCK;
    		   msg.flowId = paraThread->flowId;
    		   if (sendPipeMsg(paraThread->rPipe, &msg) < 0) {
    			   printf(" sending msg error");
    		   }
    	   }
    	   sleep(INFINITE);
       }

       if (hComm != INVALID_HANDLE_VALUE){
              DTR_Disable(hComm);
              DTR_Enable(hComm);
       }

       
       GET_TIME_OF_DAY(&RcvTime, _tend, _tstart, secs, msecs, 0, RECEIVER);

		
		
		if ((logCheck!=0) || (logRemote != 0)){  
		

	         if (firstpacket == 1) {
	        	 if(passiveMode==false){													
	        		 getInfo(&SrcAddress, tmpPort_SrcPort, HelpSrcAddress);
	        		 getInfo(&paraThread->destHost, tmpPort_DstPort, HelpDstAddress);
	        	 }else{																		
	        		 
#ifdef WIN32
	        		int len=SrcAddress.ai_addrlen;										
					getsockname(sock,SrcAddress.ai_addr,&len);							
					SrcAddress.ai_addrlen=len;											
#else
	        		getsockname(sock,SrcAddress.ai_addr,&SrcAddress.ai_addrlen);		
#endif
	        		getInfo(&paraThread->destHost, tmpPort_SrcPort, HelpSrcAddress);		
	        		getInfo(&SrcAddress, tmpPort_DstPort, HelpDstAddress);					
	        	 }																			
	        	 firstpacket = 0;
	         }
			
		if (paraThread->l7Proto == L7_PROTO_TELNET)
					size = size - 20; 

			if (logCheck != 0){


				int net_TimeSec = ntohl(*(uint32_t *) ptrTimeSec); 
				int net_TimeUsec = ntohl(*(uint32_t *) ptrTimeUsec); 

				if ( paraThread ->payloadLogType == PL_STANDARD ) 
					writeInBufferStandard(&infos[paraThread->count],*(unsigned int *) payload,*(unsigned int *) ptrSeqNum, HelpSrcAddress, HelpDstAddress, tmpPort_SrcPort, tmpPort_DstPort, net_TimeSec, RcvTime.tv_sec % 86400L, net_TimeUsec, RcvTime.tv_usec, size);
				else if ( paraThread ->payloadLogType == PL_SHORT ) 
					writeInBufferShort(&infos[paraThread->count],*(unsigned int *) payload,*(unsigned int *) ptrSeqNum, HelpSrcAddress, HelpDstAddress, tmpPort_SrcPort, tmpPort_DstPort, RcvTime.tv_sec % 86400L, RcvTime.tv_usec, size);
				else 
					writeInBufferNone(&infos[paraThread->count],*(unsigned int *) payload, HelpSrcAddress, HelpDstAddress, tmpPort_SrcPort, tmpPort_DstPort,RcvTime.tv_sec % 86400L, RcvTime.tv_usec, size);

				
				
				
				
				

			}
			
			
			
			if ( logRemote != 0){
				if ( paraThread ->payloadLogType == PL_STANDARD ) 
					writeInBufferStandard(&infos[paraThread->count],*(unsigned int *) payload,*(unsigned int *) ptrSeqNum, HelpSrcAddress, HelpDstAddress, tmpPort_SrcPort, tmpPort_DstPort, 0, 0, 0,0,htonl(size));
				else if ( paraThread ->payloadLogType == PL_SHORT ) 
					writeInBufferShort(&infos[paraThread->count],*(unsigned int *) payload,*(unsigned int *) ptrSeqNum, HelpSrcAddress, HelpDstAddress, tmpPort_SrcPort, tmpPort_DstPort, 0, 0, htonl(size));
				else 
					writeInBufferNone(&infos[paraThread->count],*(unsigned int *) payload, HelpSrcAddress, HelpDstAddress, tmpPort_SrcPort, tmpPort_DstPort, 0, 0, htonl(size));

				infos[paraThread->count].flowId = htonl(infos[paraThread->count].flowId); 
				infos[paraThread->count].seqNum = htonl(infos[paraThread->count].seqNum); 
				infos[paraThread->count].srcPort = htonl(infos[paraThread->count].srcPort);
                infos[paraThread->count].destPort = htonl(infos[paraThread->count].destPort);
                infos[paraThread->count].txTime1 = *(unsigned int*) ptrTimeSec;
                infos[paraThread->count].txTime2 = *(unsigned int*) ptrTimeUsec;
                infos[paraThread->count].txTime3 = 0;
                infos[paraThread->count].rxTime1 = htonl(RcvTime.tv_sec % 86400L);
                infos[paraThread->count].rxTime2 = htonl(RcvTime.tv_usec);
                infos[paraThread->count].rxTime3 = 0;
			}

                
                paraThread->count++;
                
                if (paraThread->count == DIM) {
                        
						if (logCheck !=0 )
                            flushBuffer((ofstream *) paraThread->fileLog, infos , DIM);
                            
                        else
						if (logRemote !=0 )
						{
							MUTEX_THREAD_LOCK(mutexLogRem);
                            if ( sendto(paraThread->logSock, (char *) infos,
                                 DIM * sizeof(struct info), 0, paraThread->logHost->ai_addr,
               				    paraThread->logHost->ai_addrlen) < 0)
               				    reportErrorAndExit("tcpSock","sendto","Cannot send log infos to LogServer");
               				MUTEX_THREAD_UNLOCK(mutexLogRem);
		                         PRINTD(1,"TCP Pkt Receiver : Sent infos to LogServer\n");
                        }
                        
                        paraThread->count = 0;
			}	
		}	
		
		if (paraThread->meter == METER_RTTM) {
			if (sendto(newSock, (char *) payload, size, 0, SrcAddress.ai_addr,
			    SrcAddress.ai_addrlen) < 0)
					reportErrorAndExit("tcpSock","sendto","Cannot send payload back for rttm");
			PRINTD(2,"TCP Pkt Receiver : Sent RTTM infos\n");
			}	
	}	
	return NULL;	
}	


#ifdef SCTP
void *sctpSock(void *param)
{
#ifdef WIN32
	
	int first = 1;
#endif
    
    HANDLE hComm;

	
	paramThread *paraThread;
	
	paraThread = (paramThread *) param;


	
	int sock = 0;

	
	unsigned char payload[MAX_PAYLOAD_SIZE];
	
	struct info *infos = (struct info *) malloc(DIM * sizeof(info));
	
	struct addrinfo SrcAddress;
	
	in_port_t tmpPort = 0;
	
	int newSock = 0;
    
	int size = 0;
	
	
	unsigned char *ptrSeqNum = payload + sizeof(uint32_t);			
	
	
	unsigned char *ptrTimeSec = ptrSeqNum + sizeof(uint32_t);		
	
	
	unsigned char *ptrTimeUsec = ptrTimeSec + sizeof(uint32_t);		
	
	struct timeval RcvTime;
#ifdef WIN32
    

    
	LARGE_INTEGER _tstart, _tend;
	

	
	unsigned long secs = 0, msecs = 0;
#endif


	int sctpId;
	bool newSession = true;



	PRINTD(1,"sctpSock function. Start of Function. logCheck = %d logRemote = %d \n", logCheck, logRemote);

	
	paraThread->addressInfos = infos;
	
	paraThread->count = 0;

	if (strcmp(paraThread->serial,"noSerial")!=0) {
		hComm = serialUp(paraThread->serial);
        if (hComm == INVALID_HANDLE_VALUE)
             printf("Error opening interface %s \n", paraThread->serial);
    }




	unsigned int port;
	GET_PORT((&(paraThread->destHost)), port);

	MUTEX_THREAD_LOCK(mutexSctp);

	
	for (sctpId = 0; sctpId < sctpSessionCount; sctpId++) {
		if (sctpSessions[sctpId].parsedStreams > 0 && sctpSessions[sctpId].port == port) {
				newSession = false;
				break;
		}
	}

	
	if (newSession) {
		PRINTD(1,"Receiving new SCTP session on port %d...\n", ntohs(port));
		sctpId = sctpSessionCount++;
		sctpSessions[sctpId].port = port;
		sctpSessions[sctpId].busyStreams = 0;
		sctpSessions[sctpId].parsedStreams = 1;
		sctpSessions[sctpId].sock = -1;
	}
	else sctpSessions[sctpId].parsedStreams++;

 	

	PRINTD(1,"Receiving new SCTP stream...\n");




    if (newSession) {
		
		sock = socket(paraThread->destHost.ai_family, SOCK_STREAM, IPPROTO_SCTP);
		if (sock < 0)
			reportErrorAndExit("sctpSock","socket","Cannot create a STREAM socket on port");

		
		if (bind(sock, paraThread->destHost.ai_addr, paraThread->destHost.ai_addrlen) < 0) {
			
			struct pipeMsg msg;

			msg.code = MSG_FT_ERR1;
			msg.flowId = paraThread->flowId;
			if (sendPipeMsg(paraThread->rPipe, &msg) < 0) {
				printf(" sending msg error");
			}
			sleep(INFINITE);
		}

		
		if (listen(sock, SOMAXCONN) < 0)
				reportErrorAndExit("tcpSock","listen","Cannot listen on a port");
		else {
			
#ifdef WIN32
			int len=paraThread->destHost.ai_addrlen;
			getsockname(sock,paraThread->destHost.ai_addr,&len);
			paraThread->destHost.ai_addrlen=len;
#else
			getsockname(sock,paraThread->destHost.ai_addr, &(paraThread->destHost.ai_addrlen));
#endif

			
			GET_PORT((&(paraThread->destHost)), tmpPort);
			fprintf(stderr, "Listening on port: %d\n",ntohs(tmpPort));
			fflush(stderr);

			
			struct pipeMsg msg;
			msg.code = MSG_FT_OK;
			msg.flowId = paraThread->flowId;
			if (sendPipeMsg(paraThread->rPipe, &msg) < 0) {
				printf("error into sending msg to signal manager");
			}
		}

		
		SrcAddress.ai_family = paraThread->destHost.ai_family;
		
		if (SrcAddress.ai_family == PF_INET) {
			
			SrcAddress.ai_addr = (struct sockaddr *) malloc(sizeof(struct sockaddr_in));
			
			SrcAddress.ai_addrlen = sizeof(struct sockaddr_in);
		
		} else if (SrcAddress.ai_family == PF_INET6) {
			
			SrcAddress.ai_addr = (struct sockaddr *) malloc(sizeof(struct sockaddr_in6));
			
			SrcAddress.ai_addrlen = sizeof(struct sockaddr_in6);
		}

		

		if ((newSock = accept(sock, SrcAddress.ai_addr, (socklen_t *) & SrcAddress.ai_addrlen)) < 0)
				reportErrorAndExit("sctpSock","accept","Cannot accept connection");
		
		if ( closeSock(sock) == -1)
			reportErrorAndExit("sctpSock","closeSock","Cannot close socket sock");

		
		paraThread->socketClose = newSock;

		
		sctpSessions[sctpId].sock = newSock;
		
	}
	else newSock = sctpSessions[sctpId].sock;
	MUTEX_THREAD_UNLOCK(mutexSctp);

    int firstpacket =1;
	char HelpSrcAddress[INET6_ADDRSTRLEN];
	char HelpDstAddress[INET6_ADDRSTRLEN];
    int tmpPort_SrcPort;
    int tmpPort_DstPort;

	
	TSTART(_tstart, secs, msecs, first, 0, RECEIVER);
	
	while (1) {
	  
       size = SCTPrecvPacket ((unsigned char*)payload, newSock, sctpId, paraThread->preambleSize, paraThread->payloadLogType);
       PRINTD(2,"Received SCTP data. Size of the received data chinck: %d\n",size);

       if (hComm != INVALID_HANDLE_VALUE){
              DTR_Disable(hComm);
              DTR_Enable(hComm);
       }

       
       GET_TIME_OF_DAY(&RcvTime, _tend, _tstart, secs, msecs, 0, RECEIVER);


		
		
		if ((logCheck!=0) || (logRemote != 0)){  
		
			PRINTD(2,"Log received data \n");
			if (firstpacket == 1) {
				getInfo(&SrcAddress, tmpPort_SrcPort, HelpSrcAddress);
				getInfo(&paraThread->destHost, tmpPort_DstPort, HelpDstAddress);
				firstpacket = 0;
			}


			if (logCheck != 0){


				int net_TimeSec = ntohl(*(uint32_t *) ptrTimeSec); 
 		        int net_TimeUsec = ntohl(*(uint32_t *) ptrTimeUsec); 

				if ( paraThread ->payloadLogType == PL_STANDARD ) 
					writeInBufferStandard(&infos[paraThread->count],*(unsigned int *) payload,*(unsigned int *) ptrSeqNum, HelpSrcAddress, HelpDstAddress, tmpPort_SrcPort, tmpPort_DstPort, net_TimeSec, RcvTime.tv_sec % 86400L, net_TimeUsec, RcvTime.tv_usec, size);
				else if ( paraThread ->payloadLogType == PL_SHORT ) 
					writeInBufferShort(&infos[paraThread->count],*(unsigned int *) payload,*(unsigned int *) ptrSeqNum, HelpSrcAddress, HelpDstAddress, tmpPort_SrcPort, tmpPort_DstPort, RcvTime.tv_sec % 86400L, RcvTime.tv_usec, size);
				else 
					writeInBufferNone(&infos[paraThread->count],*(unsigned int *) payload, HelpSrcAddress, HelpDstAddress, tmpPort_SrcPort, tmpPort_DstPort,RcvTime.tv_sec % 86400L, RcvTime.tv_usec, size);

				
				
				
				
				
			}
			
			
			
			if ( logRemote != 0){
				if ( paraThread ->payloadLogType == PL_STANDARD ) 
					writeInBufferStandard(&infos[paraThread->count],*(unsigned int *) payload,*(unsigned int *) ptrSeqNum, HelpSrcAddress, HelpDstAddress, tmpPort_SrcPort, tmpPort_DstPort, 0, 0, 0,0,htonl(size));
				else if ( paraThread ->payloadLogType == PL_SHORT ) 
					writeInBufferShort(&infos[paraThread->count],*(unsigned int *) payload,*(unsigned int *) ptrSeqNum, HelpSrcAddress, HelpDstAddress, tmpPort_SrcPort, tmpPort_DstPort, 0, 0, htonl(size));
				else 
					writeInBufferNone(&infos[paraThread->count],*(unsigned int *) payload, HelpSrcAddress, HelpDstAddress, tmpPort_SrcPort, tmpPort_DstPort, 0, 0, htonl(size));

				infos[paraThread->count].flowId = htonl(infos[paraThread->count].flowId); 
				infos[paraThread->count].seqNum = htonl(infos[paraThread->count].seqNum); 
				infos[paraThread->count].srcPort = htonl(infos[paraThread->count].srcPort);
                infos[paraThread->count].destPort = htonl(infos[paraThread->count].destPort);
                infos[paraThread->count].txTime1 = *(unsigned int*) ptrTimeSec;
                infos[paraThread->count].txTime2 = *(unsigned int*) ptrTimeUsec;
                infos[paraThread->count].txTime3 = 0;
                infos[paraThread->count].rxTime1 = htonl(RcvTime.tv_sec % 86400L);
                infos[paraThread->count].rxTime2 = htonl(RcvTime.tv_usec);
                infos[paraThread->count].rxTime3 = 0;
			}

         
		 paraThread->count++;
                
		 PRINTD(2,"Log buffer level %d \n", paraThread->count);
		 PRINTD(2,"logCheck %d\n",logCheck);

		if (paraThread->count == DIM) {
		
		if (logCheck !=0 )
			flushBuffer((ofstream *) paraThread->fileLog, infos , DIM);
		
		else
			if (logRemote !=0 )	{
				if ( sendto(paraThread->logSock, (char *) infos,
					DIM * sizeof(struct info), 0, paraThread->logHost->ai_addr,
					paraThread->logHost->ai_addrlen) < 0)
					reportErrorAndExit("tcpSock","sendto","Cannot send log infos to LogServer");
					PRINTD(1,"SCTP Pkt Receiver : Sent infos to LogServer\n");
				}
				
				paraThread->count = 0;
			}	
		}	

		
		if (paraThread->meter == METER_RTTM) {
			if (sendto(newSock, (char *) payload, size, 0, SrcAddress.ai_addr,
			    SrcAddress.ai_addrlen) < 0)
					reportErrorAndExit("sctpSock","sendto","Cannot send payload back for rttm");
			PRINTD(2,"SCTP Pkt Receiver : Sent RTTM infos\n");
			}	
	}	
	return NULL;	
}	
#endif

#ifdef DCCP
void *dccpSock(void *param)
{
	HANDLE hComm;
	int sock = 0;
	int sendCheck = 0;
	unsigned char payload[MAX_PAYLOAD_SIZE];
	struct info *infos = (struct info *) malloc(DIM * sizeof(info));
	struct addrinfo SrcAddress;
	in_port_t tmpPort = 0;
	int newSock = 0;
	int size = 0;
	int optval = 0; 
	struct timeval RcvTime;




	unsigned char *ptrSeqNum = payload + sizeof(uint32_t);			
	unsigned char *ptrTimeSec = ptrSeqNum + sizeof(uint32_t);		
	unsigned char *ptrTimeUsec = ptrTimeSec + sizeof(uint32_t);		

	paramThread *paraThread;
	paraThread = (paramThread *) param;

	paraThread->addressInfos = infos;
	paraThread->count = 0;
	if (strcmp(paraThread->serial,"noSerial")!=0) {
		hComm = serialUp(paraThread->serial);
		if (hComm == INVALID_HANDLE_VALUE)
			printf("Error opening interface %s \n", paraThread->serial);
    }
	sock = socket(paraThread->destHost.ai_family, SOCK_DCCP, 0);
	if (sock < 0)
		reportErrorAndExit("dccpSock","socket","Cannot create a DCCP socket on port");
	if (bind(sock, paraThread->destHost.ai_addr, paraThread->destHost.ai_addrlen) < 0) {
		printf(" Error into bind function\n");
		struct pipeMsg msg;
		msg.code = MSG_FT_ERR1;
		msg.flowId = paraThread->flowId;
		if (sendPipeMsg(paraThread->rPipe, &msg) < 0)
			printf(" sending msg error");
		sleep(INFINITE);
	}
	
	
	
	
	
	if (setsockopt(sock, SOL_DCCP, DCCP_SOCKOPT_SERVICE, &optval, sizeof(optval)) < 0)
		reportErrorAndExit("dccpSock", "setsockopt", "Cannot set option DCCP_SERVICE");
	
	if (listen(sock, SOMAXCONN) < 0)
		reportErrorAndExit("dccpSock","listen","Cannot listen on a port");
	else {
		
#ifdef WIN32
		int len=paraThread->destHost.ai_addrlen;
		getsockname(sock,paraThread->destHost.ai_addr,&len);
		paraThread->destHost.ai_addrlen=len;
#else
		getsockname(sock,paraThread->destHost.ai_addr, &(paraThread->destHost.ai_addrlen));
#endif
		GET_PORT((&(paraThread->destHost)), tmpPort);
		fprintf(stderr, "Listening on port: %d\n", ntohs(tmpPort));
		fflush(stderr);
		struct pipeMsg msg;
		msg.code = MSG_FT_OK;
		msg.flowId = paraThread->flowId;
		if (sendPipeMsg(paraThread->rPipe, &msg) < 0) {
			printf("error into sending msg to signal manager");
		}
	}
	SrcAddress.ai_family = paraThread->destHost.ai_family;
	if (SrcAddress.ai_family == PF_INET) {
		SrcAddress.ai_addr = (struct sockaddr *) malloc(sizeof(struct sockaddr_in));
		SrcAddress.ai_addrlen = sizeof(struct sockaddr_in);
	} else if (SrcAddress.ai_family == PF_INET6) {
			SrcAddress.ai_addr = (struct sockaddr *) malloc(sizeof(struct sockaddr_in6));
			SrcAddress.ai_addrlen = sizeof(struct sockaddr_in6);
		}
	if ((newSock = accept(sock, SrcAddress.ai_addr, (socklen_t *) & SrcAddress.ai_addrlen)) < 0)
		reportErrorAndExit("dccpSock","accept","Cannot accept connection");
	if ( closeSock(sock) == -1)
		reportErrorAndExit("dccpSock","closeSock","Cannot close socket sock");
	paraThread->socketClose = newSock;
	int firstpacket =1;
	char HelpSrcAddress[INET6_ADDRSTRLEN];
	char HelpDstAddress[INET6_ADDRSTRLEN];
	int tmpPort_SrcPort;
	int tmpPort_DstPort;
	TSTART(_tstart, secs, msecs, first, 0, RECEIVER);
	while (1) {
		size = recv(newSock, (char *) payload, MAX_PAYLOAD_SIZE, 0);
		if (size < 0)
			reportErrorAndExit("udpSock","recvfrom","Cannot receive UDP packets");
		else if (size>0) {
			PRINTD(2,"DCCP Pkt Receiver : Received DCCP packet, size %d\n", size);
			if (hComm != INVALID_HANDLE_VALUE){
				DTR_Disable(hComm);
				DTR_Enable(hComm);
			}
			GET_TIME_OF_DAY(&RcvTime, _tend, _tstart, secs, msecs, 0, RECEIVER);
			if ((logCheck!=0) || (logRemote != 0)){
				if (firstpacket == 1) {
					getInfo(&SrcAddress, tmpPort_SrcPort, HelpSrcAddress);
					getInfo(&paraThread->destHost, tmpPort_DstPort, HelpDstAddress);
					firstpacket = 0;
				}


			if (logCheck != 0){
				
				
				int net_TimeSec = ntohl(*(uint32_t *) ptrTimeSec); 
				int net_TimeUsec = ntohl(*(uint32_t *) ptrTimeUsec); 

				if ( paraThread ->payloadLogType == PL_STANDARD ) 
					writeInBufferStandard(&infos[paraThread->count],*(unsigned int *) payload,*(unsigned int *) ptrSeqNum, HelpSrcAddress, HelpDstAddress, tmpPort_SrcPort, tmpPort_DstPort, net_TimeSec, RcvTime.tv_sec % 86400L, net_TimeUsec, RcvTime.tv_usec, size);
				else if ( paraThread ->payloadLogType == PL_SHORT ) 
					writeInBufferShort(&infos[paraThread->count],*(unsigned int *) payload,*(unsigned int *) ptrSeqNum, HelpSrcAddress, HelpDstAddress, tmpPort_SrcPort, tmpPort_DstPort, RcvTime.tv_sec % 86400L, RcvTime.tv_usec, size);
				else 
					writeInBufferNone(&infos[paraThread->count],*(unsigned int *) payload, HelpSrcAddress, HelpDstAddress, tmpPort_SrcPort, tmpPort_DstPort,RcvTime.tv_sec % 86400L, RcvTime.tv_usec, size);

				
				
				
				
				
			}
			
			
			
			if ( logRemote != 0){
				if ( paraThread ->payloadLogType == PL_STANDARD ) 
					writeInBufferStandard(&infos[paraThread->count],*(unsigned int *) payload,*(unsigned int *) ptrSeqNum, HelpSrcAddress, HelpDstAddress, tmpPort_SrcPort, tmpPort_DstPort, 0, 0, 0,0,htonl(size));
				else if ( paraThread ->payloadLogType == PL_SHORT ) 
					writeInBufferShort(&infos[paraThread->count],*(unsigned int *) payload,*(unsigned int *) ptrSeqNum, HelpSrcAddress, HelpDstAddress, tmpPort_SrcPort, tmpPort_DstPort, 0, 0, htonl(size));
				else 
					writeInBufferNone(&infos[paraThread->count],*(unsigned int *) payload, HelpSrcAddress, HelpDstAddress, tmpPort_SrcPort, tmpPort_DstPort, 0, 0, htonl(size));

				infos[paraThread->count].flowId = htonl(infos[paraThread->count].flowId); 
				infos[paraThread->count].seqNum = htonl(infos[paraThread->count].seqNum); 
				infos[paraThread->count].srcPort = htonl(infos[paraThread->count].srcPort);
                infos[paraThread->count].destPort = htonl(infos[paraThread->count].destPort);
                infos[paraThread->count].txTime1 = *(unsigned int*) ptrTimeSec;
                infos[paraThread->count].txTime2 = *(unsigned int*) ptrTimeUsec;
                infos[paraThread->count].txTime3 = 0;
                infos[paraThread->count].rxTime1 = htonl(RcvTime.tv_sec % 86400L);
                infos[paraThread->count].rxTime2 = htonl(RcvTime.tv_usec);
                infos[paraThread->count].rxTime3 = 0;
			}

         		






				paraThread->count++;
				if (paraThread->count == DIM) {
					if (logCheck !=0 )
						flushBuffer((ofstream *) paraThread->fileLog, infos , DIM);
					else if (logRemote !=0 ){
						if ( sendto(paraThread->logSock,(char *) infos, DIM*sizeof(struct info), 0, paraThread->logHost->ai_addr, paraThread->logHost->ai_addrlen) < 0)
							reportErrorAndExit("dccpSock","sendto","Cannot send log infos to LogServer");
						PRINTD(1,"DCCP Pkt Receiver : Sent infos to LogServer\n");
						}
					paraThread->count = 0;
				}
			}
			if (paraThread->meter == METER_RTTM) {
				do {
					PRINTD(2,"DCCP Pkt Receiver : Trying to send RTTM infos\n");
					sendCheck = sendto(newSock, (char *) payload, size, 0, SrcAddress.ai_addr, SrcAddress.ai_addrlen);
				} while (sendCheck < 0 && errno == EAGAIN);
				if (sendCheck < 0)
					reportErrorAndExit("dccpSock","sendto","Cannot send payload back for rttm");
				PRINTD(2,"DCCP Pkt Receiver : Sent RTTM infos\n");
			}
		} 
	} 
	return NULL;	
} 
#endif
