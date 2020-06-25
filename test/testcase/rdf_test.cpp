#include <sstream>
#include <string>
#include <vector>

#include "test_framework.h"

#include <util/chase_algorithm.h>

/* Testing RDF sintax support */

TEST(RDFTest, RDFAtom) {
    std::string stream_string = "1 4 "
                                "1 : <http://www.w3.org/2000/01/rdf-schema#label>(<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Book>, \"book1\")\n"
                                "2 : <http://www.w3.org/2000/01/rdf-schema#label>(<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Book>, \"book2\")\n"
                                "3 : <http://www.w3.org/2000/01/rdf-schema#label>(<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Book>, \"book3\")\n"
                                "4 : \n";
    std::string rule_string = " <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>(Y,X) := <http://www.w3.org/2000/01/rdf-schema#label>(X, Y)\n";
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>(\"book1\", <http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Book>)";
    expected[2] = "2 -> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>(\"book2\", <http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Book>)";
    expected[3] = "3 -> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>(\"book3\", <http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Book>)";
    expected[4] = "4 -> ";
    expected[5] = "5 -> ";
    expected[6] = "6 -> ";
    expected[7] = "7 -> ";
    expected[8] = "8 -> ";
    expected[9] = "9 -> ";
    expected[10] = "10 -> ";
    expected[11] = "11 -> ";
    expected[12] = "12 -> ";
    expected[13] = "13 -> ";
    expected[14] = "14 -> ";
    auto chase_alg = ares::util::ChaseAlgorithm::OBLIVIOUS;
    test_framework::run_test(stream_string, rule_string, expected,
                             chase_alg);
}

TEST(RDFTest, RDFBackground) {
    std::string background_string = "<http://www.w3.org/2000/01/rdf-schema#label>(<https://www.pearson.com/us/higher-education/program/Tanenbaum-Structured-Computer-Organization-6th-Edition/PGM200985.html>,  \"book\"), " 
    " <http://www.w3.org/2000/01/rdf-schema#label>(<https://www.pearson.com/us/higher-education/program/Tanenbaum-Modern-Operating-Systems-4th-Edition/PGM80736.html>, \"book\"), " 
    "<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#AuthorOf>(<https://en.wikipedia.org/wiki/Andrew_S._Tanenbaum>, <https://www.pearson.com/us/higher-education/program/Tanenbaum-Structured-Computer-Organization-6th-Edition/PGM200985.html>), " 
    "<http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#AuthorOf>(<https://www.cs.vu.nl/~herbertb/>, <https://www.pearson.com/us/higher-education/program/Tanenbaum-Modern-Operating-Systems-4th-Edition/PGM80736.html> )" ;
    std::string stream_string = "1 4 "
                                "1 : <http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Book>(<https://www.pearson.com/us/higher-education/program/Tanenbaum-Structured-Computer-Organization-6th-Edition/PGM200985.html>)\n"
                                "2 : <http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Book>(<https://www.pearson.com/us/higher-education/program/Tanenbaum-Modern-Operating-Systems-4th-Edition/PGM80736.html>)\n"
                                "3 : \n"
                                "4 : \n";

    std::string rule_string = "<http://www.w3.org/1999/02/22-rdf-syntax-ns#type>(Y,X) := <http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Book>(X) && <http://www.w3.org/2000/01/rdf-schema#label>(X, Y)\n"
    "<http://www.w3.org/1999/02/22-rdf-syntax-ns#WrittenBy>(X, Y) :=  <http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#Book>(X) && <http://www.lehigh.edu/~zhp2/2004/0401/univ-bench.owl#AuthorOf>(Y, X)\n";
    auto chase_alg = ares::util::ChaseAlgorithm::OBLIVIOUS;
    std::vector<std::string> expected(15);
    expected[0] = "0 -> ";
    expected[1] = "1 -> <http://www.w3.org/1999/02/22-rdf-syntax-ns#WrittenBy>(<https://www.pearson.com/us/higher-education/program/Tanenbaum-Structured-Computer-Organization-6th-Edition/PGM200985.html>, <https://en.wikipedia.org/wiki/Andrew_S._Tanenbaum>) <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>(\"book\", <https://www.pearson.com/us/higher-education/program/Tanenbaum-Structured-Computer-Organization-6th-Edition/PGM200985.html>) ";
    expected[2] = "2 -> <http://www.w3.org/1999/02/22-rdf-syntax-ns#WrittenBy>(<https://www.pearson.com/us/higher-education/program/Tanenbaum-Modern-Operating-Systems-4th-Edition/PGM80736.html>, <https://www.cs.vu.nl/~herbertb/>) <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>(\"book\", <https://www.pearson.com/us/higher-education/program/Tanenbaum-Modern-Operating-Systems-4th-Edition/PGM80736.html>) ";
    expected[3] = "3 -> ";
    expected[4] = "4 -> ";
    expected[5] = "5 -> ";
    expected[6] = "6 -> ";
    expected[7] = "7 -> ";
    expected[8] = "8 -> ";
    expected[9] = "9 -> ";
    expected[10] = "10 -> ";
    expected[11] = "11 -> ";
    expected[12] = "12 -> ";
    expected[13] = "13 -> ";
    expected[14] = "14 -> ";
    test_framework::run_test(stream_string, background_string, rule_string, 
           expected, chase_alg);
}
