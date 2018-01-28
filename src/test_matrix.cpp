#include "matrix.hpp"

const uint SIZE = 10;
const uint M = 50;
const uint N = 60;
const double P = 5 / (double)N;

void testMatrix(Matrix* m) {
    bool isGood;

    // std::cout << "Printing matrix occupancy..." << std::endl;
    // m->printOccupancy();

    std::cout << "Checking each row for nonzero...\t";
    int badRow = -1;
    for (uint row = 0; row < m->getNumRows(); ++row) {
        bool hasNonzero = false;

        for (uint col = 0; col < m->getNumCols(); ++col) {
            if (m->isOccupied(row, col)) {
                hasNonzero = true;
            }
        }

        if (!hasNonzero) {
            badRow = row;
            break;
        }
    }
    if (badRow == -1) {
        std::cout << "OK" << std::endl;
    } else {
        std::cout << "FAILED at " << badRow << std::endl;
    }

    std::cout << "Checking each col for nonzero...\t";
    int badCol = -1;
    for (uint col = 0; col < m->getNumCols(); ++col) {
        bool hasNonzero = false;

        for (uint row = 0; row < m->getNumRows(); ++row) {
            if (m->isOccupied(row, col)) {
                hasNonzero = true;
            }
        }

        if (!hasNonzero) {
            badCol = col;
            break;
        }
    }
    if (badCol == -1) {
        std::cout << "OK" << std::endl;
    } else {
        std::cout << "FAILED at " << badCol << std::endl;
    }

    isGood = true;
    std::cout << "Testing set/get cell (value 0)...\t";
    m->setCell(0, 0, 0);
    isGood = fabs(m->getCell(0)) <= EPSILON && !(m->isOccupied(0));
    std::cout << (isGood ? "OK" : "FAILED") << std::endl;

    isGood = true;
    std::cout << "Testing set/get cell (value 1)...\t";
    m->setCell(0, 1);
    isGood = fabs(m->getCell(0, 0) - 1) <= EPSILON && m->isOccupied(0, 0);
    std::cout << (isGood ? "OK" : "FAILED") << std::endl;

    isGood = true;
    std::cout << "Testing set/get row...\t\t\t";
    dvector row1 = dvector(m->getNumCols(), 1);
    m->setRow(0, row1);
    dvector row2 = m->getRow(0);
    for (uint i = 0; i < row2.size(); ++i) {
        if (fabs(row1[i] - row2[i]) > EPSILON ||
            fabs(m->getCell(0, i) - row2[i]) > EPSILON ||
            !(m->isOccupied(0, i))) {
            isGood = false;
            break;
        }
    }
    std::cout << (isGood ? "OK" : "FAILED") << std::endl;

    isGood = true;
    std::cout << "Testing set/get col...\t\t\t";
    dvector col1 = dvector(m->getNumRows(), 1);
    m->setCol(0, col1);
    dvector col2 = m->getCol(0);
    for (uint i = 0; i < col2.size(); ++i) {
        if (fabs(col1[i] - col2[i]) > EPSILON ||
            fabs(m->getCell(i, 0) - col2[i]) > EPSILON ||
            !(m->isOccupied(i, 0))) {
            isGood = false;
            break;
        }
    }
    std::cout << (isGood ? "OK" : "FAILED") << std::endl;

    isGood = true;
    std::cout << "Testing submatrix...\t\t\t";
    uivector rows;
    for (int i = (int)sqrt(m->getNumRows()) - 1; i >= 0; --i) {
        rows.push_back(rand() % m->getNumRows());
    }
    uivector cols;
    for (int i = (int)sqrt(m->getNumCols()) - 1; i >= 0; --i) {
        cols.push_back(rand() % m->getNumCols());
    }
    Matrix sub = m->getSubmatrix(rows, cols);
    for (uint i = 0; i < rows.size(); ++i) {
        for (uint j = 0; j < cols.size(); ++j) {
            if (fabs(sub.getCell(i, j) - m->getCell(rows[i], cols[j])) >
                    EPSILON ||
                sub.isOccupied(i, j) != m->isOccupied(rows[i], cols[j])) {
                isGood = false;
                break;
            }
        }
    }
    std::cout << (isGood ? "OK" : "FAILED") << std::endl;

    // std::cout << "Printing submatrix..." << std::endl;
    // sub.print();

    std::cout << std::endl << std::endl;
}

int main() {
    Matrix* mDefault = new Matrix;
    Matrix* mSize = new Matrix(SIZE);
    Matrix* mMN = new Matrix(M, N, P);

    std::cout << "Testing default matrix..." << std::endl << std::endl;
    testMatrix(mDefault);
    std::cout << "Testing size " << SIZE << " matrix..." << std::endl
              << std::endl;
    testMatrix(mSize);
    std::cout << "Testing " << M << "x" << N << " matrix, sparcity " << P
              << "..." << std::endl
              << std::endl;
    testMatrix(mMN);

    delete mDefault;
    delete mSize;
    delete mMN;
}