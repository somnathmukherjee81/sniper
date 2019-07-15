/***********************************************************
  * File Name: movegen.c
  * Author: Somnath Mukherjee
  * Description:
  * C functions for move generation
  **********************************************************/

#ifndef MOVEGEN_C
#define MOVEGEN_C

#include "defs.h"
#include "stdio.h"

#define MOVE(f,t,ca,pro,fl) ((f) | ((t) << 7) | ((ca) << 14) | ((pro) << 20) | (fl))
#define SQOFFBOARD(sq) (FilesBrd[(sq)]==OFFBOARD)

/*
MoveGen(board, list)
    Loop all pieces
        -> If slider, then loop each direction and add move
            -> AddMove list->moves[list->count] = move; list->count++;
*/

const int LoopSlidePce[8] = {
 wB, wR, wQ, 0, bB, bR, bQ, 0
};

const int LoopNonSlidePce[6] = {
 wN, wK, 0, bN, bK, 0
};

const int LoopSlideIndex[2] = { 0, 4 };
const int LoopNonSlideIndex[2] = { 0, 3 };

/**
  * Directions and square offsets each piece can move to.Note: This array is not used for pawns
  */
const int PceDir[NUM_PIECES][8] = {
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ -8, -19,	-21, -12, 8, 19, 21, 12 },
	{ -9, -11, 11, 9, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ -8, -19,	-21, -12, 8, 19, 21, 12 },
	{ -9, -11, 11, 9, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, 0, 0, 0, 0 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 },
	{ -1, -10,	1, 10, -9, -11, 11, 9 }
};

/**
  * Number of directions that each piece can move to. Note: This array is not used for pawns
  */
const int NumDir[NUM_PIECES] = {
 0, 0, 8, 4, 4, 8, 8, 0, 8, 4, 4, 8, 8
};

/*
Search PV Move
Search Captures: MVVLVA (Most Valuable Victim, Least Valuable Valuable Attacker) or SEE (Static Exchange Evaluator)
Search Killers (moves which have made beta cut-off)
Search according to History Score (improving alpha)

MVVLVA
----------
Q (500): P x Q (505), N x Q (504), B x Q (503), R x Q (502), Q x Q (501)
R (400): P x R (405), N x R (404), B x R (403), R x R (402), Q x R (401)
B (300): P x B, N x B, B x B, R x B, Q x B
N (200): P x N, N x N, B x N, R x N, Q x N
P (100): P x P, N x P, B x P, R x P, Q x P
*/
const int VictimScore[NUM_PIECES] = { 0, 100, 200, 300, 400, 500, 600, 100, 200, 300, 400, 500, 600 };
static int MvvLvaScores[NUM_PIECES][NUM_PIECES];

/**
  * Function to initialize MVVLVA
  *
  */
int InitMvvLva() {
	int attacker;
	int victim;

    // Loop through the attackers and the victims and set the MVVLVA scores based on who captures whom
	for(attacker = wP; attacker <= bK; ++attacker) {
		for(victim = wP; victim <= bK; ++victim) {
			MvvLvaScores[victim][attacker] = VictimScore[victim] + 6 - ( VictimScore[attacker] / 100);
		}
	}

	/*for(victim = wP; victim <= bK; ++victim) {
		for(attacker = wP; attacker <= bK; ++attacker) {
			printf("%c x %c = %d\n",PceChar[attacker],PceChar[victim],MvvLvaScores[victim][attacker]);
		}
	}*/
}

/**
  * Function to check if the move exist in the current position
  *
  * @param *pos Pointer to the board structure
  * @param move The move
  */
int MoveExists(S_BOARD *pos, const int move) {

	S_MOVELIST list[1];
	// Generate all the moves
    GenerateAllMoves(pos,list);

    int moveNum = 0;
    // Loop through all the moves
	for(moveNum = 0; moveNum < list->count; ++moveNum) {
        // Make the move. If invalid move, ignore and continue
        if (!MakeMove(pos,list->moves[moveNum].move))  {
            continue;
        }

        // Take back move
        TakeMove(pos);

        // Check if the move is same, return true
		if(list->moves[moveNum].move == move) {
			return TRUE;
		}
    }

    // If the move is not found, return false
	return FALSE;
}


/**
  * Function to add a quiet move
  *
  * @param *pos Pointer to the board structure
  * @param move The move
  * @param *list Pointer to the move list
  */
