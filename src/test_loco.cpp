#include "loco.hpp"

int main() {
    Matrix matrix;
    Solution s = loco(matrix);

    // std::cout << "Printing matrix..." << std::endl;
    // matrix.print();

    std::cout << "Solution: [ ";
    for (auto& xi : s.x) {
        std::cout << xi << " ";
    }
    std::cout << "]" << std::endl;

    std::cout << "Messages: " << s.messages << std::endl << std::endl;
}