/* The Chess Engine Entry Point.
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

#include <stdio.h>
#include "chessboard.h"

void GetUserInput (LPMOVE move);

void printOut (LPMOVE pMove);

INT computerPlayAsBlack ();

int main ()
{
	/*int computerIsWhat = CHESS_BLACK;
	MOVE move1, move2, userMove;
	LONG points1, points2;
	BOOL WM, BM, WRRM, WLRM, BRRM, BLRM;

	//printf ("What do yo want the computer to be? (1=WHITE; 2=BLACK)\n");
	//scanf ("%d", &computerIsWhat);

	ChessBoard *pChessBoard = new ChessBoard (computerIsWhat);
	pChessBoard->printBoard ();

	do {
		GetUserInput (&userMove);

		if (pChessBoard->isMovePossible(&userMove, FALSE) != MOVE_OK)
			continue;

		printf ("Move OK\n");

		pChessBoard->getData (&BM, &WM, &WLRM, &WRRM, &BLRM, &BRRM);
		ChessBoard * pVcb = new ChessBoard ((CELL **)pChessBoard->cells, 3, -2000, -2000, BM, WM, WRRM, BRRM,
			WLRM, BLRM);


		pChessBoard->MakeMove (&userMove, FALSE);

		printf ("Vcb->MakeMove");
		pVcb->MakeMove (&userMove, FALSE);

		printf ("The Virtual ChessBoard:\n");
		pVcb->printBoard ();

		printf ("\nOnCheck: %d\n", pVcb->isCheckMated (CHESS_WHITE));

		printf ("The computer is thinking...");

		//pVcb->level = 3;

		printf ("\nNumber of moves thought: %d", pVcb->TriggerThinking ());

		pChessBoard->getMoves (&move1, &move2, &points1, &points2);

		printf ("\nThe computer's move is:%d,%d-%d,%d\n Points for this move: %d\n", move1.x, move1.y, move1.cx, move1.cy, points1);

		pChessBoard->printBoard ();
	}while (false);

	//pChessBoard->printBoard ();

	getch ();*/

	printf ("Chess Engine by Kurian Jacob.\n © Kurian Jacob 1993 - 2009.\n\n");

	computerPlayAsBlack ();

	return 0;
}

void GetUserInput (LPMOVE move)
{
	char str [7];
	printf ("Enter your move: (eg: B5-C7)\n");

	gets (str);
	move->x = toupper (str[0])-65;
	move->y = 7 - (str[1] - 49);
	move->cx = toupper (str[3])-65;
	move->cy = 7 - (str [4] - 49);
}

INT computerPlayAsBlack ()
{
	MOVE usermove1, usermove2, usermove3, computermove1, computermove2, computermove3;
	LONG points1, points2;
	BOOL WM, BM, WRRM, WLRM, BRRM, BLRM, result;
	int numberOfMoves = 0;

	ChessBoard *pChessBoard = new ChessBoard (CHESS_BLACK);
	pChessBoard->printBoard ();

	do {
		GetUserInput (&usermove1);

		if (pChessBoard->isMovePossible (&usermove1, FALSE) != MOVE_OK)
		{
			printf ("You can not make this move. This move is invalid.");
			continue;
		}

		pChessBoard->getData (&BM, &WM, &WLRM, &WRRM, &BLRM, &BRRM);

		ChessBoard * pVcb = new ChessBoard ((CELL **)pChessBoard->cells, CHESS_DIFFICULTY, 0, 0, BM, WM, WRRM, BRRM,
			WLRM, BLRM);

		if (pVcb->MakeMove (&usermove1, FALSE) == MOVE_CHECK)
		{
			printf ("You can not make this move because that will cause you to be on check.");
			delete pVcb;
			continue;
		}

		pChessBoard->MakeMove (&usermove1, FALSE);

		pChessBoard->printBoard ();

		result = pVcb->OnCheck (CHESS_BLACK);

		if (result)
		{
			if (pVcb->isCheckMated (CHESS_BLACK))
			{
				printf ("Excellent. You win by checkmate.");
				break;
			}

			printf ("The computer is now on check.");
		}

		printf ("The computer is thinking");

		if (!pVcb->TriggerThinking ())
		{
			printf ("The computer has no more plays. The computer is resigning");
			break;
		}

		pChessBoard->getMoves (&computermove1, &computermove2, &points1, &points2);
		printf ("The computer's move is:");
		printOut (&computermove1);
		pChessBoard->MakeMove (&computermove1, FALSE);

		pChessBoard->printBoard ();

		result = pVcb->OnCheck (CHESS_WHITE);

		if (result)
		{
			if (pVcb->isCheckMated (CHESS_WHITE))
			{
				printf ("The computer wins by checkmate.");
				break;
			}

			printf ("The enemy is now on check");
		}

		++numberOfMoves;

		delete pVcb;
	} while(true);

	getch ();

	return numberOfMoves;
}

void printOut (
			   LPMOVE pMove
			   )
{
	printf ("%c%d-%c%d\n", (pMove->x + 65), 8-pMove->y, pMove->cx + 65, 8 - pMove->cy);
}