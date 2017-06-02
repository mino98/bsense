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

static int socket_r = 0;

#ifdef WIN32
int InitializeWinsock(WORD wVersionRequested)
{
	WSADATA wsaData;
	int err;

	err = WSAStartup(wVersionRequested, &wsaData);

	
	
	if (err != 0)
		return 0;	

	
	
	
	

	
	
	
	
	

	
	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
		return 0;

	
	return 1;
}


char nameProgram[]="ITGapi";
#endif



int DITGsend(char *sender, char *message)
{
#ifdef WIN32
	if (InitializeWinsock(MAKEWORD(1,1)) != 1)	{
		fprintf(stderr, "WSAStartup() failed");
		exit(1);
	}
#endif

	struct addrinfo* locale = 0;
	
	freeaddrinfo(locale);


	
	getaddrinfo("0.0.0.0", NULL, NULL, &locale);
	socket_r = socket(locale->ai_family, SOCK_DGRAM, 0);
	if (socket_r  < 0)
		printf("error into create a socket  - DITGSend function \n");	

#ifdef LINUX_OS
	int flags;
	flags = fcntl(socket_r, F_GETFL, 0);
	fcntl(socket_r, F_SETFL, flags | O_NONBLOCK);
#endif

#ifdef WIN32
	unsigned long i = 1;
	ioctlsocket(socket_r, FIONBIO, &i);
#endif


	
	
	
	


	struct sockaddr_in Sender;
        struct hostent *host;

       Sender.sin_family = AF_INET;
        if (!(host = gethostbyname(sender))) {
                cerr << endl << "DITGsend: Invalid destination address" << endl;
                return -1;
        }
        memcpy((char *) &Sender.sin_addr, host->h_addr, host->h_length);
        Sender.sin_port = htons(DEFAULT_PORT_SENDER_MANAGER);
        if (sendto(socket_r, message, strlen(message), 0, (struct sockaddr *) &Sender,  sizeof(Sender)) != (int) strlen(message))
                return -1;
        return 0;
}


int catchManagerMsg(char **senderIP, char **msg)
{
        const int MaxBufferSize = 300;
        struct sockaddr_in Sender;
        socklen_t SenderSlen = sizeof(Sender);
        int length, msgtype;
        if (!socket_r)
                return MNG_NOMSG;
        char *buffer = (char *) malloc(sizeof(char) * MaxBufferSize);
        if (recvfrom(socket_r, buffer, MaxBufferSize, 0, (struct sockaddr *) &Sender, &SenderSlen) < 0) {
                free(buffer);
                return MNG_NOMSG;
        }
        *senderIP = (char *) malloc(sizeof(char) * 30);
        strcpy(*senderIP, inet_ntoa(Sender.sin_addr));
        msgtype = *(int *) buffer;
        length = ((int *) buffer)[1];
        memmove(buffer, ((int *) buffer) + 2, length);
        buffer[length] = '\0';
        *msg = buffer;
        return msgtype;
}





