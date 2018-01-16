#include "loco.hpp"

Solution loco(const Matrix matrix) {
    uint numPrimal = matrix.getNumCols();
    uint numDual = matrix.getNumRows();
    std::vector<uint> ranks = generateRanks(numDual);

    Solution s;
    std::vector<Matrix> submatrices;

    for (int i = 0; i < numPrimal; ++i) {
        submatrices.push_back(setupLocalProblem(matrix, ranks, i, s.messages));
    }

    for (int i = 0; i < numPrimal; ++i) {
        s.x.push_back(solveLocalProblem(submatrices[i]));
    }

    return s;
}

std::vector<uint> generateRanks(uint num) {
    std::vector<uint> ranks(num);
    std::iota(ranks.begin(), ranks.end(), 0);
    std::random_shuffle(ranks.begin(), ranks.end());
    return ranks;
}

Matrix setupLocalProblem(const Matrix& matrix, const std::vector<uint>& ranks,
                         uint ind, uint& messages) {
    std::vector<uint> x;
    std::vector<uint> y;
    RankStack stack;

    uint k = maxRank(matrix.getCol(ind), ranks);
    loadStack(stack, matrix.getRow(k), ranks[k]);

    while (!stack.empty()) {
        uint x0 = (stack.top())[0];
        uint rank0 = (stack.top())[1];
        stack.pop();

        std::vector<int> xCol = matrix.getCol(x0);
        for (uint i = 0; i < xCol.size(); ++i) {
            if (uint y0 = xCol[i]) {
                if (ranks[y0] != rank0) {
                    ++messages;
                    // x0, y0, checks etc
                }
            }
        }
    }

    return matrix.getSubmatrix(x, y);
}

uint maxRank(const std::vector<int>& x, const std::vector<uint>& ranks) {
    std::vector<int> y;
    for (uint i = 0; i < x.size(); ++i) {
        if (x[i]) {
            y.push_back(i);
        }
    }

    uint max = 0;
    uint k = ranks.size();

    for (auto& ind : y) {
        if (ranks[ind] > max) {
            max = ranks[ind];
            k = ind;
        }
    }

    return k;
}

void loadStack(RankStack& stack, const std::vector<int>& y, uint rank) {
    for (uint i = 0; i < y.size(); ++i) {
        if (y[i]) {
            stack.push({i, rank});
        }
    }
}

int solveLocalProblem(const Matrix& problem) {
    // TODO
    return 0;
}
