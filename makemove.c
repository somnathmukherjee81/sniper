/***********************************************************
  * File Name: makemove.c
  * Author: Somnath Mukherjee
  * Description:
  * C functions for the make move
  **********************************************************/

#ifndef MAKEMOVE_C
#define MAKEMOVE_C

// Hashing Macros
#define HASH_PCE(pce,sq) (pos->posKey ^= (PiecesKeys[(pce)][(sq)]))
#define HASH_CA (pos->posKey ^= (CastleKeys[(pos->castlePerm)]))
#define HASH_SIDE (pos->posKey ^= (SideKey))
#define HASH_EP (pos->posKey ^= (PiecesKeys[EMPTY][(pos->enPas)]))

#include "defs.h"
#include "stdio.h"

/**
  * Castle Permissions. All 120 bit-board squares are set to 15 except the a1, e1, h1 for White and a8, e8 and h8
  * When the rook or the king moves ca_perm &= CastlePerm[square_from]
  */
const int CastlePerm[BRD_SQ_NUM] = {
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 13, 15, 15, 15, 12, 15, 15, 14, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15,  7, 15, 15, 15,  3, 15, 15, 11, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15
};

/**
  * @brief Function to clear piece from the board
  *
  * @param sq Square to clear from
  * @param *pos Pointer to the board structure
  */
static void ClearPiece(const int sq, S_BOARD *pos) {

	ASSERT(SqOnBoard(sq));

    // Get the piece on the square
    int pce = pos->pieces[sq];

    ASSERT(PieceValid(pce));

    // Get color of the piece
	int col = PieceCol[pce];
	int index = 0;
	int t_pceNum = -1;

    // Hash out the piece from the square
    HASH_PCE(pce,sq);

    // Set the square to empty
	pos->pieces[sq] = EMPTY;
	// Reduce material value by the piece's value
    pos->material[col] -= PieceVal[pce];

    // Big Piece or Pawn
	if(PieceBig[pce]) {
        // Reduce big piece
        pos->bigPce[col]--;

        // Reduce major or minor piece
		if(PieceMaj[pce]) {
			pos->majPce[col]--;
		} else {
			pos->minPce[col]--;
		}
	} else {
	    // Otherwise, clear the pawns for both the current color and both
		CLRBIT(pos->pawns[col], SQ64(sq));
		CLRBIT(pos->pawns[BOTH], SQ64(sq));
	}

	/*
		pos->pceNum[wP] == 5 Looping from 0 to 4
		pos->pList[pce][0] == sq0
		pos->pList[pce][1] == sq1
		pos->pList[pce][2] == sq2
		pos->pList[pce][3] == sq3
		pos->pList[pce][4] == sq4

		sq==sq3 so t_pceNum = 3;
	*/

    // Loop through the piece index for the piece
	for(index = 0; index < pos->pceNum[pce]; ++index) {
        // When we find the square
		if(pos->pList[pce][index] == sq) {
		    // Get the index of the piece
			t_pceNum = index;
			break;
		}
	}

	ASSERT(t_pceNum != -1);

    // Reduce the piece number for the piece
	pos->pceNum[pce]--;
	// pos->pceNum[wP] == 4

    // Replace the piece
	pos->pList[pce][t_pceNum] = pos->pList[pce][pos->pceNum[pce]];
    /*
        pos->pList[wP][3]	= pos->pList[wP][4] = sq4

		pos->pceNum[wP] == 4 Looping from 0 to 3
		pos->pList[pce][0] == sq0
		pos->pList[pce][1] == sq1
		pos->pList[pce][2] == sq2
		pos->pList[pce][3] == sq4
	*/

}

/**
  * @brief Function to add a piece to the board
  *
  * @param sq Square to clear from
  * @param *pos Pointer to the board structure
  * @param pce Piece to add
  */
static void AddPiece(const int sq, S_BOARD *pos, const int pce) {

    ASSERT(PieceValid(pce));
    ASSERT(SqOnBoard(sq));

    // Get the color of the piece
	int col = PieceCol[pce];

    // Hash in the piece to the square
    HASH_PCE(pce,sq);

    // Set the piece on the square
	pos->pieces[sq] = pce;

    // Big Piece or Pawn
    if(PieceBig[pce]) {
        // Increase big piece
        pos->bigPce[col]++;

        // Increase major or minor piece
		if(PieceMaj[pce]) {
			pos->majPce[col]++;
		} else {
			pos->minPce[col]++;
		}
	} else {
	    // Otherwise, set the pawns for both the current color and both
		SETBIT(pos->pawns[col], SQ64(sq));
		SETBIT(pos->pawns[BOTH], SQ64(sq));
	}

    // Increase material value by the piece's value
	pos->material[col] += PieceVal[pce];
	// Add the piece to the piece list
	pos->pList[pce][pos->pceNum[pce]++] = sq;
}

