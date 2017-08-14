/* The Chess Engine Chessboard class.
 * Written and published by Kurian Jacob for Ozone Inc.
 * Copyright Kurian Jacob. All rights reserved.
 * Anyone may edit, modify or redistribute this program
 * or it's source code in any form.
 * This program is written with the intention that it
 * may be useful in some manner, however, fitness
 * or merchantibility of this program for a
 * commercial purpose is not guaranteed.
 *
 * Project initiated by Kurian Jacob.
 * TO DO: Add copyright notices and CHANGELOG here.
 */

#include "ChessBoard.h"

long lastpoints, newpoints, enemylastpoints;
MOVE lastMove, newMove, currentMove;
short computerIsWho;
BOOL intrinsicBM, intrinsicWM, intrinsicWLRM, intrinsicWRRM, intrinsicBLRM, intrinsicBRRM;
long lastCommonDifference;


ChessBoard::ChessBoard(short cIW)
{
	int i, j;

	computerIsWho = cIW;

	for (i = 0; i < 8; ++i)
	{
		cells[1][i].bContainingWhat = cells[6][i].bContainingWhat = CELL_PAWN;
		cells[1][i].bBlackOrWhite = cells[0][i].bBlackOrWhite = CHESS_BLACK;
		cells[6][i].bBlackOrWhite = cells[7][i].bBlackOrWhite = CHESS_WHITE;
	}

	for (i = 2; i < 6; i++)
	{
		for (j = 0; j < 8; ++j)
		{
			cells[i][j].bBlackOrWhite = 0;
			cells[i][j].bContainingWhat = CELL_NOTHING;
		}
	}

	intrinsicBM = intrinsicWM = intrinsicWRRM = intrinsicWLRM = intrinsicBLRM = intrinsicBRRM = 0;

	enemylastpoints = newpoints = -2000;

	isBoardVirtual = FALSE;
	triggerThinkingFocus = cIW;
	level = CHESS_DIFFICULTY;

	cells[0][0].bContainingWhat = cells[0][7].bContainingWhat = cells[7][0].bContainingWhat = cells[7][7].bContainingWhat = CELL_ROOK;
	cells[0][1].bContainingWhat = cells[0][6].bContainingWhat = cells[7][1].bContainingWhat = cells[7][6].bContainingWhat = CELL_KNIGHT;
	cells[0][2].bContainingWhat = cells[0][5].bContainingWhat = cells[7][2].bContainingWhat = cells[7][5].bContainingWhat = CELL_BISHOP;
	cells[0][3].bContainingWhat = cells[7][3].bContainingWhat = CELL_QUEEN;
	cells[0][4].bContainingWhat = cells[7][4].bContainingWhat = CELL_KING;
}

/* The virtual chessboard constructor */

ChessBoard::ChessBoard (
	CELL **base,
	int lev,
	long point,
	long enemyPoint,
	BOOL hBM,
	BOOL hWM,
	BOOL hWRRM,
	BOOL hBRRM,
	BOOL hWLRM,
	BOOL hBLRM
	)
{
	int i, j;

	points = point;
	enemyPoints = enemyPoint;
	level = lev;
	hasBlackMoved = hBM;
	hasWhiteMoved = hWM;
	hasWhiteRightRookMoved = hWRRM;
	hasBlackRightRookMoved = hBRRM;
	hasWhiteLeftRookMoved = hWLRM;
	hasBlackLeftRookMoved = hBLRM;

	isBoardVirtual = TRUE;

	triggerThinkingFocus = computerIsWho;

	/*for (i = 0; i < 8; ++i)
		for (j = 0; j < 8; ++j)
			cells[i][j] = base [i][j];*/

	//memcpy_s (cells, sizeof(int), base, 64);
	RtlCopyMemory (cells, base, sizeof(CELL) * 64);
}

INT ChessBoard::getMoves (
						  LPMOVE pMove1,
						  LPMOVE pMove2,
						  PLONG points1,
						  PLONG points2
						  )
{
	*pMove1 = newMove;
	*pMove2 = lastMove;
	*points1 = newpoints;
	*points2 = lastpoints;
	return 0;
}

