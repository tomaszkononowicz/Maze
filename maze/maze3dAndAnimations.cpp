#include "maze3dAndAnimations.h"
#include "conio2.h"
#include <time.h>
#include <iostream>

using namespace std;

void clear3D() {
	for (int i = 1; i < 22; i++) {
		for (int j = 1; j < 57; j++) {
			gotoxy(j, i);
			cout << " ";
		}
	}
}


void drawColumn(position_t anchor, int height, char type, block_t fill) {
	switch (type) {
	case EDGE_GRADIENT_LEFT: break;
	case EDGE_GRADIENT_RIGHT: break;
	case EDGE_FLAT: break;
	case NORMAL: break;
	}

	//First char
	gotoxy(anchor.column, anchor.row);
	switch (type) {
	case EDGE_GRADIENT_LEFT:
		height--;
		anchor.row++;
		gotoxy(anchor.column, anchor.row);
		cout << EDGE_GRADIENT_LEFT;
		break;
	case EDGE_GRADIENT_RIGHT:
		height--;
		anchor.row++;
		gotoxy(anchor.column, anchor.row);
		cout << EDGE_GRADIENT_RIGHT;
		break;
	case EDGE_FLAT: break;
	case NORMAL: cout << NORMAL; break;
	}
	anchor.row++;
	height -= 2; //Correct height for filling
	for (int i = 0; i < height; i++) {
		gotoxy(anchor.column, anchor.row + i);
		switch (type) {
		case EDGE_GRADIENT_LEFT:
		case EDGE_GRADIENT_RIGHT:
		case EDGE_FLAT:
			cout << EDGE;
			break;
		case NORMAL: cout << (char)fill; break;
		}
	}


	//Last char
	anchor.row += height;
	gotoxy(anchor.column, anchor.row);
	switch (type) {
	case EDGE_GRADIENT_LEFT:
		cout << EDGE_GRADIENT_RIGHT;
		break;
	case EDGE_GRADIENT_RIGHT:
		cout << EDGE_GRADIENT_LEFT;
		break;
	case EDGE_FLAT:
		cout << EDGE;
		break;
	case NORMAL: cout << (char)fill; break;
	}
}

void drawWall(position_t anchor, int height, int width, sides side, block_t fill, bool pit) {
	if (pit) {
		anchor.row++;
		height -= 2;
		switch (side) {
		case LEFT_SIDE:
			for (int i = 0; i < width; i++) {
				anchor.column++;
				if (i == width - 1) drawColumn(anchor, height, EDGE_FLAT, fill);
				else drawColumn(anchor, height, NORMAL, fill);
			}
			break;
		case RIGHT_SIDE:
			for (int i = width - 1; i >= 0; i--) {
				anchor.column--;
				if (i == 0) drawColumn(anchor, height, EDGE_FLAT, fill);
				else drawColumn(anchor, height, NORMAL, fill);
			}
			break;
		}
	}
	else {
		switch (side) {
		case LEFT_SIDE:
			for (int i = 0; i < width; i++) {
				anchor.column++;
				if (i == width - 1) drawColumn(anchor, height, EDGE_GRADIENT_LEFT, fill);
				else drawColumn(anchor, height, NORMAL, fill);
			}
			break;
		case RIGHT_SIDE:
			for (int i = width - 1; i >= 0; i--) {
				anchor.column--;
				if (i == 0) drawColumn(anchor, height, EDGE_GRADIENT_RIGHT, fill);
				else drawColumn(anchor, height, NORMAL, fill);
			}
			break;
		}
	}
}

void drawEndWall(position_t anchor, int height, int width, block_t fill) {
	for (int i = 0; i < width; i++) {
		anchor.column++;
		drawColumn(anchor, height, NORMAL, fill);
	}
}

