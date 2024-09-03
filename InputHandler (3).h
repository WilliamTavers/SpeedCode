#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <vector>
#include <unordered_map>
#include <string>

class InputHandler {
public:
    // Constructor
    InputHandler();

    // Main method to parse the input data
    void parseInput();

private:
    // Stores the dimensions of the block model (x_count, y_count, z_count, parent_x, parent_y, parent_z)
    std::vector<int> dimensions;


    // Stores the mapping of tags (characters) to labels as provided in the tag table
    std::vector<std::string> tagTable;

    // Helper method to parse the dimensions from the first line of input
    void parseDimensions(const std::string& line);

    // Helper method to parse the tag table from the input
    void parseTagTable();

    // Helper method to parse the block data and directly output it
    void parseBlockData();
};

#endif // INPUTHANDLER_H
