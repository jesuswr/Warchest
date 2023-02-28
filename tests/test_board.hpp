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
};

#endif