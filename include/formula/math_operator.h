#ifndef ARES_FORMULA_MATH_OPERATOR_H
#define ARES_FORMULA_MATH_OPERATOR_H

namespace ares::formula {

enum class MathOperator {
    ASSIGNMENT,
    PLUS,
    MINUS,
    MULTIPLICATION,
    DIVISION,
    EQUALS,
    NOT_EQUAL,
    LESSER,
    GREATHER,
    LESSER_OR_EQUAL,
    GREATHER_OR_EQUAL,
    NONE
};

} // namespace ares::formula

#endif // ARES_FORMULA_MATH_OPERATOR_H
