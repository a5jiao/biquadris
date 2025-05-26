#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <iomanip>
#include <sstream>

#include "ModelBoard.h"
#include "ModelCell/ModelCell.h"

const int SPACE_BETWEEN_CONTENT = 10;
const int SPACE_BETWEEN_NEXT_BLOCKS = 22;
const int COLUMNS = 11;
const int ROWS = 18;
const int RESERVE_ROWS = 3;  

ModelBoard::ModelBoard() {
    // CREATES THE BOARD:
    BoardMatrix = std::vector<std::vector<ModelCell>>();
    for (int col = 0; col < COLUMNS; ++col) {
        std::vector<ModelCell> vectorcol;
        for (int row = 0; row < ROWS; ++row) {
            vectorcol.emplace_back(ModelCell(COLUMNS-1-col, ROWS-1-row));
        }
        BoardMatrix.emplace_back(vectorcol);
    }

    ModelBoard::DefBlockMap = {
        {BlockType::IBlock, {{   ModelBoard::GetCell({0,14}), ModelBoard::GetCell({1,14}),
                                ModelBoard::GetCell({2,14}), ModelBoard::GetCell({3,14})}   , 'I'}},
        {BlockType::JBlock, {{   ModelBoard::GetCell({0,14}), ModelBoard::GetCell({1,14}),
                                ModelBoard::GetCell({2,14}), ModelBoard::GetCell({0,15})}   , 'J'}},
        {BlockType::LBlock, {{   ModelBoard::GetCell({0,14}), ModelBoard::GetCell({1,14}),
                                ModelBoard::GetCell({2,14}), ModelBoard::GetCell({2,15})}   , 'L'}},
        {BlockType::OBlock, {{   ModelBoard::GetCell({0,14}), ModelBoard::GetCell({1,14}),
                                ModelBoard::GetCell({0,15}), ModelBoard::GetCell({1,15})}   , 'O'}},
        {BlockType::SBlock, {{   ModelBoard::GetCell({0,14}), ModelBoard::GetCell({1,14}),
                                ModelBoard::GetCell({1,15}), ModelBoard::GetCell({2,15})}   , 'S'}},
        {BlockType::ZBlock, {{   ModelBoard::GetCell({1,14}), ModelBoard::GetCell({2,14}),
                                ModelBoard::GetCell({0,15}), ModelBoard::GetCell({1,15})}   , 'Z'}},
        {BlockType::TBlock, {{   ModelBoard::GetCell({0,15}), ModelBoard::GetCell({1,15}),
                                ModelBoard::GetCell({2,15}), ModelBoard::GetCell({1,14})}   , 'T'}},
        {BlockType::XBlock, {{   ModelBoard::GetCell({5, ROWS - 1})    }   , '*'}}
    };
}

std::pair<std::vector<ModelCell>, char> ModelBoard::GetStandardBlock (BlockType t) {
    return DefBlockMap.at(t);
}

ModelCell &ModelBoard::GetCell(std::pair<int, int> c) {
    int x = c.first; int y = c.second;
    try {
        return (BoardMatrix.at(COLUMNS - 1 - x)).at(ROWS - 1 - y);
    } catch (...) {
        std::cout << "Out of bounds";
    }

}


std::vector<std::vector<ModelCell>> &ModelBoard::GetBoardMatrix() {
    return BoardMatrix;
}

std::vector<shared_ptr<ModelBlock>> &ModelBoard::GetBlocks() {
    return BoardBlocks;
}



void ModelBoard::ClearBoard() {
    for (int x = 0; x < COLUMNS; ++x) {
        for (int y = 0; y < ROWS - 1; ++ y) {
            pair<int,int> cc = {x,y};
            this->GetCell(cc).Clear();
        }
    }
    BoardBlocks = {};
}

void ModelBoard::PushBlock(shared_ptr<ModelBlock> mb) {
    BoardBlocks.push_back(mb);
}

