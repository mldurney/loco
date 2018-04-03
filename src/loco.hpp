#ifndef LOCO_HPP
#define LOCO_HPP

// Includes
#include <unsupported/Eigen/autodiff>
#include "matrix.hpp"

// Typedefs and constants
typedef std::vector<double> dvector;
typedef double (*fun)(double);  // Type for problem constraint functions
typedef std::vector<fun> fvector;
// Type for online algorithms
typedef DVec (*online)(const Matrix&, const fvector&, double);
typedef struct {
    double primal;
    unsigned messages;
} LocoSolution;  // Solution from local problem for primal variable x_k
typedef struct {
    dvector primals;
    unsigned messages;
} MatrixSolution;  // Solution for all primal variables of matrix
const double CHANGE = 1e-6;

MatrixSolution solve(online alg, const Matrix& matrix, const fvector& funs,
                     dvector ranks = dvector());
LocoSolution loco(online alg, const Matrix& matrix, const fvector& funs,
                  const dvector& ranks, unsigned ind);
dvector generateRanks(unsigned num);
unsigned maxRank(const SpVec& x, const dvector& ranks);
fvector restrictFunction(const fvector& unrestricted, uivector y);
inline double derive(const fun& f, double x, double h = CHANGE);
DVec onlineFractional(const Matrix& matrix, const fvector& funs, double delta);

#endif  // LOCO_HPP

/**
 * Solve local problem associated with mxn matrix given delta parameter
 * Local problem solved with respect to leading primal (x) variable for matrix
 *
 * Param:   matrix  - convex program of form minimize sum_{j=1}^{n} f_j(x_j),
 *                      with sum_{j=1}^{n} f_j(a_ij x_j) >= b_i, x_j >= 0,
 *                      i in [m], j in [m]
 *          approx  - real delta (change in x) parameter
 * Return:  x       - dense vector of primal variables
 */