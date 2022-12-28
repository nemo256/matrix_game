#include <curses.h>
#include <stdlib.h>
#include <time.h>

#define STARTX 9
#define STARTY 3
#define WIDTH  5
#define HEIGHT 2

#define TRACE_VALUE TRACE_MAXIMUM 

int **mat, N, sum, i, j;


void
initialize(int **mat) {
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      mat[i][j] = rand() % 26;
}

void
rotate(int **mat) {
  for (int i = 0; i < N / 2; i++)
    for (int j = i, temp; j < N - i - 1; j++) {
      temp = mat[i][j];
      mat[i][j] = mat[N - 1 - j][i];
      mat[N - 1 - j][i] = mat[N - 1 - i][N - 1 - j];
      mat[N - 1 - i][N - 1 - j] = mat[j][N - 1 - i];
      mat[j][N - 1 - i] = temp;
    }
}

void
show(int **mat) {
  printf("Matrix = {\n");
  for (int i = 0; i < N; i++) {
    sum = 0;
    printf(" {", i);
    for (int j = 0; j < N; j++) {
      sum += mat[i][j];
      printf(" %2d,", mat[i][j]);
    }
    printf(" }, - %d\n", sum);
  }

  printf("  ");
  for (int i = 0; i < N; i++)
    printf("  | ");
  printf("\n  ");

  for (int i = 0; i < N; i++) {
    sum = 0;
    for (int j = 0; j < N; j++)
      sum += mat[j][i];
    printf(" %d ", sum);
  }
  printf("\n}\n");
}

void
magic(int **mat, int n) {	
	int i,j,k;
	int row,col;
	for(i = 0;i < n;++i)
		for(j = 0;j < n;++j)
			mat[i][j] = -1;
	row = 0;
	col = n / 2;

	k = 1;
	mat[row][col] = k;
	
	while(k != n * n)
	{	
		if(row == 0 && col != n - 1)
		{	row = n - 1;
			col ++;
			mat[row][col] = ++k;
		}
		else if(row != 0 && col != n - 1)
		{	if(mat[row - 1][col + 1] == -1)
			{	row --;
				col ++;	
				mat[row][col] = ++k;
			}
			else
			{	
				row ++;
				mat[row][col] = ++k;
			}
		}
		else if(row != 0 && col == n - 1)
		{	
			row --;
			col = 0;
			mat[row][col] = ++k;
		}
		else if(row == 0 && col == n - 1)
		{	row ++;
			mat[row][col] = ++k;	
		}
			
	}
	return;
}

void
print(int **mat, int N) {
	int x,y;
	x = STARTX;
	y = STARTY;
	mvprintw(1,30, "MAGIC SQUARE");
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			mvprintw(y, x, "%d", mat[i][j]);
			if (N > 9)
				x += 4;
			else
				x += 6;
		}
		x = STARTX;
		if (N > 7)
			y += 2;
		else
			y += 3;
	}
	refresh();
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

void matrix_board(int **mat) {
	int deltax, deltay;
	int startx, starty;

	starty = (LINES - N * HEIGHT) / 2;
	startx = (COLS  - N * WIDTH) / 2;
	board(stdscr, starty, startx, N, N, WIDTH, HEIGHT);
	deltay = HEIGHT / 2;
	deltax = WIDTH  / 2;
	for (i = 0; i < N; ++i)
		for (j = 0; j < N; ++j)
			mvprintw(starty + j * HEIGHT + deltay,
				 startx + i * WIDTH  + deltax,
				 "%d", mat[i][j]);
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

  // Declaring the matrix
  mat = (int**)malloc(N * sizeof(int*));
  for(i = 0; i < N; ++i)
    mat[i] = (int*)malloc(N * sizeof(int));

  // Generate a random matrix
  initialize(mat);

  // ncurses initialization and showing the matrix
	initscr();
	curs_set(0);
	noecho();
	matrix_board(mat);
	getch();
	endwin();


  /* // Showing the matrix */
  /* show(matrix); */

  /* // Rotate a matrix by 90deg */
  /* rotate(matrix); */

  /* // Showing the matrix */
  /* show(matrix); */
}
