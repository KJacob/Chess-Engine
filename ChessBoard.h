#pragma once

#include <stdio.h>
#include "Macro.h"

class ChessBoard
{
public:
	CELL cells[8][8];
	int level;

private:
	
	long points, enemyPoints;
	BOOL hasBlackMoved, hasWhiteMoved, hasWhiteLeftRookMoved, hasWhiteRightRookMoved, hasBlackLeftRookMoved, hasBlackRightRookMoved, isBoardVirtual;
	short triggerThinkingFocus;

public:
	ChessBoard(short cIW);
	ChessBoard (CELL **base, int lev, long point, long enemyPoint, BOOL hBM, BOOL hWM, BOOL hWRRM, BOOL hBRRM, BOOL hWLRM, BOOL hBLRM);
	INT isMovePossible (LPMOVE pMove, BOOL isSpecialCase);
	INT MakeMove (LPMOVE pMove, BOOL bIsSpecialCase);
	BOOL isPathClear (LPMOVE pMove);
	BOOL OnCheck (short teamToBeTested);
	BOOL isCheckMated (short teamToBeTested);
	INT TriggerThinking (void);
	INT getMoves (LPMOVE pMove1, LPMOVE pMove2, LPLONG points1, LPLONG points2);
	INT getData (LPBOOL BM, LPBOOL WM, LPBOOL WLRM, LPBOOL WRRM, LPBOOL BLRM, LPBOOL BRRM);
	short QueryChessPromotion ();
	void printBoard (void);
	~ChessBoard(void);
};