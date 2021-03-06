/***********************************************************
  * File Name: xboard.c
  * Author: Somnath Mukherjee
  * Description:
  * C functions for the xboard protocol
  **********************************************************/

#ifndef XBOARD_C
#define XBOARD_C

#include "stdio.h"
#include "defs.h"
#include "string.h"

/**
  * Function for checking three-fold repetition
  *
  * @param *pos Pointer to the board structure
  */
int ThreeFoldRep(const S_BOARD *pos) {
	int i = 0, r = 0;
	for (i = 0; i < pos->hisPly; ++i)	{
	    if (pos->history[i].posKey == pos->posKey) {
		    r++;
		}
	}
	return r;
}

/**
  * Function for checking insufficient material
  *
  * @param *pos Pointer to the board structure
  */
int DrawMaterial(const S_BOARD *pos) {

    if (pos->pceNum[wP] || pos->pceNum[bP]) {
            return FALSE;
    }

    if (pos->pceNum[wQ] || pos->pceNum[bQ] || pos->pceNum[wR] || pos->pceNum[bR]) {
            return FALSE;
    }

    if (pos->pceNum[wB] > 1 || pos->pceNum[bB] > 1) {
            return FALSE;
    }

    if (pos->pceNum[wN] > 1 || pos->pceNum[bN] > 1) {
            return FALSE;
    }

    if (pos->pceNum[wN] && pos->pceNum[wB]) {
            return FALSE;
    }

    if (pos->pceNum[bN] && pos->pceNum[bB]) {
            return FALSE;
    }

    return TRUE;
}

/**
  * Function for checking results, if the game is over
  *
  * @param *pos Pointer to the board structure
  */
int checkresult(S_BOARD *pos) {

    if (pos->fiftyMove > 100) {
            printf("1/2-1/2 {fifty move rule (claimed by Sniper)}\n");
            return TRUE;
    }

    if (ThreeFoldRep(pos) >= 2) {
            printf("1/2-1/2 {3-fold repetition (claimed by Sniper)}\n");
            return TRUE;
    }

	if (DrawMaterial(pos) == TRUE) {
            printf("1/2-1/2 {insufficient material (claimed by Sniper)}\n");
            return TRUE;
    }

	S_MOVELIST list[1];
    GenerateAllMoves(pos,list);

    int moveNum = 0;
	int found = 0;
	for(moveNum = 0; moveNum < list->count; ++moveNum) {

        if ( !MakeMove(pos,list->moves[moveNum].move))  {
            continue;
        }
        found++;
		TakeMove(pos);
		break;
    }

	if(found != 0) {
            return FALSE;
	}

	int inCheck = SqAttacked(pos->kingSq[pos->side],pos->side^1,pos);

	if(inCheck == TRUE)	{
	    if(pos->side == WHITE) {
                printf("0-1 {black mates (claimed by Sniper)}\n");
                return TRUE;
        } else {
                printf("0-1 {white mates (claimed by Sniper)}\n");
                return TRUE;
        }
    } else {
        printf("\n1/2-1/2 {stalemate (claimed by Sniper)}\n");
        return TRUE;
    }

	return FALSE;
}

/**
  * Function for printing options
  *
  */
void PrintOptions() {
	printf("feature ping=1 setboard=1 colors=0 usermove=1\n");
	printf("feature done=1\n");
}

/**
  * Function for main XBoard loop
  * XBoard (Chess Engine Communication Protocol) protocol in http://www.open-aurec.com/wbforum/WinBoard/engine-intf.html
  *
  * @param *pos Pointer to the board structure
  * @param *info Pointer to the search info
  */
