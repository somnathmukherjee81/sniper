/***********************************************************
  * File Name: init.c
  * Author: Somnath Mukherjee
  * Description:
  * C functions for initializing the sniper engine.
  **********************************************************/

#ifndef INIT_C
#define INIT_C

#include "defs.h"
#include "stdlib.h"

/**
  * Macro to fill 64 bits with random numbers
  * rand() generates 15 bit random number
  *
  * 0000 000000000000000 000000000000000 000000000000000 000000000000000
  *
  * 0000 000000000000000 000000000000000 000000000000000 111111111111111
  * 0000 000000000000000 000000000000000 111111111111111 000000000000000
  * 0000 000000000000000 111111111111111 000000000000000 000000000000000
  * 0000 111111111111111 000000000000000 000000000000000 000000000000000
  * 1111 000000000000000 000000000000000 000000000000000 000000000000000
  *
  * 1111 111111111111111 111111111111111 111111111111111 111111111111111
  */
#define RAND_64 ((U64)rand() +  \
                 ((U64)rand() << 15) +    \
                 ((U64)rand() << 30) +    \
                 ((U64)rand() << 45) +    \
                 (((U64)rand() & 0xf) << 60))

int Sq120ToSq64[BRD_SQ_NUM];
int Sq64ToSq120[BRD_SQUARES];
U64 SetMask[BRD_SQUARES];
U64 ClearMask[BRD_SQUARES];

U64 PiecesKeys[NUM_PIECES][BRD_SQ_NUM];
U64 SideKey;
U64 CastleKeys[16];

// Square to File mapping
int FilesBrd[BRD_SQ_NUM];
// Square to Rank mapping
int RanksBrd[BRD_SQ_NUM];

/**
  * @brief Function to initialize the Files and Rank mapping from square
  */
void InitFilesRanksBrd() {
    int index = 0;
    int file = FILE_A;
    int rank = RANK_1;
    int sq = A1;

    for(index = 0 ; index < BRD_SQ_NUM ; ++index) {
        FilesBrd[index] = OFFBOARD;
        RanksBrd[index] = OFFBOARD;
    }

    for(rank = RANK_1 ; rank <= RANK_8 ; ++rank) {
        for(file = FILE_A ; file <= FILE_H ; ++file) {
            sq = FR2SQ(file, rank);
            FilesBrd[sq] = file;
            RanksBrd[sq] = rank;
        }
    }
}

/**
  * @brief Function to initialize the hash keys
  */
void InitHashKeys() {
    int pieceIndex = 0;
    int squareIndex = 0;
    int castleIndex = 0;

    for(pieceIndex = 0 ; pieceIndex < NUM_PIECES ; ++pieceIndex) {
        for(squareIndex = 0 ; squareIndex < BRD_SQ_NUM ; ++squareIndex) {
            PiecesKeys[pieceIndex][squareIndex] = RAND_64;
        }
    }

    SideKey = RAND_64;

    for(castleIndex = 0 ; castleIndex < 16 ; ++castleIndex) {
        CastleKeys[castleIndex] = RAND_64;
    }
}

/**
  * @brief Function to initialize the bit masks - SetMask and ClearMask
  */
void InitBitMasks() {
    int index = 0;

    for(index = 0 ; index < BRD_SQUARES ; index++) {
        SetMask[index] = 0ULL;
        ClearMask[index] = 0ULL;
    }

    for(index = 0 ; index < BRD_SQUARES ; index++) {
        SetMask[index] |= (ONE_BIT << index);
        ClearMask[index] = ~SetMask[index];
    }
}

/**
  * @brief Function to initialize the mapping arrays between logical and physical boards - Sq120ToSq64 and Sq64ToSq120
  */
void InitMapping() {
    int index = 0;
    int file = FILE_A;
    int rank = RANK_1;
    int sq = A1;
    int sq64 = 0;

    for(index = 0 ; index < BRD_SQ_NUM ; ++index) {
        Sq120ToSq64[index] = 65;
    }

    for(index = 0 ; index < BRD_SQUARES ; ++index) {
        Sq64ToSq120[index] = 120;
    }

    for(rank = RANK_1 ; rank <= RANK_8 ; ++rank) {
        for(file = FILE_A ; file <= FILE_H ; ++file) {
            sq = FR2SQ(file, rank);
            Sq64ToSq120[sq64] = sq;
            Sq120ToSq64[sq] = sq64;
            sq64++;
        }
    }
}

/**
  * @brief Function to initialize the Sniper
  */
void AllInit() {
    InitMapping();
    InitBitMasks();
    InitHashKeys();
    InitFilesRanksBrd();
    InitMvvLva();
}

#endif // INIT_C
