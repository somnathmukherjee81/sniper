/***********************************************************
  * File Name: test.c
  * Author: Somnath Mukherjee
  * Description:
  * C functions for unit testing the sniper engine. Not part of the actual engine
  **********************************************************/

#ifndef TEST_C
#define TEST_C

#include "stdio.h"
#include "defs.h"

#define FEN1 "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define FEN2 "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define FEN3 "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"
#define FEN4 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
#define FEN5 "8/3q1p2/8/5P2/4Q3/8/8/8 w - - 0 2 "
#define PAWNMOVESW "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define PAWNMOVESB "rnbqkbnr/p1p1p3/3p3p/1p1p4/2P1Pp2/8/PP1P1PpP/RNBQKB1R b KQkq e3 0 1"
#define KNIGHTSKINGSW "5k2/1n6/4n3/6N1/8/3N4/8/5K2 w - - 0 1"
#define KNIGHTSKINGSB "5k2/1n6/4n3/6N1/8/3N4/8/5K2 b - - 0 1"
#define ROOKSW "6k1/8/5r2/8/1nR5/5N2/8/6K1 w - - 0 1"
#define ROOKSB "6k1/8/5r2/8/1nR5/5N2/8/6K1 b - - 0 1"
#define QUEENSW "6k1/8/4nq2/8/1nQ5/5N2/1N6/6K1 w - - 0 1 "
#define QUEENSB "6k1/8/4nq2/8/1nQ5/5N2/1N6/6K1 b - - 0 1 "
#define BISHOPSW "6k1/1b6/4n3/8/1n4B1/1B3N2/1N6/2b3K1 w - - 0 1 "
#define BISHOPSB "6k1/1b6/4n3/8/1n4B1/1B3N2/1N6/2b3K1 b - - 0 1 "
#define CASTLE1W "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1"
#define CASTLE1B "r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1"
#define CASTLE2W "3rk2r/8/8/8/8/8/6p1/R3K2R w KQk - 0 1"
#define CASTLE2B "3rk2r/8/8/8/8/8/6p1/R3K2R b KQk - 0 1"
#define MOVEGENW "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
#define MOVEGENB "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq - 0 1"
#define PERFTFEN "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
#define PARSEMOVEFEN "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N w - - 0 1"
#define WAC1 "2rr3k/pp3pp1/1nnqbN1p/3pN3/2pP4/2P3Q1/PPB4P/R4RK1 w - -"
#define WAC2 "r1b1k2r/ppppnppp/2n2q2/2b5/3NP3/2P1B3/PP3PPP/RN1QKB1R w KQkq - 0 1"

/**
  * Tests the PrintBitBoard function
  */
void TestPrintBitBoard() {
    printf("\n\n==================== TestPrintBitBoard - Start ====================\n\n");
    U64 playBitBoard = 0ULL;

    printf("\nStart - Empty Bit Board:\n\n");
    PrintBitBoard(playBitBoard);
    printf("End - Empty Bit Board:\n\n");

    playBitBoard |= (ONE_BIT << SQ64(D2));

    printf("Start - D2 Added:\n\n");
    PrintBitBoard(playBitBoard);
    printf("End - D2 Added:\n\n");

    playBitBoard |= (ONE_BIT << SQ64(G2));

    printf("Start - G2 Added:\n\n");
    PrintBitBoard(playBitBoard);
    printf("End - G2 Added:\n\n");

    playBitBoard = 0b0000000000000000000000000000000000000000000000001111111100000000;

    printf("Start - All White Pawns Added:\n\n");
    PrintBitBoard(playBitBoard);
    printf("End - All White Pawns Added:\n\n");

    playBitBoard = 0b0000000011111111000000000000000000000000000000000000000000000000;

    printf("Start - All Black Pawns Added:\n\n");
    PrintBitBoard(playBitBoard);
    printf("End - All Black Pawns Added:\n\n");

    playBitBoard = 0b0000000011111111000000000000000000000000000000001111111100000000;

    printf("Start - Both Pawns Added:\n\n");
    PrintBitBoard(playBitBoard);
    printf("End - Both Pawns Added:\n\n");

    printf("\n\n==================== TestPrintBitBoard - End ====================\n\n");
}

