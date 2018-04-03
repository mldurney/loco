#include "loco.hpp"

int main() {
    Matrix matrix;
    MatrixSolution s = solve(matrix);

    //std::cout << "Printing matrix...\n";
    //matrix.printDense();

    std::cout << "Solution: [ ";
    for (auto& xi : s.primals) {
        std::cout << xi << " ";
    }
    std::cout << "]" << std::endl;

    std::cout << "Messages: " << s.messages << std::endl << std::endl;
}