/**
  * @brief Function to move a piece
  *
  * @param from Square from where the piece is moving
  * @param to Square to where the piece is moving
  * @param *pos Pointer to the board structure
  */
static void MovePiece(const int from, const int to, S_BOARD *pos) {

    ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));

	int index = 0;
	// Get the piece on the from square
	int pce = pos->pieces[from];
	// Get the color of the piece on the from square
	int col = PieceCol[pce];

#ifdef DEBUG
	int t_PieceNum = FALSE;
#endif

    // Hash off the piece from the 'from' square
	HASH_PCE(pce,from);
	// Clear the from square
	pos->pieces[from] = EMPTY;

    // Hash in the piece on the 'to' square
	HASH_PCE(pce,to);
	// Set the piece on the 'to' square
	pos->pieces[to] = pce;

    // Big Piece or Pawn
	if(!PieceBig[pce]) {
        // Clear the pawns for both the current color and both
		CLRBIT(pos->pawns[col],SQ64(from));
		CLRBIT(pos->pawns[BOTH],SQ64(from));
        // Set the pawns for both the current color and both
		SETBIT(pos->pawns[col],SQ64(to));
		SETBIT(pos->pawns[BOTH],SQ64(to));
	}

    // Loop through the piece index for the piece
	for(index = 0; index < pos->pceNum[pce]; ++index) {
        // If the piece is on the 'from' square, replace the square to the 'to' square
		if(pos->pList[pce][index] == from) {
			pos->pList[pce][index] = to;

#ifdef DEBUG
			t_PieceNum = TRUE;
#endif

			break;
		}
	}

	ASSERT(t_PieceNum);
}

/**
  * @brief Function to make a move
  *
  * @param *pos Pointer to the board structure
  * @param move Move to make
  * @return TRUE is the move is successfully made, FALSE otherwise
  *
  * Returns false or zero if after making the move the side which has made the move has made themselves on a check
  */
int MakeMove(S_BOARD *pos, int move) {

	ASSERT(CheckBoard(pos));

    // Get the from and to square and the side to move
	int from = FROMSQ(move);
    int to = TOSQ(move);
    int side = pos->side;

	ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));
    ASSERT(SideValid(side));
    ASSERT(PieceValid(pos->pieces[from]));

    // Store the current position to history before making the move
	pos->history[pos->hisPly].posKey = pos->posKey;

    // If the move is an en passant
	if(move & MFLAGEP) {
        if(side == WHITE) {
            // Remove the black pawn (a square below the to square) as a result of en passant capture
            ClearPiece(to - 10, pos);
        } else {
            // // Remove the white pawn (a square above the to square) as a result of en passant capture
            ClearPiece(to + 10, pos);
        }
    } else if (move & MFLAGCA) {
        // The move is a castling move, move the rook
        switch(to) {
            case C1:
                MovePiece(A1, D1, pos);
			break;
            case C8:
                MovePiece(A8, D8, pos);
			break;
            case G1:
                MovePiece(H1, F1, pos);
			break;
            case G8:
                MovePiece(H8, F8, pos);
			break;
            default: ASSERT(FALSE); break;
        }
    }

    // Hash out the en passant square if it's set
	if(pos->enPas != NO_SQ) HASH_EP;
	// Hash out the castling permission
    HASH_CA;

    // Update the history
	pos->history[pos->hisPly].move = move;
    pos->history[pos->hisPly].fiftyMove = pos->fiftyMove;
    pos->history[pos->hisPly].enPas = pos->enPas;
    pos->history[pos->hisPly].castlePerm = pos->castlePerm;

    // Update castle permission
    pos->castlePerm &= CastlePerm[from];
    pos->castlePerm &= CastlePerm[to];
    // Reset en passant square
    pos->enPas = NO_SQ;

    // Hash in the castling permission
	HASH_CA;

    // Get the piece that is captured
	int captured = CAPTURED(move);
	// Increment fifty move counter
    pos->fiftyMove++;

    // If something was captured
	if(captured != EMPTY) {
        ASSERT(PieceValid(captured));
        // Remove the captured piece from to square
        ClearPiece(to, pos);
        // Reset the fifty move counter
        pos->fiftyMove = 0;
    }

    // Increment the ply and history ply
	pos->hisPly++;
	pos->ply++;

    // Check if there is a need to set new en passant square
	if(PiecePawn[pos->pieces[from]]) {
        // Pawn move reset fifty move counter
        pos->fiftyMove = 0;
        // If a pawn start move
        if(move & MFLAGPS) {
            // Set the en passant square to above or below the to square
            if(side == WHITE) {
                pos->enPas = from+10;
                ASSERT(RanksBrd[pos->enPas]==RANK_3);
            } else {
                pos->enPas=from-10;
                ASSERT(RanksBrd[pos->enPas]==RANK_6);
            }

            // Hash in the en passant square
            HASH_EP;
        }
    }

    // Move the piece
	MovePiece(from, to, pos);

    // Check for promotion
    // Get the promotion piece
	int prPce = PROMOTED(move);
    if(prPce != EMPTY)   {
        ASSERT(PieceValid(prPce) && !PiecePawn[prPce]);
        // Clear the piece (pawn) from the to square
        ClearPiece(to, pos);
        // Add the promoted piece on the to square
        AddPiece(to, pos, prPce);
    }

    // Set the king's square
	if(PieceKing[pos->pieces[to]]) {
        pos->kingSq[pos->side] = to;
    }

    // Flip the side to move
	pos->side ^= 1;
	// Hash the side to move
    HASH_SIDE;

    ASSERT(CheckBoard(pos));

    // If after the move the king is attacked, take back the move and return false (illegal move)
	if(SqAttacked(pos->kingSq[side], pos->side, pos))  {
        TakeMove(pos);
        return FALSE;
    }

    // Return true as a legal move
	return TRUE;

}