/**
  * Tests the PrintBitBoard function
  */
void TestCountAndPopBit() {
    printf("\n\n==================== TestCountAndPopBit - Start ====================\n\n");
    U64 playBitBoard = 0ULL;

    playBitBoard |= (ONE_BIT << SQ64(D2));
    playBitBoard |= (ONE_BIT << SQ64(D3));
    playBitBoard |= (ONE_BIT << SQ64(D4));

    printf("Initial Bit Board\n");
    PrintBitBoard(playBitBoard);

    int count = CNT(playBitBoard);
    printf("Initial Count: %d\n", count);
    printf("\n");

    printf("Popping 1st time\n");
    int index = POP(&playBitBoard);
    printf("Index: %d\n", index);

    printf("Bit Board after Pop 1\n");
    PrintBitBoard(playBitBoard);

    count = CNT(playBitBoard);
    printf("Count after Pop 1: %d\n", count);
    printf("\n");


    printf("Popping 2nd time\n");
    index = POP(&playBitBoard);
    printf("Index: %d\n", index);

    printf("Bit Board after Pop 2\n");
    PrintBitBoard(playBitBoard);

    count = CNT(playBitBoard);
    printf("Count after Pop 2: %d\n", count);
    printf("\n");


    printf("Popping 3rd time\n");
    index = POP(&playBitBoard);
    printf("Index: %d\n", index);

    printf("Bit Board after Pop 3\n");
    PrintBitBoard(playBitBoard);

    count = CNT(playBitBoard);
    printf("Count after Pop 3: %d\n", count);

    printf("\n\n==================== TestCountAndPopBit - End ====================\n\n");
}

/**
  * Test masking
  */
void TestMasking() {
    printf("\n\n==================== TestMasking - Start ====================\n\n");

    int index = 0;

    U64 playBitBoard = 0ULL;

    for(index = 0 ; index < BRD_SQUARES ; index++) {
        printf("SetMask at index %d\n", index);
        PrintBitBoard(SetMask[index]);
        printf("\n");
        printf("ClearMask at index %d\n", index);
        PrintBitBoard(ClearMask[index]);
    }

    SETBIT(playBitBoard, 61);
    printf("\n");
    printf("Bit set at square 61\n");
    PrintBitBoard(playBitBoard);

    CLRBIT(playBitBoard, 61);
    printf("\n");
    printf("Bit cleared at square 61\n");
    PrintBitBoard(playBitBoard);

    printf("\n\n==================== TestMasking - End ====================\n\n");
}

/**
  * Learn XOR operation
  */
void LearnXOR() {
    printf("\n\n==================== LearnXOR - Start ====================\n\n");

    int PieceOne = rand();
    int PieceTwo = rand();
    int PieceThree = rand();
    int PieceFour = rand();

    printf("PieceOne: %X\n", PieceOne);
    printf("PieceTwo: %X\n", PieceTwo);
    printf("PieceThree: %X\n", PieceThree);
    printf("PieceFour: %X\n", PieceFour);

    int Key = PieceOne ^ PieceTwo ^ PieceFour;
    int TempKey = PieceTwo;
    TempKey ^= PieceThree;
    TempKey ^= PieceFour;
    TempKey ^= PieceOne;

    printf("Key: %X\n", Key);
    printf("TempKey: %X\n", TempKey);

    TempKey ^= PieceThree;
    printf("TempKey (Three Out): %X\n", TempKey);

    TempKey ^= PieceThree;
    printf("TempKey (Three In Again): %X\n", TempKey);

    printf("\n\n==================== LearnXOR - End ====================\n\n");
}

/**
  * Test parse FEN and print board
  */
