#include <iostream>
#include <iomanip>
#include <limits>
using namespace std;

const int BOARD_SIZE = 3;
const char PLAYER_X = 'X';
const char PLAYER_O = 'O';
const char EMPTY = ' ';

// Game board
char board[BOARD_SIZE][BOARD_SIZE];

// Function prototypes
void initializeBoard();
void displayBoard();
bool makeMove(int row, int col, char player);
bool checkWin(char player);
bool checkDraw();
void clearScreen();
bool playAgain();
void displayWelcome();
void displayGameRules();

// Initialize the game board with empty spaces
void initializeBoard() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = EMPTY;
        }
    }
}

// Clear screen (works on most terminals)
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Display welcome message
void displayWelcome() {
    cout << "\n========================================" << endl;
    cout << "       TIC-TAC-TOE GAME                 " << endl;
    cout << "========================================" << endl;
    cout << "        Two Player Edition              " << endl;
    cout << "========================================\n" << endl;
}

// Display game rules
void displayGameRules() {
    cout << "GAME RULES:" << endl;
    cout << "------------" << endl;
    cout << "1. The game is played on a 3x3 grid" << endl;
    cout << "2. Player 1 is X, Player 2 is O" << endl;
    cout << "3. Players take turns placing their mark" << endl;
    cout << "4. First to get 3 in a row wins!" << endl;
    cout << "5. Rows are numbered 1-3, Columns are numbered 1-3" << endl;
    cout << "\nPress Enter to start...";
    cin.ignore();
    cin.get();
}

// Display the game board
void displayBoard() {
    cout << "\n     1   2   3  " << endl;
    cout << "   -------------" << endl;
    
    for (int i = 0; i < BOARD_SIZE; i++) {
        cout << " " << (i + 1) << " |";
        for (int j = 0; j < BOARD_SIZE; j++) {
            cout << " " << board[i][j] << " ";
            if (j < BOARD_SIZE - 1) {
                cout << "|";
            }
        }
        cout << "|" << endl;
        
        if (i < BOARD_SIZE - 1) {
            cout << "   |---|---|---|" << endl;
        }
    }
    cout << "   -------------" << endl;
}

// Make a move on the board
bool makeMove(int row, int col, char player) {
    // Validate input
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        cout << "Error: Invalid position! Row and column must be 1-3." << endl;
        return false;
    }
    
    // Check if position is already occupied
    if (board[row][col] != EMPTY) {
        cout << "Error: Position already occupied! Choose another spot." << endl;
        return false;
    }
    
    // Place the move
    board[row][col] = player;
    return true;
}

// Check if a player has won
bool checkWin(char player) {
    // Check rows
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player) {
            return true;
        }
    }
    
    // Check columns
    for (int j = 0; j < BOARD_SIZE; j++) {
        if (board[0][j] == player && board[1][j] == player && board[2][j] == player) {
            return true;
        }
    }
    
    // Check diagonal (top-left to bottom-right)
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player) {
        return true;
    }
    
    // Check diagonal (top-right to bottom-left)
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player) {
        return true;
    }
    
    return false;
}

// Check if the game is a draw
bool checkDraw() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == EMPTY) {
                return false; // Found an empty space, not a draw
            }
        }
    }
    return true; // No empty spaces and no winner = draw
}

// Ask if players want to play again
bool playAgain() {
    char choice;
    cout << "\nPlay again? (y/n): ";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return (choice == 'y' || choice == 'Y');
}

// Main game loop
void playGame() {
    int row, col;
    char currentPlayer = PLAYER_X;
    int playerNumber = 1;
    int moveCount = 0;
    bool gameOver = false;
    
    initializeBoard();
    
    while (!gameOver) {
        clearScreen();
        displayWelcome();
        displayBoard();
        
        // Display current player
        cout << "\nPlayer " << playerNumber << "'s turn (" << currentPlayer << ")" << endl;
        cout << "Enter row (1-3): ";
        
        // Input validation
        if (!(cin >> row)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Invalid input! Please enter a number." << endl;
            cout << "Press Enter to continue...";
            cin.get();
            continue;
        }
        
        cout << "Enter column (1-3): ";
        if (!(cin >> col)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Invalid input! Please enter a number." << endl;
            cout << "Press Enter to continue...";
            cin.get();
            continue;
        }
        
        // Convert to 0-indexed
        row--;
        col--;
        
        // Try to make the move
        if (makeMove(row, col, currentPlayer)) {
            moveCount++;
            
            // Check for win
            if (checkWin(currentPlayer)) {
                clearScreen();
                displayWelcome();
                displayBoard();
                cout << "\n========================================" << endl;
                cout << "   CONGRATULATIONS! Player " << playerNumber << " (" << currentPlayer << ") WINS!" << endl;
                cout << "========================================" << endl;
                gameOver = true;
            }
            // Check for draw
            else if (checkDraw()) {
                clearScreen();
                displayWelcome();
                displayBoard();
                cout << "\n========================================" << endl;
                cout << "          IT'S A DRAW!                  " << endl;
                cout << "========================================" << endl;
                gameOver = true;
            }
            // Switch players
            else {
                currentPlayer = (currentPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;
                playerNumber = (playerNumber == 1) ? 2 : 1;
            }
        } else {
            cout << "Press Enter to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    }
    
    cout << "\nTotal moves: " << moveCount << endl;
}

int main() {
    displayWelcome();
    displayGameRules();
    
    int gamesPlayed = 0;
    
    do {
        gamesPlayed++;
        cout << "\n--- Game #" << gamesPlayed << " ---" << endl;
        playGame();
    } while (playAgain());
    
    cout << "\n========================================" << endl;
    cout << "   Thanks for playing Tic-Tac-Toe!     " << endl;
    cout << "   Total games played: " << gamesPlayed << "          " << endl;
    cout << "========================================" << endl;
    
    return 0;
}