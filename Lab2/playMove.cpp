//
//  playMove.cpp
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
//  ECE244 Student: write your code for playMove in this file

#include "GameState.h"
#include "globals.h"

// Function prototypes (if you need)

bool check_diagonal(GameState& game_state, int row, int col, bool direction);
bool check_row(GameState& game_state, int row, int col);
bool check_col(GameState& game_state, int row, int col);

void playMove(GameState& game_state) {
  // Change turn if move is valid,
  // Change gameOver
  // Change winner

  // first lets implement the adding of the piece

  // to start, find which row to add the piece at
  int cur_col = game_state.get_selectedColumn();
  int cur_row = 0;
  while(game_state.get_gameBoard(cur_row,cur_col) != Empty){
    cur_row ++;
  }

  // check whose turn it is
  int fill_val;
  if(game_state.get_turn()){
    fill_val = R;
  } 
  else{
    fill_val = Y;
  }

  // set the space to whoevers turn it is
  game_state.set_gameBoard(cur_row, cur_col, fill_val);

  // then lets implement checking of winners

  // check across each row, up each column, the diagonal

  // start across each row

  // initialize an array to keep track of previous 3 values where prev[0] is most recent

  // go along each row to find 4 in a row
  for(int row = 0; row < boardSize; row ++){
    for(int col = 0; col <= boardSize-4; col ++){
       if(check_row(game_state, row, col)){
        game_state.set_gameOver(true);
        game_state.set_winner(game_state.get_gameBoard(row, col));
      }
    }
  }
  
  for(int col = 0; col < boardSize; col ++){
    for(int row = 0; row <= boardSize-4; row ++) {
      if(check_col(game_state, row, col)){
        game_state.set_gameOver(true);
        game_state.set_winner(game_state.get_gameBoard(row, col));
      }
    }
  }

  // do diagonals, going to the right and upwards
  // start on row boardSize - 4 and go along checking diagonals, 
  // then go down a row.

  // only check diagonals if they are at least 4 long
  // to get the length of diagonal = boardSize (== 7) - row_num
  // therefore: 4 <= boardSize - row_num ---> row_num <= boardSize - 4

  // down left column
  for (int row = boardSize - 4; row >= 0; row --){
    for (int col = 0; col <= boardSize - 4; col ++){
      if(check_diagonal(game_state, row, col, true)){
        game_state.set_gameOver(true);
        game_state.set_winner(game_state.get_gameBoard(row, col));
      }
    }
  }

  // along bottom row

  for (int row = boardSize - 4; row >= 0; row --){
    for (int col = boardSize; col >= 0; col --){
      if(check_diagonal(game_state, row, col, false)){
        game_state.set_gameOver(true);
        game_state.set_winner(game_state.get_gameBoard(row, col));
      }
    }
  }

  // if they didnt win, switch turn

  game_state.set_turn(not game_state.get_turn());
}

bool check_row(GameState& game_state, int row, int col){
  for (int i = 0; i < 4; i ++){
    if (game_state.get_gameBoard(row, col) != game_state.get_gameBoard(row, col + i)
        or game_state.get_gameBoard(row, col) == Empty){
          return false;
    }
  }
  return true;
}

bool check_col(GameState& game_state, int row, int col){
  for (int i = 0; i < 4; i ++){
    if (game_state.get_gameBoard(row, col) != game_state.get_gameBoard(row + i, col)
        or game_state.get_gameBoard(row, col) == Empty){
          return false;
    }
  }
  return true;
}

// checks if 4 in a row along a diagonal, going upwards and to the right
// if direction is true, go right and up, else go left and up
bool check_diagonal(GameState& game_state, int row, int col, bool direction){
  if(direction){
    for (int i = 0; i < 4; i++) {
      if (game_state.get_gameBoard(row, col) != game_state.get_gameBoard(row + i, col + i)
          or game_state.get_gameBoard(row, col) == Empty) {
          return false;
      }
    }
  }
  else{
    for (int i = 0; i < 4; i++) {
      if (game_state.get_gameBoard(row, col) != game_state.get_gameBoard(row + i, col - i)
          or game_state.get_gameBoard(row, col) == Empty) {
          return false;
      }
    }
  }

  return true;
}