#include "Board.h"
#include "Rook.h"
#include "Pawn.h"
#include "Bishop.h"
#include "Knight.h"
#include "Queen.h"
#include "King.h"
#include "Promotion.h"
#include <iostream>
#include <Windows.h>
using namespace std;

Board::Board() {
    // Initialize the board with nullptrs
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            BoardArray[i][j] = nullptr;
        }
    }

    // Create white pawns and black pawns
    for (int i = 0; i < 8; i++) {
        BoardArray[1][i] = new Pawn('B');  // Black pawns
        BoardArray[6][i] = new Pawn('W');  // White pawns
    }
    BoardArray[0][0] = new Rook('B');
    BoardArray[0][7] = new Rook('B');
    BoardArray[7][0] = new Rook('W');
    BoardArray[7][7] = new Rook('W');

    BoardArray[0][2] = new Bishop('B');
    BoardArray[0][5] = new Bishop('B');
    BoardArray[7][2] = new Bishop('W');
    BoardArray[7][5] = new Bishop('W');

    BoardArray[0][1] = new Knight('B');
    BoardArray[0][6] = new Knight('B');
    BoardArray[7][1] = new Knight('W');
    BoardArray[7][6] = new Knight('W');

    BoardArray[0][3] = new Queen('B');
    BoardArray[7][3] = new Queen('W');

    BoardArray[0][4] = new King('B');
    BoardArray[7][4] = new King('W');
}

Board::~Board() {
    // Clean up dynamically allocated pieces
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (BoardArray[i][j] != nullptr) {
                delete BoardArray[i][j];
            }
        }
    }
}

void Board::display() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int c = 0;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (c % 2 == 0) {
               SetConsoleTextAttribute(hConsole, 144);  // Light square 
                if (BoardArray[i][j] != nullptr) {
                    if (BoardArray[i][j]->GetColor() == 'W') {
                        SetConsoleTextAttribute(hConsole, 159);
                    }
                    if (BoardArray[i][j]->GetColor() == 'B') {
                        SetConsoleTextAttribute(hConsole, 144);
                    }
                }
            }
            else {
                SetConsoleTextAttribute(hConsole, 112);  // Dark square
                if (BoardArray[i][j] != nullptr) {
                    if (BoardArray[i][j]->GetColor() == 'W') {
                        SetConsoleTextAttribute(hConsole, 127);
                    }
                    if (BoardArray[i][j]->GetColor() == 'B') {
                        SetConsoleTextAttribute(hConsole, 112);
                    }
                }
            }

            // Display the piece symbol if a piece exists
            if (BoardArray[i][j] != nullptr) {
                cout << BoardArray[i][j]->GetPiece();
            }
            else {
                cout << " ";  // Empty square
            }

            cout << " ";
            c++;
        }
        SetConsoleTextAttribute(hConsole, 7);
        cout << "  " << 8 - i << endl;  // Show row numbers on the side
        c++;
    }
    cout << endl;

    // Print column labels (A-H)
    for (int i = 65; i < 73; i++) {
        char value = char(i);
        cout << value << " ";
    }
    cout << endl;
}

bool Board::Move(char currentPlayer) {
    char startCol, destCol;
    int startRow, destRow;

    cout << "Enter move (e.g., A2 A3): ";
    cin >> startCol >> startRow >> destCol >> destRow;

    // Convert input to board indices
    int startColIndex = startCol - 'A';
    int startRowIndex = 8 - startRow;
    int destColIndex = destCol - 'A';
    int destRowIndex = 8 - destRow;

    // Check if the starting position is valid
    if (startRowIndex < 0 || startRowIndex >= 8 || startColIndex < 0 || startColIndex >= 8 ||
        destRowIndex < 0 || destRowIndex >= 8 || destColIndex < 0 || destColIndex >= 8) {
        cout << "Invalid move! Out of bounds." << endl;
        return false;
    }

    // Check if there is a piece at the starting position
    if (BoardArray[startRowIndex][startColIndex] == nullptr) {
        cout << "No piece at the starting position!" << endl;
        return false;
    }

    // Get the selected piece
    BasePiece* piece = BoardArray[startRowIndex][startColIndex];

    // Check if the selected piece belongs to the current player
    if (piece->GetColor() != currentPlayer) {
        cout << "It's not your turn to move this piece!" << endl;
        return false;
    }

    // Check if the move is legal for this piece
    if (piece->LegalSquare(startRowIndex, startColIndex, destRowIndex, destColIndex, BoardArray)) {
        // Move the piece to the destination
        delete BoardArray[destRowIndex][destColIndex];  // Capture if a piece is there
        BoardArray[destRowIndex][destColIndex] = piece;
        BoardArray[startRowIndex][startColIndex] = nullptr;

        cout << "Move successful!" << endl;

        if (piece->GetPiece() == 'P' && (destRowIndex == 0 || destRowIndex == 7)) {
            cout << "Pawn can be promoted!" << endl;
            cout << piece->GetColor() << "color" << endl;
            char color = piece->GetColor();
            delete BoardArray[destRowIndex][destColIndex];  // Remove the pawn
            
            BoardArray[destRowIndex][destColIndex] = Promotion::PromotePawn(color);  // Promote
           
            
        }

        return true;
    }

   

    else {
        cout << "Illegal move for this piece!" << endl;
        return false;
    }
}
