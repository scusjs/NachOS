// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	is in machine.h.
//----------------------------------------------------------------------

/*
come on yaan.
*/
void ExceptionHandler(ExceptionType which)
{
    int type = kernel->machine->ReadRegister(2); //��ȡϵͳ���ô���
    //ϵͳ���ô���SC_Halt������r2�Ĵ�����

    DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

    switch (which) {
    case SyscallException:
      switch(type) {
	case SC_Write:
 		DEBUG(dbgSys,"Write from buffer to consoleOutput"<<kernel->machine->ReadRegister(4)<<kernel->machine->ReadRegister(5)<<kernel->machine->ReadRegister(6)<<"\n");
		char wbuffer[60];//the buffer to store information from  userprogram
		int size ;
		size=(int)kernel->machine->ReadRegister(5);//register 5 save the address of next process
		OpenFileId printOut;
		printOut= (OpenFileId)kernel->machine->ReadRegister(6);//The id of the file that will be write.
		int addressOne; 
		addressOne=(int)kernel->machine->ReadRegister(4);//register 4 save the address of prev process
		int i;
  		i=0;
		do{
 			kernel->machine->ReadMem(addressOne++,1,(int*)&wbuffer[i++]);
		}while(i<size);
		int n1;
 		n1=SysWrite((char*)wbuffer,size,printOut);//write wbuffer to printOut
		kernel->machine->WriteRegister(2,n1);
		/* Modify return point */
		{
			/* set previous programm counter (debugging only)*/
			kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
			/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
			kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
			/* set next programm counter for brach execution */
			kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
		} return;
		ASSERTNOTREACHED();
		break;


	case SC_Read:
		DEBUG(dbgSys, "read from buffer" << kernel->machine->ReadRegister(4) << kernel->machine->ReadRegister(5)<< kernel->machine->ReadRegister(6) << "\n");
		int rsize ;
		rsize=(int)kernel->machine->ReadRegister(5);//register 5 save the address of next process
		OpenFileId enterIn;
		enterIn = (OpenFileId)kernel->machine->ReadRegister(6);//The id of the file that will be write.
		char addressTwo;
		addressTwo = (char)kernel->machine->ReadRegister(4);//register 4 save the address of prev process
		int local ;
		local= (int)kernel->machine->ReadRegister(4);
		int n2 ;
		n2= SysRead(&addressTwo,rsize,enterIn);//read from addressTwo to enterIn
		kernel->machine->WriteMem(local,1,(int)addressTwo);
		kernel->machine->WriteRegister(2,n2);
 		/* Modify return point */
		{
			/* set previous programm counter (debugging only)*/
			kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
			/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
			kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
			/* set next programm counter for brach execution */
			kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
		}
		return;
		ASSERTNOTREACHED();
		break;


	case SC_Exec:
		DEBUG(dbgSys,"execute a command"<<kernel->machine->ReadRegister(4)<<"\n");
		char ebuffer[60];
		int addressThree;
		addressThree = kernel->machine->ReadRegister(4);//register 4 save the address of prev process
		int c;
		c = 0;
		do{
			kernel->machine->ReadMem(addressThree++,1,(int*)&ebuffer[c++]);
		}while(c<60);
		kernel->machine->WriteRegister(2,SysExec(ebuffer));
		/* Modify return point */
		{
			/* set previous programm counter (debugging only)*/
			kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
			/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
			kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
			/* set next programm counter for brach execution */
			kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
		}
		return;
		ASSERTNOTREACHED();
		break;

	case SC_Join:
		DEBUG(dbgSys,"jion "<<kernel->machine->ReadRegister(4)<<"\n");
              
		kernel->machine->WriteRegister(2,SysJoin((SpaceId)kernel->machine->ReadRegister(4)));
		/* Modify return point */
		{
			/* set previous programm counter (debugging only)*/
			kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
			/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
			kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
			/* set next programm counter for brach execution */
			kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
		}
		return;
		ASSERTNOTREACHED();
		break;
	case SC_Strncmp:
		DEBUG(dbgSys, "compare two string " << kernel->machine->ReadRegister(4) <<kernel->machine->ReadRegister(5) << kernel->machine->ReadRegister(6) << "\n");
		int addressFour;
		addressFour = kernel->machine->ReadRegister(4);//register 4 save the address of prev process
		int addressFive;
		addressFive = kernel->machine->ReadRegister(5);//register 5 save the address of next process
		char str1buffer[60];
		char str2buffer[60];
		int d,e;
		d= 0;
		e=0;
		do{
			kernel->machine->ReadMem(addressFour++,1,(int*)&str1buffer[d++]);//read form addressFour to str1buffer
		}while(d<60);
		--addressFour;
 		do{ 
			kernel->machine->ReadMem(addressFive++,1,(int*)&str2buffer[e++]);
		}while(e<60);
		--addressFive;
		int n3; 
		n3=SysStrncmp(str1buffer,str2buffer,kernel->machine->ReadRegister(6));
		kernel->machine->WriteRegister(2,n3);
		/* Modify return point */
		{
			/* set previous programm counter (debugging only)*/
			kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
			/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
			kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
			/* set next programm counter for brach execution */
			kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
		}
		return;
		ASSERTNOTREACHED();
		break;

	
		
	case SC_Halt:                 //������Haltϵͳ����
		DEBUG(dbgSys, "Shutdown, initiated by user program.\n");

		SysHalt();

		ASSERTNOTREACHED();
		break;

	case SC_Add:
		DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");
	
		/* Process SysAdd Systemcall*/
		int result;
		result = SysAdd(/* int op1 */(int)kernel->machine->ReadRegister(4),
			/* int op2 */(int)kernel->machine->ReadRegister(5));

		DEBUG(dbgSys, "Add returning with " << result << "\n");
		/* Prepare Result */
		kernel->machine->WriteRegister(2, (int)result);
	
		/* Modify return point */
		{
			/* set previous programm counter (debugging only)*/
			kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

			/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
			kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
	  
			/* set next programm counter for brach execution */
			kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
		}

		return;
	
		ASSERTNOTREACHED();

		break;

	default:
		cerr << "Unexpected system call " << type << "\n";
		break;
	}
      break;
      default:
        cerr << "Unexpected user mode exception" << (int)which << "\n";
        break;
    }
    ASSERTNOTREACHED();
}