void XBoard_Loop(S_BOARD *pos, S_SEARCHINFO *info) {

	info->GAME_MODE = XBOARDMODE;
	info->POST_THINKING = TRUE;
	setbuf(stdin, NULL);
    setbuf(stdout, NULL);
	PrintOptions(); // HACK

	int depth = -1, movestogo[2] = {30,30 }, movetime = -1;
	int time = -1, inc = 0;
	int engineSide = BOTH;
	int timeLeft;
	int sec;
	int mps;
	int move = NOMOVE;
	int i, score;
	char inBuf[80], command[80];

	engineSide = BLACK;
	ParseFen(START_FEN, pos);
	depth = -1;
	time = -1;

	while(TRUE) {

		fflush(stdout);

		if(pos->side == engineSide && checkresult(pos) == FALSE) {
			info->starttime = GetTimeMs();
			info->depth = depth;

			if(time != -1) {
				info->timeset = TRUE;
				time /= movestogo[pos->side];
				time -= 50;
				info->stoptime = info->starttime + time + inc;
			}

			if(depth == -1 || depth > MAXDEPTH) {
				info->depth = MAXDEPTH;
			}

			printf("time:%d start:%d stop:%d depth:%d timeset:%d movestogo:%d mps:%d\n",
				time,info->starttime,info->stoptime,info->depth,info->timeset, movestogo[pos->side], mps);
				SearchPosition(pos, info);

			if(mps != 0) {
				movestogo[pos->side^1]--;
				if(movestogo[pos->side^1] < 1) {
					movestogo[pos->side^1] = mps;
				}
			}

		}

		fflush(stdout);

		memset(&inBuf[0], 0, sizeof(inBuf));
		fflush(stdout);
		if (!fgets(inBuf, 80, stdin))
		continue;

		sscanf(inBuf, "%s", command);

		printf("command seen:%s\n",inBuf);

		if(!strcmp(command, "quit")) {
			info->quit = TRUE;
			break;
		}

		if(!strcmp(command, "force")) {
			engineSide = BOTH;
			continue;
		}

		if(!strcmp(command, "protover")){
			PrintOptions();
		    continue;
		}

		if(!strcmp(command, "sd")) {
			sscanf(inBuf, "sd %d", &depth);
		    printf("DEBUG depth:%d\n",depth);
			continue;
		}

		if(!strcmp(command, "st")) {
			sscanf(inBuf, "st %d", &movetime);
		    printf("DEBUG movetime:%d\n",movetime);
			continue;
		}

		if(!strcmp(command, "time")) {
			sscanf(inBuf, "time %d", &time);
			time *= 10;
		    printf("DEBUG time:%d\n",time);
			continue;
		}

		if(!strcmp(command, "level")) {
			sec = 0;
			movetime = -1;
			if( sscanf(inBuf, "level %d %d %d", &mps, &timeLeft, &inc) != 3) {
			  sscanf(inBuf, "level %d %d:%d %d", &mps, &timeLeft, &sec, &inc);
		      printf("DEBUG level with :\n");
			}	else {
		      printf("DEBUG level without :\n");
			}
			timeLeft *= 60000;
			timeLeft += sec * 1000;
			movestogo[0] = movestogo[1] = 30;
			if(mps != 0) {
				movestogo[0] = movestogo[1] = mps;
			}
			time = -1;
		    printf("DEBUG level timeLeft:%d movesToGo:%d inc:%d mps%d\n",timeLeft,movestogo[0],inc,mps);
			continue;
		}

		if(!strcmp(command, "ping")) {
			printf("pong%s\n", inBuf+4);
			continue;
		}

		if(!strcmp(command, "new")) {
			engineSide = BLACK;
			ParseFen(START_FEN, pos);
			depth = -1;
			time = -1;
			continue;
		}

		if(!strcmp(command, "setboard")){
			engineSide = BOTH;
			ParseFen(inBuf+9, pos);
			continue;
		}

		if(!strcmp(command, "go")) {
			engineSide = pos->side;
			continue;
		}

		if(!strcmp(command, "usermove")){
			movestogo[pos->side]--;
			move = ParseMove(inBuf+9, pos);
			if(move == NOMOVE) continue;
			MakeMove(pos, move);
            pos->ply=0;
		}
    }
}

/**
  * Function for main Console loop
  *
  * @param *pos Pointer to the board structure
  * @param *info Pointer to the search info
  */
void Console_Loop(S_BOARD *pos, S_SEARCHINFO *info) {

	printf("Welcome to Sniper In Console Mode!\n");
	printf("Type help for commands\n\n");

	info->GAME_MODE = CONSOLEMODE;
	info->POST_THINKING = TRUE;
	setbuf(stdin, NULL);
    setbuf(stdout, NULL);

	int depth = MAXDEPTH, movetime = 3000;
	int engineSide = BOTH;
	int move = NOMOVE;
	char inBuf[80], command[80];

	engineSide = BLACK;
	ParseFen(START_FEN, pos);

	while(TRUE) {

		fflush(stdout);

		if(pos->side == engineSide && checkresult(pos) == FALSE) {
			info->starttime = GetTimeMs();
			info->depth = depth;

			if(movetime != 0) {
				info->timeset = TRUE;
				info->stoptime = info->starttime + movetime;
			}

			SearchPosition(pos, info);
		}

		printf("\nSniper > ");

		fflush(stdout);

		memset(&inBuf[0], 0, sizeof(inBuf));
		fflush(stdout);
		if (!fgets(inBuf, 80, stdin))
		continue;

		sscanf(inBuf, "%s", command);

		if(!strcmp(command, "help")) {
			printf("Commands:\n");
			printf("quit - quit game\n");
			printf("force - computer will not think\n");
			printf("print - show board\n");
			printf("post - show thinking\n");
			printf("nopost - do not show thinking\n");
			printf("new - start new game\n");
			printf("go - set computer thinking\n");
			printf("depth x - set depth to x\n");
			printf("time x - set thinking time to x seconds (depth still applies if set)\n");
			printf("view - show current depth and movetime settings\n");
			printf("** note ** - to reset time and depth, set to 0\n");
			printf("enter moves using b7b8q notation\n\n\n");
			continue;
		}

		if(!strcmp(command, "quit")) {
			info->quit = TRUE;
			break;
		}

		if(!strcmp(command, "post")) {
			info->POST_THINKING = TRUE;
			continue;
		}

		if(!strcmp(command, "print")) {
			PrintBoard(pos);
			continue;
		}

		if(!strcmp(command, "nopost")) {
			info->POST_THINKING = FALSE;
			continue;
		}

		if(!strcmp(command, "force")) {
			engineSide = BOTH;
			continue;
		}

		if(!strcmp(command, "view")) {
			if(depth == MAXDEPTH) printf("depth not set ");
			else printf("depth %d",depth);

			if(movetime != 0) printf(" movetime %ds\n",movetime/1000);
			else printf(" movetime not set\n");

			continue;
		}

		if(!strcmp(command, "depth")) {
			sscanf(inBuf, "depth %d", &depth);
		    if(depth==0) depth = MAXDEPTH;
			continue;
		}

		if(!strcmp(command, "time")) {
			sscanf(inBuf, "time %d", &movetime);
			movetime *= 1000;
			continue;
		}

		if(!strcmp(command, "new")) {
			engineSide = BLACK;
			ParseFen(START_FEN, pos);
			continue;
		}

		if(!strcmp(command, "go")) {
			engineSide = pos->side;
			continue;
		}

		move = ParseMove(inBuf, pos);
		if(move == NOMOVE) {
			printf("Command unknown:%s\n",inBuf);
			continue;
		}
		MakeMove(pos, move);
		pos->ply=0;
    }
}

#endif // XBOARD_C
