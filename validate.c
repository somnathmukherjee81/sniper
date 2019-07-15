/***********************************************************
  * File Name: validate.c
  * Author: Somnath Mukherjee
  * Description:
  * C functions for validations
  **********************************************************/

#ifndef VALIDATE_C
#define VALIDATE_C

#include "defs.h"

/**
  * Function to validate that a square is on board
  *
  * @param sq Square to validate
  */
int SqOnBoard(const int sq) {
	return FilesBrd[sq]==OFFBOARD ? 0 : 1;
}

/**
  * Function to validate the side
  *
  * @param side Side to validate
  */
int SideValid(const int side) {
	return (side==WHITE || side == BLACK) ? 1 : 0;
}

/**
  * Function to validate Files or Ranks
  *
  * @param fr Files or Rank to validate
  */
int FileRankValid(const int fr) {
	return (fr >= 0 && fr <= 7) ? 1 : 0;
}

/**
  * Function to validate piece or empty
  *
  * @param pce Piece or empty to validate
  */
int PieceValidEmpty(const int pce) {
	return (pce >= EMPTY && pce <= bK) ? 1 : 0;
}

/**
  * Function to validate piece
  *
  * @param pce Piece to validate
  */
int PieceValid(const int pce) {
	return (pce >= wP && pce <= bK) ? 1 : 0;
}

#endif // VALIDATE_C
