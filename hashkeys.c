/***********************************************************
  * File Name: hashkeys.c
  * Author: Somnath Mukherjee
  * Description:
  * C functions for the hash keys or position keys
  **********************************************************/

#ifndef HASHKEYS_C
#define HASHKEYS_C

#include "defs.h"
#include "stdio.h"

/**
  * @brief Function to generate hash key for a position
  *
  * @param *pos Pointer to a position in the game
  * @return Position Key/Hash Key (U64) for the position
  */
U64 GeneratePosKey(const S_BOARD *pos) {
    int sq = 0;
    U64 finalKey = 0;
    int piece = EMPTY;

    // Pieces
    for(sq = 0 ; sq < BRD_SQ_NUM; ++sq) {
        // Get the piece from the square
        piece = pos->pieces[sq];
        // If square is valid and it is not empty
        if(piece != NO_SQ && piece != EMPTY && piece != OFFBOARD) {
            ASSERT(piece>=wP && piece<=bK);
            // XOR the piece key for the piece and square
            finalKey ^= PiecesKeys[piece][sq];
        }
    }

    // If White to move then hash in the Side Key
    if(pos->side == WHITE) {
        finalKey ^= SideKey;
    }

    // If en passant square is set
    if(pos->enPas != NO_SQ) {
        ASSERT(pos->enPas>=0 && pos->enPas<BRD_SQ_NUM);
        // Hash in the piece key for empty piece for the en passant square
        finalKey ^= PiecesKeys[EMPTY][pos->enPas];
    }

    ASSERT(pos->castlePerm>=0 && pos->castlePerm<=15);

    // Hash in castle permissions
    finalKey ^= CastleKeys[pos->castlePerm];

    return finalKey;
}

#endif // HASHKEYS_C
