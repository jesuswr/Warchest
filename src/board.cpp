#include <vector>
#include <string>
#include <map>
#include "board.hpp"

using namespace std;


board::board() {

}


string board::get_current_player() {
    return "";
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

