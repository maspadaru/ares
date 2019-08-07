#ifndef LASER_FORMULA_FORMULA_TYPE_H
#define LASER_FORMULA_FORMULA_TYPE_H

namespace laser {
namespace formula {


enum class FormulaType {
    ATOM,
    CONJUNCTION,
    DISJUNCTION,
    IMPLICATION,
    NEGATION,
    TIME_REFERENCE,
    TUPLE_WINDOW,
    TIME_WINDOW,
    BOX,
    DIAMOND,
    EXISTENTIAL 
};


} // namespace formula
} // namespace laser

#endif // LASER_FORMULA_FORMULA_TYPE_H