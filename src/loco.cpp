#include "loco.hpp"

Solution loco(const Matrix& matrix) {
    uint numPrimal = matrix.getNumCols();
    uint numDual = matrix.getNumRows();
    uivector ranks = generateRanks(numDual);

    Solution s;
    s.messages = 0;
    std::vector<Matrix> submatrices;

    for (uint i = 0; i < numPrimal; ++i) {
        submatrices.push_back(setupLocalProblem(matrix, ranks, i, s.messages));
    }

    for (uint i = 0; i < numPrimal; ++i) {
        s.x.push_back(solveLocalProblem(submatrices[i]));
    }

    return s;
}

uivector generateRanks(uint num) {
    uivector ranks(num);
    std::iota(ranks.begin(), ranks.end(), 0);
    std::random_shuffle(ranks.begin(), ranks.end());
    return ranks;
}

Matrix setupLocalProblem(const Matrix& matrix, const uivector& ranks, uint ind,
                         uint& messages) {
    uivector x;
    uivector y;
    RankStack stack;

    uint k = maxRank(matrix.getCol(ind), ranks);
    loadStack(stack, matrix.getRow(k), ranks[k]);

    while (!stack.empty()) {
        uint x0 = (stack.top())[0];
        uint rank0 = (stack.top())[1];
        stack.pop();

        dvector xCol = matrix.getCol(x0);
        for (uint i = 0; i < xCol.size(); ++i) {
            double y0 = xCol[i];
            if (fabs(y0) > EPSILON) {
                ++messages;
                if (std::find(x.begin(), x.end(), x0) == x.end()) {
                    x.push_back(x0);
                }
                if (ranks[y0] != rank0) {
                    if (std::find(y.begin(), y.end(), y0) == y.end()) {
                        y.push_back(y0);
                        loadStack(stack, matrix.getRow(y0), ranks[y0]);
                    }
                }
            }
        }
    }

    return matrix.getSubmatrix(x, y);
}

uint maxRank(const dvector& x, const std::vector<uint>& ranks) {
    dvector y;
    for (uint i = 0; i < x.size(); ++i) {
        if (fabs(x[i]) > EPSILON) {
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

void loadStack(RankStack& stack, const dvector& y, uint rank) {
    for (uint i = 0; i < y.size(); ++i) {
        if (fabs(y[i]) > EPSILON) {
            stack.push({i, rank});
        }
    }
}

double solveLocalProblem(const Matrix& problem) {
    // TODO
    return 0;
}
