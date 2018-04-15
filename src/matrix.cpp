#include "matrix.hpp"

Matrix::Matrix(unsigned r, unsigned c, double p, double noise)
	: rows(r), cols(c), cells(r * c) {
	// Create uniform random real number generator for range [0, 1)
	unsigned seed =
		(unsigned)std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine gen(seed);
	std::uniform_real_distribution<double> dist(0, 1);

	// Generate random order for filling rows and columns in matrix
	uivector rowOrder(rows);
	uivector colOrder(cols);
	for (unsigned row = 0; r < rows; ++row) {
		rowOrder[row] = row;
	}
	for (unsigned col = 0; col < cols; ++col) {
		colOrder[col] = col;
	}
	std::shuffle(rowOrder.begin(), rowOrder.end(), gen);
	std::shuffle(colOrder.begin(), colOrder.end(), gen);

	// Generate (row, col, value) triplets to insert into matrix
	std::vector<T> triplets;  // Values to insert into matrix

	// Cycle through rows, columns in random order so each has >= 1 nonzero
	if (rows >= cols) {
		for (unsigned row = 0, col = 0; row < rows; ++row, ++col) {
			if (col >= cols) {
				col = 0;
			}
			triplets.push_back(T(rowOrder[row], colOrder[col], dist(gen)));
		}
	} else {
		for (unsigned row = 0, col = 0; col < cols; ++row, ++col) {
			if (row >= rows) {
				row = 0;
			}
			triplets.push_back(T(rowOrder[row], colOrder[col], dist(gen)));
		}
	}

	// Construct constraint matrix
	matrix = SpMat(rows, cols);
	matrix.setFromTriplets(triplets.begin(), triplets.end());

	// Generate noisy indices (1-D) from sparsity probability
	uivector noisyIndices;
	for (unsigned i = 0; i < cells; ++i) {
		if (dist(gen) < p) {
			noisyIndices.push_back(i);
		}
	}

	// Next, cycle through noisy 1-D indices, adding corresponding triplets
	triplets.clear();
	for (unsigned i : noisyIndices) {
		triplets.push_back(T(toRow(i), toCol(i), dist(gen)));
	}

	// Construct sparsity noise matrix
	SpMat sparsityMatrix(rows, cols);
	sparsityMatrix.setFromTriplets(triplets.begin(), triplets.end());

	// Add sparsity noise to constraint matrix
	matrix += sparsityMatrix;

	// Normalize columns of constraint matrix
	for (unsigned col = 0; col < cols; ++col) {
		matrix.col(col) /= matrix.col(col).norm();
	}

	b = matrix * DVec::Random(cols) + noise * DVec::Random(rows);
}

Matrix::Matrix(unsigned r, unsigned c, const std::vector<T> triplets, DVec b_)
	: rows(r), cols(c), cells(r * c), b(b_) {
	// Construct constraint matrix
	matrix = SpMat(rows, cols);
	matrix.setFromTriplets(triplets.begin(), triplets.end());
	// Check vector b matches number of rows (m) of matrix
	if (b.size() != rows) {
		std::cout << "Matrix ERROR: b, rows size mismatch!\n" << std::endl;
		exit(EXIT_FAILURE);
	}
}

Matrix Matrix::getSubmatrix(uivector rows_, uivector cols_) const {
	checkRows(rows_);
	checkCols(cols_);

	DVec b_ = DVec(rows_.size());  // All b_i corresponding to indices in rows_
	std::map<unsigned, unsigned> mapCols, mapRows;
	for (unsigned i = 0; i < cols_.size(); ++i) {
		mapCols[cols_[i]] = i;
	}
	for (unsigned i = 0; i < rows_.size(); ++i) {
		mapRows[rows_[i]] = i;
		b_(i) = b(rows_[i]);
	}

	std::vector<T> triplets;    // Values to insert into submatrix
	for (unsigned c : cols_) {  // Iterate through columns in cols
		for (SpMat::InnerIterator it(matrix, c); it;
			 ++it) {  // Iterate through rows in matrix
			if (std::find(rows_.begin(), rows_.end(), it.row()) !=
				rows_.end()) {
				triplets.push_back(
					T(mapRows[(unsigned)it.row()], mapCols[c], it.value()));
			}
		}
	}

	return std::move(
		Matrix((unsigned)rows_.size(), (unsigned)cols_.size(), triplets, b_));
}