static void AddQuietMove(const S_BOARD *pos, int move, S_MOVELIST *list) {
    ASSERT(SqOnBoard(FROMSQ(move)));
	ASSERT(SqOnBoard(TOSQ(move)));

	list->moves[list->count].move = move;

    // If it's the first killer move score as 900000; score 800000 for the second killer
	if(pos->searchKillers[0][pos->ply] == move) {
		list->moves[list->count].score = 900000;
	} else if(pos->searchKillers[1][pos->ply] == move) {
		list->moves[list->count].score = 800000;
	} else {
	    // Otherwise get score from search history
		list->moves[list->count].score = pos->searchHistory[pos->pieces[FROMSQ(move)]][TOSQ(move)];
	}

	list->count++;
}

/**
  * Function to add a capture move
  *
  * @param *pos Pointer to the board structure
  * @param move The move
  * @param *list Pointer to the move list
  */
static void AddCaptureMove(const S_BOARD *pos, int move, S_MOVELIST *list) {
    ASSERT(SqOnBoard(FROMSQ(move)));
	ASSERT(SqOnBoard(TOSQ(move)));
	ASSERT(PieceValid(CAPTURED(move)));

	list->moves[list->count].move = move;
	// 1000000 added for killer and history
	list->moves[list->count].score = MvvLvaScores[CAPTURED(move)][pos->pieces[FROMSQ(move)]] + 1000000;
	list->count++;
}

/**
  * Function to add a en passant move
  *
  * @param *pos Pointer to the board structure
  * @param move The move
  * @param *list Pointer to the move list
  */
static void AddEnPassantMove(const S_BOARD *pos, int move, S_MOVELIST *list) {
    ASSERT(SqOnBoard(FROMSQ(move)));
	ASSERT(SqOnBoard(TOSQ(move)));

	list->moves[list->count].move = move;
	// 1000000 added for killer and history
	list->moves[list->count].score = 105 + 1000000;
	list->count++;
}

/**
  * Function to add a white pawn's capture move
  *
  * @param *pos Pointer to the board structure
  * @param from From square
  * @param to To square
  * @param cap Captured piece
  * @param *list Pointer to the move list
  */
static void AddWhitePawnCapMove(const S_BOARD *pos, const int from, const int to, const int cap, S_MOVELIST *list) {
    ASSERT(PieceValidEmpty(cap));
	ASSERT(SqOnBoard(from));
	ASSERT(SqOnBoard(to));

    // If the pawn is capturing from 7th rank, it's going to promote
	if(RanksBrd[from] == RANK_7) {
		AddCaptureMove(pos, MOVE(from, to, cap, wQ, 0), list);
		AddCaptureMove(pos, MOVE(from, to, cap, wR, 0), list);
		AddCaptureMove(pos, MOVE(from, to, cap, wB, 0), list);
		AddCaptureMove(pos, MOVE(from, to, cap, wN, 0), list);
	} else {
		AddCaptureMove(pos, MOVE(from, to, cap, EMPTY, 0), list);
	}
}

/**
  * Function to add a white pawn's move
  *
  * @param *pos Pointer to the board structure
  * @param from From square
  * @param to To square
  * @param *list Pointer to the move list
  */
static void AddWhitePawnMove(const S_BOARD *pos, const int from, const int to, S_MOVELIST *list) {
    ASSERT(SqOnBoard(from));
	ASSERT(SqOnBoard(to));

    // If the pawn is moving from the 7th rank, it's going to promote
	if(RanksBrd[from] == RANK_7) {
		AddQuietMove(pos, MOVE(from, to, EMPTY, wQ, 0), list);
		AddQuietMove(pos, MOVE(from, to, EMPTY, wR, 0), list);
		AddQuietMove(pos, MOVE(from, to, EMPTY, wB, 0), list);
		AddQuietMove(pos, MOVE(from, to, EMPTY, wN, 0), list);
	} else {
		AddQuietMove(pos, MOVE(from, to, EMPTY, EMPTY, 0), list);
	}
}

/**
  * Function to add a black pawn's capture move
  *
  * @param *pos Pointer to the board structure
  * @param from From square
  * @param to To square
  * @param cap Captured piece
  * @param *list Pointer to the move list
  */
