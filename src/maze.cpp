/* Here in the .cpp you should define and implement everything declared in the
 * .h file.
 */

#include "maze.h"

string *build_matrix(int rows) {
  string *temp = new string[rows];
  return temp;
}

void fill_matrix(string *matrix, int rows) { // FIX?
  for (short iter = 0; iter < rows; iter++) {
    getline(cin, matrix[iter]);
  }
}

// Free function!
void print_matrix(const string *matrix, int rows) {
  for (int j = 0; j < rows; j++) {
    cout << matrix[j] << "\n";
  }
}

void delete_matrix(string *&matrix) {
  // delete matrix;
  // matrix = nullptr;
}

void find_start(string *matrix, int rows, int &row, int &col) {
  for (short iter = 0; iter < rows; iter++) {
    if (matrix[iter].find("S") != std::string::npos) {
      row = iter;
      col = matrix[iter].find("S");
      break;
    }
  }
}

// Recursive backtracking function.
bool find_exit(string *matrix, int row, int col) {
  if (at_end(matrix, row, col)) // base case
    return true;

  const string cardinal_directions[4] = {"NORTH", "SOUTH", "EAST", "WEST"};
  bool solution;

  for (string direction :
       cardinal_directions) { // Ask about STDIO index out of range
    if (valid_move(matrix, row, col, direction)) {
      matrix[row].at(col) = '@';
      if (direction == "NORTH") {
        solution = find_exit(matrix, row - 1, col);
      } else if (direction == "SOUTH") {
        solution = find_exit(matrix, row + 1, col);
      } else if (direction == "EAST") {
        solution = find_exit(matrix, row, col + 1);
      } else {
        solution = find_exit(matrix, row, col - 1);
      }

      if (solution)
        return true;
      else {
        matrix[row].at(col) = ' ';
      }
    }
  }

  return false;
}

bool at_end(const string *matrix, int row, int col) {
  if (matrix[row].at(col) == 'E')
    return true;
  return false;
}

bool valid_move(const string *matrix, int row, int col,
                const string &direction) {
  if (direction == "NORTH") {
    row--;
  } else if (direction == "SOUTH") {
    row++;
  } else if (direction == "EAST") {
    col++;
  } else if (direction == "WEST") {
    col--;
  }

  if (matrix[row].at(col) == ' ' || matrix[row].at(col) == 'E')
    return true;
  return false;
}
