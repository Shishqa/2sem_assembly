#include <iostream>
#include "vector.hpp"

int main() {

    Vector<int> vec(10, 100);
    std::cout << "5th elem = " << vec[5];
    std::cout << "\n";

    Vector<int> vec1(vec);    
    std::cout << "5th elem = " << vec[5];
    std::cout << "\n";

    Vector<int> vec2(std::move(vec1));
    std::cout << "5th elem = " << vec[5];
    std::cout << "\n";

    vec = vec2;
    std::cout << "\n";

    vec = std::move(vec2);
    std::cout << "\n";

    std::cout << "vec = [";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << " ";
    }
    std::cout << "]\n"; 

    vec.resize(6);
    std::cout << "vec = [";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << " ";
    }
    std::cout << "]\n";

    vec.resize(10);
    std::cout << "vec = [";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << " ";
    }
    std::cout << "]\n\n\n";

    Vector<Vector<int> > vecc(10, Vector<int>(5, 100));
    for (size_t i = 0; i < vecc.size(); ++i) {
        for (size_t j = 0; j < vecc[i].size(); ++j) {
            std::cout << vecc[i][j] << " ";   
        }
        std::cout << "\n";
    }

    vecc.push_back(std::move(Vector<int>(5, 5)));
    for (size_t i = 0; i < vecc.size(); ++i) {
        for (size_t j = 0; j < vecc[i].size(); ++j) {
            std::cout << vecc[i][j] << " ";   
        }
        std::cout << "\n";
    }

    std::cout << "hello\n";

    return 0;
}
