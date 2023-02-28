#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <time.h>
#include <random>
#include "board.hpp"

using namespace std;

bool is_position_inside(position p)
{
    return (max(p.first, p.second) <= 4 || min(p.first, p.second) >= 0);
}

bool are_positions_adjacent(position a, position b)
{
    return (a.first == b.first && abs(a.second - b.second) == 1) || (a.second == b.second && abs(a.first - b.first) == 1);
}

position ask_for_position()
{
    position p;
    cout << "Enter two numbers representig row and column" << endl;
    cin >> p.first >> p.second;
    if (!cin.good())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw string("Wrong position input.");
    }
    return p;
}

token ask_for_token()
{
    string s;
    cout << "Enter one of Archer, Crossbowman, Knight, Mercenary or Royal" << endl;
    cin >> s;
    if (s == "Archer")
        return Archer;
    if (s == "Crossbowman")
        return Crossbowman;
    if (s == "Knight")
        return Knight;
    if (s == "Mercenary")
        return Mercenary;
    if (s == "Royal")
        return Royal;
    throw string("Wrong input unit.");
}

vector<position> get_adjacent_positions(position p)
{
    vector<position> ret;
    vector<pair<int, int>> moves = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    for (auto [dr, dc] : moves)
    {
        position new_p = {p.first + dr, p.second + dc};
        if (is_position_inside(new_p))
            ret.push_back(new_p);
    }
    return ret;
}

bool token_in(const vector<token> &tokens, token t)
{
    for (token k : tokens)
    {
        if (k == t)
            return true;
    }
    return false;
}

void erase_token_from(vector<token> &tokens, token t)
{
    for (auto it = tokens.begin(); it != tokens.end(); ++it)
    {
        if (*it == t)
        {
            tokens.erase(it);
            return;
        }
    }
}

vector<vector<pair<int, token>>> assign_units()
{
    // randomize the units, first two to a player and the other to the other player
    vector<pair<int, token>> units = {{4, Archer}, {5, Crossbowman}, {5, Knight}, {5, Mercenary}};
    random_shuffle(units.begin(), units.end());
    return {{units[0], units[1]}, {units[2], units[3]}};
}

string get_player_name(int player)
{
    return PLAYER_NAME[player];
}

string get_token_name(token tk)
{
    return TOKEN_NAME[tk];
}

// init bag, recruitment and control
board::board()
{
    // init random seed for random shuffles later
    std::srand ( unsigned ( time(0) ) );

    // assign initial control tokens to corners of board
    board_map[{0, 0}].push_back({Control, 0});
    board_map[{4, 4}].push_back({Control, 1});

    // shuffle units
    auto player_units = assign_units();

    // assing units to the players bag and recruitment
    for (int p = 0; p < 2; ++p)
    {
        for (auto [n_unit, unit] : player_units[p])
        {
            // assign two of the units to the bag and the rest to the recruitment
            for (int i = 0; i < 2; ++i)
                bag[p].push_back(unit);
            for (int i = 0; i < n_unit - 2; ++i)
                recruitment[p].push_back(unit);
        }
        bag[p].push_back(Royal);
    }
}

bool board::is_control_zone(position p)
{
    for (position pz : control_zones)
    {
        if (p == pz)
            return true;
    }
    return false;
}

string board::print_board()
{
    string ret;
    ret += "  | 0  1  2  3  4 \n";
    ret += "------------------\n";
    for (int row = 0; row < 5; ++row)
    {
        string row_str;
        row_str += to_string(row) + " |";
        for (int col = 0; col < 5; ++col)
        {
            // first look for a control token, if found add it, else add a dot
            string player_name;
            for (auto [tk, ply] : board_map[{row, col}])
            {
                if (tk == Control)
                    player_name = get_player_name(ply);
            }

            row_str += " ";
            if (!player_name.empty())
                row_str += player_name[0];
            else if (is_control_zone({row, col}))
                row_str += 'X';
            else
                row_str += ".";
            row_str += " ";
        }
        row_str += "\n";
        ret += row_str;
    }
    return ret;
}

string board::print_game_status()
{
    string ret;
    ret += "CURRENT PLAYER: " + get_player_name(current_player) + "\n";
    ret += "IN HAND:";
    for (token x : hand[current_player])
        ret += "  " + get_token_name(x);
    ret += "\nIN BAG:";
    for (token x : bag[current_player])
        ret += "  " + get_token_name(x);
    ret += "\nIN RECRUITMENT:";
    for (token x : recruitment[current_player])
        ret += "  " + get_token_name(x);
    ret += "\nIN DISCARD:";
    for (token x : discard[current_player])
        ret += "  " + get_token_name(x);

    for (auto &[pos, tokens_in_pos] : board_map)
    {
        if (!tokens_in_pos.empty())
        {
            ret += "\nUnits/Token in position (" + to_string(pos.first) + ", " + to_string(pos.second) + "): ";
            for (auto [tk, player] : tokens_in_pos)
            {
                ret += get_token_name(tk) + "(" + get_player_name(player) + ") ";
            }
        }
    }
    ret += "\n";
    return ret;
}

