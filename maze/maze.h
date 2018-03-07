#ifndef MAZE_H
#define MAZE_H

#ifndef NULL
#define NULL 0 //like in iostream
#endif

#include "position_t.h"

typedef enum {
	PATH = ' ',
	WALL1 = '@',
	WALL2 = '#',
	WALL3 = '$',
	WALL4 = '%',
	WALL5 = '&',
	DOOR_OPEN = 'O',
	DOOR_CLOSE = 'X',
	DOOR_KEY = 'D',
	KEY = 'K',
} block_t;

typedef struct {
	block_t** board;
	position_t in;
	position_t out;
	int width;
	int height;
} maze_t;

void freeMaze(maze_t* maze);
bool freeBlock(const maze_t* maze, int row, int column);
void editMazeBoard(maze_t* maze, position_t position, block_t block);
void drawOneBlockOfMaze(const maze_t* maze, position_t position, position_t anchor);
void drawBoard(const maze_t* maze, position_t anchor);
#endif
