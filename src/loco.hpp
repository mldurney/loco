#ifndef LOCO_HPP
#define LOCO_HPP

// Includes
#include <array>
#include <stack>
#include "matrix.hpp"

// Typedefs
typedef struct {
    std::vector<int> x;
    uint messages;
} Solution;
typedef std::stack<std::array<uint, 2>> RankStack;

Solution loco(const Matrix& matrix);
std::vector<uint> generateRanks(uint num);
Matrix setupLocalProblem(const Matrix& matrix, const std::vector<uint>& ranks,
                         uint ind, uint& messages);
uint maxRank(const std::vector<int>& x, const std::vector<uint>& ranks);
void loadStack(RankStack& stack, const std::vector<int>& y, uint rank);
int solveLocalProblem(const Matrix& problem);

#endif  // LOCO_HPP