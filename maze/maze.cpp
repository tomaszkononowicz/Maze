#include <iostream>
#include "maze.h"
#include "conio2.h"

using namespace std;

void freeMaze(maze_t* maze) {
	for (int i = 0; i < maze->height; i++) {
		free(maze->board[i]);
	}
	free(maze->board);
}

bool freeBlock(const maze_t* maze, int row, int column) {
	switch (maze->board[row][column]) {
	case PATH:
	case DOOR_OPEN:
	case KEY: return true; break;
	default: return false; break;
	}
}

void editMazeBoard(maze_t* maze, position_t position, block_t block) {
	maze->board[position.row][position.column] = block;
}

void drawOneBlockOfMaze(const maze_t* maze, position_t position, position_t anchor) {
	gotoxy(anchor.column + position.column, anchor.row + position.row);
	cout << (char)maze->board[position.row][position.column];
}

void drawBoard(const maze_t* maze, position_t anchor) {
	for (int i = 0; i < maze->height; i++) {
		for (int j = 0; j < maze->width; j++) {
			gotoxy(j + anchor.column, i + anchor.row);
			cout << (char)(maze->board[i][j]);
		}
	}
}