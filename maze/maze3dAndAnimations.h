#ifndef MAZE_3D_AND_ANIMATIONS
#define MAZE_3D_AND_ANIMATIONS

#include "position_t.h"
#include "player.h"
#include "maze.h"

#define EDGE_GRADIENT_LEFT '\\'
#define EDGE_GRADIENT_RIGHT '/'
#define EDGE_FLAT ' '
#define EDGE '|'
#define NORMAL '_'
#define MAX_DEPTH 7
#define FRAMES 6
#define HEIGHT_3D 20

enum sides {
	LEFT_SIDE = -1,
	RIGHT_SIDE = 1
};

void drawColumn(position_t anchor, int height, char type, block_t fill);
void drawWall(position_t anchor, int height, int width, sides side, block_t fill, bool pit);
void drawEndWall(position_t anchor, int height, int width, block_t fill);
void drawMaze3D(const maze_t *maze, const player_t *player, bool animation, bool animationForward, position_t anchor3DLeft, position_t anchor3DRight);
void doorAnimation(const maze_t *maze, const player_t *player, position_t door, position_t anchor3DLeft, position_t anchor3DRight);
void clear3D();

#endif