static void AddBlackPawnCapMove(const S_BOARD *pos, const int from, const int to, const int cap, S_MOVELIST *list) {
    ASSERT(PieceValidEmpty(cap));
	ASSERT(SqOnBoard(from));
	ASSERT(SqOnBoard(to));

    // If the pawn is capturing from 7th rank, it's going to promote
	if(RanksBrd[from] == RANK_2) {
		AddCaptureMove(pos, MOVE(from, to, cap, bQ, 0), list);
		AddCaptureMove(pos, MOVE(from, to, cap, bR, 0), list);
		AddCaptureMove(pos, MOVE(from, to, cap, bB, 0), list);
		AddCaptureMove(pos, MOVE(from, to, cap, bN, 0), list);
	} else {
		AddCaptureMove(pos, MOVE(from, to, cap, EMPTY, 0), list);
	}
}

/**
  * Function to add a black pawn's move
  *
  * @param *pos Pointer to the board structure
  * @param from From square
  * @param to To square
  * @param *list Pointer to the move list
  */
static void AddBlackPawnMove(const S_BOARD *pos, const int from, const int to, S_MOVELIST *list) {
    ASSERT(SqOnBoard(from));
	ASSERT(SqOnBoard(to));

	if(RanksBrd[from] == RANK_2) {
		AddQuietMove(pos, MOVE(from, to, EMPTY, bQ, 0), list);
		AddQuietMove(pos, MOVE(from, to, EMPTY, bR, 0), list);
		AddQuietMove(pos, MOVE(from, to, EMPTY, bB, 0), list);
		AddQuietMove(pos, MOVE(from, to, EMPTY, bN, 0), list);
	} else {
		AddQuietMove(pos, MOVE(from, to, EMPTY, EMPTY, 0), list);
	}
}

/**
  * Function to generate all moves
  *
  * @param *pos Pointer to the board structure
  * @param *list Pointer to the move list
  */