INT ChessBoard::getData (
						 LPBOOL BM,
						 LPBOOL WM,
						 LPBOOL WLRM,
						 LPBOOL WRRM,
						 LPBOOL BLRM,
						 LPBOOL BRRM
						 )
{
	*BM = intrinsicBM;
	*WM = intrinsicWM;
	*WLRM = intrinsicWLRM;
	*WRRM = intrinsicWRRM;
	*BLRM = intrinsicBLRM;
	*BRRM = intrinsicBRRM;

	return 0;
}

INT ChessBoard::isMovePossible (
	LPMOVE pMove,
	BOOL isSpecialCase
	)
{
	INT result = MOVE_UNREACHABLE;
	MOVE move;

	if (pMove->x == pMove->cx && pMove->cy == pMove->y)
		return MOVE_INVALID;

	/*The procedure for movement for a pawn */

	if (cells[pMove->y][pMove->x].bContainingWhat == CELL_PAWN)
	{
		if (((cells[pMove->y][pMove->x].bBlackOrWhite == CHESS_WHITE && pMove->cy < pMove->y) || (cells[pMove->y][pMove->x].bBlackOrWhite == CHESS_BLACK && pMove->cy > pMove->y)))
		{
			if (abs(pMove->y - pMove->cy) == 2 && pMove->x == pMove->cx)
			{
				if (pMove->y == 6 || pMove->y == 1)
				{
					move = *pMove;
					cells[pMove->y][pMove->x].bBlackOrWhite == CHESS_BLACK ? move.cy++ : move.cy--;

					if (isPathClear(&move))
					{
						result = MOVE_OK;
					}
				}
			}

			else if (abs(pMove->y-pMove->cy) == 1 && pMove->x == pMove->cx)
			{
				move = *pMove;
				cells[pMove->y][pMove->x].bBlackOrWhite == CHESS_BLACK ? move.cy++ : move.cy--;
				if (isPathClear(&move))
				{
					result = MOVE_OK;
				}
			}

			else if ((abs(pMove->x - pMove->cx) == 1) && (abs (pMove->cy - pMove->y) == 1))
			{
				if (cells[pMove->cy][pMove->cx].bContainingWhat != CELL_NOTHING)
					if (cells[pMove->cy][pMove->cx].bBlackOrWhite != cells[pMove->y][pMove->x].bBlackOrWhite)
						result = MOVE_OK;
			}
		}
	}

	else if ((cells[pMove->cy][pMove->cx].bContainingWhat == CELL_NOTHING) || ((cells[pMove->cy][pMove->cx].bBlackOrWhite != cells[pMove->y][pMove->x].bBlackOrWhite))){
	switch (cells[pMove->y][pMove->x].bContainingWhat)
	{

	case CELL_KNIGHT:
		if (((abs(pMove->cx - pMove->x) == 2) && (abs(pMove->cy - pMove->y) == 1)) || ((abs(pMove->cx - pMove->x) == 1) && (abs(pMove->cy - pMove->y) == 2)))
			result = MOVE_OK;
		break;

	case CELL_BISHOP:
		if (abs(pMove->cx - pMove->x) == abs(pMove->cy - pMove->y))
		{
			if (isPathClear(pMove))
				result = MOVE_OK;
		}
		break;

	case CELL_ROOK:
		if (pMove->cx == pMove->x || pMove->cy == pMove->y)
		{
			if (isPathClear(pMove))
				result = MOVE_OK;
		}
		break;

	case CELL_QUEEN:
		if (pMove->cx == pMove->x || pMove->cy == pMove->y || (abs(pMove->cx - pMove->x) == abs(pMove->cy - pMove->y)))
		{
			if (isPathClear (pMove))
				result = MOVE_OK;
		}
		break;

	case CELL_KING:
		{
			short deltax, deltay;
			deltax = abs (pMove->cx - pMove->x);
			deltay = abs (pMove->cy - pMove->y);

			if ((deltax == 1 && deltay == 0) || (deltay == 1 && deltax == 0) || (deltay == 1 && deltax == 1))
				result = MOVE_OK;

			/* Code for checking whether castling is possible or not. */

			if (!isSpecialCase)
			{
				if (!(cells[pMove->y][pMove->x].bBlackOrWhite == CHESS_BLACK ? (isBoardVirtual ? hasBlackMoved : intrinsicBM): isBoardVirtual ? hasWhiteMoved : intrinsicWM))
				{
					if (pMove->x == 4 && (pMove->y == 0 || pMove->y == 7))
					{
						if ((pMove->y == pMove->cy) && (pMove->cx == 2 || pMove->cx == 6))
						{
							move.x = pMove->x;
							move.y = pMove->y;

							if (!OnCheck(cells[pMove->y][pMove->x].bBlackOrWhite))
							{
								if (pMove->y == 0 && pMove->cx == 2)
								{
									move.cx = move.cy = 0;

									if (isPathClear (&move))
									{
										if (isBoardVirtual ? !hasBlackLeftRookMoved : !intrinsicBLRM) result = MOVE_OK;
									}
								}
								else if (pMove->y == 0 && pMove->cx == 6)
								{
									move.cx = 7;
									move.cy = 0;

									if (isPathClear (&move))
									{
										if (isBoardVirtual ? !hasBlackRightRookMoved : !intrinsicBRRM) result = MOVE_OK;
									}
								}
								else if (pMove->y == 7 && pMove->cx == 2)
								{
									move.cy = 7;
									move.cx = 0;
									if (isPathClear (&move))
									{
										if (isBoardVirtual ? !hasWhiteLeftRookMoved : !intrinsicWLRM) result = MOVE_OK;
									}
								}
								else if (pMove->y == 7 && pMove->cx == 6)
								{
									move.cy = 7;
									move.cx = 7;
									if(isPathClear (&move))
									{
										if (isBoardVirtual ? !hasWhiteRightRookMoved : !intrinsicWRRM) result = MOVE_OK;
									}
								}
							}
						}
					}
				}
			}
		}

		break;
	}
	}

	if (!isSpecialCase)
	{
		if (cells[pMove->cy][pMove->cx].bContainingWhat == CELL_KING)
			result = MOVE_UNREACHABLE;
	}

	return result;
}

