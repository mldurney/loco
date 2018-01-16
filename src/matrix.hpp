#ifndef MATRIX_HPP
#define MATRIX_HPP

// Includes
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

// Typedefs
typedef unsigned int uint;

class Matrix {
   public:
    Matrix(uint r = 10, uint c = 10, double sparsity = 0.3);
    Matrix(uint r, uint c, std::vector<int> nums);
    ~Matrix() {}

    // Getters
    uint getCell(uint ind) const { return data[checkInd(ind)]; }
    uint getCell(uint row, uint col) const {
        return rows[checkRow(row)][checkCol(col)];
    }
    std::vector<int> getRow(uint row) const { return rows[checkRow(row)]; }
    std::vector<int> getCol(uint col) const { return cols[checkCol(col)]; }
    std::vector<std::vector<int>> getRows() const { return rows; }
    std::vector<std::vector<int>> getCols() const { return cols; }
    uint getNumRows() const { return numRows; }
    uint getNumCols() const { return numCols; }
    uint getNumCells() const { return numRows * numCols; }
    Matrix getSubmatrix(std::vector<uint> rows, std::vector<uint> cols) const;

    // Setters
    void setCell(uint ind, int num);
    void setCell(uint row, uint col, int num);
    void setRow(uint row, const std::vector<int> &nums);
    void setCol(uint col, const std::vector<int> &nums);

    void print() const;

   private:
    uint numRows;
    uint numCols;
    std::vector<std::vector<int>> rows;
    std::vector<std::vector<int>> cols;
    std::vector<int> data;

    // Convert 1D index to 2D row, column
    uint toRow(uint ind) const { return ind / numCols; }
    uint toCol(uint ind) const { return ind % numCols; }
    // Convert 2D row, column to 1D index
    uint toInd(uint row, uint col) const { return row * numCols + col; }

    // Check indices in valid range
    uint checkInd(uint ind) const;
    uint checkRow(uint row) const;
    uint checkCol(uint col) const;
    void checkRows(std::vector<uint> rows) const;
    void checkCols(std::vector<uint> cols) const;
};

#endif  // MATRIX_HPP