void GenerateAllMoves(const S_BOARD *pos, S_MOVELIST *list) {

	ASSERT(CheckBoard(pos));

	list->count = 0;

	int pce = EMPTY;
	int side = pos->side;
	int sq = 0;
	int t_sq = 0;
	int pceNum = 0;
	int dir = 0;
	int index = 0;
	int pceIndex = 0;

	//printf("\n\nSide:%d\n",side);

    // White to move
	if(side == WHITE) {
        // Generate White Pawn Moves
        // Loop through each of the white pawns on the board
		for(pceNum = 0; pceNum < pos->pceNum[wP]; ++pceNum) {
		    // Find which square the pawn is on
			sq = pos->pList[wP][pceNum];
			ASSERT(SqOnBoard(sq));

            // If the square above the pawn's square if empty
			if(pos->pieces[sq + 10] == EMPTY) {
				AddWhitePawnMove(pos, sq, sq + 10, list);

				// If the pawn is on the second rank (starting position) and
				// the square two squares above is also empty, it can move two squares
				if(RanksBrd[sq] == RANK_2 && pos->pieces[sq + 20] == EMPTY) {
					AddQuietMove(pos, MOVE(sq, (sq + 20), EMPTY, EMPTY, MFLAGPS), list);
				}
			}

            // If there is a black piece sitting on the square diagonally left of the pawn's square, the pawn can capture it
			if(!SQOFFBOARD(sq + 9) && PieceCol[pos->pieces[sq + 9]] == BLACK) {
				AddWhitePawnCapMove(pos, sq, sq + 9, pos->pieces[sq + 9], list);
			}

			// If there is a black piece sitting on the square diagonally right of the pawn's square, the pawn can capture it
			if(!SQOFFBOARD(sq + 11) && PieceCol[pos->pieces[sq + 11]] == BLACK) {
				AddWhitePawnCapMove(pos, sq, sq + 11, pos->pieces[sq + 11], list);
			}

            if(pos->enPas != NO_SQ) {
                // Add en passant capture left, if one is available
                if(sq + 9 == pos->enPas) {
                    AddEnPassantMove(pos, MOVE(sq, sq + 9, EMPTY, EMPTY, MFLAGEP), list);
                }

                // Add en passant capture right, if one is available
                if(sq + 11 == pos->enPas) {
                    AddEnPassantMove(pos, MOVE(sq, sq + 11, EMPTY, EMPTY, MFLAGEP), list);
                }
            }
		}

		// Castling for White
		// White king-side castling is allowed?
		if(pos->castlePerm & WKCA) {
            // If there is no piece on the f1 and g1 square
			if(pos->pieces[F1] == EMPTY && pos->pieces[G1] == EMPTY) {
			    // If e1, f1 and g1 are not attacked by Black
				if(!SqAttacked(E1,BLACK,pos) && !SqAttacked(F1,BLACK,pos) && !SqAttacked(G1,BLACK,pos)) {
				    // Generate White king-side castling
					//printf("WKCA MoveGen\n");
					AddQuietMove(pos, MOVE(E1, G1, EMPTY, EMPTY, MFLAGCA), list);
				}
			}
		}

        // White queen-side castling is allowed?
		if(pos->castlePerm & WQCA) {
            // If there is no piece on the d1, c1 and b1 square
			if(pos->pieces[D1] == EMPTY && pos->pieces[C1] == EMPTY && pos->pieces[B1] == EMPTY) {
			    // If e1, d1 and c1 are not attacked by Black
				if(!SqAttacked(E1,BLACK,pos) && !SqAttacked(D1,BLACK,pos) && !SqAttacked(C1,BLACK,pos)) {
				    // Generate White queen-side castling
					//printf("WQCA MoveGen\n");
					AddQuietMove(pos, MOVE(E1, C1, EMPTY, EMPTY, MFLAGCA), list);
				}
			}
		}
	} else {
	    // Generate Black Pawn Moves
        // Loop through each of the black pawns on the board
        for(pceNum = 0; pceNum < pos->pceNum[bP]; ++pceNum) {
            // Find which square the pawn is on
			sq = pos->pList[bP][pceNum];
			ASSERT(SqOnBoard(sq));

            // If the square above the pawn's square if empty
			if(pos->pieces[sq - 10] == EMPTY) {
				AddBlackPawnMove(pos, sq, sq-10, list);

				// If the pawn is on the second rank (starting position) and
				// the square two squares above is also empty, it can move two squares
				if(RanksBrd[sq] == RANK_7 && pos->pieces[sq - 20] == EMPTY) {
					AddQuietMove(pos, MOVE(sq, (sq - 20), EMPTY, EMPTY, MFLAGPS),list);
				}
			}

            // If there is a black piece sitting on the square diagonally left of the pawn's square, the pawn can capture it
			if(!SQOFFBOARD(sq - 9) && PieceCol[pos->pieces[sq - 9]] == WHITE) {
				AddBlackPawnCapMove(pos, sq, sq - 9, pos->pieces[sq - 9], list);
			}

            // If there is a black piece sitting on the square diagonally right of the pawn's square, the pawn can capture it
			if(!SQOFFBOARD(sq - 11) && PieceCol[pos->pieces[sq - 11]] == WHITE) {
				AddBlackPawnCapMove(pos, sq, sq - 11, pos->pieces[sq - 11], list);
			}

            if(pos->enPas != NO_SQ) {
                // Add en passant capture left, if one is available
                if(sq - 9 == pos->enPas) {
                    AddEnPassantMove(pos, MOVE(sq, sq - 9, EMPTY, EMPTY, MFLAGEP), list);
                }

                // Add en passant capture right, if one is available
                if(sq - 11 == pos->enPas) {
                    AddEnPassantMove(pos, MOVE(sq, sq - 11, EMPTY, EMPTY, MFLAGEP), list);
                }
            }
		}

        // Castling for Black
        // Black king-side castling is allowed?
		if(pos->castlePerm & BKCA) {
            // If there is no piece on the f8 and g8 square
			if(pos->pieces[F8] == EMPTY && pos->pieces[G8] == EMPTY) {
			    // If e8, f8 and g8 are not attacked by Black
				if(!SqAttacked(E8,WHITE,pos) && !SqAttacked(F8,WHITE,pos) && !SqAttacked(G8,WHITE,pos)) {
				    // Generate Black king-side castling
					//printf("BKCA MoveGen\n");
					AddQuietMove(pos, MOVE(E8, G8, EMPTY, EMPTY, MFLAGCA), list);
				}
			}
		}

        // Black queen-side castling is allowed?
		if(pos->castlePerm &  BQCA) {
            // If there is no piece on the d8, c8 and b8 square
			if(pos->pieces[D8] == EMPTY && pos->pieces[C8] == EMPTY && pos->pieces[B8] == EMPTY) {
                // If e8, d8 and c8 are not attacked by Black
				if(!SqAttacked(E8,WHITE,pos) && !SqAttacked(D8,WHITE,pos) && !SqAttacked(C8,WHITE,pos)) {
				    // Generate Black queen-side castling
					//printf("BQCA MoveGen\n");
					AddQuietMove(pos, MOVE(E8, C8, EMPTY, EMPTY, MFLAGCA), list);
				}
			}
		}
	}

	/* Loop for slide pieces */
	pceIndex = LoopSlideIndex[side];
	pce = LoopSlidePce[pceIndex++];

	while( pce != 0) {
		ASSERT(PieceValid(pce));
		//printf("sliders pceIndex:%d pce:%d\n", pceIndex, pce);

        // Loop through each of the current piece of that type on the board
		for(pceNum = 0; pceNum < pos->pceNum[pce]; ++pceNum) {
            // Find which square the piece is on
			sq = pos->pList[pce][pceNum];
			ASSERT(SqOnBoard(sq));

			//printf("Piece:%c on %s\n", PceChar[pce], PrSq(sq));

            // Loop through all the directions for the current piece
			for(index = 0; index < NumDir[pce]; ++index) {
                // Get the current direction
				dir = PceDir[pce][index];
				t_sq = sq + dir;

                // Slide until it goes off-board or hits another piece
				while(!SQOFFBOARD(t_sq)) {
                    // If it encounters a piece
					// Find other side: BLACK ^ 1 == WHITE, WHITE ^ 1 == BLACK
					if(pos->pieces[t_sq] != EMPTY) {
					    // If the piece on the target square is of the other side
						if( PieceCol[pos->pieces[t_sq]] == (side ^ 1)) {
						    // Generate a capture move
							//printf("\t\tCapture on %s\n", PrSq(t_sq));
							AddCaptureMove(pos, MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0), list);
						}

                        // Stop sliding
						break;
					}

					// Otherwise generate a normal move
					//printf("\t\tNormal on %s\n", PrSq(t_sq));
					AddQuietMove(pos, MOVE(sq, t_sq, EMPTY, EMPTY, 0), list);

					// Continue sliding
					t_sq += dir;
				}
			}
		}

		pce = LoopSlidePce[pceIndex++];
	}

	/* Loop for non slide */
	pceIndex = LoopNonSlideIndex[side];
	pce = LoopNonSlidePce[pceIndex++];

	while( pce != 0) {
		ASSERT(PieceValid(pce));
		//printf("non sliders pceIndex:%d pce:%d\n", pceIndex, pce);

        // Loop through each of the current piece of that type on the board
		for(pceNum = 0; pceNum < pos->pceNum[pce]; ++pceNum) {
            // Find which square the piece is on
			sq = pos->pList[pce][pceNum];
			ASSERT(SqOnBoard(sq));

			//printf("Piece:%c on %s\n", PceChar[pce], PrSq(sq));

            // Loop through all the directions for the current piece
			for(index = 0; index < NumDir[pce]; ++index) {
                // Get the current direction
				dir = PceDir[pce][index];
                // Find the target square
				t_sq = sq + dir;

                // Skip if the target square is off-board
				if(SQOFFBOARD(t_sq)) {
					continue;
				}

                // If there is a piece on the target square
				// Find other side: BLACK ^ 1 == WHITE, WHITE ^ 1 == BLACK
				if(pos->pieces[t_sq] != EMPTY) {
                    // If the piece on the target square is of the other side
					if(PieceCol[pos->pieces[t_sq]] == (side ^ 1)) {
					    // Generate a capture move
						//printf("\t\tCapture on %s\n", PrSq(t_sq));
						AddCaptureMove(pos, MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0), list);
					}

					continue;
				}

				// If the target square is empty, generate a normal move
				//printf("\t\tNormal on %s\n", PrSq(t_sq));
				AddQuietMove(pos, MOVE(sq, t_sq, EMPTY, EMPTY, 0), list);
			}
		}

		pce = LoopNonSlidePce[pceIndex++];
	}
}

