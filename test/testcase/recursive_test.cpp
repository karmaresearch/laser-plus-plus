#include <string>
#include <vector>

#include <gtest/gtest.h>
#include <formula/extended/atom.h>
#include <formula/formula.h>
#include <program/program.h>
#include <rule/rule.h>

#include "simple_io_manager.h"
#include "simple_parser.h"
#include "simple_reader.h"
#include "simple_writer.h"
 

TEST(RecursiveTest, AAA) {

    std::string stream_string = "1 14 "
                                "1 : q(1), p(1)\n"
                                "2 : p(1)\n"
                                "3 : p(1), q(1)\n"
                                "4 : \n";

    std::string rule_string = "a(X) := b(X) && c(X)\n"
                              "b(X) := [$, 3] [D] d(X)\n"
                              "c(X) := [$, 3] [B] e(X)\n"
                              "e(X) := b(X) && p(X)\n"
                              "d(X) := q(X) && p(X)\n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> a(1), b(1), c(1), e(1), d(1)";
    expected[2] = "2 -> a(1), b(1), c(1), e(1)";
    expected[3] = "3 -> a(1), b(1), c(1), e(1), d(1)";
    expected[4] = "4 -> b(1)";
    expected[5] = "5 -> b(1)";
    expected[6] = "6 -> b(1)";
    expected[7] = "7 -> ";
    expected[8] = "8 -> ";
    expected[9] = "9 -> ";
    expected[10] = "10 -> ";
    expected[11] = "11 -> ";
    expected[12] = "12 -> ";
    expected[13] = "13 -> ";
    expected[14] = "14 -> ";

    auto simple_io_manager = SimpleIOManager(stream_string);
    auto program = laser::program::Program(rule_string, &simple_io_manager);
    int current_time = 1;
    program.set_start_time(current_time);

    while (!program.is_done()) {
        program.evaluate();
        std::string result = simple_io_manager.get_output();
        EXPECT_EQ(result, expected[current_time]);
        current_time++;
    }
    // ASSERT_EQ(-1.0, squareRoot(-0.2));
    // EXPECT_TRUE(has_derived_conclusions);
}
