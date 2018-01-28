#ifndef LOCO_HPP
#define LOCO_HPP

// Includes
#include <array>
#include <stack>
#include "matrix.hpp"

// Typedefs
typedef struct {
    dvector x;
    uint messages;
} Solution;
typedef std::stack<std::array<uint, 2>> RankStack;

Solution loco(const Matrix& matrix);
uivector generateRanks(uint num);
Matrix setupLocalProblem(const Matrix& matrix, const uivector& ranks, uint ind,
                         uint& messages);
uint maxRank(const dvector& x, const uivector& ranks);
void loadStack(RankStack& stack, const dvector& y, uint rank);
double solveLocalProblem(const Matrix& problem);

#endif  // LOCO_HPP