/***********************************************************
  * File Name: board.c
  * Author: Somnath Mukherjee
  * Description:
  * C functions for the board
  **********************************************************/

#ifndef BOARD_C
#define BOARD_C

#include "defs.h"
#include "stdio.h"

/**
  * @brief Function to check or validate the board/position
  *
  * @param *pos Pointer to the board structure
  */
int CheckBoard(const S_BOARD *pos) {

    // Temporary variables to hold information to mirror what we are parsing
	int t_pceNum[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int t_bigPce[2] = {0, 0};
	int t_majPce[2] = {0, 0};
	int t_minPce[2] = {0, 0};
	int t_material[2] = {0, 0};

	int sq64,t_piece,t_pce_num,sq120,colour,pcount;

    // Array to store the pawn bit-board
	U64 t_pawns[3] = {0ULL, 0ULL, 0ULL};

	t_pawns[WHITE] = pos->pawns[WHITE];
	t_pawns[BLACK] = pos->pawns[BLACK];
	t_pawns[BOTH] = pos->pawns[BOTH];

	// Check piece lists
	// Loop by the piece type
	for(t_piece = wP; t_piece <= bK; ++t_piece) {
        // Then loop by the piece number for each type
		for(t_pce_num = 0; t_pce_num < pos->pceNum[t_piece]; ++t_pce_num) {
		    // Get the square for the current piece
			sq120 = pos->pList[t_piece][t_pce_num];
            // Validate the piece
			ASSERT(pos->pieces[sq120]==t_piece);
		}
	}

	// check piece count and other counters
	for(sq64 = 0; sq64 < 64; ++sq64) {
		sq120 = SQ120(sq64);
		// Get the piece on this square
		t_piece = pos->pieces[sq120];
		// Increment number of pieces for that piece type
		t_pceNum[t_piece]++;
		// Get the color of the piece
		colour = PieceCol[t_piece];
		// Update the big, major and minor pieces
		if(PieceBig[t_piece] == TRUE) t_bigPce[colour]++;
		if(PieceMin[t_piece] == TRUE) t_minPce[colour]++;
		if(PieceMaj[t_piece] == TRUE) t_majPce[colour]++;

        // Update the material value
		t_material[colour] += PieceVal[t_piece];
	}

    // Validate each piece type
	for(t_piece = wP; t_piece <= bK; ++t_piece) {
		ASSERT(t_pceNum[t_piece]==pos->pceNum[t_piece]);
	}

	// Check bit-boards count for pawns
	pcount = CNT(t_pawns[WHITE]);
	ASSERT(pcount == pos->pceNum[wP]);
	pcount = CNT(t_pawns[BLACK]);
	ASSERT(pcount == pos->pceNum[bP]);
	pcount = CNT(t_pawns[BOTH]);
	ASSERT(pcount == (pos->pceNum[bP] + pos->pceNum[wP]));

	// Check bit-boards squares for White Pawns
	while(t_pawns[WHITE]) {
		sq64 = POP(&t_pawns[WHITE]);
		ASSERT(pos->pieces[SQ120(sq64)] == wP);
	}

    // Check bit-boards squares for Black Pawns
	while(t_pawns[BLACK]) {
		sq64 = POP(&t_pawns[BLACK]);
		ASSERT(pos->pieces[SQ120(sq64)] == bP);
	}

    // Check bit-boards squares for Both Pawns
	while(t_pawns[BOTH]) {
		sq64 = POP(&t_pawns[BOTH]);
		ASSERT((pos->pieces[SQ120(sq64)] == bP) || (pos->pieces[SQ120(sq64)] == wP));
	}

    // Validate material, minor pieces, major pieces, big pieces
	ASSERT(t_material[WHITE]==pos->material[WHITE] && t_material[BLACK]==pos->material[BLACK]);
	ASSERT(t_minPce[WHITE]==pos->minPce[WHITE] && t_minPce[BLACK]==pos->minPce[BLACK]);
	ASSERT(t_majPce[WHITE]==pos->majPce[WHITE] && t_majPce[BLACK]==pos->majPce[BLACK]);
	ASSERT(t_bigPce[WHITE]==pos->bigPce[WHITE] && t_bigPce[BLACK]==pos->bigPce[BLACK]);

    // Validate side to move and position key
	ASSERT(pos->side==WHITE || pos->side==BLACK);
	ASSERT(GeneratePosKey(pos)==pos->posKey);

    // Validate en passant
	ASSERT(pos->enPas==NO_SQ || ( RanksBrd[pos->enPas]==RANK_6 && pos->side == WHITE)
		 || ( RanksBrd[pos->enPas]==RANK_3 && pos->side == BLACK));

    // Validate King position
	ASSERT(pos->pieces[pos->kingSq[WHITE]] == wK);
	ASSERT(pos->pieces[pos->kingSq[BLACK]] == bK);

	return TRUE;
}

/**
  * @brief Function to reset the board/position structure
  *
  * @param *pos Pointer to the board structure
  *
  * Removes all the pieces off the board and resets all the values inside the structure
  */
void ResetBoard(S_BOARD *pos) {
    int index = 0;

    // Set all the squares to off board
    for(index = 0 ; index < BRD_SQ_NUM ; ++index) {
        pos->pieces[index] = OFFBOARD;
    }

    // Set the squares in the valid board to empty
    for(index = 0 ; index < BRD_SQUARES ; ++index) {
        pos->pieces[SQ120(index)] = EMPTY;
    }

    // Reset the big pieces, major pieces, minor pieces
    for(index = 0 ; index < NUM_COLORS - 1 ; ++index) {
        pos->bigPce[index] = 0;
        pos->majPce[index] = 0;
        pos->minPce[index] = 0;
        pos->material[index] = 0;
    }

    // Reset the pawns
    for(index = 0 ; index < NUM_COLORS ; ++index) {
        pos->pawns[index] = 0ULL;
    }

    // Set the number of pieces to zero
    for(index = 0 ; index < NUM_PIECES ; ++index) {
        pos->pceNum[index] = 0;
    }

    // Set the king's square for both sides to no square
    pos->kingSq[WHITE] = pos->kingSq[BLACK] = NO_SQ;

    // Reset side to move
    pos->side = BOTH;
    // Reset en passant square
    pos->enPas = NO_SQ;
    // Reset fifty move counter
    pos->fiftyMove = 0;

    // Reset ply counter
    pos->ply = 0;
    // Reset ply counter history
    pos->hisPly = 0;

    // Reset castle permissions
    pos->castlePerm = 0;

    // Reset position key/hash key
    pos->posKey = 0ULL;
}

/**
  * @brief Function to initialize a board
  *
  * @param *pos Board Position
  */
void InitializeBoard(S_BOARD *board) {
    board->PvTable->pTable = NULL;
    // Initialize Principal Variation table
	InitPvTable(board->PvTable);
}

/**
  * @brief Function to clear a board
  *
  * @param *pos Board Position
  */
void ClearBoard(S_BOARD *board) {
    free(board->PvTable->pTable);
}

/**
  * @brief Parse an FEN position and set the Board
  *
  * @param *fen FEN position string
  * @param *pos Board Position
  * @return 0 on success, -1 on failure
  *
  * TODO: Parse Halfmove clock and Fullmove counter
  */
int ParseFen(char *fen, S_BOARD *pos) {
    ASSERT(fen!=NULL);
    ASSERT(pos!=NULL);

    int rank = RANK_8;
    int file = FILE_A;
    int piece = 0;
    int count = 0;
    int index = 0;
    int sq64 = 0;
    int sq120 = 0;

    ResetBoard(pos);

    while((rank >= RANK_1) && *fen) {
        count = 1;

        switch(*fen) {
            // It's a piece
            // It's Black Pawn
            case 'p' : piece = bP; break;
            // It's Black Rook
            case 'r' : piece = bR; break;
            // It's Black Knight
            case 'n' : piece = bN; break;
            // It's Black Bishop
            case 'b' : piece = bB; break;
            // It's Black King
            case 'k' : piece = bK; break;
            // It's Black Queen
            case 'q' : piece = bQ; break;
            // It's White Pawn
            case 'P' : piece = wP; break;
            // It's White Rook
            case 'R' : piece = wR; break;
            // It's White Knight
            case 'N' : piece = wN; break;
            // It's White Bishop
            case 'B' : piece = wB; break;
            // It's White King
            case 'K' : piece = wK; break;
            // It's White Queen
            case 'Q' : piece = wQ; break;

            // It's one or more empty square
            case '1' :
            case '2' :
            case '3' :
            case '4' :
            case '5' :
            case '6' :
            case '7' :
            case '8' :
                piece = EMPTY;
                count = *fen - '0';
                break;

            // It's end of rank or end of pieces list
            case '/':
            case ' ':
                // Go to next rank
                rank--;
                // Reset to a-file
                file = FILE_A;
                // Go to next character in FEN
                fen++;
                continue;

            default:
                printf("FEN Error.\n");
                return -1;
        }

        // Set the piece or keep empty
        for(index = 0 ;index < count ; index++) {
            if(piece != EMPTY) {
                sq64 = SQINDEX_64(file, rank);
                sq120 = SQ120(sq64);
                pos->pieces[sq120] = piece;
            }

            file++;
        }

        // Go to next character in FEN
        fen++;
    }

    ASSERT(*fen=='w' || *fen=='b');

    // Set the side
    pos->side = (*fen == 'w') ? WHITE : BLACK;
    fen += 2;

    // Set Castling Permissions
    for(index = 0 ; index < 4 ; index++) {
        if(*fen == ' ') {
            break;
        }

        switch(*fen) {
            // White has king-side castling permission
            case 'K': pos->castlePerm |= WKCA; break;
            // White has queen-side castling permission
            case 'Q': pos->castlePerm |= WQCA; break;
            // Black has king-side castling permission
            case 'k': pos->castlePerm |= BKCA; break;
            // Black has queen-side castling permission
            case 'q': pos->castlePerm |= BQCA; break;
            // No castling permission
            default: break;
        }

        fen++;
    }

    fen++;

    ASSERT(pos->castlePerm >= 0 && pos->castlePerm <= 15);

    // Set En Passant square
    if(*fen != '-') {
        file = fen[0] - 'a';
        rank = fen[1] - '1';

        ASSERT(file >= FILE_A && file <= FILE_H);
        ASSERT(rank >= RANK_1 && rank <= RANK_8);

        pos->enPas = FR2SQ(file, rank);
    }

    // Generate the position key / hash key
    pos->posKey = GeneratePosKey(pos);

    // Update the material list
    UpdateListsMaterial(pos);

    return 0;
}

/**
  * @brief Function to print the board position
  *
  * @param *pos Board position
  */
void PrintBoard(const S_BOARD *pos) {

	int sq, file, rank, piece;

	printf("\nGame Board:\n\n");

	for(rank = RANK_8 ; rank >= RANK_1 ; rank--) {
		printf("%d\t", rank+1);
		for(file = FILE_A ; file <= FILE_H ; file++) {
			sq = FR2SQ(file, rank);
			piece = pos->pieces[sq];
			printf("%c\t",PceChar[piece]);
		}

		printf("\n\n");
	}

	printf("\n\t");
	for(file = FILE_A ; file <= FILE_H ; file++) {
		printf("%c\t",'a'+file);
	}

	printf("\n\n");
	printf("side:%c\n", SideChar[pos->side]);
	char* enPasSqNm = GetSquareNameFromNumber120(pos->enPas);
	printf("enPas:%d (%s)\n", pos->enPas, enPasSqNm);
	// printf("enPas:%d\n", pos->enPas);

	printf("castle:%c%c%c%c\n",
			pos->castlePerm & WKCA ? 'K' : '-',
			pos->castlePerm & WQCA ? 'Q' : '-',
			pos->castlePerm & BKCA ? 'k' : '-',
			pos->castlePerm & BQCA ? 'q' : '-'
			);

    // Unknown conversion type character 'l' in format
	// printf("PosKey:%llX\n", pos->posKey);
	printf("PosKey:%I64X\n", pos->posKey);
}

/**
  * @brief Get square name from 64 based square index
  *
  * @param sq64 64 based square number
  * @return Name of the square
  */
char* GetSquareNameFromNumber64(int sq64) {
    if(sq64 < 0 || sq64 >= BRD_SQUARES) {
        return SquareNames[65];
    } else {
        return SquareNames[sq64];
    }
}

/**
  * @brief Get square name from 120 based square index
  *
  * @param sq120 120 based square number
  * @return Name of the square
  */
char* GetSquareNameFromNumber120(int sq120) {
    if(sq120 == OFFBOARD) {
        return SquareNames[65];
    }

    if(sq120 == NO_SQ) {
        return SquareNames[64];
    }

    int sq64 = SQ64(sq120);

    return GetSquareNameFromNumber64(sq64);
}

/**
  * @brief Get square name from file and rank index
  *
  * @param file File number
  * @param rank Rank number
  * @return Name of the square
  */
char* GetSquareNameFromFileRank(int file, int rank) {
    if(file < FILE_A || file > FILE_H || rank < RANK_1 || rank > RANK_8) {
        return SquareNames[65];
    }

    int sq64 = FR2SQ64(file, rank);

    return GetSquareNameFromNumber64(sq64);
}

/**
  * @brief Updates the Lists Material for a board position
  *
  * @param *pos Board position
  */
void UpdateListsMaterial(S_BOARD *pos) {

	int piece, sq, index, colour;

	for(index = 0; index < BRD_SQ_NUM; ++index) {
		sq = index;
		piece = pos->pieces[index];
		if(piece!=OFFBOARD && piece!= EMPTY) {
			colour = PieceCol[piece];

		    if( PieceBig[piece] == TRUE) {
                    pos->bigPce[colour]++;
		    }

		    if( PieceMin[piece] == TRUE) {
                    pos->minPce[colour]++;
		    }

		    if( PieceMaj[piece] == TRUE) {
                    pos->majPce[colour]++;
		    }

			pos->material[colour] += PieceVal[piece];

			// Piece List
			pos->pList[piece][pos->pceNum[piece]] = sq;
			pos->pceNum[piece]++;

            // Set King Position
			if(piece==wK) {
                    pos->kingSq[WHITE] = sq;
			}

			if(piece==bK) {
                    pos->kingSq[BLACK] = sq;
			}

            // Set Pawn Position
			if(piece==wP) {
				SETBIT(pos->pawns[WHITE],SQ64(sq));
				SETBIT(pos->pawns[BOTH],SQ64(sq));
			} else if(piece==bP) {
				SETBIT(pos->pawns[BLACK],SQ64(sq));
				SETBIT(pos->pawns[BOTH],SQ64(sq));
			}
		}
	}
}

#endif // BOARD_C