std::ostream &operator<<(std::ostream &os, const BlockType bt) {
    switch (bt) {
        case BlockType::IBlock:
            os << "IIII\n    ";
            break;
        case BlockType::JBlock:
            os << "J\nJJJ ";
            break;
        case BlockType::LBlock:
            os << "  L\nLLL ";
            break;
        case BlockType::OBlock:
            os << "OO\nOO  ";
            break;
        case BlockType::SBlock:
            os << " SS\nSS  ";
            break;
        case BlockType::ZBlock:
            os << "ZZ\n ZZ  ";
            break;
        case BlockType::TBlock:
            os << "TTT\n T  ";
            break;
    }
    return os;
}

// Overloaded output operator that prints one board
std::ostream &operator<<(std::ostream &os, ModelBoard mb) {
    // FOR DEBUGGING:
    os << '+';
    for (int c = 0; c < COLUMNS; ++c) {os << '-';}
    os << '+';
    os << std::endl;
    for (int r = 0; r < ROWS; ++r) {
        os << '|';
        for (int c = 0; c < COLUMNS; ++c) {
            os << (((mb.GetBoardMatrix().at(c)).at(r)).GetValue());
        }
        os << '|';
        os << std::endl;
    }
    os << '+';
    for (int c = 0; c < COLUMNS; ++c) {os << '-';}
    os << '+';
    return os;
}

// Overloaded output operator that prints both boards in correct format
std::ostream &operator<<(std::ostream &os, std::pair<ModelBoard, ModelBoard> boards) {

    // Top border
    os << '+';
    for (int c = 0; c < COLUMNS; ++c) {os << '-';}
    os << '+';
    os << setw(SPACE_BETWEEN_CONTENT);
    os << '+';
    for (int c = 0; c < COLUMNS; ++c) {os << '-';}
    os << '+';
    os << endl;

    // Board contents
    for (int r = 15 + RESERVE_ROWS - 1; r >= 0; --r) {
        os << '|';
        for (int c = 0; c < COLUMNS; ++c) {
            os << boards.first.GetCell(pair{c,r}).GetValue();
        }
        os << '|';
        os << setw(SPACE_BETWEEN_CONTENT);
        os << '|';
        for (int c = 0; c < COLUMNS; ++c) {
            os << boards.second.GetCell(pair{c,r}).GetValue();
        }
        os << '|';
        os << endl;
    }

    // Bottom border
    os << '+';
    for (int c = 0; c < COLUMNS; ++c) {os << '-';}
    os << '+';
    os << setw(SPACE_BETWEEN_CONTENT);
    os << '+';
    for (int c = 0; c < COLUMNS; ++c) {os << '-';}
    os << '+';
    os << endl;

    return os;

}

std::ostream &operator<<(std::ostream &os, std::pair<BlockType, BlockType> blockTypes) {
    ostringstream next1, next2;
    next1 << blockTypes.first;
    next2 << blockTypes.second;

    istringstream stream1(next1.str());
    istringstream stream2(next2.str());
    
    vector<string> lines1, lines2;
    string line;

    // Store all lines from first block
    while (getline(stream1, line)) {
        lines1.push_back(line);
    }
    
    // Store all lines from second block
    while (getline(stream2, line)) {
        lines2.push_back(line);
    }

    // Fixed spacing from the start of line to where player 2's block should begin
    const int FIXED_PLAYER2_POSITION = SPACE_BETWEEN_NEXT_BLOCKS;  // Adjust this number as needed

    // Output the lines
    size_t maxLines = max(lines1.size(), lines2.size());
    for (size_t i = 0; i < maxLines; i++) {
        if (i < lines1.size()) {
            os << lines1[i];
        }
        
        if (i < lines2.size()) {
            // Calculate remaining spaces needed
            int spacesNeeded = FIXED_PLAYER2_POSITION;
            if (i < lines1.size()) {
                spacesNeeded -= lines1[i].length();
            }
            os << string(spacesNeeded, ' ') << lines2[i];
        }
        os << endl;
    }

    return os;
}