INT ChessBoard::MakeMove (
	LPMOVE pMove,
	BOOL bIsSpecialCase
	)
{
	triggerThinkingFocus = cells[pMove->y][pMove->x].bBlackOrWhite == CHESS_WHITE ? CHESS_BLACK : CHESS_WHITE;

	if (level == CHESS_DIFFICULTY && (computerIsWho == cells[pMove->y][pMove->x].bBlackOrWhite)){
		currentMove = *pMove;
		//printf ("MOVE: (x,y)-(x,y) = (%d,%d) - (%d,%d)\n", pMove->x, pMove->y, pMove->cx, pMove->cy);
	}

	if (computerIsWho == cells[pMove->y][pMove->x].bBlackOrWhite && isBoardVirtual)
		--level;

	switch (cells[pMove->cy][pMove->cx].bContainingWhat)
	{
	case CELL_PAWN:
		points += computerIsWho == cells[pMove->y][pMove->x].bBlackOrWhite ? POINTS_PAWN: 0;
		enemyPoints += computerIsWho == cells[pMove->y][pMove->x].bBlackOrWhite ? 0 : (POINTS_PAWN);
		break;

	case CELL_KNIGHT:
		points += computerIsWho == cells[pMove->y][pMove->x].bBlackOrWhite ? POINTS_KNIGHT : 0;
		enemyPoints += computerIsWho == cells[pMove->y][pMove->x].bBlackOrWhite ? 0 : (POINTS_KNIGHT);
		break;

	case CELL_BISHOP:
		points += computerIsWho == cells[pMove->y][pMove->x].bBlackOrWhite ? POINTS_BISHOP : 0;
		enemyPoints += computerIsWho == cells[pMove->y][pMove->x].bBlackOrWhite ? 0 : (POINTS_BISHOP);
		break;

	case CELL_ROOK:
		points += computerIsWho == cells[pMove->y][pMove->x].bBlackOrWhite ? POINTS_ROOK : 0;
		enemyPoints += computerIsWho == cells[pMove->y][pMove->x].bBlackOrWhite ? 0 : (POINTS_ROOK);
		break;

	case CELL_QUEEN:
		points += computerIsWho == cells[pMove->y][pMove->x].bBlackOrWhite ? POINTS_QUEEN : 0;
		enemyPoints += computerIsWho == cells[pMove->y][pMove->x].bBlackOrWhite ? 0 : (POINTS_QUEEN);
		break;
	}

	/* Code for implementing castling */
	if (cells[pMove->y][pMove->x].bContainingWhat ==CELL_KING && abs (pMove->cx - pMove->x) == 2)
	{
		//Implement that
		if(pMove->x < pMove->cx)
		{
			cells [pMove->y][5] = cells[pMove->y][7];
			cells [pMove->y][7].bBlackOrWhite = 0;
			cells [pMove->y][7].bContainingWhat = CELL_NOTHING;
			if (!pMove->y)
			{
				if (isBoardVirtual)
				{
					if (!hasBlackRightRookMoved) hasBlackRightRookMoved = TRUE;
				}
				else {
					if (!intrinsicBRRM) intrinsicBRRM = TRUE;
				}
			}
			else {
				if (isBoardVirtual)
				{
					if (!hasWhiteRightRookMoved) hasWhiteRightRookMoved = TRUE;
				}
				else {
					if (!intrinsicWRRM) intrinsicWRRM = TRUE;
				}
			}
		}

		else{
			cells[pMove->y][3] = cells[pMove->y][0];
			cells [pMove->y][0].bBlackOrWhite = 0;
			cells [pMove->y][0].bContainingWhat = CELL_NOTHING;
			if (!pMove->y)
			{
				if (isBoardVirtual)
				{
					if (!hasBlackLeftRookMoved) hasBlackLeftRookMoved = TRUE;
				}
				else {
					if (!intrinsicBLRM) intrinsicBLRM = TRUE;
				}
			}
			else {
				if (isBoardVirtual)
				{
					if (!hasWhiteLeftRookMoved) hasWhiteLeftRookMoved = TRUE;
				}
				else {
					if (!intrinsicWLRM) intrinsicWLRM = TRUE;
				}
			}
		}
	}

	cells[pMove->cy][pMove->cx] = cells[pMove->y][pMove->x];
	cells[pMove->y][pMove->x].bBlackOrWhite = 0;
	cells[pMove->y][pMove->x].bContainingWhat = CELL_NOTHING;

	if (cells[pMove->y][pMove->x].bContainingWhat == CELL_ROOK)
	{
		if (pMove->x == 0 && pMove->y == 0)
		{
			if (isBoardVirtual)
			{
				if (!hasBlackLeftRookMoved) hasBlackLeftRookMoved = TRUE;
			}
			else {
				if (!intrinsicBLRM) intrinsicBLRM = TRUE;
			}
		}
		else if (pMove->x == 0 && pMove->y == 7)
		{
			if (isBoardVirtual)
			{
				if (!hasBlackRightRookMoved) hasBlackRightRookMoved = TRUE;
			}
			else {
				if (!intrinsicBRRM) intrinsicBRRM = TRUE;
			}
		}
		else if (pMove->x == 7 && pMove->y == 0)
		{
			if (isBoardVirtual)
			{
				if (!hasWhiteLeftRookMoved) hasWhiteLeftRookMoved = TRUE;
			}
			else {
				if (!intrinsicWLRM) intrinsicWLRM = TRUE;
			}
		}
		else if (pMove->x == 7 && pMove->y == 7)
		{
			if (isBoardVirtual)
			{
				if (!hasWhiteRightRookMoved) hasWhiteRightRookMoved = TRUE;
			}
			else {
				if (!intrinsicWRRM) intrinsicWRRM = TRUE;
			}
		}
	}
	/* That headache is over. */

	if (cells[pMove->y][pMove->x].bContainingWhat == CELL_KING)
	{
		if (pMove->x == 4)
		{
			if (pMove->y == 0)
			{
				if (isBoardVirtual)
				{
					if (!hasBlackMoved) hasBlackMoved = TRUE;
				}
				else {
					if (!intrinsicBM) intrinsicBM = TRUE;
				}
			}
			else if (pMove->y == 7)
			{
				if (isBoardVirtual)
				{
					if (!hasWhiteMoved) hasWhiteMoved = TRUE;
				}
				else {
					if (!intrinsicWM) intrinsicWM = TRUE;
				}
			}
		}
	}

	if (cells[pMove->cy][pMove->cx].bContainingWhat == CELL_PAWN)
	{
		if (pMove->cy == 7 || pMove->cy == 0)
		{
			if (isBoardVirtual || cells[pMove->cy][pMove->cx].bBlackOrWhite == computerIsWho)
			{
				cells[pMove->cy][pMove->cx].bContainingWhat = CELL_QUEEN;
				points += cells[pMove->cy][pMove->cx].bBlackOrWhite == computerIsWho ? ((POINTS_QUEEN - POINTS_PAWN) * level) : -((POINTS_QUEEN-POINTS_PAWN) * level);
			}
			else {
				short piece;
				piece = QueryChessPromotion ();
				cells[pMove->cy][pMove->cx].bContainingWhat = piece;
			}
		}
	}
				
	if (OnCheck(cells[pMove->cy][pMove->cx].bBlackOrWhite))
		return MOVE_CHECK;

	if (!bIsSpecialCase)
	{
		if (OnCheck(cells[pMove->cy][pMove->cx].bBlackOrWhite  == CHESS_BLACK ? CHESS_WHITE : CHESS_BLACK))
		{
			//points += (computerIsWho == cells[pMove->cy][pMove->cx].bBlackOrWhite ? POINTS_CHECK * (level + 1) : 0);
			//enemyPoints += (computerIsWho == cells[pMove->cy][pMove->cx].bBlackOrWhite ? 0 : POINTS_CHECK * (level + 1));

			if (isCheckMated(cells[pMove->cy][pMove->cx].bBlackOrWhite  == CHESS_BLACK ? CHESS_WHITE : CHESS_BLACK))
			{
				points += (computerIsWho == cells[pMove->cy][pMove->cx].bBlackOrWhite ? POINTS_CHECKMATE * (level + 1) : 0);
				enemyPoints += (computerIsWho == cells[pMove->cy][pMove->cx].bBlackOrWhite ? 0 : POINTS_CHECKMATE * (level + 1));
				level = 0;
			}
		}

		if (!level)
		{
			/*if (abs(points - enemyPoints) <= lastCommonDifference)
			{*/
				if (points >= newpoints)
				{
					lastpoints = newpoints;
					newpoints = points;
					//printf ("\nPoints: %d", points);

					if (currentMove.x != newMove.x || currentMove.y != newMove.y || currentMove.cx != newMove.cx || currentMove.cy != newMove.cy)
					{
						lastMove = newMove;
						newMove = currentMove;
					}
					enemylastpoints = enemyPoints;
					lastCommonDifference = abs (points - enemyPoints);
				}
			/*}*/
		}
	}

	return MOVE_OK;
}

