#pragma once

#define CHESS_WHITE 1
#define CHESS_BLACK 2

#define CELL_NOTHING 0
#define CELL_PAWN 1
#define CELL_BISHOP 2
#define CELL_KNIGHT 3
#define CELL_ROOK 4
#define CELL_QUEEN 5
#define CELL_KING 6

#define POINTS_PAWN 1
#define POINTS_KNIGHT 3
#define POINTS_BISHOP 4
#define POINTS_ROOK  5
#define POINTS_QUEEN 7
#define POINTS_CHECK	100
#define POINTS_CHECKMATE 10000

#define CHESS_DIFFICULTY 3

/* Values returned by the ChessBoard::isMovePossible() function */
#define MOVE_OK 1
#define MOVE_INVALID 2

/* Move is not possible because that will cause the player to be on check. */
#define MOVE_CHECK 3

/* The enemy will be checkmated due to this move. */
#define MOVE_CHECKMATE 4

/*The move is impossible due to the cell is unreachable.*/
#define MOVE_UNREACHABLE 5

typedef struct _tagCELL
{
	short bContainingWhat;
	short bBlackOrWhite;
} CELL;

typedef struct _tagMOVE{
	short x, y, cx, cy;
}MOVE, *LPMOVE;

typedef int INT;
typedef INT BOOL;
typedef BOOL* LPBOOL;
typedef long LONG;
typedef LONG* LPLONG;

#define TRUE 1
#define FALSE 0