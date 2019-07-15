/***********************************************************
  * File Name: uci.c
  * Author: Somnath Mukherjee
  * Description:
  * C functions for the UCI Protocol
  **********************************************************/

#ifndef UCI_C
#define UCI_C

#include "stdio.h"
#include "defs.h"
#include "string.h"

#define INPUTBUFFER 400 * 6

/**
  * Function for Parsing the search command
  * go depth 6 wtime 180000 btime 100000 binc 1000 winc 1000 movetime 1000 movestogo 40
  *
  * @param *line Input Line
  * @param *info Pointer to search info
  * @param *pos Pointer to the board structure
  */
void ParseGo(char* line, S_SEARCHINFO *info, S_BOARD *pos) {

    // Depth
	int depth = -1;
	// Number of moves to reach time control
	int movestogo = 30;
	// Move time
	int movetime = -1;
	// Time remaining
	int time = -1;
	// Increment
	int inc = 0;
    char *ptr = NULL;

    // Assume that we are in infinite analysis mode
	info->timeset = FALSE;

    // No need to do anything special if the 'infinite' command is received; as we are already in infinite analysis mode
	if ((ptr = strstr(line,"infinite"))) {
		;
	}

    // Process Black increment if Black to move
	if ((ptr = strstr(line,"binc")) && pos->side == BLACK) {
		inc = atoi(ptr + 5);
	}

    // Process White increment if White to move
	if ((ptr = strstr(line,"winc")) && pos->side == WHITE) {
		inc = atoi(ptr + 5);
	}

    // Process White time if White to move
	if ((ptr = strstr(line,"wtime")) && pos->side == WHITE) {
		time = atoi(ptr + 6);
	}

    // Process Black increment if Black to move
	if ((ptr = strstr(line,"btime")) && pos->side == BLACK) {
		time = atoi(ptr + 6);
	}

    // Process moves to go
	if ((ptr = strstr(line,"movestogo"))) {
		movestogo = atoi(ptr + 10);
	}

    // Process move time
	if ((ptr = strstr(line,"movetime"))) {
		movetime = atoi(ptr + 9);
	}

    // Process depth
	if ((ptr = strstr(line,"depth"))) {
		depth = atoi(ptr + 6);
	}

    // If move time was specified, use the move time as time remaining and consider one move to go
	if(movetime != -1) {
		time = movetime;
		movestogo = 1;
	}

    // Set start time
	info->starttime = GetTimeMs();
	// Set depth
	info->depth = depth;

    // If time was set
	if(time != -1) {
        // Set the timeset flag
		info->timeset = TRUE;
        // Set how much time for this move
		time /= movestogo;
		// We don't want to overrun, so take 50 ms less
		time -= 50;
		// Stop time is Start Time + time for the move + increment
		info->stoptime = info->starttime + time + inc;
	}

    // If depth is not set, set it to max depth
	if(depth == -1) {
		info->depth = MAXDEPTH;
	}

	printf("time:%d start:%d stop:%d depth:%d timeset:%d\n",
		time,info->starttime, info->stoptime, info->depth, info->timeset);

    // Call search position
	SearchPosition(pos, info);
}

/**
  * Function for Parsing a Position
  * position fen fenstr
  * position startpos
  * ... moves e2e4 e7e5 b7b8q
  *
  * @param *lineIn Input Line
  * @param *pos Pointer to the board structure
  */
void ParsePosition(char* lineIn, S_BOARD *pos) {

    // Move forward character pointer to 'fen' or 'startpos'; in other words skip the 'position' command
	lineIn += 9;
    char *ptrChar = lineIn;

    // If 'startpos' is sent, set the board to the start position
    if(strncmp(lineIn, "startpos", 8) == 0){
        ParseFen(START_FEN, pos);
    } else {
        // Move to the 'fen' part
        ptrChar = strstr(lineIn, "fen");

        // If 'fen' keyword is not there, set to start position; otherwise get the FEN and set the board to that position
        if(ptrChar == NULL) {
            ParseFen(START_FEN, pos);
        } else {
            ptrChar+=4;
            ParseFen(ptrChar, pos);
        }
    }

    // If there are moves to parse
	ptrChar = strstr(lineIn, "moves");
	int move;

	if(ptrChar != NULL) {
        ptrChar += 6;
        while(*ptrChar) {
            // Parse the move
            move = ParseMove(ptrChar, pos);

            // Break if the move is not parsed properly
            if(move == NOMOVE) {
                break;
            }

            // Make the move
            MakeMove(pos, move);
            // Set Ply Count to zero
            pos->ply=0;

            // Go to the next move
            while(*ptrChar && *ptrChar!= ' ') {
                ptrChar++;
            }

            ptrChar++;
        }
    }

    // Print the board
	PrintBoard(pos);
}

/**
  * Function for main UCI loop
  * https://ucichessengine.wordpress.com/2011/03/16/description-of-uci-protocol/
  * http://download.shredderchess.com/div/uci.zip
  *
  * @param *pos Pointer to the board structure
  * @param *info Pointer to the search info
  */
void Uci_Loop(S_BOARD *pos, S_SEARCHINFO *info) {

    // Set the input and output buffers to NULL
	setbuf(stdin, NULL);
    setbuf(stdout, NULL);

	char line[INPUTBUFFER];
    printf("id name %s\n",NAME);
    printf("id author %s\n", AUTHOR);
    printf("uciok\n");


    // Go in to infinite loop
	while (TRUE) {
		memset(&line[0], 0, sizeof(line));
        fflush(stdout);

        if (!fgets(line, INPUTBUFFER, stdin)) {
            continue;
        }

        if (line[0] == '\n') {
            continue;
        }

        if (!strncmp(line, "isready", 7)) {
            printf("readyok\n");
            continue;
        } else if (!strncmp(line, "position", 8)) {
            ParsePosition(line, pos);
        } else if (!strncmp(line, "ucinewgame", 10)) {
            ParsePosition("position startpos\n", pos);
        } else if (!strncmp(line, "go", 2)) {
            ParseGo(line, info, pos);
        } else if (!strncmp(line, "quit", 4)) {
            info->quit = TRUE;
            break;
        } else if (!strncmp(line, "uci", 3)) {
            printf("id name %s\n",NAME);
            printf("id author Somnath\n");
            printf("uciok\n");
        }

        // Quit when quit signal is received
		if(info->quit) {
                break;
		}
    }
}


#endif // UCI_C
