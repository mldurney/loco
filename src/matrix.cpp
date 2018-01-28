#include "matrix.hpp"

Matrix::Matrix(uint m, uint n, double p) : numRows(m), numCols(n) {
    uint cells = numRows * numCols;

    rows = dvector2(numRows, dvector(numCols, 0));
    cols = dvector2(numCols, dvector(numRows, 0));
    data = dvector(cells, 0);
    occupied = bvector(cells, false);

    srand(time(NULL));
    for (uint row = 0; row < numRows; ++row) {
        uint ind = toInd(row, rand() % numCols);
        occupied[ind] = true;
    }
    for (uint col = 0; col < numCols; ++col) {
        uint ind = toInd(rand() % numRows, col);
        occupied[ind] = true;
    }
    for (uint i = 0; i < cells; ++i) {
        if (!occupied[i]) {
            occupied[i] = ((double)rand() / RAND_MAX) < p;
        }
    }

    for (uint i = 0; i < cells; ++i) {
        if (occupied[i]) {
            do {
                data[i] = (double)rand() / RAND_MAX;
            } while (data[i] <= EPSILON);

            uint row = toRow(i);
            uint col = toCol(i);
            rows[row][col] = data[i];
            cols[col][row] = data[i];
        }
    }
}

Matrix::Matrix(uint m, uint n, const dvector& nums)
    : numRows(m), numCols(n), data(nums) {
    uint cells = numRows * numCols;
    if (nums.size() != cells) {
        std::cout << "Constructor ERROR: nums vector wrong size" << std::endl
                  << std::endl;
        exit(EXIT_FAILURE);
    }

    rows = dvector2(numRows, dvector(numCols, 0));
    cols = dvector2(numCols, dvector(numRows, 0));
    data = dvector(cells, 0);
    occupied = bvector(cells, false);

    for (uint i = 0; i < cells; ++i) {
        if (fabs(nums[i]) > EPSILON) {  // Account for floating-point error
            uint row = toRow(i);
            uint col = toCol(i);
            rows[row][col] = nums[i];
            cols[col][row] = nums[i];
            data[i] = nums[i];
            occupied[i] = true;
        }
    }
}

Matrix Matrix::getSubmatrix(uivector rows, uivector cols) const {
    checkRows(rows);
    checkCols(cols);

    uint n = rows.size();
    uint m = cols.size();
    dvector nums(m * n);

    for (uint i = 0; i < n; ++i) {
        for (uint j = 0; j < m; ++j) {
            nums[i * n + j] = getCell(rows[i], cols[j]);
        }
    }

    return Matrix(m, n, nums);
}

void Matrix::print() const {
    for (const auto& row : rows) {
        for (const auto& val : row) {
            printf("%.3f ", val);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Matrix::printOccupancy() const {
    for (uint row = 0; row < numRows; ++row) {
        for (uint col = 0; col < numCols; ++col) {
            std::cout << (isOccupied(row, col) ? 1 : 0) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Matrix::setCell(uint ind, double num) {
    checkInd(ind);
    uint row = toRow(ind);
    uint col = toCol(ind);

    if (num > EPSILON) {
        data[ind] = num;
        rows[row][col] = num;
        cols[col][row] = num;
        occupied[ind] = true;
    } else {
        data[ind] = 0;
        rows[row][col] = 0;
        cols[col][row] = 0;
        occupied[ind] = false;
    }
}

void Matrix::setCell(uint row, uint col, double num) {
    checkRow(row);
    checkCol(col);
    uint ind = toInd(row, col);
    setCell(ind, num);
}

void Matrix::setRow(uint row, const dvector& nums) {
    checkRow(row);
    if (nums.size() != getNumCols()) {
        std::cout << "setRow ERROR: nums vector wrong size" << std::endl
                  << std::endl;
        exit(EXIT_FAILURE);
    }

    for (uint col = 0; col < getNumCols(); ++col) {
        setCell(row, col, nums[col]);
    }
}

void Matrix::setCol(uint col, const dvector& nums) {
    checkCol(col);
    if (nums.size() != getNumRows()) {
        std::cout << "setCol ERROR: nums vector wrong size" << std::endl
                  << std::endl;
        exit(EXIT_FAILURE);
    }

    for (uint row = 0; row < getNumRows(); ++row) {
        setCell(row, col, nums[row]);
    }
}

uint Matrix::checkInd(uint ind) const {
    if (ind > getNumCells()) {
        std::cout << "checkInd ERROR: ind exceeds number of cells" << std::endl
                  << std::endl;
        exit(EXIT_FAILURE);
    }
    return ind;
}

uint Matrix::checkRow(uint row) const {
    if (row > getNumRows()) {
        std::cout << "checkRow ERROR: row exceeds number of rows" << std::endl
                  << std::endl;
        exit(EXIT_FAILURE);
    }
    return row;
}

uint Matrix::checkCol(uint col) const {
    if (col > getNumCols()) {
        std::cout << "checkCol ERROR: col exceeds number of columns"
                  << std::endl
                  << std::endl;
        exit(EXIT_FAILURE);
    }
    return col;
}

void Matrix::checkRows(std::vector<uint> rows) const {
    for (auto& row : rows) {
        checkRow(row);
    }
}

void Matrix::checkCols(std::vector<uint> cols) const {
    for (auto& col : cols) {
        checkRow(col);
    }
}