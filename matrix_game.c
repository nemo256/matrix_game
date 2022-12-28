#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 5

unsigned int matrix[N][N];
int sum;

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
main() {
  // initialization (only called once)
  srand(time(NULL));

  // Generate a random matrix
  initialize(matrix);

  // Showing the matrix
  show(matrix);

  // Rotate a matrix by 90deg
  rotate(matrix);

  // Showing the matrix
  show(matrix);
}
