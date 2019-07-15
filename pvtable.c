/***********************************************************
  * File Name: pvtable.c
  * Author: Somnath Mukherjee
  * Description:
  * Principal Variation table
  **********************************************************/

#ifndef PVTABLE_C
#define PVTABLE_C

#include "defs.h"
#include "stdio.h"

/**
  * Size of the Principal Variation table, 2 MB
  */
const int PvSize = 0x100000 * 2;

/**
  * Function to clear the Principal Variation table
  *
  * @param *table Principal Variation Table
  */
void ClearPvTable(S_PVTABLE *table) {

  S_PVENTRY *pvEntry;

  for (pvEntry = table->pTable; pvEntry < table->pTable + table->numEntries; pvEntry++) {
    // Clear the position key
    pvEntry->posKey = 0ULL;
    // Clear the move
    pvEntry->move = NOMOVE;
  }
}

/**
  * Function to initialize the Principal Variation table
  *
  * @param *table Principal Variation Table
  */
void InitPvTable(S_PVTABLE *table) {
    // Initialize number of entries as total PvTable size / size of one entry
    table->numEntries = PvSize / sizeof(S_PVENTRY);
    // Reduce 2 for indexing purpose, for safety
    table->numEntries -= 2;
    // Release the memory
    free(table->pTable);
    // Allocate memory
    table->pTable = (S_PVENTRY *) malloc(table->numEntries * sizeof(S_PVENTRY));
    // Clear the table
    ClearPvTable(table);

    printf("\nPvTable initialization complete with %d entries\n", table->numEntries);

}

/**
  * Function to store a move in Principal Variation table
  *
  * @param *pos Board position
  * @param move Move to store
  */
void StorePvMove(const S_BOARD *pos, const int move) {
    // Get the index between 0 and number of entries
	int index = pos->posKey % pos->PvTable->numEntries;
	ASSERT(index >= 0 && index <= pos->PvTable->numEntries - 1);

    // Store the move
	pos->PvTable->pTable[index].move = move;
	// Store the position key
    pos->PvTable->pTable[index].posKey = pos->posKey;
}

/**
  * Function to probe the Principal Variation table
  *
  * @param *pos Board position
  * @param move Move to store
  */
int ProbePvTable(const S_BOARD *pos) {
    // Get the index between 0 and number of entries
	int index = pos->posKey % pos->PvTable->numEntries;
	ASSERT(index >= 0 && index <= pos->PvTable->numEntries - 1);

    // If the position key is same at this index, return the move as Principal Variation move
	if(pos->PvTable->pTable[index].posKey == pos->posKey ) {
		return pos->PvTable->pTable[index].move;
	}

	return NOMOVE;
}
/**
  * Function to get the Principal Variation line
  * Returns the count of moves it managed to put into the PvTable
  *
  * @param depth Current depth
  * @param *pos Board position
  */
int GetPvLine(const int depth, S_BOARD *pos) {

	ASSERT(depth < MAXDEPTH);

    // Probe the PvTable
	int move = ProbePvTable(pos);
	int count = 0;

    // Loop while a move exists
	while(move != NOMOVE && count < depth) {

		ASSERT(count < MAXDEPTH);

        // If the move exists
		if(MoveExists(pos, move)) {
            // Make the move
			MakeMove(pos, move);
            // Store the move and increment the count
			pos->PvArray[count++] = move;
		} else {
		    // Break if illegal move
			break;
		}

		// Probe the next move
		move = ProbePvTable(pos);
	}

    // Take back all the moves we made
	while(pos->ply > 0) {
		TakeMove(pos);
	}

    // Return the count
	return count;

}

#endif // PVTABLE_C
