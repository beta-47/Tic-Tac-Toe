#include <iostream>
#include <limits>
#include <chrono>
#include <thread>
#include <vector>
#define IMAX std::numeric_limits<int>::max()
#define IMIN std::numeric_limits<int>::min()
#define X 1
#define O -1
#define EMPTY 0
#define VICTORY_SCORE 1
#define DRAW_SCORE 0
#define PLAYER 0
#define COMPUTER 1
using namespace std;

struct State
{
    int player_choice;
    vector< vector<int> > board;
    State()
    {
        board = vector< vector<int> > (3, vector<int>(3));
        for(int i = 0; i < 3; ++i)
            for(int j = 0; j < 3; ++j)
                board[i][j] = EMPTY;
    }
    State(int p, vector< vector<int> > v): player_choice(p), board(v) {}
    bool valid(int i, int j)
    {
    	if(i >= 0 && i <= 2 && j >= 0 && j <= 2 && is_empty_cell(i, j)) return true;
    	else return false;
    }
    void draw_board()
    {
        //cout << "Board:\n";
        cout << " --- --- ---" << endl;
        for(int i = 0; i < 3; ++i)
        {
            cout << "|";
            for(int j = 0; j < 3; ++j)
            {
                if(board[i][j] == X) cout << " X |";
                else if(board[i][j] == O) cout << " O |";
                else cout << "   |";
            }
            cout << endl << " --- --- ---";
            cout << endl;
        }
    }
    bool is_draw()
    {
        for(int i = 0; i < 3; ++i)
            for(int j = 0; j < 3; ++j)
                if(board[i][j] == EMPTY) return false;
        return true;
    }
    bool is_row_victory()
    {
        for(int i = 0; i < 3; ++i)
        {
            int sum = 0;
            for(int j = 0; j < 3; ++j)
                sum += board[i][j];
            if(abs(sum) == 3) return true;
        }
        return false;
    }
    bool is_column_victory()
    {
        for(int i = 0; i < 3; ++i)
        {
            int sum = 0;
            for(int j = 0; j < 3; ++j)
                sum += board[j][i];
            if(abs(sum) == 3) return true;
        }
        return false;
    }
    bool is_diagonal_victory()
    {
        int sum1 = board[0][0] + board[1][1] + board[2][2];
        int sum2 = board[0][2] + board[1][1] + board[2][0];
        return abs(sum1) == 3 || abs(sum2) == 3;
    }
    bool is_winning_position() { return is_row_victory() || is_column_victory() || is_diagonal_victory(); }
    bool is_empty_cell(int i, int j) { return board[i][j] == EMPTY; }
    void mark(int i, int j, int turn) { board[i][j] = turn == PLAYER ? player_choice : -player_choice; }
};

vector<State> generate_possible_moves(State s, int turn)
{
    vector<State> moves;
    for(int i = 0; i < 3; ++i)
    {
        for(int j = 0; j < 3; ++j)
        {
            if(s.is_empty_cell(i, j))
            {
                State move = s;
                move.mark(i, j, turn);
                moves.push_back(move);
            }
        }
    }
    return moves;
}

int score(int turn, State s, int depth)
{
    int ans = turn == PLAYER ? IMAX : IMIN;
    if(s.is_winning_position()) return turn == COMPUTER ? -10 + depth : 10 - depth; //10 because the depth from a given state can at most be 9 (only 8 more cells to be filled in after the first has been filled)
    else if(s.is_draw()) return 0;
    else
    {
        vector<State> moves = generate_possible_moves(s, turn);
        for(State move: moves)
        {
            if(turn == PLAYER) ans = min(ans, score(!turn, move, depth + 1));
            else ans = max(ans, score(!turn, move, depth + 1));
        }
    }
    return ans;
}

State intialize_player_board()
{
    char choice;
    cout << "\n\nYour choice of piece (X or O): ";
    cin >> choice;
    int i, j, player_choice = choice == 'X' ? X : O;
    cout << "Select a position to place your piece: ";
    cin >> i >> j;
    State start;
    start.player_choice = player_choice;
    start.mark(i - 1, j - 1, PLAYER);
    return start;
}

void player_move(State &s)
{
    int i, j;
    cout << "\nYour turn: ";
    cin >> i >> j;
    while(!s.valid(i - 1, j - 1))
    {
    	cout << "\nInvalid Position. Please try again.\n";
    }
    
    s.mark(i - 1, j - 1, PLAYER);
}

void computer_move(State &s)
{
	int pause;
	cout << "\nComputer's turn: \n\n";
	this_thread::sleep_for(1s);
    vector<State> moves = generate_possible_moves(s, COMPUTER);
    int max_score = IMIN;
    for(State move: moves)
    {
        int move_score = score(PLAYER, move, 0);
        if(move_score > max_score)
        {
            max_score = move_score;
            s = move;
        }
    }
}

void play()
{
    State s;
    s.draw_board();
    s = intialize_player_board();
    s.draw_board();
    int turn = COMPUTER;
    while(1)
    {
        if(s.is_winning_position()) { if(turn == PLAYER) cout << "You Lose!\n"; else cout << "You win!\n\n\n"; break; }
        else if(s.is_draw()) { cout << "It's a draw!\n\n\n"; break; }
        if(turn == COMPUTER) computer_move(s);
        else player_move(s);
        turn = !turn;
        s.draw_board();
    }
}

int main()
{
	cout << "!!!WELCOME TO TIC_TAC_TOE!!!\n\n\n";
	cout << "You get to make the first move!\n\nHere's the board:\n\n\n";
    play();
    return 0;
}