bool board::is_token_from_player_in_position(position p, int player, token t)
{
    for (auto &[tok, player] : board_map[p])
    {
        if (tok == t && player == player)
            return true;
    }
    return false;
}

bool board::player_has_token_in_position(position p)
{
    for (auto &[_, player] : board_map[p])
    {
        if (player == player)
            return true;
    }
    return false;
}

bool board::adjacent_control_token_exists(position p, int player)
{
    for (position adj_p : get_adjacent_positions(p))
    {
        if (is_token_from_player_in_position(adj_p, player, Control))
            return true;
    }
    return false;
}

void board::erase_token_from_map(position p, int player, token t)
{
    for (auto it = board_map[p].begin(); it != board_map[p].end(); ++it)
    {
        if (it->first == t && it->second == player)
        {
            board_map[p].erase(it);
            return;
        }
    }
}

void board::place(position p, token t)
{
    if (!is_position_inside(p))
        throw string("Position out of board.");
    if (!adjacent_control_token_exists(p, current_player))
        throw string("Position not adjacent to a controlled position.");
    if (!token_in(hand[current_player], t))
        throw string("The token doesnt exist in the hand.");
    if (t == Royal)
        throw string("Can't place a Royal.");

    erase_token_from(hand[current_player], t);
    board_map[p].push_back({t, current_player});
}

void board::control(position p, token t)
{
    if (!is_position_inside(p))
        throw string("Position out of board.");
    if (!token_in(hand[current_player], t))
        throw string("The token doesnt exist in the hand.");
    if (is_token_from_player_in_position(p, current_player, Control))
        throw string("This position is already under control.");
    if (!player_has_token_in_position(p))
        throw string("No token in the given position.");
    if (!is_control_zone(p))
        throw string("This isn't a capture zone.");

    erase_token_from(hand[current_player], t);
    discard[current_player].push_back(t);
    board_map[p].push_back({Control, current_player});
    control_tokens[current_player]--;
    if (is_token_from_player_in_position(p, 1 - current_player, Control))
    {
        control_tokens[1 - current_player]++;
        erase_token_from_map(p, 1 - current_player, Control);
    }
}

void board::move(position p, position new_p, token t)
{
    if (!is_position_inside(p) || !is_position_inside(new_p))
        throw string("Position out of board.");
    if (!token_in(hand[current_player], t))
        throw string("The token doesnt exist in the hand.");
    if (!is_token_from_player_in_position(p, current_player, t))
        throw string("The token doesnt exist in the position..");
    if (!are_positions_adjacent(p, new_p))
        throw string("These positions aren't adjacent.");

    erase_token_from(hand[current_player], t);
    discard[current_player].push_back(t);
    erase_token_from_map(p, current_player, t);
    board_map[new_p].push_back({t, current_player});
}

void board::recruit(token t_hand, token t_rec)
{
    if (!token_in(hand[current_player], t_hand))
        throw string("The token doesnt exist in the hand.");
    if (!token_in(recruitment[current_player], t_rec))
        throw string("The token doesnt exist in the recruitment.");
    if (t_hand != t_rec && t_hand != Royal)
        throw string("Can't recruit if you don't discard the same unit or a Royal.");

    erase_token_from(hand[current_player], t_hand);
    discard[current_player].push_back(t_hand);
    erase_token_from(recruitment[current_player], t_rec);
    bag[current_player].push_back(t_rec);
}

