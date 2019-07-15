/***********************************************************
  * File Name: perft.c
  * Author: Somnath Mukherjee
  * Description:
  * Test for make move and take move
  **********************************************************/

#ifndef PERFT_C
#define PERFT_C

#include "defs.h"
#include "stdio.h"

/**
  * Leaf Nodes count
  */
long leafNodes;

/**
  * Function to run perft testing
  *
    * @param depth Depth to test
  * @param *pos Board position
  */
void Perft(int depth, S_BOARD *pos) {

    ASSERT(CheckBoard(pos));

    // If depth is zero terminate recursion after incrementing leaf nodes count
	if(depth == 0) {
        leafNodes++;
        return;
    }

    S_MOVELIST list[1];
    // Generate all the moves for the current position
    GenerateAllMoves(pos,list);

    int MoveNum = 0;
    // Loop through all the moves in this position
	for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {
        // Ignore illegal moves
        if (!MakeMove(pos,list->moves[MoveNum].move))  {
            continue;
        }

        // Call recursively again with reduced depth
        Perft(depth - 1, pos);
        // take the move back
        TakeMove(pos);
    }

    // Return when moves lits is exhausted
    return;
}


/**
  * Function to drive perft testing with depth details
  *
  * @param depth Depth to test
  * @param *pos Board position
  */
void PerftTest(int depth, S_BOARD *pos) {

    ASSERT(CheckBoard(pos));

	PrintBoard(pos);
	printf("\nStarting Test To Depth:%d\n",depth);
	// Reset leaf nodes count
	leafNodes = 0;

    // Get start time
	int startTime = GetTimeMs();

    S_MOVELIST list[1];
    // Generate all moves
    GenerateAllMoves(pos,list);

    int move;
    int MoveNum = 0;

    // Loop through all the moves in this position
	for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {
        // Get the move
        move = list->moves[MoveNum].move;

        // Ignore illegal moves
        if ( !MakeMove(pos,move))  {
            continue;
        }

        // Initialize cumulative leaf nodes count
        long cumnodes = leafNodes;
        // Run perft with reduced depth
        Perft(depth - 1, pos);
        // Take back the move
        TakeMove(pos);
        // Get the current leaf nodes count
        long oldnodes = leafNodes - cumnodes;
        printf("move %d : %s (%s) : %ld\n", MoveNum+1, PrMove(move), PrAlgMove(move, pos), oldnodes);
    }

    int endTime = GetTimeMs();
	printf("\nTest Complete : %ld nodes visited in %dms\n", leafNodes, endTime - startTime);

    return;
}

#endif // PERFT_C
