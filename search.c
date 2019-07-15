/***********************************************************
  * File Name: search.c
  * Author: Somnath Mukherjee
  * Description:
  * C functions for move search
  **********************************************************/

#ifndef SEARCH_C
#define SEARCH_C

#include "defs.h"
#include "stdio.h"

#define INFINITE 30000
#define MATE 29000

/***************** Algorithm **********************

int search(depth) {
	int currDepth = 1;
	// Do iterative deepening: search iteratively with increasing depth and do move probing to optimize the alphabeta
	iterativeDeepen(depth) {
		alphabeta(depth);
		currDepth++;
	}
}

int alphabeta(depth) {
	// Repetition leads to balanced score
	IsRepeated() {
		return 0;
	}

	// Do we need to interrupt search: Ran out of time; GUI send interrupt
	InInterrupt() {
		SetQuit();
		return 0;
	}

	// Generate moves
	GenMoves();
	// Probe moves (principal variations) into hash table for optimized alphabeta
	ProbePvMoves(); // Need Hash Table
	if(have PvMove) {
		// Order the principal variation moves based on score
		OrderPvMove();
	}

	// Loop the moves
	LoopMoves() {
		// Make move
		Make();
		// Run alphabeta with reduced depth
		alphabeta(depth - 1);
		// Take back move
		Take();

		// If it's beta cut, return beta
		isbetaCut() {
			return beta;
		}

		// If it's alpha cut note the nest move and increase alpha
		isAlphaCut() {
			note bestMove;
			increase alpha;
		}
	}

	// If there is no legal move and if in check then it's checkmate, otherwise it's stalemate
	NoLegalMoveMade() {
		if(InCheck) {
			return Mate;
		}
		else {
			return 0;
		}
	}

	if(Alpha improved) {
		store PvMove;
	}

	return alpha;
}

  ***********************************************/


/**
  * Function to check if time up or interrupted by protocol
  *
  */
static void CheckUp(S_SEARCHINFO *info) {
	// Check if time up, or interrupt from GUI
	if(info->timeset == TRUE && GetTimeMs() > info->stoptime) {
		info->stopped = TRUE;
	}

	ReadInput(info);
}

/**
  * Function to pick the next move to evaluate
  *
  * @param moveNum Move Number
  * @param *list Pointer to the move list
  */
static void PickNextMove(int moveNum, S_MOVELIST *list) {

	S_MOVE temp;
	int index = 0;
	int bestScore = 0;
	int bestNum = moveNum;

    // Loop from current move number to rest of the moves
	for (index = moveNum; index < list->count; ++index) {
        // If the score is best than the current best score
		if (list->moves[index].score > bestScore) {
			bestScore = list->moves[index].score;
			bestNum = index;
		}
	}

    // Swap the better scoring move with the current one
	temp = list->moves[moveNum];
	list->moves[moveNum] = list->moves[bestNum];
	list->moves[bestNum] = temp;
}

/**
  * Function to check if there is a move repetition
  *
  * @param *pos Pointer to the board structure
  */
static int IsRepetition(const S_BOARD *pos) {
	int index = 0;

    // Loop through the position keys in history. Note using fifty move counter improves this
    // as a capture or pawn move can't lead to repeating position
	for(index = pos->hisPly - pos->fiftyMove; index < pos->hisPly-1; ++index) {

        ASSERT(index >= 0 && index < MAXGAMEMOVES);

        // If any historical position key matches the current position key then it's a repetition
		if(pos->posKey == pos->history[index].posKey) {
			return TRUE;
		}
	}

	return FALSE;
}

/**
  * Function to clear stats and heuristic variables
  *
  * @param *pos Pointer to the board structure
  * @param *info Pointer to the search position structure
  */