/**
  * Function to generate all capture moves
  *
  * @param *pos Pointer to the board structure
  * @param *list Pointer to the move list
  */
void GenerateAllCaps(const S_BOARD *pos, S_MOVELIST *list) {

	ASSERT(CheckBoard(pos));

	list->count = 0;

	int pce = EMPTY;
	int side = pos->side;
	int sq = 0; int t_sq = 0;
	int pceNum = 0;
	int dir = 0;
	int index = 0;
	int pceIndex = 0;

	if(side == WHITE) {

		for(pceNum = 0; pceNum < pos->pceNum[wP]; ++pceNum) {
			sq = pos->pList[wP][pceNum];
			ASSERT(SqOnBoard(sq));

			if(!SQOFFBOARD(sq + 9) && PieceCol[pos->pieces[sq + 9]] == BLACK) {
				AddWhitePawnCapMove(pos, sq, sq+9, pos->pieces[sq + 9], list);
			}
			if(!SQOFFBOARD(sq + 11) && PieceCol[pos->pieces[sq + 11]] == BLACK) {
				AddWhitePawnCapMove(pos, sq, sq+11, pos->pieces[sq + 11], list);
			}

			if(pos->enPas != NO_SQ) {
				if(sq + 9 == pos->enPas) {
					AddEnPassantMove(pos, MOVE(sq,sq + 9,EMPTY,EMPTY,MFLAGEP), list);
				}
				if(sq + 11 == pos->enPas) {
					AddEnPassantMove(pos, MOVE(sq,sq + 11,EMPTY,EMPTY,MFLAGEP), list);
				}
			}
		}
	} else {

		for(pceNum = 0; pceNum < pos->pceNum[bP]; ++pceNum) {
			sq = pos->pList[bP][pceNum];
			ASSERT(SqOnBoard(sq));

			if(!SQOFFBOARD(sq - 9) && PieceCol[pos->pieces[sq - 9]] == WHITE) {
				AddBlackPawnCapMove(pos, sq, sq-9, pos->pieces[sq - 9], list);
			}

			if(!SQOFFBOARD(sq - 11) && PieceCol[pos->pieces[sq - 11]] == WHITE) {
				AddBlackPawnCapMove(pos, sq, sq-11, pos->pieces[sq - 11], list);
			}
			if(pos->enPas != NO_SQ) {
				if(sq - 9 == pos->enPas) {
					AddEnPassantMove(pos, MOVE(sq,sq - 9,EMPTY,EMPTY,MFLAGEP), list);
				}
				if(sq - 11 == pos->enPas) {
					AddEnPassantMove(pos, MOVE(sq,sq - 11,EMPTY,EMPTY,MFLAGEP), list);
				}
			}
		}
	}

	/* Loop for slide pieces */
	pceIndex = LoopSlideIndex[side];
	pce = LoopSlidePce[pceIndex++];
	while( pce != 0) {
		ASSERT(PieceValid(pce));

		for(pceNum = 0; pceNum < pos->pceNum[pce]; ++pceNum) {
			sq = pos->pList[pce][pceNum];
			ASSERT(SqOnBoard(sq));

			for(index = 0; index < NumDir[pce]; ++index) {
				dir = PceDir[pce][index];
				t_sq = sq + dir;

				while(!SQOFFBOARD(t_sq)) {
					// BLACK ^ 1 == WHITE       WHITE ^ 1 == BLACK
					if(pos->pieces[t_sq] != EMPTY) {
						if(PieceCol[pos->pieces[t_sq]] == (side ^ 1)) {
							AddCaptureMove(pos, MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0), list);
						}
						break;
					}
					t_sq += dir;
				}
			}
		}

		pce = LoopSlidePce[pceIndex++];
	}

	/* Loop for non slide */
	pceIndex = LoopNonSlideIndex[side];
	pce = LoopNonSlidePce[pceIndex++];

	while( pce != 0) {
		ASSERT(PieceValid(pce));

		for(pceNum = 0; pceNum < pos->pceNum[pce]; ++pceNum) {
			sq = pos->pList[pce][pceNum];
			ASSERT(SqOnBoard(sq));

			for(index = 0; index < NumDir[pce]; ++index) {
				dir = PceDir[pce][index];
				t_sq = sq + dir;

				if(SQOFFBOARD(t_sq)) {
					continue;
				}

				// BLACK ^ 1 == WHITE       WHITE ^ 1 == BLACK
				if(pos->pieces[t_sq] != EMPTY) {
					if( PieceCol[pos->pieces[t_sq]] == (side ^ 1)) {
						AddCaptureMove(pos, MOVE(sq, t_sq, pos->pieces[t_sq], EMPTY, 0), list);
					}
					continue;
				}
			}
		}

		pce = LoopNonSlidePce[pceIndex++];
	}
}

#endif // MOVEGEN_C