void TestParseFenAndPrintBoard() {
    printf("\n\n==================== TestParseFenAndPrintBoard - Start ====================\n\n");

    S_BOARD board[1];
    InitializeBoard(board);

	ParseFen(START_FEN, board);
	PrintBoard(board);

	ParseFen(FEN1, board);
	PrintBoard(board);

	ParseFen(FEN2, board);
	PrintBoard(board);

	ParseFen(FEN3, board);
	PrintBoard(board);

	ParseFen(FEN4, board);
	PrintBoard(board);
	printf("\nWhite Pawns: \n");
	PrintBitBoard(board->pawns[WHITE]);
	printf("\nBlack Pawns: \n");
	PrintBitBoard(board->pawns[BLACK]);
	printf("\nAll Pawns: \n");
	PrintBitBoard(board->pawns[BOTH]);

	ResetBoard(board);
	PrintBoard(board);

	ClearBoard(board);

	printf("\n\n==================== TestParseFenAndPrintBoard - End ====================\n\n");
}

/**
  * Test check board
  */
void TestCheckBoard() {
    printf("\n\n==================== TestCheckBoard - Start ====================\n\n");

    S_BOARD board[1];
    InitializeBoard(board);

	ParseFen(FEN4, board);
	PrintBoard(board);

    ASSERT(CheckBoard(board));

    /*printf("\nForced Assert failure...\n");
    board->pceNum[wP]--;
    ASSERT(CheckBoard(board));*/

    ClearBoard(board);

    printf("\n\n==================== TestCheckBoard - End ====================\n\n");
}

/**
  * Test check board
  */
void TestSquareAttacked() {
    printf("\n\n==================== TestSquareAttacked - Start ====================\n\n");

    S_BOARD board[1];
    InitializeBoard(board);

	ParseFen(FEN4, board);
	PrintBoard(board);

	printf("\n\nWhite Attacking:\n");
	ShowSqAtBySide(WHITE,board);

	printf("\n\nBlack Attacking:\n");
	ShowSqAtBySide(BLACK,board);

	ClearBoard(board);

	printf("\n\n==================== TestSquareAttacked - End ====================\n\n");
}

/**
  * Test Files and Ranks mapping
  */
void TestFilesRanksMapping() {
    printf("\n\n==================== TestFilesRanksMapping - Start ====================\n\n");

    int index = 0;

    printf("\nFilesBrd:\n");
    for(index = 0 ; index < BRD_SQ_NUM ; ++index) {
        if(index % 10 == 0 && index != 0) {
            printf("\n");
        }

        printf("%5d\t", FilesBrd[index]);
    }

    printf("\nRanksBrd:\n");
    for(index = BRD_SQ_NUM - 1 ; index >= 0 ; index--) {
        if(index % 10 == 9) {
            printf("\n");
        }

        printf("%5d\t", RanksBrd[index]);
    }

    printf("\n\n==================== TestFilesRanksMapping - End ====================\n\n");
}

/**
  * Test Move Format and Bits
  */
void TestMoveFormatAndBits() {
    printf("\n\n==================== TestMoveFormatAndBits - Start ====================\n\n");

    int move = 0;
	int from = 6;
	int to = 12;
	int cap = wR;
	int prom = bR;

	printf("\nTesting Move Format and Bits...\n");

	move = ( ( from ) | ( to << 7 ) | ( cap << 14 ) | ( prom << 20) );

	printf("\ndec:%d hex:%X\n",move,move);
	PrintBin(move);

	printf("from:%d to:%d cap:%d prom:%d\n",
		FROMSQ(move),TOSQ(move),CAPTURED(move),
		PROMOTED(move));

	move |= MFLAGPS;

	printf("is PST:%s\n",(move & MFLAGPS)?"YES":"NO");

	printf("\n\n==================== TestMoveFormatAndBits - End ====================\n\n");
}

/**
  * Test Print Move and Square
  */
void TestPrintMoveAndSquare() {
    printf("\n\n==================== TestPrintMoveAndSquare - Start ====================\n\n");

    int move = 0;
	int from = A2; int to = H7;
	int cap = wR; int prom = bN;

	printf("\nTesting Print Move and Square...\n");

	move = ( ( from ) | ( to << 7 ) | ( cap << 14 ) | ( prom << 20) );

	printf("from:%d to:%d cap:%d prom:%d\n",
		FROMSQ(move),TOSQ(move),CAPTURED(move),
		PROMOTED(move));

	printf("Algebraic from:%s\n",PrSq(from));
	printf("Algebraic to:%s\n",PrSq(to));
	printf("Algebraic move:%s\n",PrMove(move));

	printf("\n\n==================== TestPrintMoveAndSquare - End ====================\n\n");
}

