/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls 
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__ 
#define __USERPROG_KSYSCALL_H__ 

#include "kernel.h"
#include "sys/types.h"
#include "sys/wait.h"

#define SHELL "/bin/sh"


void SysHalt()
{
  kernel->interrupt->Halt();
}


int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

 /*
 * Send length of size bytes in id to buffer
 */
int SysWrite(char* buffer,int size,OpenFileId id)
{
	return write(id,buffer,(size_t)size);
}

 /*
 * Send length of size bytes in buffer to file at id
 */
int SysRead(char* buffer, int size, OpenFileId id)
{
	return read(id , buffer , (size_t)size);
}
 /*
 * Creat a child process, and execute the path that SHELL stand for with "-c".
 * It's work like: /bin/sh -c filename
 */
SpaceId SysExec(char* exec_name)
 {
 	pid_t child;
	child = vfork();
	if(child==0)
	{
		execl(SHELL,SHELL,"-c",exec_name,NULL);
		_exit(EXIT_FAILURE);
	}
	else if(child<0)
	{
		return EPERM;
	}
	return (SpaceId)child;
 }
 /*
 * join a new process and pause current process until receive a signal or the new
 * process stop.
 */
 int SysJoin(SpaceId id)
 {
 	return waitpid((pid_t)id,(int*)0,0);
 }
/*
 *compare str and buffer to find if buffer is "exit" command
 */
int SysStrncmp(char* buffer,char* str, int n)
{
	return strncmp(buffer,str,(size_t)n);
}



#endif /* ! __USERPROG_KSYSCALL_H__ */
