#include <curses.h>
#include <stdlib.h>
#include <time.h>

#define STARTX 9
#define STARTY 3
#define WIDTH  3
#define HEIGHT 2

#define TRACE_VALUE TRACE_MAXIMUM 

int **mat, N, sum, i, j;


void
initialize(unsigned int matrix[N][N]) {
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      matrix[i][j] = rand() % 26;
}

void
rotate(unsigned int matrix[N][N]) {
  for (int i = 0; i < N / 2; i++)
    for (int j = i, temp; j < N - i - 1; j++) {
      temp = matrix[i][j];
      matrix[i][j] = matrix[N - 1 - j][i];
      matrix[N - 1 - j][i] = matrix[N - 1 - i][N - 1 - j];
      matrix[N - 1 - i][N - 1 - j] = matrix[j][N - 1 - i];
      matrix[j][N - 1 - i] = temp;
    }
}

void
show(unsigned int matrix[N][N]) {
  printf("Matrix = {\n");
  for (int i = 0; i < N; i++) {
    sum = 0;
    printf(" {", i);
    for (int j = 0; j < N; j++) {
      sum += matrix[i][j];
      printf(" %2d,", matrix[i][j]);
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
      sum += matrix[j][i];
    printf(" %d ", sum);
  }
  printf("\n}\n");
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

  /* // Generate a random matrix */
  /* initialize(matrix); */

  /* // Showing the matrix */
  /* show(matrix); */

  /* // Rotate a matrix by 90deg */
  /* rotate(matrix); */

  /* // Showing the matrix */
  /* show(matrix); */
}
