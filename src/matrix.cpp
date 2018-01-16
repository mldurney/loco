#include "matrix.hpp"

Matrix::Matrix(uint r, uint c, double sparsity) : numRows(r), numCols(c) {
    uint cells = numRows * numCols;
    uint nonzero = (uint)cells * sparsity;

    std::vector<uint> indices(cells);
    std::iota(indices.begin(), indices.end(), 0);
    std::random_shuffle(indices.begin(), indices.end());

    data = std::vector<int>(cells);
    rows = std::vector<std::vector<int>>(numRows, std::vector<int>(numCols));
    cols = std::vector<std::vector<int>>(numCols, std::vector<int>(numRows));

    for (uint i = 0; i < nonzero; ++i) {
        uint ind = indices[i];
        uint row = toRow(ind);
        uint col = toCol(ind);
        rows[row][col] = 1;
        cols[col][row] = 1;
        data[ind] = 1;
    }
}

Matrix::Matrix(uint r, uint c, std::vector<int> nums)
    : numRows(r), numCols(c), data(nums) {
    uint cells = numRows * numCols;
    if (nums.size() != cells) {
        std::cout << "Constructor ERROR: nums vector wrong size" << std::endl
                  << std::endl;
        exit(EXIT_FAILURE);
    }

    rows = std::vector<std::vector<int>>(numRows, std::vector<int>(numCols));
    cols = std::vector<std::vector<int>>(numCols, std::vector<int>(numRows));

    for (uint i = 0; i < cells; ++i) {
        uint row = toRow(i);
        uint col = toCol(i);
        rows[row][col] = data[i];
        cols[col][row] = data[i];
    }
}

Matrix Matrix::getSubmatrix(std::vector<uint> rows,
                            std::vector<uint> cols) const {
    checkRows(rows);
    checkCols(cols);

    uint r = rows.size();
    uint c = cols.size();
    std::vector<int> nums(r * c);

    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            nums[i * c + j] = getCell(i, j);
        }
    }

    return Matrix(r, c, nums);
}

void Matrix::print() const {
    for (const auto& row : rows) {
        for (const auto& val : row) {
            printf("%3d", val);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Matrix::setCell(uint ind, int num) {
    checkInd(ind);
    uint row = toRow(ind);
    uint col = toCol(ind);

    data[ind] = num;
    rows[row][col] = num;
    cols[col][row] = num;
}

void Matrix::setCell(uint row, uint col, int num) {
    checkRow(row);
    checkCol(col);

    data[toInd(row, col)] = num;
    rows[row][col] = num;
    cols[col][row] = num;
}

void Matrix::setRow(uint row, const std::vector<int>& nums) {
    checkRow(row);
    if (nums.size() != getNumCols()) {
        std::cout << "setRow ERROR: nums vector wrong size" << std::endl
                  << std::endl;
        exit(EXIT_FAILURE);
    }

    for (uint col = 0; col < getNumCols(); ++col) {
        data[toInd(row, col)] = nums[col];
        rows[row][col] = nums[col];
        cols[col][row] = nums[col];
    }
}

void Matrix::setCol(uint col, const std::vector<int>& nums) {
    checkCol(col);
    if (nums.size() != getNumRows()) {
        std::cout << "setCol ERROR: nums vector wrong size" << std::endl
                  << std::endl;
        exit(EXIT_FAILURE);
    }

    for (uint row = 0; row < getNumRows(); ++row) {
        data[toInd(row, col)] = nums[col];
        rows[row][col] = nums[col];
        cols[col][row] = nums[col];
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