BOOL ChessBoard::isPathClear (
	LPMOVE pMove
	)
{
	BOOL bPathClear = TRUE;
	int i, j, commonDifference;

	if (pMove->cx == pMove->x)
	{
		for (i = pMove->cy > pMove->y ?  pMove->y + 1 : pMove->y - 1; pMove->cy > pMove->y ? (i < pMove->cy) : (pMove->cy < i); pMove->cy > pMove->y ? i++ : --i)
		{
			if (cells[i][pMove->x].bContainingWhat != CELL_NOTHING)
			{
				bPathClear = FALSE;
				break;
			}
		}
	}
	else if (pMove->cy == pMove->y)
	{
		for (j = pMove->cx > pMove->x ? pMove->x + 1 : pMove->x - 1; pMove->cx > pMove->x ? (j < pMove->cx) : (pMove->cx < j); pMove->cx > pMove->x ? j++ : --j)
		{
			if (cells[pMove->y][j].bContainingWhat != CELL_NOTHING)
			{
				bPathClear = FALSE;
				break;
			}
		}
	}
	else {
		commonDifference = abs (pMove->cx - pMove->x) - 1;
		for (i = 1; i <= commonDifference; ++i)
		{
			if (cells[pMove->cy > pMove->y ? pMove->y + i : pMove->y - i][pMove->cx > pMove->x ? pMove->x + i : pMove->x - i].bContainingWhat != CELL_NOTHING)
			{
				bPathClear = FALSE;
				break;
			}
		}
	}

	return bPathClear;
}

