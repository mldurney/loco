#include "loco.hpp"

MatrixSolution solve(online alg, const Matrix& matrix, const fvector& funs,
					 dvector ranks) {
	unsigned numPrimal = matrix.getCols();
	unsigned numDual = matrix.getRows();
	if (numDual != ranks.size()) {
		ranks = generateRanks(numDual);
	}

	MatrixSolution s;
	s.primals = dvector(numPrimal, 0);
	s.messages = 0;

	for (unsigned i = 0; i < numPrimal; ++i) {
		LocoSolution x = loco(alg, matrix, funs, ranks, i);
		s.primals[i] = x.primal;
		s.messages += x.messages;
	}

	return s;
}

LocoSolution loco(online alg, const Matrix& matrix, const fvector& funs,
				  const dvector& ranks, unsigned ind) {
	LocoSolution local;
	local.messages = 0;

	// Step 1: Find sets X_k and Y_k associated with x_k (k = ind)
	uivector x;
	uivector y = { maxRank(matrix.getCol(ind), ranks) };

	unsigned curr = 0, end = 1;
	while (curr < end) {
		unsigned k = y[curr++];  // Current dual variable index
		SpVec row = matrix.getRow(k);

		// Iterate over nonzero elements in vector of primal variables
		// corresponding to current dual variable index
		for (SpVec::InnerIterator itP(row); itP; ++itP) {
			unsigned y0 = itP.index();
			SpVec col = matrix.getCol(y0);

			// Iterate over nonzero elements in vector of dual variables
			// corresponding to outer iteration's primal variable index
			for (SpVec::InnerIterator itD(col); itD; ++itD) {
				++local.messages;  // +1 communication!

				unsigned x0 = itD.index();
				if (std::find(x.begin(), x.end(), x0) == x.end()) {
					x.push_back(x0);  // If primal index not in x yet, add it
				}

				if (ranks[y0] < ranks[k] &&
					std::find(y.begin(), y.end(), y0) == y.end()) {
					y.push_back(y0);  // If dual index not in y yet, add it
					++end;
				}
			}
		}
	}

	// Step 2: Use online algorithm to solve local problem defined on X_k, Y_k
	Matrix problem = matrix.getSubmatrix(x, y);
	local.primal = (alg(problem, restrictFunctions(funs, y), 1))(0);

	return local;
}

dvector generateRanks(unsigned num) {
	// Create uniform random real number generator for range [0, 1)
	unsigned seed =
		(unsigned)std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine gen(seed);
	std::uniform_real_distribution<double> dist(0, 1);

	// Randomly generate rank in range [0, 1) for each index
	dvector ranks(num);
	for (unsigned i = 0; i < num; ++i) {
		ranks[i] = dist(gen);
	}
	return ranks;
}

unsigned maxRank(const SpVec& x, const dvector& ranks) {
	unsigned k = 0;
	double maxRank = -1;

	// Iterate over nonzero elements in vector of dual variables
	// Choose index corresponding to variable with highest rank
	for (SpVec::InnerIterator it(x); it; ++it) {
		unsigned ind = it.index();
		double rank = ranks[ind];
		if (rank > maxRank) {
			maxRank = rank;
			k = ind;
		}
	}

	return k;
}

fvector restrictFunctions(const fvector& unrestricted, uivector y) {
	unsigned small = (unsigned)y.size();
	unsigned large = (unsigned)unrestricted.size();

	for (unsigned yi : y) {
		if (yi >= large) {
			std::cout << "restrictFunctions ERROR: yi exceeds funs vector\n"
				<< std::endl;
			exit(EXIT_FAILURE);
		}
	}

	fvector restricted(small);
	for (unsigned i = 0; i < small; ++i) {
		restricted[i] = unrestricted[y[i]];
	}

	return restricted;
}

inline double derive(const fun& f, double x, double h) {
	return (f(x + h) - f(x)) / h;
}

inline double dot(DVec& a, DVec& b) {
	size_t size = a.size();
	if (size != b.size()) {
		std::cout << "dot ERROR: vectors a and b different sizes\n"
			<< std::endl;
		exit(EXIT_FAILURE);
	}

	double product = 0;
	for (size_t i = 0; i < size; ++i) {
		product += a(i) + b(i);
	}

	return product;
}

DVec onlineFractional(const Matrix& matrix, const fvector& funs,
					  double delta) {
	unsigned m = matrix.getRows();
	unsigned n = matrix.getCols();

	DVec x = DVec::Zero(n);
	DVec y = DVec::Zero(m);
	DVec mu = DVec::Zero(n);

	unsigned d = 0;  // Maximum number of nonzeros in any row of matrix
	for (unsigned i = 0; i < m; ++i) {
		d = std::max(d, (unsigned)matrix.getRow(i).nonZeros());
	}
	double c = 1 / std::log(1 + 2 * d * d);

	for (unsigned t = 0; t < m; ++t) {
		// Constraint t arrives, associated with y_t
		DVec tRow = matrix.getRow(t);
		while (dot(tRow, x) < 1) {
			// 1. Update primal variables
			for (unsigned j = 0; j < n; ++j) {
				if (tRow(j) > 0) {
					x(j) = x(j) +
						(tRow(j) * x(j) + 1.0 / d) / derive(funs[j], x(j));
				}
			}

			// 2. Update dual variables
			for (unsigned j = 0; j < n; ++j) {
				mu[j] = derive(funs[j], delta * x(j));
			}
			double s = derive(funs[0], delta * x(0)) / derive(funs[0], x(0));
			for (unsigned k = 1; k < n; ++k) {
				s = std::min(
					s, derive(funs[k], delta * x(k)) / derive(funs[k], x(k)));
			}
			s *= c;
			y(t) += s;

			for (unsigned j = 0; j < n; ++j) {
				DVec jCol = DVec(matrix.getCol(j));
				if (checkError(dot(jCol, y), mu(j))) {
					unsigned ind = 0;
					for (unsigned i = 0; i < t; ++i) {
						if (y(i) > 0) {
							ind = i;
						}
						y(ind) -= jCol(t) / jCol(ind) * s;
					}
				}
			}
		}
	}

	return x;
}  // TODO: Test dense vs sparse performance
