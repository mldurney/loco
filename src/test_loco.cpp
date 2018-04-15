#include "loco.hpp"

int main() {
	// Create uniform random real number generator for range [0, 1)
	unsigned seed =
		(unsigned)std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine gen(seed);
	std::uniform_real_distribution<double> dist(0, 1);

	// Create matrix and set function pointer to online algorithm
	Matrix matrix;
	online alg = onlineFractional;

	// Generate vector of functions, such that f(x_i) = c x_i^2, 0 <= c < 1
	unsigned count = matrix.getCols();
	fvector funs;
	for (unsigned i = 0; i < count; ++i) {
		double c = dist(gen);
		std::function<double(double)> fun = [c](double x) { return c * x * x; };
		funs.push_back(fun);
	}

	MatrixSolution s = solve(alg, matrix, funs);

	// std::cout << "Printing matrix...\n";
	// matrix.printDense();

	std::cout << "Solution: [ ";
	for (auto& xi : s.primals) {
		std::cout << xi << " ";
	}
	std::cout << "]" << std::endl;

	std::cout << "Messages: " << s.messages << std::endl;

	std::cout << "Press [Enter] to quit...\n" << std::endl;
	std::cin.get();
}