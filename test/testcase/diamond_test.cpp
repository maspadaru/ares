#include <sstream>
#include <string>
#include <vector>

#include "test_framework.h"

#include <util/chase_algorithm.h>
 

TEST(DiamondTest, DiamondAtom) {
    std::string stream_string = "1 14 "
                                "1 : \n"
                                "2 : c(x2, y2)\n"
                                "2 : d(x2, y2)\n"
                                "4 : \n"
                                "5 : \n"
                                "5 : e(x5, y5), e(a5, b5), e(x5, y5)\n"
                                "5 : e(x5, y5)\n"
                                "6 : a(x6, y6, z6)\n"
                                "7 : \n"
                                "8 : a(x8, y8, z8), b(y8, z8)\n"
                                "9 : \n"
                                "10 : a(x10, y10, z10)\n"
                                "11 : f(a11)\n"
                                "11 : f(-9.099)\n"
                                "11 : f(Z)\n"
                                "12 : e(x12, x12)\n"
                                "13 : \n"
                                "14 : \n";

    std::string rule_string = "q(X, Y, Z) := [D] a(X, Y, Z)\n"
                              "u(X, X) := [D]f(X)\n";

    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> ";
    expected[2] = "2 -> ";
    expected[3] = "3 -> ";
    expected[4] = "4 -> ";
    expected[5] = "5 -> ";
    expected[6] = "6 -> q(x6, y6, z6)";
    expected[7] = "7 -> q(x6, y6, z6)";
    expected[8] = "8 -> q(x6, y6, z6) q(x8, y8, z8)";
    expected[9] = "9 -> q(x6, y6, z6) q(x8, y8, z8)";
    expected[10] = "10 -> q(x8, y8, z8) q(x10, y10, z10) q(x6, y6, z6)";
    expected[11] = "11 -> q(x8, y8, z8) q(x10, y10, z10) q(x6, y6, z6) "
        "u(Z, Z) u(a11, a11) u(-9.099, -9.099)";
    expected[12] = "12 -> q(x8, y8, z8) q(x10, y10, z10) q(x6, y6, z6) "
        "u(Z, Z) u(a11, a11) u(-9.099, -9.099)";
    expected[13] = "13 -> q(x8, y8, z8) q(x10, y10, z10) q(x6, y6, z6) "
        "u(Z, Z) u(a11, a11) u(-9.099, -9.099)";
    expected[14] = "14 -> q(x8, y8, z8) q(x10, y10, z10) q(x6, y6, z6) "
        "u(Z, Z) u(a11, a11) u(-9.099, -9.099)";

    test_framework::run_test(stream_string, rule_string, expected,
                             ares::util::ChaseAlgorithm::OBLIVIOUS);
}
