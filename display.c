/***********************************************************
  * File Name: display.c
  * Author: Somnath Mukherjee
  * Description:
  * C functions for display operations of the sniper engine.
  **********************************************************/

#ifndef DISPLAY_C
#define DISPLAY_C

#include "stdio.h"
#include "defs.h"

/**
  * @brief Function to display the Logical to Physical board mapping
  */
void DisplayLogicalBoardMapping() {
    printf("\n\n==================== Logical Board Mappings - Start ====================\n\n");

    int index = 0;

    for(index = 0 ; index < BRD_SQ_NUM ; ++index) {
        if(index % 10 == 0) {
            printf("\n");
        }

        printf("%5d\t", Sq120ToSq64[index]);
    }

    printf("\n\n==================== Logical Board Mappings - End ====================\n\n");
}

/**
  * @brief Function to display the Physical to Logical board mapping
  */
void DisplayPhysicalBoardMapping() {
    printf("\n\n==================== Physical Board Mappings - Start ====================\n\n");

    int index = 0;

    for(index = 0 ; index < BRD_SQUARES ; ++index) {
        if(index % 8 == 0) {
            printf("\n");
        }

        printf("%5d\t", Sq64ToSq120[index]);
    }

    printf("\n\n==================== Physical Board Mappings - End ====================\n\n");
}

/**
  * @brief Function to display the board mappings
  */
void DisplayBoardMappings() {
    printf("\n\n==================== Board Mappings - Start ====================\n\n");

    DisplayLogicalBoardMapping();

    printf("\n");
    printf("\n");

    DisplayPhysicalBoardMapping();

    printf("\n\n==================== Board Mappings - End ====================\n\n");
}

/**
  * @brief Function to display the board mappings
  */
void PrintEmptyBoard() {
    printf("\n\n==================== Print Empty Board - Start ====================\n\n");

    int file = 0;
    int rank = 0;

    for(rank = RANK_8 ; rank >= RANK_1 ; --rank) {
        // Loop through all files starting with File a
        for(file = FILE_A ; file <= FILE_H ; ++file) {
            if(ISSQLIGHT(file,rank)) {
                // printf("%c%c", 178, 178);
                printf("L\t");
            } else {
                // printf("%c%c", 177, 177);
                printf("D\t");
            }
        }

        printf("\n");
    }

    printf("\n\n==================== Print Empty Board - End ====================\n\n");
}

/**
  * @brief Function to display the squares attacked by a side
  *
  * @param side Which side is doing the attack
  * @param *pos Pointer to the board structure
  */
void ShowSqAtBySide(const int side, const S_BOARD *pos) {

	int rank = 0;
	int file = 0;
	int sq = 0;

	printf("\n\nSquares attacked by:%c\n", SideChar[side]);
	for(rank = RANK_8; rank >= RANK_1; --rank) {
		for(file = FILE_A; file <= FILE_H; ++file) {
			sq = FR2SQ(file,rank);
			if(SqAttacked(sq, side, pos)==TRUE) {
				// printf("%c", 248);
                printf("x\t");
			} else {
				// printf("%c", 249);
                printf("-\t");
			}

		}
		printf("\n");
	}
    printf("\n\n");

}

/**
  * @brief Function to display a number in binary
  *
  * @param number The number to display in binary
  */
void PrintBin(int number) {
	int index = 0;
	printf("As binary:\n");
	for(index = 27; index >= 0; index--) {
		if( (1<<index) & number) printf("1");
		else printf("0");
		if(index!=28 && index%4==0) printf(" ");
	}
	printf("\n");
}

#endif // DISPLAY_C
