#ifndef IMPORT_H
#define IMPORT_H

#include "conio2.h"
#include "maze.h"
#include "position_t.h"

using namespace std;

#define RETURN_MAZEIN if (maze->board[row][column] == 0) { mazeIn.row = row; mazeIn.column = column; return mazeIn; };
#define RETURN_MAZEOUT if (maze->board[row][column] == 0) { mazeOut.row = row; mazeOut.column = column; return mazeOut; };

/*IMPORTANT: You must call freeMaze(maze_t* maze) in the end on of program - it will free memory of maze board*/
bool importMaze(char* fileName, maze_t* maze);
char* loadFileNameMaze();
bool is2x2(const maze_t* maze);

#endif