static void ClearForSearch(S_BOARD *pos, S_SEARCHINFO *info) {

	int index = 0;
	int index2 = 0;

    // Clear the Search History array
	for(index = 0; index < 13; ++index) {
		for(index2 = 0; index2 < BRD_SQ_NUM; ++index2) {
			pos->searchHistory[index][index2] = 0;
		}
	}

    // Clear the Search Killers array
	for(index = 0; index < 2; ++index) {
		for(index2 = 0; index2 < MAXDEPTH; ++index2) {
			pos->searchKillers[index][index2] = 0;
		}
	}

    // Clear the Principal Variation Table
	ClearPvTable(pos->PvTable);
	// Reset the ply
	pos->ply = 0;

    // Set Start Time: Now set from UCI
	//info->starttime = GetTimeMs();
	// Reset the Stopped flag
	info->stopped = 0;
	// Reset the number positions the engine has visited
	info->nodes = 0;

    // Reset fail high and fail high first
	info->fh = 0;
	info->fhf = 0;
}

/**
  * Function for Quiescence search
  * Quiescence search in Wiki: http://en.wikipedia.org/wiki/Quiescence_search
  * Quiescence search in Chess Programming Wikispace: https://chessprogramming.wikispaces.com/Quiescence+Search
  *
  * @param *pos Pointer to the board structure
  * @param *info Pointer to the search position structure
  */
static int Quiescence(int alpha, int beta, S_BOARD *pos, S_SEARCHINFO *info) {

	ASSERT(CheckBoard(pos));

    // Do we need to interrupt search: Ran out of time; GUI send interrupt. Check only after 2047 nodes
	if((info->nodes & 2047 ) == 0) {
		CheckUp(info);
	}

	// Increment number of nodes visited
	info->nodes++;

    // Repetition leads to balanced score
    // If there is repetition or fifty moves rule is met, it's a balanced score
	if(IsRepetition(pos) || pos->fiftyMove >= 100) {
		return 0;
	}

    // If the depth has reached its limit, return the evaluation of the current position
	if(pos->ply > MAXDEPTH - 1) {
		return EvalPosition(pos);
	}

    // Evaluate score without even making a move
	int score = EvalPosition(pos);

    // If our score is greater than or equal to beta return beta
	if(score >= beta) {
		return beta;
	}

    // If our score is greater than alpha update alpha
	if(score > alpha) {
		alpha = score;
	}

	S_MOVELIST list[1];
	// Generate all capture moves
    GenerateAllCaps(pos, list);

    int moveNum = 0;
	int legal = 0;
	int oldAlpha = alpha;
	int bestMove = NOMOVE;
	score = -INFINITE;
	int pvMove = ProbePvTable(pos);

    // Loop through the moves
	for(moveNum = 0; moveNum < list->count; ++moveNum) {

		PickNextMove(moveNum, list);

        if (!MakeMove(pos, list->moves[moveNum].move))  {
            continue;
        }

		legal++;
		score = -Quiescence(-beta, -alpha, pos, info);
        TakeMove(pos);

        // If interrupted, break and ignore evaluation
        if(info->stopped == TRUE) {
			return 0;
		}

		if(score > alpha) {
			if(score >= beta) {
				if(legal == 1) {
					info->fhf++;
				}
				info->fh++;
				return beta;
			}

			alpha = score;
			bestMove = list->moves[moveNum].move;
		}
    }

	if(alpha != oldAlpha) {
		StorePvMove(pos, bestMove);
	}

	return alpha;
}

/**
  * Function for Alpha Beta Search and Pruning
  * MiniMax Search in Wiki: http://en.wikipedia.org/wiki/Minimax
  * MiniMax Search in Chess Programming Wikispace: https://chessprogramming.wikispaces.com/Minimax
  * Alpha Beta Search in Wiki: http://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning
  * Alpha Beta Search in Chess Programming Wikispace: http://chessprogramming.wikispaces.com/Alpha-Beta
  * Alpha Beta Search  in You Tube: https://www.youtube.com/watch?v=xBXHtz4Gbdo
  * Horizon Effect in Wiki: http://en.wikipedia.org/wiki/Horizon_effect
  * Horizon Effect in Chess Programming Wikispace: https://chessprogramming.wikispaces.com/Horizon+Effect
  *
  * @param *pos Pointer to the board structure
  * @param *info Pointer to the search position structure
  */
