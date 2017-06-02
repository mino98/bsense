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



#include "thread.h"
#include "debug.h"




int createThread(void *argument, void *(nameFunction) (void *), void *attrib, pthread_t &idThread)
{
	
	int ret = 0;
#ifdef WIN32	
	
	void *pid;
	
	(HANDLE)idThread =
	    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) nameFunction, argument, (DWORD) NULL,
	    (unsigned long *) &pid);
    PRINTD(2,"Return value CreateThread (WIN32) %d\n",(int)idThread);
	if (idThread == NULL) ret = -1;
	PRINTD(2,"Return value createThread : %d\n",ret);
#endif

#ifdef LINUX_OS
	
	 
	ret = pthread_create(&idThread, NULL, nameFunction, argument);
	if (ret != 0) ret = -1;
    PRINTD(2,"Return value pthread_create (LINUX) %d\n",(int)idThread);
    PRINTD(2,"Return value createThread : %d\n",ret);
#endif
	return ret;
};


int terminateThread(pthread_t idThread)
{
	
	int ret = 0;
#ifdef WIN32
	
	ret = TerminateThread((HANDLE) idThread, 0);
	if (ret == 0) ret = -1;
	else ret = 0;
#endif
#ifdef LINUX_OS
	
	ret = pthread_cancel(idThread);
	if (ret != 0) ret = -1;
#endif
    PRINTD(2,"Return value terminateThread : %d\n",ret);
	return ret;
}; 


void exitThread()
{
#ifdef WIN32
	
	ExitThread(0);
#endif
#ifdef LINUX_OS
	
	pthread_exit(0);
#endif
};


int joinThread(int numFlow, pthread_t hThr[])
{
	int ret = 0;
#ifdef WIN32
	DWORD temp = 0;
	
	temp = WaitForMultipleObjects(numFlow, (const HANDLE *) hThr, TRUE, INFINITE);
    PRINTD(2,"Return value WaitForMultipleObjects(WIN32) : %d\n",(int)temp);
	if (temp == WAIT_FAILED) ret = -1;
#endif
#ifdef LINUX_OS
	
	int ret2 = 0;
	for(int i = 0; i < numFlow; i++) {
		ret2 = pthread_join(hThr[i], NULL);
		if (ret2 != 0)
			ret = -1;
	}
#endif
    PRINTD(2,"Return value joinThread : %d\n",ret);
    return ret;
};



#ifdef WIN32

int mutexThreadInit(HANDLE &mutex)
{
	int ret = 0;
	
	mutex = CreateSemaphore(NULL,2,2,"MYMUTEX");
	PRINTD(2,"Return value CreateSemaphore (WIN32) : %d \n",(int)mutex);
	if (mutex == NULL) ret = -1;
	PRINTD(2,"Return value mutexThreadInit : %d \n",ret);
	return ret;
}
#endif

#ifdef LINUX_OS

int mutexThreadInit(void* mutex)
{
	int ret = 0;
	
	ret = pthread_mutex_init((pthread_mutex_t *)mutex, NULL);
	PRINTD(2,"Return value pthread_mutex_init (LINUX) : %d \n",mutex);
	PRINTD(2,"Return value mutexThreadInit : %d \n",ret);
	return ret;
}
#endif
	

int mutexThreadLock(void *mutex)
{
	int ret = 0;
#ifdef LINUX_OS
	
	ret = pthread_mutex_lock((pthread_mutex_t *) mutex);
	if (ret != 0 ) ret = -1;
#endif
#ifdef WIN32
	DWORD temp = 0;
	
	PRINTD(1,"Try to lock mutex : %d\n",(int)mutex);
	temp = WaitForSingleObject((HANDLE)mutex,INFINITE);
    PRINTD(2,"Return value WaitForSingleObject(WIN32) : %d\n",(int)temp);
	if (temp == WAIT_FAILED) ret = -1;
#endif
    PRINTD(2,"Return value mutexThreadLock : %d\n",ret);
	return ret;
};


int mutexThreadUnlock(void *mutex)
{
	int ret = 0;
#ifdef LINUX_OS
	
	ret = pthread_mutex_unlock((pthread_mutex_t *) mutex);
	if (ret != 0 ) ret = -1;
#endif
#ifdef WIN32
	BOOL flag = 0;
	LONG cPreviousCount;

	PRINTD(1,"Try to unlock mutex : %d\n",(int)mutex);
	flag = ReleaseSemaphore((HANDLE)mutex,1,&cPreviousCount);
	PRINTD(1,"Previous Count value : %d\n",cPreviousCount);
    PRINTD(2,"Return value ReleaseSemaphore(WIN32) in mutexThreadUnlock : %d\n",flag);
	if (flag == 0) ret = -1;
#endif
    PRINTD(2,"Return value mutexThreadUnlock : %d\n",ret);
	return ret;
};



int mutexThreadRelease(void* mutex)
{
	int ret = 0;
#ifdef LINUX_OS
	
	ret = pthread_mutex_destroy((pthread_mutex_t *)mutex);
	if (ret != 0) ret = -1;
#endif
#ifdef WIN32
		BOOL flag = 0;
		
		flag = CloseHandle((HANDLE)mutex);
		if (flag == 0) ret = -1;
#endif
    PRINTD(2,"Return value mutexThreadRelease : %d\n",ret);
	return ret;
}





int closeSock(int socket)
{
	int ret = 0;
#ifdef LINUX_OS
	
	ret = close(socket);
#endif
#ifdef WIN32
	
	ret = closesocket(socket);
	if (ret == SOCKET_ERROR) ret = -1;
#endif
    PRINTD(2,"Return value closeSock : %d\n",ret);
	return ret;
};
