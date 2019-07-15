/***********************************************************
  * File Name: data.c
  * Author: Somnath Mukherjee
  * Description:
  * Global Data and constants
  **********************************************************/

#ifndef DATA_C
#define DATA_C

#include "defs.h"

char PceChar[] = ".PNBRQKpnbrqk";
char PceCharNoSide[] = ".PNBRQKPNBRQK";
char SideChar[] = "wb-";
char RankChar[] = "12345678";
char FileChar[] = "abcdefgh";

int PieceBig[NUM_PIECES] = { FALSE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE };
int PieceMaj[NUM_PIECES] = { FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE };
int PieceMin[NUM_PIECES] = { FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE };
int PieceVal[NUM_PIECES] = { 0, 100, 325, 325, 550, 1000, 50000, 100, 325, 325, 550, 1000, 50000 };
int PieceCol[NUM_PIECES] = { BOTH, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK };

int PiecePawn[NUM_PIECES] = { FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE };
int PieceKnight[NUM_PIECES] = { FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE };
int PieceKing[NUM_PIECES] = { FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE };
int PieceRookQueen[NUM_PIECES] = { FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE };
int PieceBishopQueen[NUM_PIECES] = { FALSE, FALSE, FALSE, TRUE, FALSE, TRUE, FALSE, FALSE, FALSE, TRUE, FALSE, TRUE, FALSE };
int PieceSlides[NUM_PIECES] = { FALSE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE };

/*int PieceRook[NUM_PIECES] = { FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE };
int PieceQueen[NUM_PIECES] = { FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE };
int PieceBishop[NUM_PIECES] = { FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE };
int PieceBishop[NUM_PIECES] = { FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE };*/

/**
  * @brief Square Names
  */
char* SquareNames[] = {
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1", // 1st Rank
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2", // 2nd Rank
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3", // 3rd Rank
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4", // 4th Rank
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5", // 5th Rank
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6", // 6th Rank
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7", // 7th Rank
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8", // 8th Rank
    "-" /**<  Invalid Square or No Square*/,
    "X" /**<  Off Board Square*/
};

#endif // DATA_C
