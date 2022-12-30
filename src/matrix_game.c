/* See LICENSE file for copyright and license details.
 *
 * Author: Neggazi Mohamed Lamine
 * Email: neggazimedlamine@gmail.com
 * 
 * A game where a player has to defeat the computer 
 * by finding the same sum of 2 matrix rows and a column or 3 rows.
 * Each player has two moves, to either rotate 
 * the matrix or to swap two elements in the matrix.
 */

#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

/* useful macro to get the size (length) of an array */
#define LENGTH(X) (sizeof X / sizeof X[0])

/* ncurses dimensions */
#define STARTX 9
#define STARTY 3
#define OFFSETX 0
#define OFFSETY 1
#define WIDTH  5
#define HEIGHT 2

/* enums */
enum player { You, Computer }; 	/* player type */
enum color { PRIMARY = 1, SECONDARY, ACCENT, iPRIMARY, RED, CYAN };
char ch = ' '; /* keyboard input character */

/* position of elements to swap */
typedef struct {
	unsigned int x, y;
} Position;

/* function declarations */
static void shuffle(int **a);
static void initialize(int **a);
static void rotate(int **a);
static void swap(int **a, Position *p1, Position *p2);
static int  count(int *a, int x);
static int  max(int *a);
static void board(WINDOW *win, int starty, int startx, int lines, int cols, int tile_width, int tile_height);
static void matrix_board(int **a);
static void print(enum color COLOR, int x, int y, const char *str);

/* declarations */
static int **a, N;
static int i, j, temp;

/* messages */
static const char* welcome = "Welcome to the matrix game!";
static const char* instructions[] = {
	"You have to defeat the computer by finding",
	"the same sum of 2 rows and a column or 3 rows.",
	"Each player has two moves, to either rotate the matrix",
	"or to swap two elements in the matrix."
};
static const char* turn[] = {
	"Your turn --> ",
	"Wait... -->   "
};
static const char* keys[] = {
	"<Press P to permute>",
	"<Press R to rotate>"
};

/* function implementations */
void
shuffle(int **a) {
	int r;
	for (i = 0; i < N; ++i) {
     r = rand() % N;
     for (j = 0; j < N; ++j) {
         temp =  a[i][j];
         a[i][j] = a[r][j];
         a[r][j] = temp;
     }
  }
}

void
initialize(int **a) {
	for (i = 0; i < N; ++i)
		for (j = 0; j < N; ++j)
			a[i][j] = i * N + j + 1;

	shuffle(a);
}

void
rotate(int **a) {
  for (i = 0; i < N / 2; ++i)
    for (j = i; j < N - i; ++j) {
      temp = a[i][j];
      a[i][j] = a[N - 1 - j][i];
      a[N - 1 - j][i] = a[N - 1 - i][N - 1 - j];
      a[N - 1 - i][N - 1 - j] = a[j][N - 1 - i];
      a[j][N - 1 - i] = temp;
    }
}

void
swap(int **a, Position *p1, Position *p2) {
	temp = a[p1->x][p1->y];
	a[p1->x][p1->y] = a[p2->x][p2->y];
	a[p2->x][p2->y] = temp;
}

int
count(int *a, int x) {
	int result = 0;

	for (i = 0; i < N + 1; ++i)
		if (a[i] == x)
			result++;

	return result;
}

int
max(int *a) {
	int max = 0;

	for (i = 0; i < N * 2 + 1; ++i)
		if (max < a[i])
			max = a[i];

	return max;
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
matrix_board(int **a) {
	int deltax, deltay;
	int startx, starty;

	starty = (LINES - N * HEIGHT) / 2 + OFFSETY;
	startx = (COLS  - N * WIDTH)  / 2 + OFFSETX;
	attron(COLOR_PAIR(PRIMARY));
	board(stdscr, starty, startx, N, N, WIDTH, HEIGHT);
	attroff(COLOR_PAIR(PRIMARY));
	deltay = HEIGHT / 2;
	deltax = WIDTH  / 2;

	/* find all sums */
	a[N][N] = 0;
	for (i = 0; i < N; ++i) {
		a[i][N] = 0;
		a[N][i] = 0;
		a[N][N] += a[i][i];
		for (j = 0; j < N; ++j) {
			a[i][N] += a[i][j];
			a[N][i] += a[j][i];
		}
	}

	for (i = 0; i < N + 1; ++i) {
		for (j = 0; j < N + 1; ++j) {
			if (i == N || j == N) {
				attron(COLOR_PAIR(SECONDARY));
				mvprintw(starty + j * HEIGHT + deltay,
						startx + i * WIDTH  + deltax,
						"%03d", a[i][j]);
				attroff(COLOR_PAIR(SECONDARY));
			} else {
				mvprintw(starty + j * HEIGHT + deltay,
						startx + i * WIDTH  + deltax,
						"%2d", a[i][j]);
			}
		}
	}
}

void
print(enum color COLOR, int x, int y, const char *str) {
	attron(COLOR_PAIR(COLOR));
	mvwprintw(stdscr, x, y, "%s", str);
	attroff(COLOR_PAIR(COLOR));
}

int
main(int argc, char *argv[]) {
  /* initialization (should only be called once) */
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

  /* declaring the matrix */
  a = (int**)malloc((N + 1) * sizeof(int*));
  for(i = 0; i < N + 1; ++i)
    a[i] = (int*)malloc((N + 1) * sizeof(int));

  /* generate a random matrix */
  initialize(a);

  /* ncurses initialization and showing the matrix */
	initscr();

	/* start color function from ncurses */
	start_color();
	use_default_colors();

	/* initialize ncurses color pairs */
	init_pair(PRIMARY, COLOR_MAGENTA, COLOR_BLACK);		// magenta
	init_pair(SECONDARY, COLOR_GREEN, COLOR_BLACK);		// green
	init_pair(ACCENT, COLOR_YELLOW, COLOR_BLACK);			// yellow
	init_pair(iPRIMARY, COLOR_BLACK, COLOR_MAGENTA); 	// magenta - inverted
	init_pair(RED, COLOR_RED, COLOR_BLACK);						// red
	init_pair(CYAN, COLOR_CYAN, COLOR_BLACK);					// cyan

	/* welcome message / game instructions */
	print(ACCENT, 1, (COLS - strlen(welcome)) / 2 , welcome);
	for (i = 0; i < LENGTH(instructions); ++i)
		print(ACCENT, i + 2, (COLS - strlen(instructions[i])) / 2, instructions[i]);
	for (i = 0; i < LENGTH(keys); ++i)
		print(ACCENT, LINES - i - 1, 1, keys[i]);
	refresh();

	curs_set(0);
	noecho();
	keypad(stdscr, TRUE);

	/* start the game */
	print(ACCENT, LINES / 2 + 1, COLS / 12, turn[0]);

	enum player PLAYER = You;
	matrix_board(a);
	while((ch = getch()) != 'q') {
		if (PLAYER == You) {
			switch(ch) {
				case 'r':
					rotate(a);
					matrix_board(a);
					PLAYER = Computer;
					print(ACCENT, LINES / 2 + 1, COLS / 12, turn[1]);
					break;
				case 'p':
					matrix_board(a);
					PLAYER = Computer;
					print(ACCENT, LINES / 2 + 1, COLS / 12, turn[1]);
					break;
				default:
					break;
			}
		} else {
			sleep(2);
			PLAYER = You;
			print(ACCENT, LINES / 2 + 1, COLS / 12, turn[0]);
			refresh();
		}
	}
	endwin();
}
