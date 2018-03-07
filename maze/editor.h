#ifndef EDITOR_H
#define EDITOR_H

//Keyboard
#define LEFT_ARROW 0x4b
#define RIGHT_ARROW 0x4d
#define UP_ARROW 0x48
#define DOWN_ARROW 0x50
#define OPEN_CLOSE_DOORS 'd'
#define LOAD_DEFAULT_MAZE 'i'
#define QUIT 'q'
#define HELP 'h'
#define EDIT 'e'
#define LOAD_FILE_MAZE 'o'
#define SAVE_FILE_MAZE 's'
#define RESTART 'r'
#define ANIMATION 'a'
//keyboard extenstion - editor
#define NUM0 '0'
#define NUM1 '1'
#define NUM2 '2'
#define NUM3 '3'
#define NUM4 '4'
#define NUM5 '5'
#define NUM6 '6'
#define NUM7 '7'
#define NUM8 '8'
#define NUM9 '9'

#include "import.h"
#include "maze.h"
#include <iostream>

char* exportMaze(char* fileName, maze_t* maze);
bool wantPlay();
char* editor(char* fileName, maze_t* maze, position_t anchor2D);
void help();


#endif
