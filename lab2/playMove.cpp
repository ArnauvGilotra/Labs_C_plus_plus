//
//  playMove.cpp
//  TicTacToe
//
//  Created by Tarek Abdelrahman on 2019-06-07.
//  Modified by Tarek Abdelrahman on 2020-09-17.
//  Copyright © 2019-2020 Tarek Abdelrahman. All rights reserved.
//
//  Permission is hereby granted to use this code in ECE244 at
//  the University of Toronto. It is prohibited to distribute
//  this code, either publicly or to third parties.
//
//  ECE244 Student: write your code for playMove in this file
#include <iostream>
#include <string>
using namespace std;
#include "globals.h"
#include "GameState.h"
void setWinCode (GameState& game_state, int counter);
int checkWinCode (GameState& game_state);
int validMoveCounter = 0;

void playMove(GameState& game_state) {
    int row = game_state.get_selectedRow();
    int col = game_state.get_selectedColumn();
    int position = game_state.get_gameBoard(row, col);
    
    //checking move validity
    if (position == Empty) {
        validMoveCounter = validMoveCounter + 1;
        game_state.set_moveValid(true);
    } 
    else {
        game_state.set_moveValid(false);
    }

    if (game_state.get_moveValid()) { //if move is valid; change the cell ownership to appropriate player 
        switch (game_state.get_turn()) {
            case (true):
                game_state.set_gameBoard(row, col, X);
                break;
            case (false):
                game_state.set_gameBoard(row, col, O);
                break;
        }
        //Switch Player
        game_state.set_turn(game_state.get_turn() ? false : true);
        setWinCode(game_state, validMoveCounter);
    }
}

void setWinCode (GameState& game_state, int counter){
    //bool anyMoveLeft = false;
    int winCode = checkWinCode(game_state);
    if (winCode != 0){
        game_state.set_gameOver(true);
    } else if (counter > 8){
        game_state.set_gameOver(true);
    } else  {
        game_state.set_gameOver(false);
    }
    
}

int checkWinCode (GameState& game_state){
//Start Checking if someone won
    int cell1, cell2, cell3;
    int winCode = 0;
    //HORTIZONTAL - either of the 3 rows
    for (int i = 0; i < 3; i++) {
        cell1 = game_state.get_gameBoard(i, 0);
        cell2 = game_state.get_gameBoard(i, 1);
        cell3 = game_state.get_gameBoard(i, 2);
            
        if (cell1 != Empty && cell1 == cell2 && cell2 == cell3) {
            //game_state.set_gameOver(true);
            game_state.set_winCode(i + 1);
        }
    }
        //VERTICALLY - either of the 3 cols
    for (int i = 0; i < 3; i++) {
        cell1 = game_state.get_gameBoard(0, i);
        cell2 = game_state.get_gameBoard(1, i);
        cell3 = game_state.get_gameBoard(2, i);
            
        if (cell1 != Empty && cell1 == cell2 && cell2 == cell3) {
            //game_state.set_gameOver(true);
            game_state.set_winCode(i + 4);
        }
    }

    //DIAGONAL topleft to bottomright
    cell1 = game_state.get_gameBoard(0, 0);
    cell2 = game_state.get_gameBoard(1, 1);
    cell3 = game_state.get_gameBoard(2, 2);
        
    if (cell1 != Empty && cell1 == cell2 && cell2 == cell3) {
        //game_state.set_gameOver(true);
        game_state.set_winCode(7);
    }

    //DIAGONAL bottomleft to topright
    cell1 = game_state.get_gameBoard(2, 0);
    cell2 = game_state.get_gameBoard(1, 1);
    cell3 = game_state.get_gameBoard(0, 2);
        
    if (cell1 != Empty && cell1 == cell2 && cell2 == cell3) {
        //game_state.set_gameOver(true);
        game_state.set_winCode(8);
    }

    winCode = game_state.get_winCode();
    return winCode;
}