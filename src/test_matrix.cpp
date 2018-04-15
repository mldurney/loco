#include "matrix.hpp"

const unsigned SIZE = 10;
const unsigned M = 15;
const unsigned N = 20;
const double P = 5 / (double)N;

void testMatrix(Matrix* m) {
	bool isGood;
	std::cout << "Checking each row for nonzero...\t";
	int badRow = -1;
	for (unsigned row = 0; row < m->getRows(); ++row) {
		bool hasNonzero = false;

		for (unsigned col = 0; col < m->getCols(); ++col) {
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
	for (unsigned col = 0; col < m->getCols(); ++col) {
		bool hasNonzero = false;

		for (unsigned row = 0; row < m->getRows(); ++row) {
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
	isGood = checkError(m->getCell(0, 0), 0) && !(m->isOccupied(0));
	std::cout << (isGood ? "OK" : "FAILED") << std::endl;

	isGood = true;
	std::cout << "Testing set/get cell (value 1)...\t";
	m->setCell(0, 1);
	isGood =
		checkError(m->getCell(0, 0) - 1, 0) < EPSILON && m->isOccupied(0, 0);
	std::cout << (isGood ? "OK" : "FAILED") << std::endl;

	// std::cout << "Testing get row...\n";
	// std::cout << "Row 0: \n" << DVec(m->getRow(0)) << "\n";

	// std::cout << "Testing get col...\n";
	// std::cout << "Col 0: \n" << DVec(m->getCol(0)) << "\n";

	isGood = true;
	std::cout << "Testing submatrix...\t\t\t";
	uivector rows;
	for (int i = (int)sqrt(m->getRows()) - 1; i >= 0; --i) {
		rows.push_back(rand() % m->getRows());
	}
	uivector cols;
	for (int i = (int)sqrt(m->getCols()) - 1; i >= 0; --i) {
		cols.push_back(rand() % m->getCols());
	}
	Matrix sub = m->getSubmatrix(rows, cols);
	for (unsigned i = 0; i < rows.size(); ++i) {
		for (unsigned j = 0; j < cols.size(); ++j) {
			if (!checkError(sub.getCell(i, j), m->getCell(rows[i], cols[j])) ||
				sub.isOccupied(i, j) != m->isOccupied(rows[i], cols[j])) {
				isGood = false;
				break;
			}
		}
	}
	std::cout << (isGood ? "OK" : "FAILED") << std::endl;

	std::cout << "Printing submatrix..." << std::endl;
	sub.printDense();

	std::cout << "Printing matrix..." << std::endl;
	m->printDense();

	std::cout << "Printing matrix occupancy..." << std::endl;
	m->printSparse();

	std::cout << std::endl << std::endl;
}

int main() {
	Matrix* mDefault = new Matrix;
	Matrix* mSize = new Matrix(SIZE);
	Matrix* mMN = new Matrix(M, N, P);

	// std::cout << "Testing default matrix..." << std::endl << std::endl;
	// testMatrix(mDefault);
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