static int AlphaBeta(int alpha, int beta, int depth, S_BOARD *pos, S_SEARCHINFO *info, int DoNull) {

	ASSERT(CheckBoard(pos));

    // Return current Quiescence evaluation for depth zero and increment the number of nodes visited
	if(depth == 0) {
		// info->nodes++;
		// return EvalPosition(pos);
		return Quiescence(alpha, beta, pos, info);
	}

    // Do we need to interrupt search: Ran out of time; GUI send interrupt. Check only after 2047 nodes
	if(( info->nodes & 2047 ) == 0) {
		CheckUp(info);
	}

    // Increment number of nodes visited
	info->nodes++;

    // Repetition leads to balanced score
    // If there is repetition or fifty moves rule is met, it's a balanced score
	if((IsRepetition(pos) || pos->fiftyMove >= 100) && pos->ply) {
		return 0;
	}

    // If the depth has reached its limit, return the evaluation of the current position
	if(pos->ply > MAXDEPTH - 1) {
		return EvalPosition(pos);
	}

    // If we are in check, then increase the depth to go further
	int inCheck = SqAttacked(pos->kingSq[pos->side], pos->side^1, pos);

	if(inCheck == TRUE) {
		depth++;
	}

	S_MOVELIST list[1];
	// Generate all moves
    GenerateAllMoves(pos, list);

    int moveNum = 0;
	int legal = 0;
	// Store the old alpha
	int oldAlpha = alpha;
	int bestMove = NOMOVE;
	int score = -INFINITE;
	int pvMove = ProbePvTable(pos);

    // If a principal variations move is found update the score to 2000000
	if(pvMove != NOMOVE) {
		for(moveNum = 0; moveNum < list->count; ++moveNum) {
			if( list->moves[moveNum].move == pvMove) {
				list->moves[moveNum].score = 2000000;
				break;
			}
		}
	}

    // Loop through the moves
	for(moveNum = 0; moveNum < list->count; ++moveNum) {

        // Pick the next move
        PickNextMove(moveNum, list);

        // Make the move
        if (!MakeMove(pos, list->moves[moveNum].move))  {
            continue;
        }

        // It's a legal move, increment the legal move count
		legal++;
		// Run alpha beta (as nega-max) with reduced depth
		score = -AlphaBeta(-beta, -alpha, depth - 1, pos, info, TRUE);
		// Take back the move
        TakeMove(pos);

        // If interrupted, break and ignore evaluation
        if(info->stopped == TRUE) {
			return 0;
		}

        // If it's alpha cut note the nest move and increase alpha
		if(score > alpha) {
            // If it's beta cut, return beta
			if(score >= beta) {
			    // For first legal move increment fail high first
                if(legal == 1) {
					info->fhf++;
				}

                // Increment fail high
				info->fh++;

                // The move has a beta cut-off and not a capture
                // Set the killer moves
				if(!(list->moves[moveNum].move & MFLAGCAP)) {
					pos->searchKillers[1][pos->ply] = pos->searchKillers[0][pos->ply];
					pos->searchKillers[0][pos->ply] = list->moves[moveNum].move;
				}

				return beta;
			}

            // Set the new/improved alpha
			alpha = score;

			// Take the current move as the best move
			bestMove = list->moves[moveNum].move;

            // For alpha cut-off update search history by prioritizing it by depth
			if(!(list->moves[moveNum].move & MFLAGCAP)) {
				pos->searchHistory[pos->pieces[FROMSQ(bestMove)]][TOSQ(bestMove)] += depth;
			}
		}
    }

    // If there is no legal move and if in check then it's checkmate, otherwise it's stalemate
	if(legal == 0) {
		if(inCheck) {
            // How many moves to mate
			return -MATE + pos->ply;
		} else {
		    // Draw as stalemate
			return 0;
		}
	}

    // If alpha has improved, store the best move as Principal Variation Move
	if(alpha != oldAlpha) {
		StorePvMove(pos, bestMove);
	}

    // Return alpha
	return alpha;
}