/**
  * Test White Moves
  */
void TestWhiteMoves() {
    printf("\n\n==================== TestWhiteMoves - Start ====================\n\n");

    S_BOARD board[1];
    S_MOVELIST list[1];
    InitializeBoard(board);

    printf("\nTesting White Moves...\n");

	ParseFen(PAWNMOVESW,board);
	PrintBoard(board);

	GenerateAllMoves(board,list);

	PrintMoveList(list, board);

	ClearBoard(board);

	printf("\n\n==================== TestWhiteMoves - End ====================\n\n");
}

/**
  * Test Black Moves
  */
void TestBlackMoves() {
    printf("\n\n==================== TestBlackMoves - Start ====================\n\n");

    S_BOARD board[1];
    S_MOVELIST list[1];
    InitializeBoard(board);

    printf("\nTesting Black Moves...\n");

	ParseFen(PAWNMOVESB,board);
	PrintBoard(board);

	GenerateAllMoves(board,list);

	PrintMoveList(list, board);

	ClearBoard(board);

	printf("\n\n==================== TestBlackMoves - End ====================\n\n");
}

/**
  * Test Generate Moves for White Non-Sliding pieces
  */
void TestGenerateWhiteMovesForNonSlide() {
    printf("\n\n==================== TestGenerateWhiteMovesForNonSlide - Start ====================\n\n");

    S_BOARD board[1];
    S_MOVELIST list[1];
    InitializeBoard(board);

    printf("\nTesting Generate Moves for White Non-Sliding pieces...\n");

	ParseFen(KNIGHTSKINGSW,board);
	PrintBoard(board);

	GenerateAllMoves(board,list);

	PrintMoveList(list, board);

	ClearBoard(board);

	printf("\n\n==================== TestGenerateWhiteMovesForNonSlide - End ====================\n\n");
}

/**
  * Test Generate Moves for Black Non-Sliding pieces
  */
void TestGenerateBlackMovesForNonSlide() {
    printf("\n\n==================== TestGenerateBlackMovesForNonSlide - Start ====================\n\n");

    S_BOARD board[1];
    S_MOVELIST list[1];
    InitializeBoard(board);

    printf("\nTesting Generate Moves for Black Non-Sliding pieces...\n");

	ParseFen(KNIGHTSKINGSB,board);
	PrintBoard(board);

	GenerateAllMoves(board,list);

	PrintMoveList(list, board);

	ClearBoard(board);

	printf("\n\n==================== TestGenerateBlackMovesForNonSlide - End ====================\n\n");
}

/**
  * Test Generate Moves for White Rooks
  */
void TestGenerateWhiteMovesForRooks() {
    printf("\n\n==================== TestGenerateWhiteMovesForRooks - Start ====================\n\n");

    S_BOARD board[1];
    S_MOVELIST list[1];
    InitializeBoard(board);

    printf("\nTesting Generate Moves for White Rooks...\n");

	ParseFen(ROOKSW,board);
	PrintBoard(board);

	GenerateAllMoves(board,list);

	PrintMoveList(list, board);

	ClearBoard(board);

	printf("\n\n==================== TestGenerateWhiteMovesForRooks - End ====================\n\n");
}

/**
  * Test Generate Moves for Black Rooks
  */
void TestGenerateBlackMovesForRooks() {
    printf("\n\n==================== TestGenerateBlackMovesForRooks - Start ====================\n\n");

    S_BOARD board[1];
    S_MOVELIST list[1];
    InitializeBoard(board);

    printf("\nTesting Generate Moves for Black Rooks...\n");

	ParseFen(ROOKSB,board);
	PrintBoard(board);

	GenerateAllMoves(board,list);

	PrintMoveList(list, board);

	ClearBoard(board);

	printf("\n\n==================== TestGenerateBlackMovesForRooks - End ====================\n\n");
}

/**
  * Test Generate Moves for White Queens
  */
