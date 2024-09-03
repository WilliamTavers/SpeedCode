// InputHandler.cpp
#include "InputHandler.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>

// Include OpenMP headers only if OpenMP is enabled
#ifdef _OPENMP
#include <omp.h>
#endif


InputHandler::InputHandler() : dimensions(6), tagTable(52) {}

void InputHandler::parseInput() {

    std::string line;
    std::getline(std::cin, line);
    parseDimensions(line);

    parseTagTable();  

    parseBlockData();
}


void InputHandler::parseDimensions(const std::string& line) {
    // new approach
    size_t start = 0;
    size_t end = 0;

    for (int i = 0; i < 6; ++i) {
        end = line.find(',', start);
        dimensions[i] = std::stoi(line.substr(start, end - start));
        start = end + 1;
    }
}

void InputHandler::parseTagTable() {
    std::string line;
    while (std::getline(std::cin, line) && !line.empty()) {
        size_t commaPos = line.find(',');
        if (commaPos == std::string::npos) {
            break;
        }
        char tag = line[0];
        std::string label = line.substr(commaPos + 1);
        label.erase(0, label.find_first_not_of(" \t"));
        label.erase(label.find_last_not_of(" \t") + 1);
        
        int index = (tag >= 'a') ? (tag - 'a' + 26) : (tag - 'A'); // second half if lower case
        tagTable[index] = label;
    }
}


void InputHandler::parseBlockData() {
    int cols = dimensions[0], rows = dimensions[1], slices = dimensions[2];
    int parentX = dimensions[3], parentY = dimensions[4], parentZ = dimensions[5];

    std::ostringstream resultStream;

    // Parallelize the loop only if OpenMP is available
    #ifdef _OPENMP
    #pragma omp parallel
    {
        std::ostringstream localStream;

        #pragma omp for nowait
        for (int z = 0; z < slices; z += parentZ) {
            for (int y = 0; y < rows; y += parentY) {
                std::string line;
                for (int subY = 0; subY < parentY && (y + subY) < rows; ++subY) {
                    std::getline(std::cin, line);

                    for (int x = 0; x < cols; x += parentX) {
                        for (int subX = 0; subX < parentX && (x + subX) < cols; ++subX) {
                            char tag = line[x + subX];
                            int index = (tag >= 'a') ? (tag - 'a' + 26) : (tag - 'A');
                            const std::string& label = tagTable[index];

                            char formattedBlock[128];
                            int len = sprintf(formattedBlock, "%d,%d,%d,%d,%d,%d,", x + subX, y + subY, z, 1, 1, 1);
                            memcpy(formattedBlock + len, label.c_str(), label.size());
                            formattedBlock[len + label.size()] = '\n';
                            formattedBlock[len + label.size() + 1] = '\0';

                            localStream << formattedBlock;
                        }
                    }
                }
            }
            std::string line;
            std::getline(std::cin, line); // Read the blank line separating slices
        }

        #pragma omp critical
        {
            resultStream << localStream.str();
        }
    }
    #else
    // Non-parallel version (no OpenMP)
    for (int z = 0; z < slices; z += parentZ) {
        for (int y = 0; y < rows; y += parentY) {
            std::string line;
            for (int subY = 0; subY < parentY && (y + subY) < rows; ++subY) {
                std::getline(std::cin, line);

                for (int x = 0; x < cols; x += parentX) {
                    for (int subX = 0; subX < parentX && (x + subX) < cols; ++subX) {
                        char tag = line[x + subX];
                        int index = (tag >= 'a') ? (tag - 'a' + 26) : (tag - 'A');
                        const std::string& label = tagTable[index];

                        char formattedBlock[128];
                        snprintf(formattedBlock, sizeof(formattedBlock), "%d,%d,%d,%d,%d,%d,%s\n", x + subX, y + subY, z, 1, 1, 1, label.c_str());
                        resultStream << formattedBlock;
                    }
                }
            }
            std::getline(std::cin, line); // Read the blank line separating slices
        }
    }
    #endif

    std::cout << resultStream.str();
}