BOOL ChessBoard::OnCheck (
	short teamToBeTested
	)
{
	int kingX, kingY, x, y;
	MOVE move;

	kingX = kingY = -1;

	for (x = 0; x < 8; x++)
	{
		for (y = 0; y < 8; ++y)
		{
			if (cells[y][x].bContainingWhat == CELL_KING && cells[y][x].bBlackOrWhite  == teamToBeTested)
			{
				kingX = x;
				kingY = y;
				goto outer;
			}
		}
	}

outer:
	move.cx = kingX;
	move.cy = kingY;

	for (x = 0; x < 8; ++x)
	{
		for (y = 0; y < 8; ++y)
		{
			if (cells[y][x].bContainingWhat != CELL_NOTHING && cells[y][x].bBlackOrWhite != teamToBeTested)
			{
				move.x = x;
				move.y = y;

				if (isMovePossible (&move, TRUE) == MOVE_OK)
					return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL ChessBoard::isCheckMated (
	short teamToBeTested
	)
{
	int i, j, k, l;
	MOVE move;

	for (i = 0; i < 8; ++i)
	{
		for (j = 0; j < 8; j++)
		{
			if (cells[i][j].bBlackOrWhite == teamToBeTested)
			{
				for (k = 0; k < 8; ++k)
				{
					for (l = 0; l < 8; ++l)
					{
						move.y = i;
						move.x = j;
						move.cy = k;
						move.cx = l;
						if (isMovePossible (&move, TRUE) == MOVE_OK)
						{
							ChessBoard *pVcb  = new ChessBoard((CELL **)cells, level, points, enemyPoints, hasBlackMoved, hasWhiteMoved, hasWhiteRightRookMoved,
								hasBlackRightRookMoved, hasWhiteLeftRookMoved, hasBlackLeftRookMoved);
							if (pVcb->MakeMove (&move, TRUE) != MOVE_CHECK)
								return FALSE;
							delete pVcb;
						}
					}
				}
			}
		}
	}

	return TRUE;
}

short ChessBoard::QueryChessPromotion ()
{
	short result;

	printf ("Enter the piece into which you wish to promote your pawn.bishop=2;knight=3;rook=4;queen=5\n");
	scanf ("%d", &result);
	return result;
}

void ChessBoard::printBoard (void)
{
	int i, j;

	for (i = 0; i < 8; ++i)
	{
		printf ("%d", (8 - i));
		for (j = 0; j < 8; ++j)
		{
			switch (cells[i][j].bContainingWhat)
			{
			case CELL_PAWN:
				printf (" %c ", cells[i][j].bBlackOrWhite == CHESS_BLACK ? 'p' : 'P'); break;

			case CELL_BISHOP:
				printf (" %c ", cells[i][j].bBlackOrWhite == CHESS_BLACK ? 'b' : 'B'); break;

			case CELL_KNIGHT:
				printf (" %c ", cells[i][j].bBlackOrWhite == CHESS_BLACK ? 'k' : 'K'); break;

			case CELL_ROOK:
				printf (" %c ", cells[i][j].bBlackOrWhite == CHESS_BLACK ? 'r' : 'R'); break;

			case CELL_QUEEN:
				printf (" %c ", cells[i][j].bBlackOrWhite == CHESS_BLACK ? 'q' : 'Q'); break;

			case CELL_KING:
				printf (" %c ", cells[i][j].bBlackOrWhite == CHESS_BLACK ? 'x' : 'X'); break;

			default:
				printf ("   ");
			}
		}

		printf ("\n\n");
	}

	printf ("\n  A  B  C  D  E  F  G  H\n");
}

INT ChessBoard::TriggerThinking (void)
{
	int x, y, cx, cy, numberOfMoves = 0;
	MOVE move;

	if (!level)
		return -1;

	if (level == CHESS_DIFFICULTY)
	{
		newpoints = enemyPoints = enemylastpoints = -20000;
		lastCommonDifference = 1004245;
	}

	for (x = 0; x < 8; ++x)
	{
		for (y = 0; y < 8; ++y)
		{
			if (cells[y][x].bBlackOrWhite == triggerThinkingFocus)
			{
				for (cx = 0; cx < 8; ++cx)
				{
					for (cy = 0; cy < 8; ++cy)
					{
						move.x = x;
						move.y = y;
						move.cx = cx;
						move.cy = cy;

						if (isMovePossible (&move, FALSE) == MOVE_OK)
						{
							ChessBoard *pVcb = new ChessBoard ((CELL **) cells, level, points, enemyPoints, hasBlackMoved, hasWhiteMoved,
								hasWhiteRightRookMoved, hasBlackRightRookMoved, hasWhiteLeftRookMoved, hasBlackLeftRookMoved);
							if (pVcb->MakeMove (&move, FALSE) != MOVE_OK)
							{
								delete pVcb;
								continue;
							}
							pVcb->TriggerThinking ();
							++numberOfMoves;

							if (level == CHESS_DIFFICULTY)
								printf (".");
							delete pVcb;
						}
					}
				}
			}
		}
	}

	return numberOfMoves;
}

ChessBoard::~ChessBoard(void)
{
	/* Nothing to do. */
}