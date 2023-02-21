// Pawns in console
//  - Author: Tomash Mikulevich
//  - Visual Studio Code 1.51.1 (user setup)  |  g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0  |  GNU gdb (GDB) 8.1
//  - Last edited: 24.12.2020

#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>  

using namespace std;

const int MAX_PAWNS = 10;   

struct players {
    int player_1 = 0;
    int player_2 = 0;
};

// Initialization and preparation
void game(players& score);
void gen_board(int numb_of_rows, string* board);
void gen_pawns(int rand_pawns[MAX_PAWNS], int& numb_of_pawns, int i);

// Implementation
void move(string* board, int numb_of_rows, bool& defeat, players& score);
void edit_board(string* board, int numb_of_rows);
bool check_defeat(string* board, bool& defeat, int numb_of_rows);
void print_board(int numb_of_rows, string* board);

// Additional options
void print_points(players score);
void rules();


int main() {
    players score;      
    int option;

    cout << "Welcome to the board game for two players!" << endl;

    do {
        cout << "-----------------------" << endl;
        cout << "Available options: " << endl;
        cout << " 1 - start a new game \n"
             << " 2 - show score \n"
             << " 3 - game rules \n"
             << " 0 - exit" << endl;
        cout << "Choice: "; cin >> option;
        cout << "-----------------------" << endl;

        switch (option) {
        case 1: { game(score); break; }

        case 2: { print_points(score); break; }

        case 3: { rules(); break; }

        default: { cout << "Exit..." << endl; option = 0; }
        }
    } while (option);

    return 0;
}

// Initialization
void game(players& score) {
    static int game_number = 1;
    cout << "Game #" << game_number++ << endl;

    int numb_of_rows;
    cout << "Specify the number of rows in the range [4, " << MAX_PAWNS << "]: ";
    cin >> numb_of_rows;

    while (numb_of_rows < 4 || numb_of_rows > MAX_PAWNS) {   
        cout << "!!! Wrong data. Try again: ";
        cin >> numb_of_rows;
    }

    string* board = new string[numb_of_rows];             

    gen_board(numb_of_rows, board);

    bool defeat = false;
    while (!defeat) {                                       
        print_board(numb_of_rows, board);
        move(board, numb_of_rows, defeat, score);
    }

    delete[] board;                                        
}

// Board generation
void gen_board(int numb_of_rows, string* board) {
    srand(time(NULL));

    int rand_pawns[MAX_PAWNS];                             
    int numb_of_pawns;

    for (int i = 0; i < numb_of_rows; i++) {
        gen_pawns(rand_pawns, numb_of_pawns, i);
        string raw(numb_of_pawns, 'O');                     
        board[i] = raw;                                     
    }
}

// Generation of unique pawn numbers
void gen_pawns(int rand_pawns[MAX_PAWNS], int& numb_of_pawns, int i) {
    bool no_repeat;

    do {
        no_repeat = true;
        numb_of_pawns = rand() % MAX_PAWNS + 1;               
        for (int k = 0; k <= i; k++)                        
            if (rand_pawns[k] == numb_of_pawns)
                no_repeat = false;
    } while (!no_repeat);                                   

    rand_pawns[i] = numb_of_pawns;                          
}

// Movement of a player
void move(string* board, int numb_of_rows, bool& defeat, players& score) {
    static int move_number = 1;                                     

    cout << "Movement #" << move_number << ". ";
    cout << "Player " << 2 - move_number % 2 << ". Your choice: ";      

    edit_board(board, numb_of_rows);

    if (check_defeat(board, defeat, numb_of_rows)) {               
        cout << "Player " << 2 - move_number % 2 << "  took the last pawn (pawns) and loses! "
             << "Game over. " << endl;

        if (move_number % 2)                                          
            score.player_2++;
        else
            score.player_1++;

        move_number = 1;
    }
    else
        move_number++;
}

// Editing the board
void edit_board(string* board, int numb_of_rows) {
    bool error = true;

    while (error) {                                                  
        error = false;

        string action;                                              
        cin >> action;

        int action_raw = 0;                                                     // Part 1 of movement: row number
        for (int i = 0; i < action.length() - 1; i++) {
            if (i >= 1)                                             
                action_raw = 10 * action_raw + action[i] - '0';        
            else
                action_raw = action[i] - '0';                      
        }

        if (action_raw > numb_of_rows || board[action_raw - 1].empty()) {
            cout << "!!! It seems you made a mistake (most likely the row about the given number does not exist or is already empty). Try again: ";
            error = true;
            continue;
        }

        int action_effect = action.length() - 1;                                // Part 2 of movement: action (what to do with pawns)
        if (action[action_effect] == 'a' || action[action_effect] == 'A')
            board[action_raw - 1].clear();                                      // Remove all pawns from the row
        else if (action[action_effect] == 'o' || action[action_effect] == 'O')
            board[action_raw - 1].erase(0, 1);                                  // Remove one pawn from the row
        else {
            cout << "!!! It seems you made a mistake (most likely you gave the wrong action letter). It must be a ' a 'or a 'o'. Try again: ";
            error = true;
        }
    }
}

// Check if a player has lost this game
bool check_defeat(string* board, bool& defeat, int numb_of_rows) {
    defeat = true;
    for (int i = 0; i < numb_of_rows; i++)
        if (!board[i].empty())             
            defeat = false;

    return(defeat);
}

// Drawing the board
void print_board(int numb_of_rows, string* board) {
    for (int i = 0; i < numb_of_rows; i++) {
        cout << setw(3) << i + 1 << " |";
        cout << board[i] << endl;
    }
}

// Printing player points
void print_points(players score) {
    cout << "Score: " << endl;
    cout << "Player 1: " << score.player_1 << endl;
    cout << "Player 2: " << score.player_2 << endl;
}

// Game rules
void rules() {
    cout << "There are pawns on the board in at least 4 rows (players choose this number). Players take turns making moves."
         << "The move consists of taking a single pawn or all the pawn pieces from one row (players give the row number and the letters 'o' (one pawn) or 'a' (all pawns) - e.g. 1a, 7o)."
         << "The player who takes the last pawn loses." << endl;
}
