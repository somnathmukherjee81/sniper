/***********************************************************
  * File Name: attack.c
  * Author: Somnath Mukherjee
  * Description:
  * C functions for the square attacked functions
  **********************************************************/

#ifndef ATTACK_C
#define ATTACK_C

#include "defs.h"
#include "stdio.h"

const int KnDir[8] = {-8, -19, -21, -12, 8, 19, 21, 12};
const int RkDir[4] = {-1, -10, 1, 10};
const int BiDir[4] = {-9, -11, 11, 9};
const int KiDir[8] = {-1, -10,	1, 10, -9, -11, 11, 9};

/**
  * @brief Function to check is a square is attacked by a side
  *
  * @param sq Square which we are testing
  * @param side Which side is doing the attack
  * @param *pos Pointer to the board structure
  */
int SqAttacked(const int sq, const int side, const S_BOARD *pos) {

	int pce, index, t_sq, dir;

    ASSERT(SqOnBoard(sq));
	ASSERT(SideValid(side));
	ASSERT(CheckBoard(pos));

	// Pawns
	if(side == WHITE) {
		if(pos->pieces[sq - 11] == wP || pos->pieces[sq - 9] == wP) {
			return TRUE;
		}
	} else {
		if(pos->pieces[sq + 11] == bP || pos->pieces[sq + 9] == bP) {
			return TRUE;
		}
	}

    // Knights
	for(index = 0; index < 8; ++index) {
		pce = pos->pieces[sq + KnDir[index]];
		if(pce != OFFBOARD && pce != EMPTY && IsKn(pce) && PieceCol[pce]==side) {
			return TRUE;
		}
	}

	// Rooks or Queens
	for(index = 0; index < 4; ++index) {
		dir = RkDir[index];
		t_sq = sq + dir;
		pce = pos->pieces[t_sq];

		// Go until it goes off-board or hits a piece
		while(pce != OFFBOARD) {
			if(pce != EMPTY) {
				if(IsRQ(pce) && PieceCol[pce] == side) {
					return TRUE;
				}
				break;
			}

			t_sq += dir;
			pce = pos->pieces[t_sq];
		}
	}

    // Bishops or Queens
	for(index = 0; index < 4; ++index) {
		dir = BiDir[index];
		t_sq = sq + dir;
		pce = pos->pieces[t_sq];

        // Go until it goes off-board or hits a piece
		while(pce != OFFBOARD) {
			if(pce != EMPTY) {
				if(IsBQ(pce) && PieceCol[pce] == side) {
					return TRUE;
				}
				break;
			}

			t_sq += dir;
			pce = pos->pieces[t_sq];
		}
	}

	// Kings
	for(index = 0; index < 8; ++index) {
		pce = pos->pieces[sq + KiDir[index]];
		if(pce != OFFBOARD && pce != EMPTY && IsKi(pce) && PieceCol[pce]==side) {
			return TRUE;
		}
	}

	return FALSE;
}

#endif // ATTACK_C
