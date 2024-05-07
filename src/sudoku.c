/**************************************/
/* sudoku.c                           */
/* Solves a sudoku puzzle through     */
/* recursion and backtracking         */
/**************************************/

#include <stdio.h>
#include <ctype.h>
#define MAX 9
#define INVLINE 1
#define NOSOL 2

int puzzle[MAX][MAX];
int tempGrid[MAX][MAX];
char resLine[1024];
int error = 0;

/************************************************************
 * @param line line of input as a char array
 * @param puzzGrid the 9 x 9 grid to be filled
 * fills a 9 x 9 grid with elements from a character array
 ************************************************************/
void fillGrid(char* line, int puzzGrid[MAX][MAX])
{
  int i;
  for(i = 0; i < 81; i++) {
    if (isdigit(line[i])) puzzGrid[i%9][i/9] = line[i] - '0';
    else puzzGrid[i%9][i/9] = 0;
  }
}


/************************************************************
 * clears global variables puzzle, tempGrid, and resLine
 * clear() is called after fillAndSolve()
 ************************************************************/
void clear()
{
  int i, j;
  for(i = 0; i < MAX; i++) {
    for(j = 0; j < MAX; j++) {
      puzzle[i][j] = 0;
      tempGrid[i][j] = 0;
      resLine[i + j] = 0;
    }
  }
  error = 0;
}

/************************************************************
 * converts a puzzle grid into a line (1D char array)
 ************************************************************/
void gridToLine()
{
  int i, j, k;
  k = 0;
  for(i = 0; i < MAX; i++) {
    for(j = 0; j < MAX; j++) {
      resLine[k++] = puzzle[j][i] + '0';
    }
  }
}

/************************************************************
 * @param line line of input as a char array
 * Counts the length of a line and returns the length
 * Method is from CS241L getBits Hints lecture slides
 ************************************************************/
int strLen(char* line)
{
  char *x = line;
  while (*x) x++;
  return x - line;
}

/************************************************************
 * @param line line of input as a char array
 * Takes a line (1D char array) as input and
 * checks if it is valid
 * 1. checks if the length of line is equal to 81
 * 2. checks for invalid characters present in line
 * 3. checks if initial sudoku value are valid by
 *    creating a temporary puzzle grid and looping
 *    through each cell
 ************************************************************/
int isValidLine(char* line)
{
  int i, r, c, m, n, tempColSq, tempRowSq;
  
  /* checks if the length of line is equal to 81 */
  if (strLen(line) != 81) return 0;

  /* checks for invalid characters present in line */
  for(i = 0; i < strLen(line); i++) {
    char ch = line[i];
    if (!(isdigit(ch) || ch == '\n' || ch == '.')) return 0;
  }

  /* checks if initial sudoku value are valid by creating a temporary puzzle grid */
  fillGrid(line, tempGrid);
  /* loops through every cell */
  for(r = 0; r < MAX; r++) {
    for(c = 0; c < MAX; c++) {
      /* checks if a cell is empty */
      int val = tempGrid[r][c];
      if (val > 0) {
        tempColSq = c - c % 3;
        tempRowSq = r - r % 3;
        for(n = 0; n < MAX; n++) {
          /* loops through current cell's row, col, and 3x3 matrix to check for validity */
          if (tempGrid[n][c] == val && (r + c) != (n + c)) return 0;
          if (tempGrid[r][n] == val && (r + c) != (n + r)) return 0;
          for(m = 0; m < MAX; m++) {
            if (tempGrid[tempRowSq + m/3][tempColSq + n/3] == val &&
            (r + c) != (tempRowSq + m/3 + tempColSq + n/3)) return 0;
          }
        }
      }
    }
  }

  return 1;
}

/************************************************************
 * @param puzzGrid puzzle grid (2D int array)
 * @param col int value of col in puzzle grid
 * @param row int value of row in puzzle grid
 * @param val int value to check for
 * checks if an input val (val) is valid in a given
 * empty cell,
 * loops through current cell's row, col, and
 * its 3x3 matrix to check for validity
 ************************************************************/
int isSolvable(int puzzGrid[MAX][MAX], int col, int row, int val)
{
  int i, j, tempColSq, tempRowSq;
  tempColSq = col - col % 3;
  tempRowSq = row - row % 3;
  for(i = 0; i < MAX; i++) {
    if (puzzGrid[row][i] == val) return 0;
    if (puzzGrid[i][col] == val) return 0;
    for(j = 0; j < MAX; j++) {
      if (puzzGrid[tempRowSq + j/3][tempColSq + i/3] == val) return 0;
    }
  }
  return 1;
}

/************************************************************
 * @param puzzGrid puzzle grid (2D int array)
 * @param col int value of col in puzzle grid
 * @param row int value of row in puzzle grid
 * Solves sudoku puzzle through recursion and backtracking.
 * First, it checks if it has reached the end,
 * Next, it checks if it has the end of a row and then jumps
 * to the next row,
 * Then, it checks if a cell is empty or not, if it is then
 * increment col and call solvePuzzle(),
 * After all that, it loops from 1-9 and checks if each
 * value is a valid in a given cell.
 * If it is valid, then that cell is set to that value and
 * solvePuzzle() is called again with a new cell filled.
 * If it is not valid, then set that cell to 0 and move on
 * to the next value.
 * solvePuzzle() is called recursively until the end of the
 * grid is reached.
 ************************************************************/
int solvePuzzle(int puzzGrid[MAX][MAX], int col, int row)
{
  if (row == 9) return 1;
  else if (col == 9) return solvePuzzle(puzzGrid, 0, row + 1);
  else if (puzzGrid[row][col] != 0) return solvePuzzle(puzzGrid, col + 1, row);
  else
  {
    int i;
    for(i = 1; i <= MAX; i++) {
      if (isSolvable(puzzGrid, col, row, i)) {
        puzzle[row][col] = i;
        if (solvePuzzle(puzzGrid, col + 1, row)) return 1;
        puzzle[row][col] = 0;
      }
    }
    return 0;
  }
}

/************************************************************
 * @param line line of input as a char array
 * Takes a line (1D char array) as input,
 * and calls isValidLine() to check if it is valid,
 * calls fillGrid() to fill puzzle grid with line,
 * then calls solvePuzzle() to solve the puzzle grid
 * if solved, convert grid to line (1D char array) and print,
 * also checks for errors and prints if there are any
 ************************************************************/
void fillAndSolve(char* line)
{
  printf("%s\n", line);
  if (isValidLine(line)) {
    fillGrid(line,puzzle);
    if (solvePuzzle(puzzle, 0, 0)) {
      gridToLine();
      printf("%s\n", resLine);
    }
    else error |= NOSOL;
  }
  else error |= INVLINE;

  if (error & INVLINE) printf("Error\n");
  else if (error & NOSOL) printf("No solution\n");
  printf("\n");
}

/************************************************************
 * Reads from stdin and puts each line of input in a
 * char array.
 * This array is then passed onto fillAndSolve(), and then
 * global variables are cleared
 ************************************************************/
void readInput()
{
  int c;
  int i;
  char line[1024];
  i = 0;
  while ((c = getchar()) != EOF) {
    if (c != '\n') {
      line[i++] = (char) c;
    }
    else {
      line[i] = '\0';
      fillAndSolve(line);
      i = 0;
      clear();
    }
  }
}

/************************************************************
 * main function, calls readInput()
 ************************************************************/
int main()
{
  readInput();
  return 0;
}
