#ifndef _BOARD_HPP
#define _BOARD_HPP

#include <vector>
#include <string>
#include <iostream>
#include <map>

using namespace std;

const string PLAYER_NAME[2] = {"Wolf", "Crow"};

// Enum to represent the different tokens of the game
enum token {Archer = 0, Crossbowman = 1, Knight = 2, Mercenary = 3, Royal = 4, Control = 5};

// Tokens names to print
const string TOKEN_NAME[6] = {"Archer", "Crossbowman", "Knight", "Mercenary", "Royal", "Control"};

// type to represent a position in the board and a token owned by a player (player 0 and 1)
typedef pair<int, int> position;
typedef pair<int, int> order;
typedef pair<token, int> token_with_player;


class board {
private:
    int turn = 0, current_player = 0;
    vector<int> players_order = {0, 1}, next_players_order = {0, 1};
    map<position, vector<token_with_player>> board_map;
    vector<token> bag[2], hand[2], recruitment[2], discard[2];
    int control_tokens[2] = {3, 3};
    vector<position> control_zones = {{0, 0}, {1, 3}, {3, 1}, {2, 3}, {3, 2}, {4, 4}};
    string print_board();
    string print_game_status();
    void place(position p, token t);
    void control(position p, token t);
    void move(position p, token t);
    void recruit(token k);
    void attack(position p, token t, token rival_t);
    void initiative(token k);
    bool won(int player);
    bool lost(int player);
    bool is_token_from_player_in_position(position p, int a, token t);
    bool adjacent_control_token_exists(position p, int player);

public:
    board();
    void play();
};

#endif