void board::attack(position p, token t, position rival_p, token rival_t)
{
    if (!token_in(hand[current_player], t))
        throw string("The token doesnt exist in the hand.");
    if (!is_token_from_player_in_position(p, current_player, t))
        throw string("The token doesnt exist in the position.");
    if (!is_token_from_player_in_position(rival_p, 1 - current_player, rival_t))
        throw string("The rival token doesnt exist in the position.");

    if (t == Archer)
    {
        if (max(abs(p.first - rival_p.first), abs(p.second - rival_p.second)) > 2)
            throw string("Can't reach that far.");
    }
    else if (t == Crossbowman)
    {
        if (p.first != rival_p.first && p.second != rival_p.second)
            throw string("Can't attack diagonally.");
        if (abs(p.first - rival_p.first) + abs(p.second - rival_p.second) > 2)
            throw string("Can't reach that far.");
    }
    else if (t == Knight)
    {
        if (p.first != rival_p.first && p.second != rival_p.second)
            throw string("Can't attack diagonally.");
        if (abs(p.first - rival_p.first) + abs(p.second - rival_p.second) > 1)
            throw string("Can't reach that far.");
    }
    else if (t == Mercenary)
    {
        if (p.first != rival_p.first && p.second != rival_p.second)
            throw string("Can't attack diagonally.");
        if (abs(p.first - rival_p.first) + abs(p.second - rival_p.second) > 1)
            throw string("Can't reach that far.");
    }

    erase_token_from(hand[current_player], t);
    discard[current_player].push_back(t);
    erase_token_from_map(rival_p, 1 - current_player, rival_t);
}

void board::initiative(token t)
{
    if (!token_in(hand[current_player], t))
        throw string("The token doesnt exist in the hand.");
    if (next_players_order[0] == current_player)
        throw string("Already have the initiave.");

    erase_token_from(hand[current_player], t);
    discard[current_player].push_back(t);
    swap(next_players_order[0], next_players_order[1]);
}

void board::get_hand(int p)
{
    random_shuffle(bag[p].begin(), bag[p].end());
    while (hand[p].size() < 3 && !bag[p].empty())
    {
        hand[p].push_back(bag[p].back());
        bag[p].pop_back();
    }

    if (hand[p].size() < 3)
    {
        bag[p] = discard[p];
        discard[p].clear();
        random_shuffle(bag[p].begin(), bag[p].end());
        while (hand[p].size() < 3 && !bag[p].empty())
        {
            hand[p].push_back(bag[p].back());
            bag[p].pop_back();
        }
    }
}

bool board::won(int player)
{
    return control_tokens[player] == 0;
}

void board::play_turn()
{
    cout << "=================================================" << endl;
    cout << print_board() << print_game_status() << endl;
    cout << "Enter Place, Control, Move, Recruit, Attack, Initiative or Pass" << endl;

    try
    {
        string command;
        cin >> command;
        if (command == "Place")
        {
            cout << "Enter the position and unit" << endl;
            position p = ask_for_position();
            token t = ask_for_token();
            place(p, t);
        }
        else if (command == "Control")
        {
            cout << "Enter the position and unit" << endl;
            position p = ask_for_position();
            token t = ask_for_token();
            control(p, t);
        }
        else if (command == "Move")
        {
            cout << "Enter the old position, the unit and the new position" << endl;
            position p = ask_for_position();
            token t = ask_for_token();
            position new_p = ask_for_position();
            move(p, new_p, t);
        }
        else if (command == "Recruit")
        {
            cout << "Enter the unit you want to discard and the one you want to recruit" << endl;
            token t = ask_for_token();
            token k = ask_for_token();
            recruit(t, k);
        }
        else if (command == "Attack")
        {
            cout << "Enter your position and unit, then the opponent position and unit" << endl;
            position p = ask_for_position();
            token t = ask_for_token();
            position op_p = ask_for_position();
            token op_t = ask_for_token();
            attack(p, t, op_p, op_t);
        }
        else if (command == "Initiative")
        {
            cout << "Enter the unit" << endl;
            token t = ask_for_token();
            initiative(t);
        }
        else if (command == "Pass") {
            for(token tk : hand[current_player])
                discard[current_player].push_back(tk);
            hand[current_player].clear();    
        }
        else
        {
            cout << "Wrong input." << endl;
        }
    }
    catch (string s)
    {
        cout << s << endl;
    }

    cout << endl;
}


bool board::cant_play(int p) {
    if (recruitment[p].size() > 0)
        return false;
    vector<token> aux;
    for(token t : bag[p])
        aux.push_back(t);
    for(token t : discard[p])
        aux.push_back(t);

    // if the only card remaining is Royal and there is nothing on the recruitment, no possible move remains
    if (aux.size() == 1 && aux[0] == Royal)
        return true;
    return false;
}


void board::play()
{
    while (true)
    {
        for (int _i = 0; _i < 2; _i++)
        {
            if (cant_play(0) && cant_play(1)) {
                cout << "No player can make a move, it's a tie" << endl;
                return;
            }

            current_player = players_order[_i];
            get_hand(current_player);
            while (!hand[current_player].empty()) {
                play_turn();
                if (won(current_player)) {
                    cout << "Player " << get_player_name(current_player) << " won!" << endl;
                    return;
                }
            }
        }
        players_order = next_players_order;
    }
}
