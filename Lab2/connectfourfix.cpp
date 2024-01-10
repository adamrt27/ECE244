//
//  connectfour.cpp
//  Connect Four Game
//
//  Created by Tarek Abdelrahman on 2019-06-07.
//  Modified by Tarek Abdelrahman on 2020-09-17.
//  Modified by Salma Emara and Ethan Hugh on 2023-09-06
//  Copyright © 2019-2020 Tarek Abdelrahman. All rights reserved.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.
//
//  ECE244 Student: Complete the skeletal code of the main function in this file

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
  const int numOfRounds = 3;
  GameState game_state[numOfRounds];

  // Read one integer from the user that represents the column
  // the player would like to place their piece (R or Y) in
  // You can assume there will be no formatting errors in the input

  int col;
  int round = 0;
  int winner[3];

  cout << "Game " << 1 << "\n";
  while (!game_state[round].get_gameOver() and round <= 3) {
    cout << "Enter column to place piece: ";
    cin >> col;
 
    if( cin.eof() ) {
      cerr << endl << "Game ended by user." << endl ;
      exit( 0 ) ;
    }
    if( cin.fail() ) {
      cin.clear() ;
      cin.ignore( 1000, '\n' ) ;
      col = -1 ; // giving col invalid value so it will be handled as invalid input below
    }
  
    // Check validity of input and if not valid, handle accordingly

    // check if between 0 and boardSize
    if(col < 0 or col >= boardSize){
      cout << "Invalid column!\n";
      continue;
    }

    // check if column is full
    if (game_state[round].get_gameBoard(boardSize - 1, col) != Empty){
      cout << "Invalid column!\n";
      continue;
    }

    cout << "column chosen: " << col << "\n";
    
    // The coordinates are valid; set the selectedRow and selectedColumn
    // members of the game state to the read values
    // Note that the corresponding mutators of GameState must be first
    // implemented before this works
    
    game_state[round].set_selectedColumn(col);

    // get the row
    int row = 0;
    while(game_state[round].get_gameBoard(row,col) != Empty){
      row ++;
    }

    game_state[round].set_selectedRow(row);

    // Call playMove

    playMove(game_state[round]);

    // Print the GameState object, as prescribed in the handout

    printGameState(game_state[round]);

    // Check if a player won this round and if so handle accordingly

    if(game_state[round].get_gameOver()){
      cout << return_winner(game_state[round].get_winner()) << " won this round!\n";
      winner[round] = game_state[round].get_winner();
      game_state[round].set_gameOver(true);
      round ++;
      game_state[round].set_turn(true);
	if(round <= 2 && checkOverallWinner(winner) == 0){
        	cout << "Game " << round + 1 << "\n";
      	}

    }

    // Check if a player won this match and if so handle accordingly

    if(checkOverallWinner(winner) != 0){
      
      cout << return_winner(checkOverallWinner(winner)) << " won the match!\n";
      game_state[round].set_gameOver(true);
    }
  }
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
