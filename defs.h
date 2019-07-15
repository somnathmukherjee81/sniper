/***********************************************************
  * File Name: defs.h
  * Author: Somnath Mukherjee
  * Description:
  * Header file for function and common macro declarations
  **********************************************************/

#ifndef DEFS_H
#define DEFS_H

#include "stdlib.h"

// Define DEBUG. Comment in release mode
//#define DEBUG

/**
  * Macro for Assert
  */
#ifndef DEBUG
#define ASSERT(n)
#else
#define ASSERT(n)   \
if(!(n)) {  \
    printf("%s - Failed", #n);  \
    printf("On %s ", __DATE__);  \
    printf("At %s ", __TIME__);  \
    printf("In the File %s ", __FILE__);  \
    printf("At Line %d\n", __LINE__);  \
    exit(1);}
#endif // DEBUG

/**
  * Type definition for unsigned 64 bit number
  */
typedef unsigned long long U64;

/**
  * Name of the Engine
  */
#define NAME "Sniper 1.0"
#define AUTHOR "Somnath Mukherjee"

/**
  * Number of squares in the Board Representation
  */
#define BRD_SQ_NUM 120

/**
  * Number of squares in chess board
  */
#define BRD_SQUARES 64

/**
  * Maximum number of expected moves in a game
  */
#define MAXGAMEMOVES 2048

/**
  * Maximum number of moves we expect
  */
#define MAXPOSITIONMOVES 256

/**
  * Maximum search depth
  */
#define MAXDEPTH 64

/**
  * Start FEN Position
  */
#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

/**
  * Number of sides or colors - White, Black, Both
  */
#define NUM_COLORS 3

/**
  * Number of pieces including empty
  */
#define NUM_PIECES 13

/**
  * Maximum number of pieces of one piece type
  */
#define MAX_PIECE_NUM 10

// Little endian rank-file (LERF) constants for 64 based index
/**
  * Little endian 64 based rank-file (LERF) constant for A File
  */
#define A_FILE_64 0x0101010101010101
/**
  * Little endian 64 based rank-file (LERF) constant for H File
  */
#define H_FILE_64 0x8080808080808080
/**
  * Little endian 64 based rank-file (LERF) constant for First Rank
  */
#define FIRST_RANK_64 0x00000000000000FF
/**
  * Little endian 64 based rank-file (LERF) constant for Eight Rank
  */
#define EIGHT_RANK_64 0xFF00000000000000
/**
  * Little endian 64 based rank-file (LERF) constant for a1-h8 Diagonal
  */
#define A1_H8_DIAGONAL_64 0x8040201008040201
/**
  * Little endian 64 based rank-file (LERF) constant for h1-a8 Anti-diagonal
  */
#define H1_A8_ANTIDIAGONAL_64 0x0102040810204080
/**
  * Little endian 64 based rank-file (LERF) constant for Light Squares
  */
#define LIGHT_SQUARES_64 0x55AA55AA55AA55AA
/**
  * Little endian 64 based rank-file (LERF) constant for Dark Squares
  */
#define DARK_SQUARES_64 0xAA55AA55AA55AA55

/**
  * Enumeration for Pieces
  */
enum {
    EMPTY, /**< 0 - Empty or no piece */
    wP, /**< 1 - White Pawn */
    wN, /**< 2 - White Knight */
    wB, /**< 3 - White Bishop */
    wR, /**< 4 - White Rook */
    wQ, /**< 5 - White Queen */
    wK, /**< 6 - White King */
    bP, /**< 7 - Black Pawn */
    bN, /**< 8 - Black Knight */
    bB, /**< 9 - Black Bishop */
    bR, /**< 10 - Black Rook */
    bQ, /**< 11 - Black Queen */
    bK /**< 12 - Black King */
};

/**
  * Enumeration for Files
  */
enum {
    FILE_A, /**< 0 - A File */
    FILE_B, /**< 1 - B File */
    FILE_C, /**< 2 - C File */
    FILE_D, /**< 3 - D File */
    FILE_E, /**< 4 - E File */
    FILE_F, /**< 5 - F File */
    FILE_G, /**< 6 - G File */
    FILE_H, /**< 7 - H File */
    FILE_NONE /**< 8 - Invalid File */
};

/**
  * Enumeration for Ranks
  */
enum {
    RANK_1, /**< 0 - 1st Rank */
    RANK_2, /**< 1 - 2nd Rank */
    RANK_3, /**< 2 - 3rd Rank */
    RANK_4, /**< 3 - 4th Rank */
    RANK_5, /**< 4 - 5th Rank */
    RANK_6, /**< 5 - 6th Rank */
    RANK_7, /**< 6 - 7th Rank */
    RANK_8, /**< 7 - 8th Rank */
    RANK_NONE /**< 8 - Invalid Rank */
};

/**
  * Enumeration for Side or Color
  */
enum {
    WHITE, /**< 0 - White */
    BLACK, /**<  1 - Black*/
    BOTH /**<  2 - Both sides*/
};

/**
  * Enumeration for protocol mode
  */
enum {
    UCIMODE,
    XBOARDMODE,
    CONSOLEMODE
};

/**
  * Enumeration for Squares of the Board (a1 to h8 and invalid square)
  */
enum {
    A1 = 21, B1, C1, D1, E1, F1, G1, H1, // 1st Rank
    A2 = 31, B2, C2, D2, E2, F2, G2, H2, // 2nd Rank
    A3 = 41, B3, C3, D3, E3, F3, G3, H3, // 3rd Rank
    A4 = 51, B4, C4, D4, E4, F4, G4, H4, // 4th Rank
    A5 = 61, B5, C5, D5, E5, F5, G5, H5, // 5th Rank
    A6 = 71, B6, C6, D6, E6, F6, G6, H6, // 6th Rank
    A7 = 81, B7, C7, D7, E7, F7, G7, H7, // 7th Rank
    A8 = 91, B8, C8, D8, E8, F8, G8, H8, // 8th Rank
    NO_SQ /**<  Invalid Square or No Square*/,
    OFFBOARD /**<  Off Board Square*/
};

/**
  * Enumeration for True and False
  */
enum {
    FALSE, /**< 0 - False */
    TRUE /**< 1 - True */
};

/**
  * Enumeration for Castling Permissions
  */
enum {
    WKCA = 1, /**<  1 - White king-side castling*/
    WQCA = 2, /**<  2 - White queen-side castling*/
    BKCA = 4, /**< 4 - Black king-side castling */
    BQCA = 8 /**< 8 - Black queen-side castling */
};

/**
  * Structure for Move
  */
typedef struct {
	int move;
	int score;
} S_MOVE;

/**
  * Structure for Move List
  */
typedef struct {
	S_MOVE moves[MAXPOSITIONMOVES];
	// Number of moves in the moves list
	int count;
} S_MOVELIST;

/**
  * Structure for Principal Variation Table entry
  */
typedef struct {
	U64 posKey;
	int move;
} S_PVENTRY;

/**
  * Structure for Principal Variation Table
  */
typedef struct {
	S_PVENTRY *pTable;
	int numEntries;
} S_PVTABLE;

/**
  * Structure for Undo Moves
  */
typedef struct {
    int move;
    int castlePerm;
    int enPas;
    int fiftyMove;
    U64 posKey;
} S_UNDO;

/**
  * Structure for the Board
  */
typedef struct {
    /**
      * Pieces array
      */
    int pieces[BRD_SQ_NUM];

    /**
      * Pawns array. Represented by side/color - White, Black or Both.
      * One bit for each square for 64 squares of the board - 1 means a pawn is there on the board for that color, 0 mean there is no pawn on the board for that color
      */
    U64 pawns[NUM_COLORS];

    /**
      * Square the King is on
      */
    int kingSq[2];

    /**
      * Current side to move
      */
    int side;

    /**
      * En Passant square
      */
    int enPas;

    /**
      * 50 moved counter
      */
    int fiftyMove;

    /**
      * Ply count
      */
    int ply;

    /**
      * History of number of ply
      */
    int hisPly;

    /**
      * Castling permissions
      */
    int castlePerm;

    /**
      * Unique key for each position - Hash Key
      */
    U64 posKey;

    /**
      * Number of pieces on board
      */
    int pceNum[NUM_PIECES];

    /**
      * Number of big pieces by color
      */
    int bigPce[2];

    /**
      * Number of major pieces by color
      */
    int majPce[2];

    /**
      * Number of minor pieces by color
      */
    int minPce[2];

    /**
      * Value of the material for both side
      */
    int material[2];

    /**
      * Array of history for undo
      */
    S_UNDO history[MAXGAMEMOVES];

    /**
      * Piece List
      * For example -
      * A White Knight on e1: pList[wN][0] = E1;
      * Another White Knight on d4: pList[wN][1] = D4;
      */
    int pList[NUM_PIECES][MAX_PIECE_NUM];

    /**
      * Principal Variation Table
      */
    S_PVTABLE PvTable[1];

    /**
      * Principal Variation array
      */
    int PvArray[MAXDEPTH];

    /**
      * History of search
      */
    int searchHistory[13][BRD_SQ_NUM];
    /**
      * History of killer moves, stores 2 moves. Used for move ordering and beta cutoff
      */
	int searchKillers[2][MAXDEPTH];


} S_BOARD;

/**
  * Structure for the Move Search
  */
typedef struct {
    /**
      * Start time of search
      */
	int starttime;
	/**
      * Stop time of search
      */
	int stoptime;
	/**
      * Depth of search
      */
	int depth;
	/**
      * Depth set by protocol
      */
	int depthset;
	/**
      * Time set by protocol
      */
	int timeset;
	/**
      * Moves to go set by protocol based on time control
      */
	int movestogo;
	/**
      * If set to true, the search continues until the protocol asks to stop it
      */
	int infinite;

    /**
      * Count of all the positions the engine visits in the search tree
      */
	long nodes;

    /**
      * Set when the protocol sets it to quit search
      */
	int quit;
	/**
      * Signal by protocol to stop searching and send result
      */
	int stopped;

    /**
      * Fail high and fail high first. Used for move ordering.
      */
	float fh;
	float fhf;

    /**
      * Game mode and Post thinking
      */
	int GAME_MODE;
	int POST_THINKING;

} S_SEARCHINFO;

/* GAME MOVE */
/*

1, 2, 4, 8, 16, 32, 64: We need up to 7 bits to represent a from and to square (max 98)
1, 2, 4, 8: We need 4 bits to represent a piece captured (max 12)

0000 0000 0000 0000 0000 0111 1111 -> From 0x7F
0000 0000 0000 0011 1111 1000 0000 -> To >> 7, 0x7F
0000 0000 0011 1100 0000 0000 0000 -> Captured >> 14, 0xF
0000 0000 0100 0000 0000 0000 0000 -> EP 0x40000
0000 0000 1000 0000 0000 0000 0000 -> Pawn Start 0x80000
0000 1111 0000 0000 0000 0000 0000 -> Promoted Piece >> 20, 0xF
0001 0000 0000 0000 0000 0000 0000 -> Castle 0x1000000

*/

#define FROMSQ(m) ((m) & 0x7F)
#define TOSQ(m) (((m)>>7) & 0x7F)
#define CAPTURED(m) (((m)>>14) & 0xF)
#define PROMOTED(m) (((m)>>20) & 0xF)

#define MFLAGEP 0x40000
#define MFLAGPS 0x80000
#define MFLAGCA 0x1000000

#define MFLAGCAP 0x7C000
#define MFLAGPROM 0xF00000

#define NOMOVE 0

/*  MACROS  */

#define C64(constantU64) constantU64##ULL

/**
  * One Bit
  */
#define ONE_BIT C64(1)

/**
  * Get 120 based index from file and rank numbers
  */
#define FR2SQ(f,r) ((21 + (f)) + ((r) * 10))
/**
  * Get 64 based index from 120 based index
  */
#define SQ64(sq120) (Sq120ToSq64[(sq120)])
/**
  * Get 120 based index from 64 based index
  */
#define SQ120(sq64) (Sq64ToSq120[(sq64)])
/**
  * Ger 64 based index from file and rank numbers
  */
#define FR2SQ64(f,r) SQ64(FR2SQ(f,r))
#define POP(bb) PopBit(bb)
#define CNT(bb) CountBits(bb)

/* Alternate Definitions */
// Least Significant File (LSF) Mapping for 64 based index
#define SQINDEX_64(f,r) (8*r + f)
// File index = square index % 8
#define FILEINDEX_64(sq64) (sq64 & 7)
// Rank index = square index / 8
#define RANKINDEX_64(sq64) (sq64 >> 3)

// Based on 120 based index
#define SQINDEX_120(f,r) FR2SQ(f,r)
#define FILEINDEX_120(sq) ((sq % 10) - 1)
#define RANKINDEX_120(sq) ((sq / 10) - 2)

#define ISSQLIGHT(f,r) (((f + r) % 2) != 0)
#define ISSQDARK(f,r) (((f + r) % 2) == 0)

#define ISSQLIGHT_64(sq64) (ISSQLIGHT(FILEINDEX_64(sq64),RANKINDEX_64(sq64)))
#define ISSQDARK_64(sq64) (ISSQDARK(FILEINDEX_64(sq64),RANKINDEX_64(sq64)))

#define ISSQLIGHT_120(sq) (ISSQLIGHT(FILEINDEX_120(sq),RANKINDEX_120(sq)))
#define ISSQDARK_120(sq) (ISSQDARK(FILEINDEX_120(sq),RANKINDEX_120(sq)))

/* Compass Rose on 64 based index */
/**
  * Compass Rose for East on 64 based index
  */
#define EAST_64(sq64) (sq64 + 1)
/**
  * Compass Rose for West on 64 based index
  */
#define WEST_64(sq64) (sq64 - 1)
/**
  * Compass Rose for North on 64 based index
  */
#define NORTH_64(sq64) (sq64 + 8)
/**
  * Compass Rose for South on 64 based index
  */
#define SOUTH_64(sq64) (sq64 - 8)
/**
  * Compass Rose for North East on 64 based index
  */
#define NORTH_EAST_64(sq64) (sq64 + 9)
/**
  * Compass Rose for South East on 64 based index
  */
#define SOUTH_EAST_64(sq64) (sq64 - 7)
/**
  * Compass Rose for South West on 64 based index
  */
#define SOUTH_WEST_64(sq64) (sq64 - 9)
/**
  * Compass Rose for North West on 64 based index
  */
#define NORTH_WEST_64(sq64) (sq64 + 7)

/* Compass Rose on 120 based index */
/**
  * Compass Rose for East on 120 based index
  */
#define EAST_120(sq) (sq + 1)
/**
  * Compass Rose for West on 120 based index
  */
#define WEST_120(sq) (sq - 1)
/**
  * Compass Rose for North on 120 based index
  */
#define NORTH_120(sq) (sq + 10)
/**
  * Compass Rose for South on 120 based index
  */
#define SOUTH_120(sq) (sq - 10)
/**
  * Compass Rose for North East on 120 based index
  */
#define NORTH_EAST_120(sq) (sq + 11)
/**
  * Compass Rose for South East on 120 based index
  */
#define SOUTH_EAST_120(sq) (sq - 9)
/**
  * Compass Rose for South West on 120 based index
  */
#define SOUTH_WEST_120(sq) (sq - 11)
/**
  * Compass Rose for North West on 120 based index
  */
#define NORTH_WEST_120(sq) (sq + 9)

/**
  * Check for Empty Board
  */
#define EMPTY_BOARD(bb) (bb == 0)

// Bit by square on 64 based iindex
#define BIT_BY_SQUARE_64(sq64) (ONE_BIT << sq64)
#define IS_SET_64(bb,sq64) (bb & BIT_BY_SQUARE_64(sq64))
#define SET_64(bb,sq64) (bb |= BIT_BY_SQUARE_64(sq64))
#define TOGGLE_64(bb,sq64) (bb ^= BIT_BY_SQUARE_64(sq64))
// #define RESET_64(bb,sq64) (bb &= ~BIT_BY_SQUARE_64(sq64))
// Set and toggle (or, xor) might the faster way to reset a bit inside a register (not, and).
#define RESET_64(bb,sq64) (TOGGLE_64(SET_64(bb,sq64),sq64))

// To get a set of all upper squares or bits, either shift ~1 or -2 left by square
#define UPPER_SQUARES_64(sq64) (C64(~1) << sq64)
// Lower squares are simply Bit by Square minus one
#define LOWER_SQUARES_64(sq64) (BIT_BY_SQUARE_64(sq64) - 1)

#define LS1B_64(bb) (bb & (0 - bb))
// The intersection of a non empty bitboard with it's ones' decrement resets the LS1B
#define RESET_LS1B_64(bb) (bb &= (bb - 1))

#define SQNM_64(sq64) (((sq64) < 0 || (sq64) >= BRD_SQUARES) ? (SquareNames[65]) : (SquareNames[(sq64)]))

/*  GLOBALS  */

extern int Sq120ToSq64[BRD_SQ_NUM];
extern int Sq64ToSq120[BRD_SQUARES];
extern U64 SetMask[BRD_SQUARES];
extern U64 ClearMask[BRD_SQUARES];
extern U64 PiecesKeys[NUM_PIECES][BRD_SQ_NUM];
extern U64 SideKey;
extern U64 CastleKeys[16];

// Flags for piece types, value and color
extern int PieceBig[NUM_PIECES];
extern int PieceMaj[NUM_PIECES];
extern int PieceMin[NUM_PIECES];
extern int PieceVal[NUM_PIECES];
extern int PieceCol[NUM_PIECES];
extern int PiecePawn[NUM_PIECES];

extern int FilesBrd[BRD_SQ_NUM];
extern int RanksBrd[BRD_SQ_NUM];

// Holds the pieces we hit in the check attack
extern int PieceKnight[NUM_PIECES];
extern int PieceKing[NUM_PIECES];
extern int PieceRookQueen[NUM_PIECES];
extern int PieceBishopQueen[NUM_PIECES];
extern int PieceSlides[NUM_PIECES];

/*extern int PieceRook[NUM_PIECES];
extern int PieceQueen[NUM_PIECES];
extern int PieceBishop[NUM_PIECES];*/

/**
  * Clear a bit in the Bit Board given the 64 based index
  */
#define CLRBIT(bb,sq64) ((bb) &= ClearMask[(sq64)])
/**
  * Set a bit in the Bit Board given the 64 based index
  */
#define SETBIT(bb,sq64) ((bb) |= SetMask[(sq64)])

/**
  * Checks if the piece is a Bishop or Queen
  */
#define IsBQ(p) (PieceBishopQueen[(p)])
/**
  * Checks if the piece is a Rook or Queen
  */
#define IsRQ(p) (PieceRookQueen[(p)])
/**
  * Checks if the piece is a Knight
  */
#define IsKn(p) (PieceKnight[(p)])
/**
  * Checks if the piece is a King
  */
#define IsKi(p) (PieceKing[(p)])
/**
  * Checks if the piece is a Pawn
  */
#define IsPw(p) (PiecePawn[(p)])


/*  FUNCTIONS   */

// init.c
extern void AllInit();

// display.c
extern void DisplayLogicalBoardMapping();
extern void DisplayPhysicalBoardMapping();
extern void DisplayBoardMappings();
extern void PrintEmptyBoard();
extern void ShowSqAtBySide(const int side, const S_BOARD *pos);
extern void PrintBin(int move);

// bitboards.c
extern int PopBit(U64 *bitBoard);
extern int CountBits(U64 bitBoard);
extern void PrintBitBoard(U64 bb);

// test.c
extern void Test();

// hashkeys.c
extern U64 GeneratePosKey(const S_BOARD *pos);

// board.c
extern int CheckBoard(const S_BOARD *pos);
extern void ResetBoard(S_BOARD *pos);
extern int ParseFen(char *fen, S_BOARD *pos);
extern void PrintBoard(const S_BOARD *pos);
extern char* GetSquareNameFromNumber64(int sq64);
extern char* GetSquareNameFromNumber120(int sq120);
extern char* GetSquareNameFromFileRank(int file, int rank);
extern void UpdateListsMaterial(S_BOARD *pos);
extern void InitializeBoard(S_BOARD *board);
extern void ClearBoard(S_BOARD *board);

// data.c
extern char PceChar[];
extern char PceCharNoSide[];
extern char SideChar[];
extern char RankChar[];
extern char FileChar[];
extern char* SquareNames[];

// attack.c
extern int SqAttacked(const int sq, const int side, const S_BOARD *pos);

// io.c
extern char *PrSq(const int sq);
extern char *PrMove(const int move);
extern void PrintMove(const int move);
extern void PrintMoveList(const S_MOVELIST *list, const S_BOARD *pos);
extern int ParseMove(char* ptrChar, S_BOARD *pos);

// movegen.c
/*extern void AddQuietMove(const S_BOARD *pos, int move, S_MOVELIST *list);
extern void AddCaptureMove(const S_BOARD *pos, int move, S_MOVELIST *list);
extern void AddEnPassantMove(const S_BOARD *pos, int move, S_MOVELIST *list);
extern void AddWhitePawnCapMove(const S_BOARD *pos, const int from, const int to, const int cap, S_MOVELIST *list);
extern void AddWhitePawnMove(const S_BOARD *pos, const int from, const int to, S_MOVELIST *list);
extern void AddBlackPawnCapMove(const S_BOARD *pos, const int from, const int to, const int cap, S_MOVELIST *list);
extern void AddBlackPawnMove(const S_BOARD *pos, const int from, const int to, S_MOVELIST *list);*/
extern void GenerateAllMoves(const S_BOARD *pos, S_MOVELIST *list);
extern int InitMvvLva();
extern void GenerateAllCaps(const S_BOARD *pos, S_MOVELIST *list);

// validate.c
extern int SqOnBoard(const int sq);
extern int SideValid(const int side);
extern int FileRankValid(const int fr);
extern int PieceValidEmpty(const int pce);
extern int PieceValid(const int pce);

// makemove.c
extern int MoveExists(S_BOARD *pos, const int move);
extern int MakeMove(S_BOARD *pos, int move);
extern void TakeMove(S_BOARD *pos);

// perft.c
extern void PerftTest(int depth, S_BOARD *pos);

// search.c
//extern int IsRepetition(const S_BOARD *pos);
extern void SearchPosition(S_BOARD *pos, S_SEARCHINFO *info);

// misc.c
extern int GetTimeMs();
extern void ReadInput(S_SEARCHINFO *info);

// pvtable.c
extern void InitPvTable(S_PVTABLE *table);
extern void StorePvMove(const S_BOARD *pos, const int move);
extern int ProbePvTable(const S_BOARD *pos);

// evaluate.c
extern int EvalPosition(const S_BOARD *pos);

// uci.c
extern void Uci_Loop(S_BOARD *pos, S_SEARCHINFO *info);

#endif // DEFS_H