/**
  * @brief Function to take back a move
  *
  * @param *pos Pointer to the board structure
  */
void TakeMove(S_BOARD *pos) {

	ASSERT(CheckBoard(pos));

    // Reduce history ply and ply
	pos->hisPly--;
    pos->ply--;

    // Get the last move from history and the from and to squares fro the move
    int move = pos->history[pos->hisPly].move;
    int from = FROMSQ(move);
    int to = TOSQ(move);

	ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));

    // If the en passant square is set, hash it out
	if(pos->enPas != NO_SQ) {
            HASH_EP;
	}

    // Hash out castle permission
    HASH_CA;

    // Set the new castle permission, fifty move counter and en passant square from history
    pos->castlePerm = pos->history[pos->hisPly].castlePerm;
    pos->fiftyMove = pos->history[pos->hisPly].fiftyMove;
    pos->enPas = pos->history[pos->hisPly].enPas;

    // If the en passant square is set, hash it in
    if(pos->enPas != NO_SQ) {
            HASH_EP;
    }

    // Hash in castle permission
    HASH_CA;

    // Flip the side to move
    pos->side ^= 1;
    HASH_SIDE;

    // If it was an en passant capture
	if(MFLAGEP & move) {
        // Add the captured pawn of the correct color back to the board
        if(pos->side == WHITE) {
            AddPiece(to - 10, pos, bP);
        } else {
            AddPiece(to + 10, pos, wP);
        }
    } else if(MFLAGCA & move) {
        // If it was a castling move, reset the rook to the starting position
        switch(to) {
            case C1: MovePiece(D1, A1, pos); break;
            case C8: MovePiece(D8, A8, pos); break;
            case G1: MovePiece(F1, H1, pos); break;
            case G8: MovePiece(F8, H8, pos); break;
            default: ASSERT(FALSE); break;
        }
    }

    // Move the piece back to its residing square before the move; i.e. from 'to' square to 'from' square
	MovePiece(to, from, pos);

    // Update the king position
	if(PieceKing[pos->pieces[from]]) {
        pos->kingSq[pos->side] = from;
    }

    // If it was a capture move
	int captured = CAPTURED(move);
    if(captured != EMPTY) {
        ASSERT(PieceValid(captured));
        // Add the captured piece back to the board
        AddPiece(to, pos, captured);
    }

    // If it was a pawn promotion move
	if(PROMOTED(move) != EMPTY)   {
        ASSERT(PieceValid(PROMOTED(move)) && !PiecePawn[PROMOTED(move)]);
        // We have already taken back the promoted piece to the 'from' square, so clear it
        ClearPiece(from, pos);
        // Add the pawn of correct color back to the from square
        AddPiece(from, pos, (PieceCol[PROMOTED(move)] == WHITE ? wP : bP));
    }

    ASSERT(CheckBoard(pos));

}

#endif // MAKEMOVE_C