void TestGenerateWhiteMovesForQueens() {
    printf("\n\n==================== TestGenerateWhiteMovesForQueens - Start ====================\n\n");

    S_BOARD board[1];
    S_MOVELIST list[1];
    InitializeBoard(board);

    printf("\nTesting Generate Moves for White Queens...\n");

	ParseFen(QUEENSW,board);
	PrintBoard(board);

	GenerateAllMoves(board,list);

	PrintMoveList(list, board);

	ClearBoard(board);

	printf("\n\n==================== TestGenerateWhiteMovesForQueens - End ====================\n\n");
}

/**
  * Test Generate Moves for Black Queens
  */
void TestGenerateBlackMovesForQueens() {
    printf("\n\n==================== TestGenerateBlackMovesForQueens - Start ====================\n\n");

    S_BOARD board[1];
    S_MOVELIST list[1];
    InitializeBoard(board);

    printf("\nTesting Generate Moves for Black Queens...\n");

	ParseFen(QUEENSB,board);
	PrintBoard(board);

	GenerateAllMoves(board,list);

	PrintMoveList(list, board);

	ClearBoard(board);

	printf("\n\n==================== TestGenerateBlackMovesForQueens - End ====================\n\n");
}

/**
  * Test Generate Moves for White Bishops
  */
void TestGenerateWhiteMovesForBishops() {
    printf("\n\n==================== TestGenerateWhiteMovesForBishops - Start ====================\n\n");

    S_BOARD board[1];
    S_MOVELIST list[1];
    InitializeBoard(board);

    printf("\nTesting Generate Moves for White Bishops...\n");

	ParseFen(BISHOPSW,board);
	PrintBoard(board);

	GenerateAllMoves(board,list);

	PrintMoveList(list, board);

	ClearBoard(board);

	printf("\n\n==================== TestGenerateWhiteMovesForBishops - End ====================\n\n");
}

/**
  * Test Generate Moves for Black Bishops
  */
void TestGenerateBlackMovesForBishops() {
    printf("\n\n==================== TestGenerateBlackMovesForBishops - Start ====================\n\n");

    S_BOARD board[1];
    S_MOVELIST list[1];
    InitializeBoard(board);

    printf("\nTesting Generate Moves for Black Bishops...\n");

	ParseFen(BISHOPSB,board);
	PrintBoard(board);

	GenerateAllMoves(board,list);

	PrintMoveList(list, board);

	ClearBoard(board);

	printf("\n\n==================== TestGenerateBlackMovesForBishops - End ====================\n\n");
}

/**
  * Test Generate Moves for White Castling - Test 1
  */
void TestGenerateWhiteCastling1() {
    printf("\n\n==================== TestGenerateWhiteCastling1 - Start ====================\n\n");

    S_BOARD board[1];
    S_MOVELIST list[1];
    InitializeBoard(board);

    printf("\nTesting Generate Moves for White Castling - Test 1...\n");

	ParseFen(CASTLE1W,board);
	PrintBoard(board);

	GenerateAllMoves(board,list);

	PrintMoveList(list, board);

	ClearBoard(board);

	printf("\n\n==================== TestGenerateWhiteCastling1 - End ====================\n\n");
}

/**
  * Test Generate Moves for Black Castling - Test 1
  */
void TestGenerateBlackCastling1() {
    printf("\n\n==================== TestGenerateBlackCastling1 - Start ====================\n\n");

    S_BOARD board[1];
    S_MOVELIST list[1];
    InitializeBoard(board);

    printf("\nTesting Generate Moves for Black Castling - Test 1...\n");

	ParseFen(CASTLE1B,board);
	PrintBoard(board);

	GenerateAllMoves(board,list);

	PrintMoveList(list, board);

	ClearBoard(board);

	printf("\n\n==================== TestGenerateBlackCastling1 - End ====================\n\n");
}

/**
  * Test Generate Moves for White Castling - Test 2
  */
void TestGenerateWhiteCastling2() {
    printf("\n\n==================== TestGenerateWhiteCastling2 - Start ====================\n\n");

    S_BOARD board[1];
    S_MOVELIST list[1];
    InitializeBoard(board);

    printf("\nTesting Generate Moves for White Castling - Test 2...\n");

	ParseFen(CASTLE2W,board);
	PrintBoard(board);

	GenerateAllMoves(board,list);

	PrintMoveList(list, board);

	ClearBoard(board);

	printf("\n\n==================== TestGenerateWhiteCastling2 - End ====================\n\n");
}

