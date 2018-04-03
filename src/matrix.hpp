#ifndef MATRIX_HPP
#define MATRIX_HPP

// Includes
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <limits>
#include <map>
#include <random>
#include <vector>

// Typedefs and constants
typedef Eigen::MatrixXd DMat;               // Dynamic-sized dense matrix
typedef Eigen::VectorXd DVec;               // Dynamic-sized dense col vector
typedef Eigen::RowVectorXd DRowVec;         // Dynamic-sized dense row vector
typedef Eigen::SparseMatrix<double> SpMat;  // Column-major sparse matrix
typedef Eigen::SparseVector<double> SpVec;  // Sparse vector
typedef Eigen::Triplet<double> T;           // Triplet for filling matrix
typedef std::vector<unsigned> uivector;
const unsigned DEFAULT_SIZE = 100;
const double SPARSITY_BASE = 5;
const double DEFAULT_NOISE = .01;
const double EPSILON = std::numeric_limits<double>::epsilon() * 3;

inline bool checkError(double a, double b) { return fabs(a - b) < EPSILON; }

class Matrix {
   private:
    unsigned rows;
    unsigned cols;
    unsigned cells;
    SpMat matrix;
    DVec b;

    // Convert 1D index to 2D row, column
    inline int toRow(unsigned ind) const { return ind / cols; }
    inline unsigned toCol(unsigned ind) const { return ind % cols; }
    // Convert 2D row, column to 1D index
    inline unsigned toInd(unsigned r, unsigned c) const {
        return r * cols + c;
    }

    // Check indices in valid range
    unsigned checkInd(unsigned ind) const;
    unsigned checkRow(unsigned r) const;
    unsigned checkCol(unsigned c) const;
    void checkRows(uivector rows) const;
    void checkCols(uivector cols) const;

   public:
    Matrix(unsigned r, unsigned c, double p, double noise = DEFAULT_NOISE);
    Matrix() : Matrix(DEFAULT_SIZE) {}
    Matrix(unsigned n) : Matrix(n, n) {}
    Matrix(unsigned r, unsigned c)
        : Matrix(r, c, SPARSITY_BASE / c, DEFAULT_NOISE) {}
    Matrix(unsigned r, unsigned c, const std::vector<T> triplets, DVec b_);

    // Getters
    inline unsigned getRows() const { return rows; }
    inline unsigned getCols() const { return cols; }
    inline unsigned getCells() const { return cells; }
    double getCell(unsigned r, unsigned c) const;
    double getCell(unsigned ind) const;
    SpVec getRow(unsigned r) const;
    SpVec getCol(unsigned c) const;
    DVec getB() const;
    std::vector<T> getTriplets() const;
    Matrix getSubmatrix(uivector rows, uivector cols) const;
    DMat getDenseSubmatrix(uivector rows, uivector cols) const;

    // Setters
    void setCell(unsigned r, unsigned c, double val);
    void setCell(unsigned ind, double val) {
        setCell(toRow(ind), toCol(ind), val);
    }
    void clearCell(unsigned r, unsigned c);
    void clearCell(unsigned ind) { clearCell(toRow(ind), toCol(ind)); }

    // Utilities
    bool isOccupied(unsigned r, unsigned c) const;
    bool isOccupied(unsigned ind) const {
        return isOccupied(toRow(ind), toCol(ind));
    }
    void printDense() const;
    void printSparse() const;
};

#endif  // MATRIX_HPP
