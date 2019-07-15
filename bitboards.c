/***********************************************************
  * File Name: bitboards.c
  * Author: Somnath Mukherjee
  * Description:
  * C functions for the bit board data structure.
  **********************************************************/

#ifndef BITBOARDS_C
#define BITBOARDS_C

#include "stdio.h"
#include "defs.h"

const int BitTable[BRD_SQUARES]= {
    63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,
    51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35, 62, 31, 40, 4, 49, 5, 52,
    26, 60, 6, 23, 44, 46, 27, 56, 16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28,
    58, 20, 37, 17, 36, 8
};

/**
  * @brief Function to pop bit from bitboard
  *
  * See: https://chessprogramming.wikispaces.com/BitScan
  *
  * @param *bitBoard Pointer to the Bit Board
  * @return 64 based index (integer) of the first set bit (1) starting at the least significant bit in the Bit Board
  *
  * This function takes the first set bit (1) starting at the least significant bit in the Bit Board
  * and returns the 64 based index of this bit, so we know which square the bit was set on and sets that bit to zero.
  */
int PopBit(U64 *bitBoard) {
    // The xor with the ones' decrement, bb ^ (bb-1) contains all bits set including and below the LS1B
    U64 b = *bitBoard ^ (*bitBoard - 1);

    unsigned int fold = (unsigned) ((b & 0xffffffff) ^ (b >> 32));
    *bitBoard &= (*bitBoard - 1);

    return BitTable[(fold * 0x783a9b23) >> 26];
}

/**
  * @brief  Function to count the number of set bits (1) in the Bit Board
  *
  * @param bitBoard The Bit Board
  * @return The number (integer) of set bits (1) in the Bit Board
  *
  * Brian Kernighan's way: https://chessprogramming.wikispaces.com/Population+Count#Loop-Approaches-Brian Kernighan's way
  *
  * Consecutively reset LS1B in a loop body and counting loop cycles until the bitset becomes empty.
  * Brian Kernighan mentioned the trick in his and Ritchie's book The C Programming_Language, 2nd Edition 1988, exercise 2-9.
  * However, the method was first published in 1960 by Peter Wegner, discovered independently by Derrick Henry Lehmer, published in 1964
  */
int CountBits(U64 bitBoard) {
    int count;

    /*
    bitBoard: 10110101, 10110100, 10110000, 10100000, 10000000
    bitBoard - 1: 10110100, 10110011, 10101111, 10011111, 1111111
    bitBoard & bitBoard - 1: 10110100, 10110000, 10100000, 10000000, 00000000
    Count: 1, 2, 3, 4, 5
    */
    for(count = 0 ; bitBoard ; count++, RESET_LS1B_64(bitBoard));

    return count;
}

/**
  * @brief Function to print the Bit Board
  *
  * @param bb The Bit Board
  */
void PrintBitBoard(U64 bb) {
    // U64 shiftMe = ONE_BIT;

    // Index for the rank
    int rank = 0;
    // Index for the file
    int file = 0;
    // 120 based index of the square
    // int sq = 0;
    // 64 based square of the square
    int sq64 = 0;
    // Bit mask for a square
    // U64 bitMask = 0ULL;

    printf("\n");

    // Loop through all the ranks starting with Rank 8
    for(rank = RANK_8 ; rank >= RANK_1 ; --rank) {
        // Loop through all files starting with File a
        for(file = FILE_A ; file <= FILE_H ; ++file) {
            /*
            // Get 120 based index of the square
            // sq = FR2SQ(file,rank);
            // Get 64 based index of the square
            // sq64 = SQ64(sq);
            */
            // Get 64 based index of the square
            sq64 = FR2SQ64(file,rank);

            // Bit mask for the square
            // bitMask = shiftMe << sq64;

            // bitMask & bb
            if(IS_SET_64(bb, sq64)) {
                // printf("%c", 248);
                printf("x\t");
            } else {
                // printf("%c", 249);
                printf("-\t");
            }
        }

        printf("\n");
    }

    printf("\n");
    printf("\n");
}

#endif // BITBOARDS_C
