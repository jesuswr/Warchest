#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#include "test_board.hpp"
#include "catch_amalgamated.hpp"
#include "../src/board.hpp"
#include <vector>
#include <string>

using namespace std;

test_board TB;

TEST_CASE("place tests")
{
    board b;
    REQUIRE_NOTHROW(TB.test_place_correct(&b));
    REQUIRE_NOTHROW(TB.test_place_correct_in_mid(&b));
    REQUIRE_THROWS(TB.test_place_incorrect(&b));
    REQUIRE_THROWS(TB.test_place_royal(&b));
}

TEST_CASE("control tests")
{
    board b;
    REQUIRE_NOTHROW(TB.test_control_correct(&b));
    REQUIRE_THROWS(TB.test_control_incorrect_no_unit(&b));
    REQUIRE_THROWS(TB.test_control_incorrect_not_control_zone(&b));
    REQUIRE_NOTHROW(TB.test_control_take_control_zone_from_rival(&b));
}

TEST_CASE("move tests")
{
    board b;
    REQUIRE_NOTHROW(TB.test_move_correct(&b));
    REQUIRE_THROWS(TB.test_move_incorrect_diagonal(&b));
    REQUIRE_THROWS(TB.test_move_incorrect_too_far(&b));
    REQUIRE_THROWS(TB.test_move_incorrect_no_unit_in_hand(&b));
    REQUIRE_THROWS(TB.test_move_incorrect_no_unit_in_board(&b));
}

TEST_CASE("recruit tests")
{
    board b;
    REQUIRE_NOTHROW(TB.test_recruit_correct(&b));
    REQUIRE_NOTHROW(TB.test_recruit_correct_royal(&b));
    REQUIRE_THROWS(TB.test_recruit_incorrect(&b));

}

void test_board::test_place_correct(board *b)
{
    int player = 0;
    b->current_player = player;
    b->get_hand(player);
    token t;
    if (b->hand[player][0] != Royal)
        t = b->hand[player][0];
    else
        t = b->hand[player][1];

    b->place({1, 0}, t);
}
void test_board::test_place_correct_in_mid(board *b)
{
    int player = 1;
    b->current_player = player;
    b->get_hand(player);
    token t;
    if (b->hand[player][0] != Royal)
        t = b->hand[player][0];
    else
        t = b->hand[player][1];

    // place a control token to be able to place a unit
    b->board_map[{1, 2}].push_back({Control, player});
    b->place({1, 3}, t);
}
void test_board::test_place_incorrect(board *b)
{
    int player = 0;
    b->current_player = player;
    b->get_hand(player);
    token t;
    if (b->hand[player][0] != Royal)
        t = b->hand[player][0];
    else
        t = b->hand[player][1];

    b->place({2, 0}, t);
}
void test_board::test_place_royal(board *b)
{
    int player = 0;
    b->current_player = player;
    b->get_hand(player);
    token t = Royal;
    b->hand[player][0] = t;

    b->place({0, 1}, t);
}

void test_board::test_control_correct(board *b)
{
    int player = 0;
    position p = {1, 2};
    b->current_player = player;
    b->get_hand(player);
    b->hand[player][0] = Archer;
    b->board_map[p].push_back({Archer, player});
    b->control(p, Archer);
}
void test_board::test_control_incorrect_no_unit(board *b)
{
    int player = 0;
    position p = {2, 1};
    b->current_player = player;
    b->get_hand(player);
    b->hand[player][0] = Archer;
    b->control(p, Archer);
}
void test_board::test_control_incorrect_not_control_zone(board *b)
{
    int player = 0;
    position p = {0, 4};
    b->current_player = player;
    b->get_hand(player);
    b->hand[player][0] = Archer;
    b->board_map[p].push_back({Archer, player});
    b->control(p, Archer);
}
void test_board::test_control_take_control_zone_from_rival(board *b)
{
    int player = 0, other_player = 1;
    position p = {4, 4};
    b->current_player = player;
    b->get_hand(player);
    b->hand[player][0] = Archer;
    b->board_map[p].push_back({Archer, player});

    int old_player_free_control_tokens = b->control_tokens[player],
        old_other_player_free_control_tokens = b->control_tokens[other_player];

    b->control(p, Archer);

    int new_player_free_control_tokens = b->control_tokens[player],
        new_other_player_free_control_tokens = b->control_tokens[other_player];

    if (new_player_free_control_tokens + 1 != old_player_free_control_tokens ||
        new_other_player_free_control_tokens - 1 != old_other_player_free_control_tokens)
        throw string("Not updating control tokens correctly");
}

void test_board::test_move_correct(board *b)
{
    int player = 0;
    position p = {0, 2}, new_p = {0, 3};
    b->current_player = player;
    b->hand[player] = {Knight};
    b->board_map[p].push_back({Knight, player});

    b->move(p, new_p, Knight);
}
void test_board::test_move_incorrect_diagonal(board *b)
{
    int player = 0;
    position p = {0, 2}, new_p = {1, 3};
    b->current_player = player;
    b->hand[player] = {Knight};
    b->board_map[p].push_back({Knight, player});

    b->move(p, new_p, Knight);
}
void test_board::test_move_incorrect_too_far(board *b)
{
    int player = 0;
    position p = {0, 2}, new_p = {0, 4};
    b->current_player = player;
    b->hand[player] = {Knight};
    b->board_map[p].push_back({Knight, player});

    b->move(p, new_p, Knight);
}
void test_board::test_move_incorrect_no_unit_in_hand(board *b)
{
    int player = 0;
    position p = {2, 2}, new_p = {2, 3};
    b->current_player = player;
    b->hand[player] = {Archer};
    b->board_map[p].push_back({Knight, player});

    b->move(p, new_p, Knight);
}
void test_board::test_move_incorrect_no_unit_in_board(board *b)
{
    int player = 0;
    position p = {2, 2}, new_p = {2, 3};
    b->current_player = player;
    b->hand[player] = {Knight};
    b->board_map[p].push_back({Knight, player});

    b->move(p, new_p, Archer);
}

void test_board::test_recruit_correct(board *b)
{
    int player = 0;
    position p = {2, 2}, new_p = {2, 3};
    b->current_player = player;
    b->hand[player] = {Knight};
    b->recruitment[player] = {Archer, Knight};
    b->recruit(Knight, Knight);
}
void test_board::test_recruit_correct_royal(board *b)
{
    int player = 0;
    position p = {2, 2}, new_p = {2, 3};
    b->current_player = player;
    b->hand[player] = {Royal};
    b->recruitment[player] = {Archer, Knight};
    b->recruit(Royal, Knight);
}
void test_board::test_recruit_incorrect(board *b)
{
    int player = 0;
    position p = {2, 2}, new_p = {2, 3};
    b->current_player = player;
    b->hand[player] = {Archer};
    b->recruitment[player] = {Archer, Knight};
    b->recruit(Knight, Knight);
}