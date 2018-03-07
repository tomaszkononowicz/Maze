#include <iostream>
#include "conio2.h"
#include "import.h"
#include "position_t.h"
#include "player.h"
#include "maze.h"
#include "editor.h"
#include "maze3dAndAnimations.h"
#include <time.h>

/* Uwaga: w docelowym programie nalezy zadeklarowac odpowiednie
stale, na przyklad po to, aby wyeliminowac z programu
wartosci numeryczne umieszczajac w ich miejsce
dobrze dobrane identyfikatory */

using namespace std;

#define RETURN_MAZEIN if (maze->board[row][column] == 0) { mazeIn.row = row; mazeIn.column = column; return mazeIn; };
#define RETURN_MAZEOUT if (maze->board[row][column] == 0) { mazeOut.row = row; mazeOut.column = column; return mazeOut; };

bool startAndRestart(maze_t* maze, char* fileName, player_t* player, position_t anchor2D, int* licznikKrokow, time_t* czas) {
	clrscr();
	if (fileName == NULL) fileName = "defaultMaze.txt";
	if (!importMaze(fileName, maze)) return false;
	setPlayer(maze, player);
	drawBoard(maze, anchor2D);
	drawPlayer(player, anchor2D);
	*licznikKrokow = 0;
	*czas = clock();
	_setcursortype(_NOCURSOR);
	return true;
}

int main()
{
	//left up anchors of elements (corners (points) include)
	position_t anchor2D = { 2, 57 }, anchor3DLeft = { 2, 2 }, anchor3DRight = { 2, 55 };
	position_t door;
	int licznik_krokow;
	time_t czas;
	player_t player;
	player.key = 0;
	maze_t maze;
	maze.board = NULL;
	char* fileName = NULL;
	bool animationUser = false;
	bool animationSoftware = false;
	bool animationForward = true;

	//first start
	fileName = "defaultMaze.txt";
	startAndRestart(&maze, fileName, &player, anchor2D, &licznik_krokow, &czas);
	drawMaze3D(&maze, &player, (animationSoftware && animationUser), animationForward, anchor3DLeft, anchor3DRight);
	
	int key, i; //i initialization skipped by case label
	settitle("Tomasz Kononowicz 160839");
	textbackground(BLACK);
	do {
		gotoxy(2, 23);
		cout << "Zrobionych krokow: " << licznik_krokow << endl;
		gotoxy(15, 24);
		cout << "Czas: " << (clock() - czas) / CLOCKS_PER_SEC << endl;
		key = getch();
		if (key != 0) {
			switch (key) {
			case LOAD_DEFAULT_MAZE:
				fileName = "defaultMaze.txt";
				startAndRestart(&maze, fileName, &player, anchor2D, &licznik_krokow, &czas);
				break;
			case LOAD_FILE_MAZE:
				fileName = loadFileNameMaze();
				i = 0;
				char tmp[128];
				while (fileName[i] != '\0') {
					tmp[i] = fileName[i];
					i++;
				}
				tmp[i] = '\0';
				startAndRestart(&maze, tmp, &player, anchor2D, &licznik_krokow, &czas);
				break;
			case OPEN_CLOSE_DOORS:
				door = openDoor(&maze, &player);
				drawOneBlockOfMaze(&maze, door, anchor2D);
				doorAnimation(&maze, &player, door, anchor3DLeft, anchor3DRight);
				break;
			case HELP:
				help();
				cout << "                    Nacisnij dowolny klawisz aby kontynuowac" << endl; getch();
				clrscr();
				drawBoard(&maze, anchor2D);
				drawPlayer(&player, anchor2D);
				break;
			case EDIT:
				clrscr();
				gotoxy(1, 1);
				cout << "Chcesz edytowac aktualny labirynt?" << endl << "(klawisz 't' jezeli tak, " << endl << "dowolny inny klawisz, aby utworzyc nowy labirynt) : ";
				if (getch() == 't') {
					char *tmp = editor(fileName, &maze, anchor2D);
					char tmp2[128];
					if (tmp != NULL) {
						i = 0;
						while (tmp[i] != '\0') {
							tmp2[i] = tmp[i];
							i++;
						}
						tmp2[i] = '\0';
					}
					if (tmp != NULL) startAndRestart(&maze, tmp, &player, anchor2D, &licznik_krokow, &czas);
					else startAndRestart(&maze, fileName, &player, anchor2D, &licznik_krokow, &czas);
				}
				else {
					char *tmp = editor(NULL, NULL, anchor2D);
					char tmp2[128];
					if (tmp != NULL) {
						i = 0;
						while (tmp[i] != '\0') {
							tmp2[i] = tmp[i];
							i++;
						}
						tmp2[i] = '\0';
					}
					if (tmp != NULL) startAndRestart(&maze, tmp2, &player, anchor2D, &licznik_krokow, &czas);
					else startAndRestart(&maze, fileName, &player, anchor2D, &licznik_krokow, &czas);
				}
				break;
			case RESTART:
				startAndRestart(&maze, fileName, &player, anchor2D, &licznik_krokow, &czas);
				break;
			case ANIMATION:
				animationUser = !animationUser;
				break;
			}
		}
		else {
			key = getch();
			drawOneBlockOfMaze(&maze, player.position, anchor2D);
			switch (key) {
			case UP_ARROW:
				if (tryAndMoveForwardPlayer(&maze, &player)) {
					licznik_krokow++;
					animationSoftware = true;
					animationForward = true;
				}
				break;
			case DOWN_ARROW:
				if (tryAndMoveBackPlayer(&maze, &player)) {
					licznik_krokow++;
					animationSoftware = true;
					animationForward = false;
				}
				break;
			case LEFT_ARROW: changePlayerDirection(&player, LEFT); break;
			case RIGHT_ARROW: changePlayerDirection(&player, RIGHT); break;
			}
			drawPlayer(&player, anchor2D);
		}
		if (checkKey(&maze, &player)) editMazeBoard(&maze, player.position, PATH);
		clear3D();
		drawMaze3D(&maze, &player, (animationSoftware && animationUser), animationForward, anchor3DLeft, anchor3DRight);
		animationSoftware = false;
		if (checkExit(&maze, &player)) {
			gotoxy(18, 25);
			cout << "Gratulacje udalo ci sie ukonczyc labirynt!!!";
			key = QUIT;
			getch();
		}
	} while (key != QUIT);

	if (maze.board != NULL) freeMaze(&maze);
	return 0;
}


//TODO: IN OUT w labiryncie nie mog¹ byæ na rogach ani zakleszczone (dookola brak przejscia) i nie moze byc 3 dziur - sprawdzanie na etapie edytora NIE MOZE BYC 2X2
//		Sprawdzanie czy przez labirynt mozna przejsc
//		Testy czy dobrze wyszukuje wejscie wyjscie
//		Bez typów anonimowych
//		Zawê¿yæ wymagane parametry funkcji tylko do tych u¿ywanych w funkcji np player.direction zamiast player
//		Albo cout albo printf
//		Ulepszyæ kolejnoœæ bibliotek zeby byla zawsze taka sama
//		#ifndef tam gdzie to potrzebne
//		liczyc kroki
//		tablica blok_t a nie char - usunac rzutowanie (block_t)
//		const
//		zamieniec na angielski
//		dodac wszedzie ifndef
//		komentarze naglowkowe do funkcji