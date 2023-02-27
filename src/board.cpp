#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include "board.hpp"

using namespace std;

// init bag, recruitment and control
board::board() {
    // assign initial control tokens to corners of board
    board_map[{0, 0}].push_back({Control, 0});
    board_map[{4, 4}].push_back({Control, 1});

    // shuffle units
    auto player_units = assign_units();

    // assing units to the players bag and recruitment
    for(int p = 0; p < 2; ++p) {
        for(auto [n_unit, unit] : player_units[p]) {
            // assign two of the units to the bag and the rest to the recruitment
            for(int i = 0; i < 2; ++i) bag[p].push_back(unit);
            for(int i = 0; i < n_unit - 2; ++i) recruitment[p].push_back(unit);
        }
        bag[p].push_back(Royal);
    }
}

string board::get_current_player() {
    return PLAYER_NAME[current_player];
}


string board::print_board() {
    return "";
}


string board::print_game_status() {
    return "";
}


void board::place(position p, token t) {

}


void board::control(position p, token t) {

}


void board::move(position p, token t) {

}


void board::recruit(token k) {

}


void board::attack(position p, token t, token rival_t) {

}


void board::initiative(token k) {

}


bool board::won(int player) {
    return true;
}


bool board::lost(int player) {
    return false;
}


vector<vector<pair<int, token>>, vector<pair<int, token>>> assign_units() {
    vector<pair<int, token>> units = {{4, Archer}, {5, Crossbowman}, {5, Knight}, {5, Mercenary}};
    random_shuffle(units.begin(), units.end());
    return {{units[0], units[1]}, {units[2], units[3]}};
}