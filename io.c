/***********************************************************
  * File Name: io.c
  * Author: Somnath Mukherjee
  * Description:
  * C functions for input and output
  **********************************************************/

#ifndef IO_C
#define IO_C

#include "defs.h"
#include "stdio.h"

/**
  * @brief Function to print a Square
  *
  * @param sq Square to print
  * @return The algebric representation of the square
  */
char *PrSq(const int sq) {

	static char SqStr[3];

	int file = FilesBrd[sq];
	int rank = RanksBrd[sq];

	sprintf(SqStr, "%c%c", ('a'+file), ('1'+rank));

	return SqStr;

}

/**
  * @brief Function to return a Move string
  *
  * @param move Move to print
  * @return The algebric representation of the move, like e2e4
  */
char *PrMove(const int move) {

	static char mvStr[7];

    // File and Rank from
	int ff = FilesBrd[FROMSQ(move)];
	int rf = RanksBrd[FROMSQ(move)];
	// File andRank to
	int ft = FilesBrd[TOSQ(move)];
	int rt = RanksBrd[TOSQ(move)];

	int promoted = PROMOTED(move);

	if(promoted) {
		char pchar = 'q';
		if(IsKn(promoted)) {
			pchar = 'n';
		} else if(IsRQ(promoted) && !IsBQ(promoted))  {
			pchar = 'r';
		} else if(!IsRQ(promoted) && IsBQ(promoted))  {
			pchar = 'b';
		}
		snprintf(mvStr, sizeof(mvStr), "%c%c%c%c%c", ('a'+ff), ('1'+rf), ('a'+ft), ('1'+rt), pchar);
	} else {
		snprintf(mvStr, sizeof(mvStr), "%c%c%c%c", ('a'+ff), ('1'+rf), ('a'+ft), ('1'+rt));
	}

	return mvStr;
}

/**
  * @brief Function to return a Move string in algebric notation
  *
  * @param move Move to print
  * @return The algebric representation of the move, like e4, Nc3 etc
  */
char *PrAlgMove(const int move, const S_BOARD *pos) {

	static char mvStrAlg[7];

    // File and Rank from
	int ff = FilesBrd[FROMSQ(move)];
	int rf = RanksBrd[FROMSQ(move)];
	// File andRank to
	int ft = FilesBrd[TOSQ(move)];
	int rt = RanksBrd[TOSQ(move)];

	int promoted = PROMOTED(move);

	int fromSq = FR2SQ(ff,rf);
	int toSq = FR2SQ(ft,rt);

	int pceInFromSq = pos->pieces[fromSq];
	//int pceInToSq = pos->pieces[toSq];

	char pieceInFromSqChar = PceCharNoSide[pceInFromSq];
	//char pieceInToSqChar = PceCharNoSide[pceInToSq];

	if(promoted) {
		char promPiece = 'Q';
		if(IsKn(promoted)) {
			promPiece = 'N';
		} else if(IsRQ(promoted) && !IsBQ(promoted))  {
			promPiece = 'R';
		} else if(!IsRQ(promoted) && IsBQ(promoted))  {
			promPiece = 'B';
		}

		if(MFLAGCAP & move) {
            snprintf(mvStrAlg, sizeof(mvStrAlg), "%cx%c%c=%c", ('a'+ff), ('a'+ft), ('1'+rt), promPiece);
		} else {
            snprintf(mvStrAlg, sizeof(mvStrAlg), "%c%c=%c", ('a'+ft), ('1'+rt), promPiece);
		}
	} else {
	    if(MFLAGCAP & move) {
            if(pieceInFromSqChar == 'P') {
                snprintf(mvStrAlg, sizeof(mvStrAlg), "%cx%c%c", ('a'+ff), ('a'+ft), ('1'+rt));
            } else {
                snprintf(mvStrAlg, sizeof(mvStrAlg), "%cx%c%c", pieceInFromSqChar, ('a'+ft), ('1'+rt));
            }
	    } else if(pieceInFromSqChar == 'K' && (MFLAGCA & move)) {
                if(toSq == 27 || toSq == 97) {
                    snprintf(mvStrAlg, sizeof(mvStrAlg), "O-O");
                } else if(toSq == 23 || toSq == 93) {
                    snprintf(mvStrAlg, sizeof(mvStrAlg), "O-O-O");
                }
	    } else {
	        if(pieceInFromSqChar == 'P') {
                    snprintf(mvStrAlg, sizeof(mvStrAlg), "%c%c", ('a'+ft), ('1'+rt));
	        } else {
	            snprintf(mvStrAlg, sizeof(mvStrAlg), "%c%c%c", pieceInFromSqChar, ('a'+ft), ('1'+rt));
	        }
	    }
	}

	return mvStrAlg;
}

