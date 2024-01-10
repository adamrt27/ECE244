#include <iostream>
#include <string>
using namespace std;
// https://q.utoronto.ca/groups
#include "GameState.h"
#include "globals.h"

// Function prototypes

void printGameState(GameState& game_state);
int checkOverallWinner(int winner[]);
char return_winner(int winner);

// The main function
int main() {
    /**********************************************************************************/
    /* Create three initialized game state objects in an array */
    /**********************************************************************************/
    GameState game_state;

    // Read one integer from the user that represents the column
    // the player would like to place their piece (R or Y) in
    // You can assume there will be no formatting errors in the input

    int lol_def[7*7] = {0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0};

    int lol[7*7] = {0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,
                    0,0,0,0,0,0,0,
                    0,Y,0,0,R,0,0,
                    0,0,Y,R,R,0,0,
                    0,0,R,0,0,R,0,
                    R,R,0,R,Y,R,R};

    for (int i = 0; i < boardSize; i ++) {
        for (int j = 0; j < boardSize; j++){
            game_state.set_gameBoard(i,j, lol[j + (i * boardSize)]);
        }
    }  
    // Call playMove

    printGameState(game_state);


    playMove(game_state);

    // Print the GameState object, as prescribed in the handout

    cout << game_state.get_gameOver(), "\n";
}

void printGameState(GameState& game_state){
  for(int row = boardSize - 1; row >= 0; row --){
    for(int col = 0; col < boardSize; col ++){
      
      switch (game_state.get_gameBoard(row, col)){
        case Empty:
          cout << "_";
          break;
        case Y:
          cout << "Y";
          break;
        case R:
          cout << "R";
          break;
      }

      if(col == boardSize - 1){
        cout << "\n";
      }
    }
  }
}

int checkOverallWinner(int winner[]){
  int R_count = 0, Y_count = 0;
  for(int i = 0; i < 3; i ++){
    if(winner[i] == Y){
      Y_count ++;
    }
    else if(winner[i] == R){
      R_count ++;
    }
  }
  if (R_count >= 2){
    return R;
  }
  else if(Y_count >= 2){
    return Y;
  }
  else{
    return 0;
  }
}


// Returns character form of the winner
char return_winner(int winner){
  if(winner == Y){
    return 'Y';
  }
  else{
    return 'R';
  }
}