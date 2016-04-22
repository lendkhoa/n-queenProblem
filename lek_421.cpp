/**
 * nqueens
 * @author Jon Beck
 * @author Khoa Le 
 */
typedef unsigned int uint;
#include <cassert>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <vector>
#include "matrix.h"
using namespace std;

/**
 * This function checks to see if the current placement of queens on
 * the board is ok.  It is ok if no queen attacks another queen.  A
 * return of true means no conflict.  A return of false means at least
 * two queens conflict.
 * @param board the n x n chessboard
 * @param row x value of new queen position
 * @param col y value of new queen position
 * @return the absence of a conflict with this board arrangement
 */
bool ok( const Matrix< bool > & board, uint row, uint col)
{
  uint n = board.numrows();
  int i; 
  int j;
    for (i = 0; i < col; i++)
    {
        if (board.at(row,i)) 
            return false;
    }
    for (i = row, j = col; i >= 0 && j >= 0; i--, j--)
    {
        if (board.at(i,j))
            return false;
    }
    for (i = row, j = col; j >= 0 && i < n; i++, j--)
    {
        if (board.at(i,j))
            return false;
    }
    return true;
}

/**
 * a simple procedure to output an ASCII art horizontal rule with plus
 * signs where columns will intersect
 */
void hr( uint cols )
{
  cout << "    +";
  for( uint col = 0; col < cols; col++ )
    cout << "---+";
  cout << endl;
}

/**
 * this function dumps the board to std output
 * @param board the board whose arrangement to dump
 */
void printBoard( const Matrix< bool> & board )
{
  hr( board.numrows());
  for( uint row = 0; row < board.numrows(); row++)
  {
    cout << ' ' << setw(2) << board.numrows() - row << " |";
    for( uint col = 0; col < board.numrows(); col++ )
    {
      if( board.at(row, col) )
        cout << " X |";
      else
        cout << "   |";
    }
    cout << endl;
    hr( board.numrows());
  }

  cout << "     ";
  for( uint col = 0; col < board.numrows(); col++ )
    cout << ' ' << (char)( 'a' + col ) << "  ";
  cout << endl;
}

/**
 * This is the recursive backtracking function.  When called, k queens
 * have already been placed on the board in columns 0 .. k-1.  We're
 * trying to place the next queen in column k.
 *
 * I modified the r_backtrack() method to return a boolean value so that 
 * when the value k reached the number of rows, it did not exit the method, 
 * instead it simply signaled that it had found a solution (backtraced 
 * form the very first queen on that row) and returned true. The method
 * went on to backtrack the next row - it stopped when all the rows were 
 * processed. At the same time, the method constantly updated the 
 * 'counter' variable, for every row, to keep track of the number of nodes 
 * it had visited. It returned the tables if found or a no solution statement
 * if none had been found. 
 *
 * @param k the column in which to place the current queen
 * @param foundFirst signal when the first solution is found
 * @param count counts the number of nodes visited
 * @param hasSol returns false if there is no solution
 * @param rowHasQueen keeps track of which row already has a queen
 * @param board the board on which to place the queen
 * @return true if there is a solution.
 */
bool r_backtrack( uint k, bool &foundFirst, uint &count, bool &hasSol,

 vector <bool> &rowHasQueen, Matrix< bool > & board)
{
  // are we done?
  if( k == board.numrows())
  {
  // if so, report and exit
    printBoard( board );
    if (!foundFirst) //found the first solution?
    {
      cout << "    Nodes visited " << count << ".\n";
      foundFirst = true;
    }
    return hasSol = true;
  }

  // try each row in turn
  for( uint row = 0; row < board.numrows(); row++ )
  {
    // does row already have a queen?
    if(!rowHasQueen.at(row))
    {
      //if not, visit the position
      count++;
      rowHasQueen.at(row) = true;
      // did that cause a conflict?
      if( ok(board, row, k))
      {
        // if not, put a queen there
        board.at(row, k) = true;
        // keep going to next column 
        r_backtrack( k + 1, foundFirst, count, hasSol, rowHasQueen, board );
      }
      
      // if that didn't work, un-try the current attempt
      board.at(row, k) = false;
      rowHasQueen.at(row) = false;
    }
  }
  return hasSol;
}

/**
 * set up the board and start the backtracking algorithm at the
 * top of the search tree
 */
int main( int argc, char* argv[] )
{
  if( argc != 2 )
  {
    cout << "Usage: " << argv[0] << " n" << endl;
    cout << "       where n is the number of queens to place" << endl;
    cout << "       on an n x n chessboard, with 0 < n < 26" << endl;
    exit( 2 );
  }
  assert (argc == 2);
  stringstream ss( argv[ 1 ]); 
  uint n;
  ss >> n;
  assert (n > 0 && n < 26);

  // initialize a board with no queens
  Matrix< bool > board( n, n );
  for( uint row = 0; row < n; row++)
    for( uint col = 0; col < n; col++ )
      board.at(row, col) = false;

  // vector to keep track of the rows of queens - it reduces 
  //the spanning of the tree from n x n to n - k for each level k 
  vector <bool> rowHasQueen (n);
  for( uint row = 0; row < n; row++)
    rowHasQueen.at(row) = false;

  bool foundFirst = false; //returns true when the first solution is found
  bool hasSol = false; // returns true when the board has a solution
  uint count = 0; // counts the number of nodes visited 

  // back tracking - output
  if(!r_backtrack( 0, foundFirst, count, hasSol, rowHasQueen, board ))
    cout << "No solution" << endl;
  cout << "Total number of nodes visited: "<< count << endl;
  exit( 1 );
}
