#include <iostream>
#include "player.h"
#include "conio2.h"
#include "position_t.h"
#include "maze.h"

using namespace std;

void setPlayer(const maze_t* maze, player_t* player) {
	player->position.column = maze->in.column;
	player->position.row = maze->in.row;
	if (player->position.row == 0) player->direction = DOWN;
	else if (player->position.column == 0) player->direction = RIGHT;
	else if (player->position.row == maze->height - 1) player->direction = UP;
	else player->direction = LEFT;
	//DRAW PLAYER

}

void changePlayerDirection(player_t* player, directions direction) {
	switch (direction) {
	case RIGHT:
		switch (player->direction) {
		case LEFT: player->direction = UP; break;
		case UP: player->direction = RIGHT; break;
		case RIGHT: player->direction = DOWN; break;
		case DOWN: player->direction = LEFT; break;
		} break;
	case LEFT:
		switch (player->direction) {
		case LEFT: player->direction = DOWN; break;
		case UP: player->direction = LEFT; break;
		case RIGHT: player->direction = UP; break;
		case DOWN: player->direction = RIGHT; break;
		} break;
	}
}

bool tryAndMoveForwardPlayer(maze_t* maze, player_t* player) {
	switch (player->direction) {
	case LEFT:
		if (player->position.column - 1 < 0) break;
		if (freeBlock(maze, player->position.row, player->position.column - 1)) { player->position.column--; return true; } break;
	case RIGHT:
		if (player->position.column + 1 >= maze->width) break;
		if (freeBlock(maze, player->position.row, player->position.column + 1)) { player->position.column++; return true; } break;
	case UP:
		if (player->position.row - 1 < 0) break;
		if (freeBlock(maze, player->position.row - 1, player->position.column)) { player->position.row--; return true; } break;
	case DOWN:
		if (player->position.row + 1 >= maze->height) break;
		if (freeBlock(maze, player->position.row + 1, player->position.column)) { player->position.row++; return true; } break;
	}
	return false;
}

bool tryAndMoveBackPlayer(maze_t* maze, player_t* player) {
	switch (player->direction) {
	case LEFT:
		if (player->position.column + 1 >= maze->width) break;
		if (freeBlock(maze, player->position.row, player->position.column + 1)) { player->position.column++; return true; } break;
	case RIGHT:
		if (player->position.column - 1 < 0) break;
		if (freeBlock(maze, player->position.row, player->position.column - 1)) { player->position.column--; return true; } break;
	case UP:
		if (player->position.row + 1 >= maze->height) break;
		if (freeBlock(maze, player->position.row + 1, player->position.column)) { player->position.row++; return true; } break;
	case DOWN:
		if (player->position.row - 1 < 0) break;
		if (freeBlock(maze, player->position.row - 1, player->position.column)) { player->position.row--; return true; } break;
	}
	return false;
}

void drawPlayer(const player_t* player, position_t anchor) {
	gotoxy(anchor.column + player->position.column, anchor.row + player->position.row);
	switch (player->direction) {
	case LEFT: cout << "<"; break;
	case RIGHT: cout << ">"; break;
	case UP: cout << "^"; break;
	case DOWN: cout << "v"; break;
	}

}

position_t openDoor(maze_t* maze, player_t* player) {
	position_t nul = { NULL, NULL };
	position_t tempLeft = { player->position.row, player->position.column - 1 };
	position_t tempRight = { player->position.row, player->position.column + 1 };
	position_t tempUp = { player->position.row - 1, player->position.column };
	position_t tempDown = { player->position.row + 1, player->position.column };
	switch (player->direction) {
	case LEFT:
		if (player->position.column - 1 < 0) return nul;
		switch (maze->board[player->position.row][player->position.column - 1]) {
		case DOOR_KEY: if (player->key <= 0) return nul; else player->key--;
		case DOOR_CLOSE: editMazeBoard(maze, tempLeft, DOOR_OPEN); break;
		case DOOR_OPEN: editMazeBoard(maze, tempLeft, DOOR_CLOSE); break;
		default: return nul;
		}
		return tempLeft;
	case RIGHT:
		if (player->position.column + 1 >= maze->width) return nul;
		switch (maze->board[player->position.row][player->position.column + 1]) {
		case DOOR_KEY: if (player->key <= 0) return nul; else player->key--;
		case DOOR_CLOSE: editMazeBoard(maze, tempRight, DOOR_OPEN); break;
		case DOOR_OPEN: editMazeBoard(maze, tempRight, DOOR_CLOSE); break;
		default: return nul;
		}
		return tempRight;
	case UP:
		if (player->position.row - 1 <= 0) return nul;
		switch (maze->board[player->position.row - 1][player->position.column]) {
		case DOOR_KEY: if (player->key < 0) return nul; else player->key--;
		case DOOR_CLOSE: editMazeBoard(maze, tempUp, DOOR_OPEN); break;
		case DOOR_OPEN: editMazeBoard(maze, tempUp, DOOR_CLOSE); break;
		default: return nul;
		}
		return tempUp;
	case DOWN:
		if (player->position.row + 1 >= maze->height) return nul;
		switch (maze->board[player->position.row + 1][player->position.column]) {
		case DOOR_KEY: if (player->key <= 0) return nul; else player->key--;
		case DOOR_CLOSE: editMazeBoard(maze, tempDown, DOOR_OPEN); break;
		case DOOR_OPEN: editMazeBoard(maze, tempDown, DOOR_CLOSE); break;
		default: return nul;
		}
		return tempDown;
	}
	return nul;
}

bool checkExit(const maze_t* maze, const player_t* player) {
	if (player->position.row == maze->out.row && player->position.column == maze->out.column) return true;
	else return false;
}

bool checkKey(const maze_t* maze, player_t* player) {
	if (maze->board[player->position.row][player->position.column] == KEY) {
		player->key++;
		return true;
	}
	else return false;
}