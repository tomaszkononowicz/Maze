#include "editor.h"

void help() {
	clrscr();
	gotoxy(2, 2);
	cout << "Pomoc" << endl;
	cout << "  Klawisze" << endl;
	cout << "    <  Zmiana kierunku patrzenia o 90 stopni zgodnie ze wskazowkami zegara" << endl;
	cout << "    <  Zmiana kierunku patrzenia o 90 stopni przeciwnie do wskazzowek zegara" << endl;
	cout << "    ^   Ruch do przodu" << endl;
	cout << "    v   Ruch do tylu" << endl;
	cout << "    d   Otwieranie i zamykanie drzwi" << endl;
	cout << "    i   Wczytanie domyslengo labiryntu" << endl;
	cout << "    q   Opuszczenie programu" << endl;
	cout << "    h   Wyswietlenie pomocy" << endl;
	cout << "    e   Wejscie do edytora" << endl;
	cout << "    o   Odczytanie labiryntu z pliku" << endl;
	cout << "    s   Zapisanie labiryntu do pliku [tylko w trybie edytora]" << endl;
	cout << "    r   Restart gry" << endl;
	cout << "    a   aktywacja animacji ruchu do przodu i ty³u" << endl;
	cout << "  Edytor" << endl;
	cout << "    0   Droga" << endl;
	cout << "   1-5   Sciana" << endl;
	cout << "    6   Otwarte drzwi" << endl;
	cout << "    7   Zamkniete drzwi" << endl;
	cout << "    8   Zamkniete drzwi [wymagany klucz]" << endl;
	cout << "    9   Klucz" << endl;
}

bool wantPlay() {
	gotoxy(1, 1);
	clrscr();
	cout << "Chcesz zagrac w swoj labirynt?" << endl << "(klawisz 't' jezeli tak, " << endl << "dowolny inny klawisz, aby utworzyc nowy labirynt) : ";
	if (getch() == 't') return true;
	else return false;
}

char* exportMaze(char* fileName, maze_t* maze) {
	if (is2x2(maze)) {
		clrscr();
		gotoxy(1, 1);
		cout << "Blad - Labirynt zawiera puste pola 2x2";
		getch();
		return NULL;
	}
	if (fileName==NULL)	fileName = loadFileNameMaze();
	char tmp[128];
	if (fileName != NULL) {
		int i = 0;
		while (fileName[i] != '\0') {
			tmp[i] = fileName[i];
			i++;
		}
		tmp[i] = '\0';
	}
	FILE* file;
	file = fopen(tmp, "w");
	if (file == NULL) {
		cout << tmp << endl << "Otwarcie pliku nie powiodlo sie" << endl;
		getch();
		return NULL;
	}
	for (int i = 0; i < maze->height; i++) {
		for (int j = 0; j < maze->width; j++) {
			block_t tmp = maze->board[i][j];
			char c;
			switch (tmp) {
			case PATH: c = '0'; break;
			case WALL1: c = '1'; break;
			case WALL2: c = '2'; break;
			case WALL3: c = '3'; break;
			case WALL4: c = '4'; break;
			case WALL5: c = '5'; break;
			case DOOR_OPEN: c = '6'; break;
			case DOOR_CLOSE: c = '7'; break;
			case DOOR_KEY: c = '8'; break;
			case KEY: c = '9'; break;
			}
			fprintf(file, "%c", c);
		}
		fprintf(file, "\n\r");
	}
	fclose(file);
	if (wantPlay()) return tmp;
	else return NULL;
}

char* editor(char* fileName, maze_t* maze, position_t anchor2D) {
	clrscr();
	maze_t editMaze;
	if (fileName == NULL) {
		gotoxy(1, 1);
		cout << "Podaj szerokosc swojego labiryntu: ";
		cin >> editMaze.width;
		cout << "Podaj wysokosc swojego labiryntu: ";
		cin >> editMaze.height;
	}
	else {
		editMaze.width = maze->width;
		editMaze.height = maze->height;
	}

	editMaze.board = (block_t**)malloc(editMaze.height*sizeof(block_t*));
	for (int i = 0; i < editMaze.height; i++) {
		editMaze.board[i] = (block_t*)malloc(editMaze.width*(sizeof(block_t)));
	}

	if (fileName == NULL) {
		for (int i = 0; i < editMaze.height; i++)
			for (int j = 0; j < editMaze.width; j++)
				editMaze.board[i][j] = PATH;
	}
	else {
		for (int i = 0; i < editMaze.height; i++)
			for (int j = 0; j < editMaze.width; j++)
				editMaze.board[i][j] = maze->board[i][j];
	}
	position_t cursor = { 0,0 };
	_setcursortype(_SOLIDCURSOR);
	drawBoard(&editMaze, anchor2D);
	gotoxy(cursor.column + anchor2D.column, cursor.row + anchor2D.row);
	int key;
	do {
		key = getch();
		if (key != 0) {
			switch (key) {
			case NUM0: editMaze.board[cursor.row][cursor.column] = PATH; break;
			case NUM1: editMaze.board[cursor.row][cursor.column] = WALL1; break;
			case NUM2: editMaze.board[cursor.row][cursor.column] = WALL2; break;
			case NUM3: editMaze.board[cursor.row][cursor.column] = WALL3; break;
			case NUM4: editMaze.board[cursor.row][cursor.column] = WALL4; break;
			case NUM5: editMaze.board[cursor.row][cursor.column] = WALL5; break;
			case NUM6: editMaze.board[cursor.row][cursor.column] = DOOR_OPEN; break;
			case NUM7: editMaze.board[cursor.row][cursor.column] = DOOR_CLOSE; break;
			case NUM8: editMaze.board[cursor.row][cursor.column] = DOOR_KEY; break;
			case NUM9: editMaze.board[cursor.row][cursor.column] = KEY; break;
			case SAVE_FILE_MAZE:
				if (fileName == NULL) return(exportMaze(NULL, &editMaze));
				else return(exportMaze(fileName, &editMaze));
				break;
			case HELP:
				help();
				cout << "                    Nacisnij dowolny klawisz aby kontynuowac" << endl;
				getch();
				clrscr();
				drawBoard(&editMaze, anchor2D);
				break;
			}
		}
		else {
			key = getch();
			switch (key) {
			case UP_ARROW: if (cursor.row - 1 >= 0) cursor.row--; break;
			case DOWN_ARROW: if (cursor.row + 1 <= editMaze.height - 1) cursor.row++; break;
			case LEFT_ARROW: if (cursor.column - 1 >= 0) cursor.column--; break;
			case RIGHT_ARROW: if (cursor.column + 1 <= editMaze.width - 1) cursor.column++; break;
			}

		}
		drawBoard(&editMaze, anchor2D);
		gotoxy(cursor.column + anchor2D.column, cursor.row + anchor2D.row);
	} while (key != QUIT && key != EDIT);
	return(exportMaze("tmp.txt", &editMaze));
}