/**
  * Test Generate Moves for Black Castling - Test 2
  */
void TestGenerateBlackCastling2() {
    printf("\n\n==================== TestGenerateBlackCastling2 - Start ====================\n\n");

    S_BOARD board[1];
    S_MOVELIST list[1];
    InitializeBoard(board);

    printf("\nTesting Generate Moves for Black Castling - Test 2...\n");

	ParseFen(CASTLE2B,board);
	PrintBoard(board);

	GenerateAllMoves(board,list);

	PrintMoveList(list, board);

	ClearBoard(board);

	printf("\n\n==================== TestGenerateBlackCastling2 - End ====================\n\n");
}

/**
  * Test Generate Moves for White
  */
void TestGenerateWhiteMoves() {
    printf("\n\n==================== TestGenerateWhiteMoves - Start ====================\n\n");

    S_BOARD board[1];
    S_MOVELIST list[1];
    InitializeBoard(board);

    printf("\nTesting Generate Moves for White...\n");

	ParseFen(MOVEGENW,board);
	PrintBoard(board);

	GenerateAllMoves(board,list);

	PrintMoveList(list, board);

	ClearBoard(board);

	printf("\n\n==================== TestGenerateWhiteMoves - End ====================\n\n");
}

/**
  * Test Generate Moves for Black
  */
void TestGenerateBlackMoves() {
    printf("\n\n==================== TestGenerateBlackMoves - Start ====================\n\n");

    S_BOARD board[1];
    S_MOVELIST list[1];
    InitializeBoard(board);

    printf("\nTesting Generate Moves for Black...\n");

	ParseFen(MOVEGENB,board);
	PrintBoard(board);

	GenerateAllMoves(board,list);

	PrintMoveList(list, board);

	ClearBoard(board);

	printf("\n\n==================== TestGenerateBlackMoves - End ====================\n\n");
}

/**
  * Test Make and Take moves
  */
void TestMakeAndTakeMoves() {
    printf("\n\n==================== TestMakeAndTakeMoves - Start ====================\n\n");

    S_BOARD board[1];
	S_MOVELIST list[1];
	InitializeBoard(board);

	printf("\nTesting Make and Take moves...\n");

	ParseFen(START_FEN,board);
	GenerateAllMoves(board,list);

	PrintMoveList(list, board);

	int MoveNum = 0;
	int move = 0;

	PrintBoard(board);
	//getchar();

	for(MoveNum = 0; MoveNum < list->count; ++MoveNum) {
		move = list->moves[MoveNum].move;

        printf("\nMaking move:%s (%s)\n", PrMove(move), PrAlgMove(move, board));

		if(!MakeMove(board, move)) {
			continue;
		}

		printf("\nMade:%s (%s)\n", PrMove(move), PrAlgMove(move, board));
		PrintBoard(board);

        printf("\nTaking back move:%s (%s)\n", PrMove(move), PrAlgMove(move, board));
		TakeMove(board);
		printf("\nTaken:%s (%s)\n", PrMove(move), PrAlgMove(move, board));
		PrintBoard(board);

		//getchar();
	}

	ClearBoard(board);

	printf("\n\n==================== TestMakeAndTakeMoves - End ====================\n\n");
}

/**
  * Run Perft test 1
  */
void RunPerftTest1() {
    printf("\n\n==================== RunPerftTest1 - Start ====================\n\n");

    S_BOARD board[1];
	S_MOVELIST list[1];
	InitializeBoard(board);

	int depth;
	int maxDepth = 6;

	for(depth = 1; depth <= maxDepth; depth++) {
        ParseFen(START_FEN,board);
        printf("\nDepth: %d\n", depth);
        PerftTest(depth, board);
	}

	ClearBoard(board);

	printf("\n\n==================== RunPerftTest1 - End ====================\n\n");
}

/**
  * Run Perft test 2
  */