/**
  * @brief Function to print a move
  *
  * @param *move Pointer to the move list
  */
void PrintMove(const int move) {
    printf("Move: > %s\n", PrMove(move));
}

/**
  * Function to print Move List
  *
  * @param *move Pointer to the move list
  */
void PrintMoveList(const S_MOVELIST *list, const S_BOARD *pos) {
	int index = 0;
	int score = 0;
	int move = 0;

	printf("MoveList:\n");

	for(index = 0; index < list->count; ++index) {

		move = list->moves[index].move;
		score = list->moves[index].score;

		printf("Move:%d > %s (%s, score:%d)\n", index+1, PrMove(move), PrAlgMove(move, pos), score);
	}

	printf("MoveList Total %d Moves:\n\n", list->count);
}

/**
  * @brief Function to parse a move and return the move integer
  *
  * @param *ptrChar Input move a2a4, h7h8q etc
  * @param *pos Pointer to the board structure
  * @return Numeric representation of the parsed move
  */
int ParseMove(char* ptrChar, S_BOARD *pos) {
    // Return no move for invalid move input
    if(ptrChar[1] > '8' || ptrChar[1] < '1') {
        return NOMOVE;
    }
    if(ptrChar[3] > '8' || ptrChar[3] < '1') {
        return NOMOVE;
    }
    if(ptrChar[0] > 'h' || ptrChar[0] < 'a') {
        return NOMOVE;
    }
    if(ptrChar[2] > 'h' || ptrChar[2] < 'a') {
        return NOMOVE;
    }

    // Get from and to squares
    int from = FR2SQ(ptrChar[0] - 'a', ptrChar[1] - '1');
    int to = FR2SQ(ptrChar[2] - 'a', ptrChar[3] - '1');

    //printf("ptrChar: %s from: %d to: %d\n", ptrChar, from, to);

    ASSERT(SqOnBoard(from) && SqOnBoard(to));

    S_MOVELIST list[1];
    GenerateAllMoves(pos, list);
    int moveNum = 0;
    int move = 0;
    int promotedPiece = EMPTY;

    // Loop through all the move
    for(moveNum = 0; moveNum < list->count; ++moveNum) {
            // Get the move
        move = list->moves[moveNum].move;

        //PrintMove(move);

        // If the move matches with what is entered
        if(FROMSQ(move) == from && TOSQ(move) == to) {
            // Get the promoted piece
            promotedPiece = PROMOTED(move);
            if(promotedPiece != EMPTY) {
                if(IsRQ(promotedPiece) && !IsBQ(promotedPiece) && ptrChar[4] == 'r') {
                    return move;
                } else if(!IsRQ(promotedPiece) && IsBQ(promotedPiece) && ptrChar[4] == 'b') {
                    return move;
                } else if(IsRQ(promotedPiece) && IsBQ(promotedPiece) && ptrChar[4] == 'q') {
                    return move;
                } else if(IsKn(promotedPiece) && ptrChar[4] == 'n') {
                    return move;
                }

                continue;
            }

            return move;
        }
    }

    return NOMOVE;
}

#endif // IO_C
