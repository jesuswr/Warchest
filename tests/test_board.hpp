#ifndef _TEST_BOARD_HPP
#define _TEST_BOARD_HPP

class board;

class test_board
{
public:
    void test_place_correct(board *b);
    void test_place_correct_in_mid(board *b);
    void test_place_incorrect(board *b);
    void test_place_royal(board *b);

    void test_control_correct(board *b);
    void test_control_incorrect_no_unit(board *b);
    void test_control_incorrect_not_control_zone(board *b);
    void test_control_take_control_zone_from_rival(board *b);

    void test_move_correct(board *b);
    void test_move_incorrect_diagonal(board *b);
    void test_move_incorrect_too_far(board *b);
    void test_move_incorrect_no_unit_in_hand(board *b);
    void test_move_incorrect_no_unit_in_board(board *b);

    void test_recruit_correct(board *b);
    void test_recruit_correct_royal(board *b);
    void test_recruit_incorrect(board *b);

    void test_attack_archer_correct(board *b);
    void test_attack_archer_incorrect(board *b);
    void test_attack_Knight_correct(board *b);
    void test_attack_Knight_incorrect(board *b);
    void test_attack_Mercenary_correct(board *b);
    void test_attack_Mercenary_incorrect(board *b);
    void test_attack_Crossbowman_correct(board *b);
    void test_attack_Crossbowman_incorrect(board *b);

    void test_initiative_correct(board *b);
    void test_initiative_incorrect_already_have_it(board *b);
    void test_initiative_incorrect_not_in_hand(board *b);
};

#endif