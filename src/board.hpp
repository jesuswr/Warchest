#ifndef _BOARD_HPP
#define _BOARD_HPP

#include "../tests/test_board.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <map>

using namespace std;

const string PLAYER_NAME[2] = {"Wolf", "Crow"};

// Enum to represent the different tokens of the game
enum token
{
    Archer = 0,
    Crossbowman = 1,
    Knight = 2,
    Mercenary = 3,
    Royal = 4,
    Control = 5
};

// Tokens names to print
const string TOKEN_NAME[6] = {"Archer", "Crossbowman", "Knight", "Mercenary", "Royal", "Control"};

// type to represent a position in the board and a token owned by a player (player 0 and 1)
typedef pair<int, int> position;
typedef pair<int, int> order;
typedef pair<token, int> token_with_player;

class board
{
private:
    int current_player = 0;
    // vectors to keep player order in current and next turn, this way
    // take initiative is just a reverse on next order
    vector<int> players_order = {0, 1}, next_players_order = {0, 1};
    // Map from position to a vector of elements, to store things in the board
    map<position, vector<token_with_player>> board_map;
    vector<token> bag[2], hand[2], recruitment[2], discard[2];
    // number of control tokens of each player
    int control_tokens[2] = {3, 3};
    // vector of the positions of the control zones
    vector<position> control_zones = {{0, 0}, {1, 2}, {2, 1}, {2, 3}, {3, 2}, {4, 4}};
    // methods to print the board and status of the game
    string print_board();
    string print_game_status();

    // methods to do the different actions of the game
    void place(position p, token t);
    void control(position p, token t);
    void move(position p, position new_p, token t);
    void recruit(token t_hand, token t_rec);
    void attack(position p, token t, position rival_p, token rival_t);
    void initiative(token k);

    // method to check if a player won
    bool won(int player);

    // helper methods
    bool is_token_from_player_in_position(position p, int a, token t);
    bool adjacent_control_token_exists(position p, int player);
    bool player_has_token_in_position(position p, int player);
    bool is_control_zone(position p);
    void erase_token_from_map(position p, int player, token t);
    void get_hand(int player);
    bool cant_play(int p);

    // method to play a turn
    void play_turn();

public:
    // constructor to initialize the structure of the board
    board();

    // method to play a game
    void play();

    friend test_board;
};

#endif