void drawMaze3D(const maze_t *maze, const player_t *player, bool animation, bool animationForward, position_t anchor3DLeft, position_t anchor3DRight) {
	int height = HEIGHT_3D;
	int frame;
	if (animation) frame = 0;
	else frame = FRAMES - 1;
	int(*animationArray)[MAX_DEPTH + 1];
	if (animationForward) {
		int forwardArray[FRAMES][MAX_DEPTH + 1] =
			//FORWARD
		{ { 0, 0, 0, 0, 0, 0, 0, -1 },
		{ -1, 0, 0, 0, 0, 0, 0, 0 },
		{ -2, 1, 0, 0, 0, 0, 0, 0 },
		{ -3, 1, 0, 1, 0, 0, 0, 0 },
		{ -4, 1, 0, 1, 0, 1, 0, 0 },
		{ -5, 1, 0, 1, 0, 1, 0, 1 } };
		animationArray = forwardArray;
	}
	else {
		int backArray[FRAMES][MAX_DEPTH + 1] =
			//BACK
		{ { -5, -3, 0, 0, 0, 0, 0, 1 },
		{ -5, -2, 0, 0, 0, 0, 0, 1 },
		{ -5, -1, 0, 0, 0, 0, 0, 1 },
		{ -5,  0, 0, 1, 0, 0, 0, 1 },
		{ -5,  0, 0, 1, 0, 1, 0, 1 },
		{ -5,  1, 0, 1, 0, 1, 0, 1 } };
		animationArray = backArray;
	}

	sides side;
	int row = player->position.row;
	int column = player->position.column;
	for (; frame < FRAMES; frame++) {
		time_t animationStartTime = clock();
		int depth = 0;
		position_t anchorLeftSide = anchor3DLeft;
		position_t anchorRightSide = anchor3DRight;
		height = 20;
		clear3D();
		switch (player->direction) {
		case RIGHT:
			while ((depth <= MAX_DEPTH) && (column + depth <= maze->width - 1) && (column + depth >= 0) //field in max depth and field in array
				&& ((maze->board[row][column + depth] == PATH) || (maze->board[row][column + depth] == DOOR_OPEN) || (maze->board[row][column + depth] == KEY))) { //transparent field 
				int width = 5 - ((depth + 1) / 2) + animationArray[frame][depth];
				side = LEFT_SIDE; //side of draw
				for (int i = 0; i < 2; i++) { //draw twice: left side and right side
					if (row + side >= 0 && row + side <= maze->height - 1) {
						switch (maze->board[row + side][column + depth]) { //check field next to 
						case PATH:
						case DOOR_OPEN:
						case KEY:
							switch (maze->board[row + side][column + depth + 1]) { //check field next to next to
							case PATH:
							case DOOR_OPEN:
							case KEY:
								break; //dont draw
							default: //draw pit wall if next to exist block
								if (depth + 1 <= MAX_DEPTH) {
									if (side == LEFT_SIDE) drawWall(anchorLeftSide, height, width, LEFT_SIDE, maze->board[row + side][column + depth + 1], true);//rysuj trapez z taka tekstura maze->board[row - 1][column + depth]
									else drawWall(anchorRightSide, height, width, RIGHT_SIDE, maze->board[row + side][column + depth + 1], true);
								}
								break;
							}
							break;
						default: //draw normal wall
							if (side == LEFT_SIDE) drawWall(anchorLeftSide, height, width, LEFT_SIDE, maze->board[row + side][column + depth], false);//rysuj trapez z taka tekstura maze->board[row - 1][column + depth]
							else drawWall(anchorRightSide, height, width, RIGHT_SIDE, maze->board[row + side][column + depth], false);
							break;
						}
						if (side == LEFT_SIDE) anchorLeftSide.column += width;
						else anchorRightSide.column -= width;
						side = RIGHT_SIDE;
					}
				}
				anchorLeftSide.row++;
				anchorRightSide.row++;
				height -= 2;
				depth++;
			}
			if (depth <= MAX_DEPTH && (column + depth <= maze->width - 1) && (column + depth >= 0)) {
				drawEndWall(anchorLeftSide, height, anchorRightSide.column - anchorLeftSide.column - 1, maze->board[row][column + depth]);
			}
			break;
		case LEFT:
			while ((depth <= MAX_DEPTH) && (column - depth <= maze->width - 1) && (column - depth >= 0) //field in max depth and field in array
				&& ((maze->board[row][column - depth] == PATH) || (maze->board[row][column - depth] == DOOR_OPEN) || (maze->board[row][column - depth] == KEY))) { //transparent field 
				int width = 5 - ((depth + 1) / 2) + animationArray[frame][depth];
				side = LEFT_SIDE; //side of draw //look below player, oposite to RIGHT
				for (int i = 0; i < 2; i++) { //draw twice: left side and right side
					if (row + side >= 0 && row + side <= maze->height - 1) {
						switch (maze->board[row + side][column - depth]) { //check field next to 
						case PATH:
						case DOOR_OPEN:
						case KEY:
							switch (maze->board[row + side][column - depth - 1]) { //check field next to next to
							case PATH:
							case DOOR_OPEN:
							case KEY:
								break; //dont draw
							default: //draw pit wall if next to exist block
								if (depth + 1 <= MAX_DEPTH) {
									if (side == RIGHT_SIDE) drawWall(anchorLeftSide, height, width, LEFT_SIDE, maze->board[row + side][column - depth - 1], true); //side RIGHT when player direction is LEFT means LEFT SIDE in 3D
									else drawWall(anchorRightSide, height, width, RIGHT_SIDE, maze->board[row + side][column - depth - 1], true);
								}
								break;
							}
							break;
						default: //draw normal wall
							if (side == RIGHT_SIDE) drawWall(anchorLeftSide, height, width, LEFT_SIDE, maze->board[row + side][column - depth], false);//side RIGHT when player direction is LEFT means LEFT SIDE in 3D
							else drawWall(anchorRightSide, height, width, RIGHT_SIDE, maze->board[row + side][column - depth], false);
							break;
						}
						if (side == RIGHT_SIDE) anchorLeftSide.column += width; //side RIGHT when player direction is LEFT means LEFT SIDE in 3D
						else anchorRightSide.column -= width;
						side = RIGHT_SIDE;

					}
				}
				anchorLeftSide.row++;
				anchorRightSide.row++;
				height -= 2;
				depth++;
			}
			if (depth <= MAX_DEPTH && (column - depth <= maze->width - 1) && (column - depth >= 0)) {
				drawEndWall(anchorLeftSide, height, anchorRightSide.column - anchorLeftSide.column - 1, maze->board[row][column - depth]);
			}
			break;
		case DOWN:
			while ((depth <= MAX_DEPTH) && (row + depth <= maze->height - 1) && (row + depth >= 0) //field in max depth and field in array
				&& ((maze->board[row + depth][column] == PATH) || (maze->board[row + depth][column] == DOOR_OPEN) || (maze->board[row + depth][column] == KEY))) { //transparent field 
				int width = 5 - ((depth + 1) / 2) + animationArray[frame][depth];
				side = LEFT_SIDE; //side of draw
				for (int i = 0; i < 2; i++) { //draw twice: left side and right side
					if (column + side >= 0 && column + side <= maze->width - 1) {
						switch (maze->board[row + depth][column + side]) { //check field next to 
						case PATH:
						case DOOR_OPEN:
						case KEY:
							switch (maze->board[row + depth + 1][column + side]) { //check field next to next to
							case PATH:
							case DOOR_OPEN:
							case KEY:
								break; //dont draw
							default: //draw pit wall if next to exist block
								if (depth + 1 <= MAX_DEPTH) {
									if (side == RIGHT_SIDE) drawWall(anchorLeftSide, height, width, LEFT_SIDE, maze->board[row + depth + 1][column + side], true);//rysuj trapez z taka tekstura maze->board[row - 1][column + depth]
									else drawWall(anchorRightSide, height, width, RIGHT_SIDE, maze->board[row + depth + 1][column + side], true);
								}
								break;
							}
							break;
						default: //draw normal wall
							if (side == RIGHT_SIDE) drawWall(anchorLeftSide, height, width, LEFT_SIDE, maze->board[row + depth][column + side], false);//rysuj trapez z taka tekstura maze->board[row - 1][column + depth]
							else drawWall(anchorRightSide, height, width, RIGHT_SIDE, maze->board[row + depth][column + side], false);
							break;
						}
						if (side == RIGHT_SIDE) anchorLeftSide.column += width;
						else anchorRightSide.column -= width;
						side = RIGHT_SIDE;
					}
				}
				anchorLeftSide.row++;
				anchorRightSide.row++;
				height -= 2;
				depth++;
			}
			if (depth <= MAX_DEPTH && (row + depth <= maze->height - 1) && (row + depth >= 0)) {
				drawEndWall(anchorLeftSide, height, anchorRightSide.column - anchorLeftSide.column - 1, maze->board[row + depth][column]);
			}
			break;
		case UP:
			while ((depth <= MAX_DEPTH) && (row - depth <= maze->height - 1) && (row - depth >= 0) //field in max depth and field in array
				&& ((maze->board[row - depth][column] == PATH) || (maze->board[row - depth][column] == DOOR_OPEN) || (maze->board[row - depth][column] == KEY))) { //transparent field 
				int width = 5 - ((depth + 1) / 2) + animationArray[frame][depth];
				side = LEFT_SIDE; //side of draw
				for (int i = 0; i < 2; i++) { //draw twice: left side and right side
					if (column + side >= 0 && column + side <= maze->width - 1) {
						switch (maze->board[row - depth][column + side]) { //check field next to 
						case PATH:
						case DOOR_OPEN:
						case KEY:
							switch (maze->board[row - depth - 1][column + side]) { //check field next to next to
							case PATH:
							case DOOR_OPEN:
							case KEY:
								break; //dont draw
							default: //draw pit wall if next to exist block
								if (depth + 1 <= MAX_DEPTH) {
									if (side == LEFT_SIDE) drawWall(anchorLeftSide, height, width, LEFT_SIDE, maze->board[row - depth - 1][column + side], true);//rysuj trapez z taka tekstura maze->board[row - 1][column + depth]
									else drawWall(anchorRightSide, height, width, RIGHT_SIDE, maze->board[row - depth - 1][column + side], true);
								}
								break;
							}
							break;
						default: //draw normal wall
							if (side == LEFT_SIDE) drawWall(anchorLeftSide, height, width, LEFT_SIDE, maze->board[row - depth][column + side], false);//rysuj trapez z taka tekstura maze->board[row - 1][column + depth]
							else drawWall(anchorRightSide, height, width, RIGHT_SIDE, maze->board[row - depth][column + side], false);
							break;
						}
						if (side == LEFT_SIDE) anchorLeftSide.column += width;
						else anchorRightSide.column -= width;
						side = RIGHT_SIDE;
					}
				}
				anchorLeftSide.row++;
				anchorRightSide.row++;
				height -= 2;
				depth++;
			}
			if (depth <= MAX_DEPTH && (row - depth <= maze->height - 1) && (row - depth >= 0)) {
				drawEndWall(anchorLeftSide, height, anchorRightSide.column - anchorLeftSide.column - 1, maze->board[row - depth][column]);
			}
			break;
		}
		if (animation) while (clock() - animationStartTime < 600) {}; //min frame animation time
	}
}

void doorAnimation(const maze_t *maze, const player_t *player, position_t door, position_t anchor3DLeft, position_t anchor3DRight) {
	if (door.column != NULL) {
		if (maze->board[door.row][door.column] == DOOR_CLOSE) {
			//closing doors animation
			for (int i = 1; i < FRAMES; i++) {
				int width = (((anchor3DRight.column - anchor3DLeft.column - 1) / FRAMES)*i);
				position_t temp = { anchor3DLeft.row + 1, anchor3DLeft.column };
				drawEndWall(temp, HEIGHT_3D - 2, width, DOOR_CLOSE);
				time_t tmp = clock();
				while (clock() - tmp < 200) {};
			}
		}
		else {
			//opening doors animation
			for (int i = FRAMES; i > 0; i--) {
				int width = (((anchor3DRight.column - anchor3DLeft.column - 1) / FRAMES)*i);
				drawMaze3D(maze, player, false, false, anchor3DLeft, anchor3DRight);
				position_t temp = { anchor3DLeft.row + 1, anchor3DLeft.column };
				drawEndWall(temp, HEIGHT_3D - 2, width, DOOR_OPEN);
				time_t tmp = clock();
				while ((clock() - tmp) < 200) {};
			}
		}
	}
}
