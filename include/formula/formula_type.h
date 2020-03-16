#ifndef ARES_FORMULA_FORMULA_TYPE_H
#define ARES_FORMULA_FORMULA_TYPE_H

namespace ares::formula {


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
    EXISTENTIAL, 
    MATH
};


} // namespace ares::formula

#endif // ARES_FORMULA_FORMULA_TYPE_H