void RunPerftTest2() {
    printf("\n\n==================== RunPerftTest2 - Start ====================\n\n");

    S_BOARD board[1];
	S_MOVELIST list[1];
	InitializeBoard(board);

	int depth;
	int maxDepth = 6;

	for(depth = 1; depth <= maxDepth; depth++) {
        ParseFen(PERFTFEN,board);
        printf("\nDepth: %d\n", depth);
        PerftTest(depth, board);
	}

	ClearBoard(board);

	printf("\n\n==================== RunPerftTest2 - End ====================\n\n");
}

/**
  * Test Parse Move input by user
  */
void TestParseAndSearchMoveFromUser(char* fen) {
    printf("\n\n==================== TestParseMoveFromUser - Start ====================\n\n");

    S_BOARD board[1];
	S_MOVELIST list[1];
	S_SEARCHINFO info[1];
	const int depth = 4;
	InitializeBoard(board);

	ParseFen(fen,board);

    // Input move
	char input[6];
	int move = NOMOVE;
	int PvNum = 0;
	int max = 0;

	while(TRUE) {
        PrintBoard(board);
        printf("\nPlease enter a move: ");
        fgets(input, 6, stdin);

        // Quit if user has entered 'q'; Take a move back if user has entered 't'; otherwise parse the move and make it
        if(input[0] == 'q') {
            printf("\nQuitting test\n");
            break;
        } else  if(input[0] == 't') {
            printf("\nTaking a move back\n");
            TakeMove(board);
        } else if(input[0] == 'p') {
            printf("\nRunning perft test (depth %d)\n", depth);
			PerftTest(depth, board);
		} else if(input[0] == 'r') {
            printf("\nGetting PvLines (depth %d)\n", depth);
            max = GetPvLine(depth, board);
			printf("\nPvLine of %d Moves: ", max);
			for(PvNum = 0; PvNum < max; ++PvNum) {
				move = board->PvArray[PvNum];
				printf(" %s (%s)", PrMove(move), PrAlgMove(move, board));
			}
			printf("\n");
		} else if(input[0] == 's') {
		    printf("\nSearching (depth %d)\n", depth);
            info->depth = depth;
            info->starttime = GetTimeMs();
			info->stoptime = GetTimeMs() + 200000;
			SearchPosition(board, info);
			printf("\nSearch Completed (depth %d)\n", depth);
		} else {
            move = ParseMove(input, board);
            if(move != NOMOVE) {
                StorePvMove(board, move);
                MakeMove(board, move);
                /*if(IsRepetition(board)) {
                    printf("\nRepetition Detected\n");
                }*/
            } else {
                printf("\nMove (%s) not parsed. No Move made. Invalid.\n", input);
            }
        }

        fflush(stdin);
	}

	ClearBoard(board);

	printf("\n\n==================== TestParseMoveFromUser - End ====================\n\n");
}

/**
  * Test Parse Move
  */
void TestParseAndSearchMove(char* fen, const char *inputs[], int numInputs, int depth) {
    printf("\n\n==================== TestParseMove - Start ====================\n\n");

    S_BOARD board[1];
	S_MOVELIST list[1];
	S_SEARCHINFO info[1];
	InitializeBoard(board);

	ParseFen(fen,board);

    // Input move
    //const int numInputs = 8;
	//const char inputs[8][6] = {"b7a8r", "t", "b7b8n", "t", "a7b8", "p", "r", "q"};
	char* currentInput;
	int index;
	int move = NOMOVE;
	int PvNum = 0;
	int max = 0;

	for(index = 0; index < numInputs; index++) {
        PrintBoard(board);

        currentInput = inputs[index];

        printf("\nMove: %s\n", currentInput);

        // Quit if user has entered 'q'; Take a move back if user has entered 't'; otherwise parse the move and make it
        if(currentInput[0] == 'q') {
            printf("\nQuitting test\n");
            break;
        } else  if(currentInput[0] == 't') {
            printf("\nTaking a move back\n");
            TakeMove(board);
        } else if(currentInput[0] == 'p') {
            printf("\nRunning perft test (depth %d)\n", depth);
			PerftTest(depth, board);
		} else if(currentInput[0] == 'r') {
            printf("\nGetting PvLines (depth %d)\n", depth);
            max = GetPvLine(depth, board);
			printf("\nPvLine of %d Moves: ", max);
			for(PvNum = 0; PvNum < max; ++PvNum) {
				move = board->PvArray[PvNum];
				printf(" %s (%s)", PrMove(move), PrAlgMove(move, board));
			}
			printf("\n");
		} else if(currentInput[0] == 's') {
		    printf("\nSearching (depth %d)\n", depth);
            info->depth = depth;
            info->starttime = GetTimeMs();
			info->stoptime = GetTimeMs() + 200000;
			SearchPosition(board, info);
			printf("\nSearch Completed (depth %d)\n", depth);
		} else {
            move = ParseMove(currentInput, board);
            if(move != NOMOVE) {
                StorePvMove(board, move);
                MakeMove(board, move);
                /*if(IsRepetition(board)) {
                    printf("\nRepetition Detected\n");
                }*/
            } else {
                printf("\nMove (%s) not parsed. No Move made. Invalid.\n", currentInput);
            }
        }
	}

	ClearBoard(board);

	printf("\n\n==================== TestParseMove - End ====================\n\n");
}