DMat Matrix::getDenseSubmatrix(uivector rows_, uivector cols_) const {
	checkRows(rows_);
	checkCols(cols_);

	std::map<unsigned, unsigned> mapCols, mapRows;
	for (unsigned i = 0; i < cols_.size(); ++i) {
		mapCols[cols_[i]] = i;
	}
	for (unsigned i = 0; i < rows_.size(); ++i) {
		mapRows[rows_[i]] = i;
	}

	std::vector<T> triplets;    // Values to insert into submatrix
	for (unsigned c : cols_) {  // Iterate through columns in cols
		for (SpMat::InnerIterator it(matrix, c); it;
			 ++it) {  // Iterate through rows in matrix
			if (std::find(rows_.begin(), rows_.end(), it.row()) !=
				rows_.end()) {
				triplets.push_back(
					T(mapRows[(unsigned)it.row()], mapCols[c], it.value()));
			}
		}
	}

	SpMat sparse(rows_.size(), cols_.size());
	sparse.setFromTriplets(triplets.begin(), triplets.end());
	return DMat(sparse);
}

unsigned Matrix::checkInd(unsigned ind) const {
	if (ind > cells) {
		std::cout << "checkInd ERROR: ind exceeds number of cells\n"
			<< std::endl;
		exit(EXIT_FAILURE);
	}
	return ind;
}

unsigned Matrix::checkRow(unsigned r) const {
	if (r > rows) {
		std::cout << "checkRow ERROR: row exceeds number of rows\n"
			<< std::endl;
		exit(EXIT_FAILURE);
	}
	return r;
}

unsigned Matrix::checkCol(unsigned c) const {
	if (c > cols) {
		std::cout << "checkCol ERROR: col exceeds number of columns\n"
			<< std::endl;
		exit(EXIT_FAILURE);
	}
	return c;
}

void Matrix::checkRows(uivector rows_) const {
	for (unsigned r : rows_) {
		checkRow(r);
	}
}

void Matrix::checkCols(uivector cols_) const {
	for (unsigned c : cols_) {
		checkCol(c);
	}
}

double Matrix::getCell(unsigned r, unsigned c) const {
	checkRow(r);
	checkCol(c);
	return matrix.coeff(r, c);
}

double Matrix::getCell(unsigned ind) const {
	checkInd(ind);
	return matrix.coeff(toRow(ind), toCol(ind));
}

SpVec Matrix::getRow(unsigned r) const {
	checkRow(r);
	return matrix.row(r);
}

SpVec Matrix::getCol(unsigned c) const {
	checkCol(c);
	return matrix.col(c);
}

std::vector<T> Matrix::getTriplets() const {
	std::vector<T> triplets;
	for (unsigned i = 0; i < matrix.outerSize(); ++i) {
		for (SpMat::InnerIterator it(matrix, i); it; ++it) {
			triplets.push_back(
				T((unsigned)it.row(), (unsigned)it.col(), it.value()));
		}
	}
	return triplets;
}

void Matrix::setCell(unsigned r, unsigned c, double val) {
	checkRow(r);
	checkCol(c);
	matrix.coeffRef(r, c) = val;
	if (val < EPSILON && val > -EPSILON) {
		matrix.prune(0.0);
	}
}

void Matrix::clearCell(unsigned r, unsigned c) {
	checkRow(r);
	checkCol(c);
	matrix.coeffRef(r, c) = 0;
	matrix.prune(0.0);
}

bool Matrix::isOccupied(unsigned r, unsigned c) const {
	checkRow(r);
	checkCol(c);
	for (SpMat::InnerIterator it(matrix, c); it; ++it) {
		if (it.row() == r) {
			return true;
		}
	}
	return false;
}

void Matrix::printDense() const {
	DMat dense(matrix);
	Eigen::IOFormat clean(3, 0, ", ", "\n", "[", "]");
	std::cout << "Printing " << rows << "x" << cols << " dense matrix: \n";
	std::cout << dense.format(clean) << std::endl;
}

void Matrix::printSparse() const {
	std::cout << "Printing " << rows << "x" << cols << " sparse matrix: \n";
	for (unsigned i = 0; i < matrix.outerSize(); ++i) {
		for (SpMat::InnerIterator it(matrix, i); it; ++it) {
			std::cout << "( " << it.row() << ", \t" << it.col() << ", \t"
				<< it.value() << " )\n";
		}
	}
	std::cout << std::endl;
}
