#ifndef PLAYER_H
#define PLAYER_H

#include "position_t.h"
#include "maze.h"

enum directions {
	LEFT,
	UP,
	RIGHT,
	DOWN
};

typedef struct {
	position_t position;
	directions direction;
	int key;
} player_t;

void setPlayer(const maze_t* maze, player_t* player);
void changePlayerDirection(player_t* player, directions direction);
void tryAndMovePlayer(maze_t* maze, player_t* player);
void drawPlayer(const player_t* player, position_t anchor);
position_t openDoor(maze_t* maze, player_t* player);
bool checkExit(const maze_t* maze, const player_t* player);
bool checkKey(const maze_t* maze, player_t* player);
bool tryAndMoveForwardPlayer(maze_t* maze, player_t* player);
bool tryAndMoveBackPlayer(maze_t* maze, player_t* player);

#endif