/**
  * Driver test function
  */
void Test() {
    DisplayBoardMappings();

    TestPrintBitBoard();

    TestCountAndPopBit();

    PrintEmptyBoard();

    TestMasking();

    LearnXOR();

    TestParseFenAndPrintBoard();

    TestFilesRanksMapping();

    TestCheckBoard();

    TestSquareAttacked();

    TestMoveFormatAndBits();

    TestPrintMoveAndSquare();

    TestWhiteMoves();

    TestBlackMoves();

    TestGenerateWhiteMovesForNonSlide();

    TestGenerateBlackMovesForNonSlide();

    TestGenerateWhiteMovesForRooks();

    TestGenerateBlackMovesForRooks();

    TestGenerateWhiteMovesForQueens();

    TestGenerateBlackMovesForQueens();

    TestGenerateWhiteMovesForBishops();

    TestGenerateBlackMovesForBishops();

    TestGenerateWhiteCastling1();

    TestGenerateBlackCastling1();

    TestGenerateWhiteCastling2();

    TestGenerateBlackCastling2();

    TestGenerateWhiteMoves();

    TestGenerateBlackMoves();

    TestMakeAndTakeMoves();

    //RunPerftTest1();

    //RunPerftTest2();

    //TestParseAndSearchMoveFromUser(PARSEMOVEFEN);

    printf("\n\n==================== Testing Parse and Search Move - Start ====================\n\n");
    const char *inputs1[] = {"b7a8r", "t", "b7b8n", "t", "a7b8", "p", "q"};
    TestParseAndSearchMove(PARSEMOVEFEN, inputs1, 7, 4);
    printf("\n\n==================== Testing Parse and Search Move - End ====================\n\n");

    // To test repetition
    //TestParseAndSearchMoveFromUser(START_FEN);

    printf("\n\n==================== Testing Repetition - Start ====================\n\n");
    const char *inputs2[] = {"e2e4", "e7e5", "a2a3", "h7h6", "t", "t", "t", "t", "r", "q"};
    TestParseAndSearchMove(START_FEN, inputs2, 10, 4);
    printf("\n\n==================== Testing Repetition - End ====================\n\n");

    // Testing Search from start position
    printf("\n\n==================== Testing Search Move from Start Position - Start ====================\n\n");
    const char *inputs3[] = {"s",  "q"};
    TestParseAndSearchMove(START_FEN, inputs3, 2, 4);
    printf("\n\n==================== Testing Search Move from Start Position - End ====================\n\n");

    // Testing Search from WAC1 - Mate in 3
    printf("\n\n==================== Testing Search Move for Mate in 3 - Start ====================\n\n");
    TestParseAndSearchMove(WAC1, inputs3, 2, 4);
    printf("\n\n==================== Testing Search Move for Mate in 3 - End ====================\n\n");


    // Testing Search from WAC2 - for picking a move
    printf("\n\n==================== Testing Search Move for Picking a Move - Start ====================\n\n");
    TestParseAndSearchMove(WAC2, inputs3, 2, 6);
    printf("\n\n==================== Testing Search Move for Picking a Move - End ====================\n\n");
}

#endif // TEST_C
