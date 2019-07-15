/***********************************************************
  * File Name: sniper.c
  * Author: Somnath Mukherjee
  * Description:
  * C functions for the sniper engine. Entry point of the application
  **********************************************************/

#ifndef SNIPER_C
#define SNIPER_C

#include "stdio.h"
#include "stdlib.h"
#include "defs.h"

/**
  * Entry point of the sniper engine
  *
  * @return Exit code of the sniper engine
  */
int main()
{
    // Initialize sniper
    AllInit();

    S_BOARD pos[1];
    S_SEARCHINFO info[1];

    // Initialize PV Table
    InitPvTable(pos->PvTable);

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    printf("Welcome to Sniper! Type 'sniper' for console mode...\n");

    // For unit testing
    // Test();

    char line[256];

	while (TRUE) {
		memset(&line[0], 0, sizeof(line));

		fflush(stdout);
		if (!fgets(line, 256, stdin))
			continue;

		if (line[0] == '\n')
			continue;
		if (!strncmp(line, "uci",3)) {
		    // Go to UCI Loop
			Uci_Loop(pos, info);
			if(info->quit == TRUE) break;
			continue;
		} else if (!strncmp(line, "xboard",6))	{
		    // Go to XBoard Loop
			XBoard_Loop(pos, info);
			if(info->quit == TRUE) break;
			continue;
		} else if (!strncmp(line, "vice",4))	{
			Console_Loop(pos, info);
			if(info->quit == TRUE) break;
			continue;
		} else if(!strncmp(line, "quit",4))	{
			break;
		}
	}

    // Free PV table
    free(pos->PvTable->pTable);

    return 0;
}

#endif // SNIPER_C