/**
  * Function to search move.
  * Works with iterative deepening.
  * Iterative Deepening in Wiki: http://en.wikipedia.org/wiki/Iterative_deepening_depth-first_search
  * Iterative Deepening in Chess Programming Wikispace: https://chessprogramming.wikispaces.com/Iterative+Deepening
  *
  * @param *pos Pointer to the board structure
  * @param *info Pointer to the search position structure
  */
void SearchPosition(S_BOARD *pos, S_SEARCHINFO *info) {

	int bestMove = NOMOVE;
	int bestScore = -INFINITE;
	int currentDepth = 0;
	int pvMoves = 0;
	int pvNum = 0;

	ClearForSearch(pos, info);

	// Do iterative deepening: search iteratively with increasing depth and do move probing to optimize the alpha beta
	for(currentDepth = 1; currentDepth <= info->depth; ++currentDepth) {
        // Call Alpha Beta to get the score up to the current depth
		bestScore = AlphaBeta(-INFINITE, INFINITE, currentDepth, pos, info, TRUE);

		// If out of time or interrupted, break and return
		if(info->stopped == TRUE) {
			break;
		}

        // Get the Principal Variation
		pvMoves = GetPvLine(currentDepth, pos);
		// Get the first move from the Principal Variation as the best move
		bestMove = pos->PvArray[0];

		if(info->GAME_MODE == UCIMODE) {
                printf("info score cp %d depth %d nodes %ld time %d ",
                       bestScore,currentDepth,info->nodes,GetTimeMs()-info->starttime);
                /*printf("Depth:%d, score:%d, move:%s (%s), nodes:%ld, ",
                       currentDepth, bestScore, PrMove(bestMove), PrAlgMove(bestMove, pos), info->nodes);*/
		} else if(info->GAME_MODE == XBOARDMODE && info->POST_THINKING == TRUE) {
                printf("%d %d %d %ld ",
                       currentDepth, bestScore, (GetTimeMs()-info->starttime)/10,info->nodes);
		} else if(info->POST_THINKING == TRUE) {
                printf("score:%d depth:%d nodes:%ld time:%d(ms) ",
                       bestScore,currentDepth,info->nodes,GetTimeMs()-info->starttime);
		}

		if(info->GAME_MODE == UCIMODE || info->POST_THINKING == TRUE) {
                pvMoves = GetPvLine(currentDepth, pos);
                printf("pv");

                for(pvNum = 0; pvNum < pvMoves; ++pvNum) {
                    printf(" %s", PrMove(pos->PvArray[pvNum]));
                }

                /*printf("Principal Variation:");

                for(pvNum = 0; pvNum < pvMoves; ++pvNum) {
                        printf(" %s (%s)", PrMove(pos->PvArray[pvNum]), PrAlgMove(pos->PvArray[pvNum], pos));
                }*/

                printf("\n");

                //printf("Ordering:%.2f\n", (info->fhf / info->fh));
		}
	}

	if(info->GAME_MODE == UCIMODE) {
            //info score cp 13  depth 1 nodes 13 time 15 pv f1b5
            printf("bestmove %s\n", PrMove(bestMove));
            /*printf("bestmove %s (%s)\n",PrMove(bestMove), PrAlgMove(bestMove, pos));*/
	} else if(info->GAME_MODE == XBOARDMODE) {
            printf("move %s\n", PrMove(bestMove));
            MakeMove(pos, bestMove);
	} else {
            printf("\n\n***!! Vice makes move %s !!***\n\n", PrMove(bestMove));
            MakeMove(pos, bestMove);
            PrintBoard(pos);
	}
}

#endif // SEARCH_C
