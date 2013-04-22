#include "syscall.h"


int Strncmp(char *buffer,char* str,int size)
{
	int i;
	OpenFileId output=ConsoleOutput;
	for(i = 0 ; i<size; i++ )
	{
		if((buffer[i++])!=(str[i++]))
		return 1;
	}
	return 0;
}

int
main()
{
	SpaceId newProc;
	OpenFileId input = ConsoleInput;
	OpenFileId output = ConsoleOutput;
	char prompt[2], ch, buffer[60];
	int i;
	Write( "<-My shell ver 1.0->\n", 21, output);


	prompt[0] = '-';
	prompt[1] = '-';

	while( 1 )
	{
		Write(prompt, 2, output);

		i = 0;

		do {

			Read(&buffer[i], 1, input);

		} while( buffer[i++] != '\n' );
		buffer[--i] = '\0';
		if((i==4)&&(Strncmp(buffer,"exit",4)==0))
		{
			Halt();
		}

		if( i > 0 ) {
			newProc = Exec(buffer);
			Join(newProc);
		}
	}
}

