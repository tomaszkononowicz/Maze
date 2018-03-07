#include <iostream>
#include "conio2.h"
#include "import.h"
#include "maze.h"

using namespace std;

#define RETURN_MAZEIN if (maze->board[row][column] == PATH) { mazeIn.row = row; mazeIn.column = column; return mazeIn; };
#define RETURN_MAZEOUT if (maze->board[row][column] == PATH) { mazeOut.row = row; mazeOut.column = column; return mazeOut; };

static int getMazeWidth(const FILE* file) {
	fseek((FILE*)file, 0, SEEK_SET);
	char tmp;
	int width = 0;
	do {
		fscanf((FILE*)file, "%c", &tmp);
		if (tmp != '\r' && tmp != '\n') width++;
	} while (tmp != '\r' && tmp != '\n');
	fseek((FILE*)file, 0, SEEK_SET);
	return width;
}

static int getMazeHeight(const FILE* file, const int width) {
	fseek((FILE*)file, 0, SEEK_SET);
	char* tmp = (char*)malloc((width + 1)*sizeof(char));
	int height = 0;
	while (fscanf((FILE*)file, "%s", tmp) > 0) {
		height++;
	}
	free(tmp);
	fseek((FILE*)file, 0, SEEK_SET);
	return height;;
}

static position_t getMazeIn(const maze_t* maze) {
	position_t mazeIn;
	//Looking for enter by clockwise
	for (int row = 0, column = 0; column < maze->width; column++)	RETURN_MAZEIN;
	for (int row = 0, column = maze->width - 1; row < maze->height; row++)	RETURN_MAZEIN;
	for (int row = maze->height - 1, column = maze->width - 1; column >= 0; column--)	RETURN_MAZEIN;
	for (int row = maze->height - 1, column = 0; row > 0; row--)	RETURN_MAZEIN;
	mazeIn.row = 0; mazeIn.column = 0; return mazeIn;
}

static position_t getMazeOut(const maze_t* maze) {
	position_t mazeOut;
	//Looking for exit by counterclockwise
	for (int row = 0, column = 0; row < maze->height; row++)	RETURN_MAZEOUT;
	for (int row = maze->height - 1, column = 0; column < maze->width; column++)	RETURN_MAZEOUT;
	for (int row = maze->height - 1, column = maze->width - 1; row >= 0; row--)	RETURN_MAZEOUT;
	for (int row = 0, column = maze->width - 1; column > 0; column--)	RETURN_MAZEOUT;
	mazeOut.row = 0; mazeOut.column = 0; return mazeOut;
}

bool is2x2(const maze_t* maze) {
	for (int row = 0; row < maze->height - 1; row++) {
		for (int column = 0; column < maze->width - 1; column++) {
			if (freeBlock(maze, row, column)
				&& freeBlock(maze, row, column + 1)
				&& freeBlock(maze, row + 1, column)
				&& freeBlock(maze, row + 1, column + 1)
				) return true;
		}
	}
	return false;
}

char* loadFileNameMaze() {
	clrscr();
	gotoxy(1, 1);
	cout << "Podaj nazwe pliku (razem z rozszerzeniem): ";
	char fileName[128];
	cin >> fileName;
	return fileName;
}

/*IMPORTANT: You must call freeMaze(maze_t* maze) in the end on of program - it will free memory of maze board*/
bool importMaze(char* fileName, maze_t* maze) {
	FILE* file;
	file = fopen(fileName, "r");
	if (file == NULL) {
		cout << fileName << endl << "Otwarcie pliku nie powiodlo sie" << endl;
		getch();
		return false;
	}
	else {
		if (maze->board != NULL) freeMaze(maze);
		maze->width = getMazeWidth(file);
		maze->height = getMazeHeight(file, maze->width);

		maze->board = (block_t**)malloc(maze->height*sizeof(block_t*));
		for (int i = 0; i < maze->height; i++) {
			maze->board[i] = (block_t*)malloc(maze->width*(sizeof(block_t)));
		}
		int row = 0, column = 0;
		while (row < maze->height) {
			while (column < maze->width) {
				char tmp;
				fscanf((FILE*)file, "%c", &tmp);
				if (tmp != '\r' && tmp != '\n') {
					switch (tmp) {
					case '0': tmp = PATH; break;
					case '1': tmp = WALL1; break;
					case '2': tmp = WALL2; break;
					case '3': tmp = WALL3; break;
					case '4': tmp = WALL4; break;
					case '5': tmp = WALL5; break;
					case '6': tmp = DOOR_OPEN; break;
					case '7': tmp = DOOR_CLOSE; break;
					case '8': tmp = DOOR_KEY; break;
					case '9': tmp = KEY; break;
					}
					maze->board[row][column] = (block_t)(tmp);
					column++;
				}
			}
			column = 0;
			row++;
		}
		
		maze->in = getMazeIn(maze);
		maze->out = getMazeOut(maze);
		fclose(file);

		if (is2x2(maze)) {
			gotoxy(1, 3);
			cout << "Zly format pliku - istnieja puste pola 2x2";
			return false;
		}
	}
	return true;
}


