#include <iostream>
#include "InputHandler.h"
#include <chrono>  



int main() {
    // Record start time
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    auto start = std::chrono::high_resolution_clock::now();

    // Initialize the InputHandler
    InputHandler inputHandler;

    // Parsing input from standard input and directly outputting the result
    inputHandler.parseInput();

    // Record end time
    auto end = std::chrono::high_resolution_clock::now();
    
    // Calculate elapsed time
    std::chrono::duration<double> elapsed = end - start;

    std::cerr << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;
    return 0;
}
