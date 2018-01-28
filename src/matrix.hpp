#ifndef MATRIX_HPP
#define MATRIX_HPP

// Includes
#include <algorithm>
#include <cmath>
#include <ctime>
#include <iostream>
#include <limits>
#include <numeric>
#include <vector>

// Typedefs and constants
typedef unsigned int uint;
typedef std::vector<uint> uivector;
typedef std::vector<double> dvector;
typedef std::vector<std::vector<double>> dvector2;
typedef std::vector<bool> bvector;
const double SPARSITY_BASE = 3;
const double EPSILON = std::numeric_limits<double>::epsilon() * 3;

class Matrix {
   public:
    Matrix() : Matrix(60, 50) {}
    Matrix(uint n) : Matrix(n, n) {}
    Matrix(uint m, uint n) : Matrix(m, n, SPARSITY_BASE / n) {}
    Matrix(uint m, uint n, double p);
    Matrix(uint m, uint n, const dvector &nums);
    ~Matrix() {}

    // Getters
    double getCell(uint ind) const { return data[checkInd(ind)]; }
    double getCell(uint row, uint col) const {
        return rows[checkRow(row)][checkCol(col)];
    }
    dvector getRow(uint row) const { return rows[checkRow(row)]; }
    dvector getCol(uint col) const { return cols[checkCol(col)]; }
    dvector2 getRows() const { return rows; }
    dvector2 getCols() const { return cols; }
    uint getNumRows() const { return numRows; }
    uint getNumCols() const { return numCols; }
    uint getNumCells() const { return numRows * numCols; }
    Matrix getSubmatrix(uivector rows, uivector cols) const;

    // Setters
    void setCell(uint ind, double num);
    void setCell(uint row, uint col, double num);
    void setRow(uint row, const dvector &nums);
    void setCol(uint col, const dvector &nums);

    bool isOccupied(uint ind) const { return occupied[ind]; }
    bool isOccupied(uint row, uint col) const {
        return occupied[toInd(row, col)];
    }
    void print() const;
    void printOccupancy() const;

   private:
    uint numRows;
    uint numCols;
    dvector2 rows;
    dvector2 cols;
    dvector data;
    bvector occupied;

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