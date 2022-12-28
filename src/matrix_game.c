#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LENGTH(X)               (sizeof X / sizeof X[0])

#define STARTX 9
#define STARTY 3
#define OFFSETX 0
#define OFFSETY 1
#define WIDTH  5
#define HEIGHT 2

#define PRIMARY 2
#define SECONDARY 6
#define ACCENT 4

// sumr: sum of rows
// sumc: sum of cols
// sumd: sum of the first diagonal
int **mat, N, sumr, sumc, sumd, i, j, temp;

// random choice 1 | 0, 0 for rotating and 1 for permuting two values
unsigned int choice;
char ch;

// welcome message
static const char* welcome = "Welcome to the matrix game!";
static const char* instructions[] = {
	"You have to defeat the computer by finding",
	"the same sum of 2 rows and a column or 3 rows.",
	"Each player has two moves, to either rotate the matrix",
	"or to swap two elements in the matrix."
};
static const char* keys[] = {
	"<Press P to permute>",
	"<Press R to rotate>"
};


void
shuffle(int **mat) {
	int r;
	for (i = 0; i < N; ++i) {
     r = rand() % N;
     for (j = 0; j < N; ++j) {
         temp =  mat[i][j];
         mat[i][j] = mat[r][j];
         mat[r][j] = temp;
     }
  }
}

void
initialize(int **mat) {
	for (i = 0; i < N; ++i)
		for (j = 0; j < N; ++j)
			mat[i][j] = i * N + j + 1;

	shuffle(mat);
}

void
rotate(int **mat) {
  for (i = 0; i < N / 2; ++i)
    for (j = i; j < N - i; ++j) {
      temp = mat[i][j];
      mat[i][j] = mat[N - 1 - j][i];
      mat[N - 1 - j][i] = mat[N - 1 - i][N - 1 - j];
      mat[N - 1 - i][N - 1 - j] = mat[j][N - 1 - i];
      mat[j][N - 1 - i] = temp;
    }
}

void
board(WINDOW *win, int starty, int startx, int lines, int cols, 
	   int tile_width, int tile_height) {
  int endy, endx;
	
	endy = starty + lines * tile_height;
	endx = startx + cols  * tile_width;
	
	for (j = starty; j <= endy; j += tile_height)
		for (i = startx; i <= endx; ++i)
			mvwaddch(win, j, i, ACS_HLINE);
	for (i = startx; i <= endx; i += tile_width)
		for (j = starty; j <= endy; ++j)
			mvwaddch(win, j, i, ACS_VLINE);
	mvwaddch(win, starty, startx, ACS_ULCORNER);
	mvwaddch(win, endy, startx, ACS_LLCORNER);
	mvwaddch(win, starty, endx, ACS_URCORNER);
	mvwaddch(win, 	endy, endx, ACS_LRCORNER);
	for (j = starty + tile_height; j <= endy - tile_height; j += tile_height) {
    mvwaddch(win, j, startx, ACS_LTEE);
		mvwaddch(win, j, endx, ACS_RTEE);	
		for(i = startx + tile_width; i <= endx - tile_width; i += tile_width)
			mvwaddch(win, j, i, ACS_PLUS);
	}
	for(i = startx + tile_width; i <= endx - tile_width; i += tile_width) {
    mvwaddch(win, starty, i, ACS_TTEE);
		mvwaddch(win, endy, i, ACS_BTEE);
	}
	wrefresh(win);
}

void
matrix_board(int **mat) {
	int deltax, deltay;
	int startx, starty;

	starty = (LINES - N * HEIGHT) / 2 + OFFSETY;
	startx = (COLS  - N * WIDTH)  / 2 + OFFSETX;
	attron(COLOR_PAIR(PRIMARY));
	board(stdscr, starty, startx, N, N, WIDTH, HEIGHT);
	attroff(COLOR_PAIR(PRIMARY));
	deltay = HEIGHT / 2;
	deltax = WIDTH  / 2;

	// find all sums
	mat[N][N] = 0;
	for (i = 0; i < N; ++i) {
		mat[i][N] = 0;
		mat[N][i] = 0;
		mat[N][N] += mat[i][i];
		for (j = 0; j < N; ++j) {
			mat[i][N] += mat[i][j];
			mat[N][i] += mat[j][i];
		}
	}

	for (i = 0; i < N + 1; ++i) {
		for (j = 0; j < N + 1; ++j) {
			if (i == N || j == N) {
				attron(COLOR_PAIR(SECONDARY));
				mvprintw(starty + j * HEIGHT + deltay,
						startx + i * WIDTH  + deltax,
						"%03d", mat[i][j]);
				attroff(COLOR_PAIR(SECONDARY));
			} else {
				mvprintw(starty + j * HEIGHT + deltay,
						startx + i * WIDTH  + deltax,
						"%2d", mat[i][j]);
			}
		}
	}
}

int
main(int argc, char *argv[]) {
  // initialization (only called once)
  srand(time(NULL));

  if(argc != 2) {
    printf("Usage: %s <size of the matrix>\n", argv[0]);
    exit(0);
  }
  N = atoi(argv[1]);
  if(N < 2 || N > 9) {
    printf("Matrix size should be: 1 < N < 9\n");
    exit(0);
  }

  // declaring the matrix
  mat = (int**)malloc((N + 1) * sizeof(int*));
  for(i = 0; i < N + 1; ++i)
    mat[i] = (int*)malloc((N + 1) * sizeof(int));

  // generate a random matrix
  initialize(mat);

  // ncurses initialization and showing the matrix
	initscr();

	// start color function from ncurses
	start_color();
	use_default_colors();

	// initialize color pairs
	init_pair(1, COLOR_BLACK, COLOR_MAGENTA);		// magenta - inverted
	init_pair(2, COLOR_MAGENTA, COLOR_BLACK);		// magenta
	init_pair(3, COLOR_CYAN, COLOR_BLACK);			// cyan
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);		// yellow
	init_pair(5, COLOR_RED, COLOR_BLACK);				// red
	init_pair(6, COLOR_GREEN, COLOR_BLACK);			// green

	// welcome / game instructions
	/* print_in_middle(stdscr, LINES / 2, 0, 0, "Viola !!! In color ..."); */
	attron(COLOR_PAIR(ACCENT));
	mvwprintw(stdscr, 1, (COLS - strlen(welcome)) / 2 , "%s", welcome);
	for (i = 0; i < LENGTH(instructions); ++i)
		mvwprintw(stdscr, i + 2, (COLS - strlen(instructions[i])) / 2, "%s", instructions[i]);
	for (i = 0; i < LENGTH(keys); ++i)
		mvwprintw(stdscr, LINES - i - 1, 1, "%s", keys[i]);
	attroff(COLOR_PAIR(ACCENT));
	refresh();

	curs_set(0);
	noecho();
	keypad(stdscr, TRUE);
	matrix_board(mat);
	while((ch = getch()) != 'q') {
		switch(ch) {
			case 'r':
				rotate(mat);
				matrix_board(mat);
				break;
			case 'p':
				matrix_board(mat);
				break;
		}
	}
	endwin();
}
