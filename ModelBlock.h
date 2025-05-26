#ifndef MODELBLOCK_H
#define MODELBLOCK_H
#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include "../ModelCell/ModelCell.h"

using namespace std;
// Forward Declaration
class ModelCell;

// @class ModelBlock
// @brief Contains the data of an individual. Not even necessarily on the board.
// This isn't generally for direct use. Usually, you will use the pointer to a ModelBlock as an argument
// to a BlockController class contruction if you want to modify a block.
class ModelBlock {
    using CellMx = std::vector<std::vector<ModelCell>>;
    std::vector<ModelCell> OwnedCells;
    std::vector<std::vector<ModelCell>> CellMatrix;

    public:
        int LifeSpan;
        int Level;
        // @brief Default ctor
        ModelBlock (std::vector<ModelCell> c, int ls, int lv);
        std::vector<ModelCell> &GetCells();
};

#endif // ModelBlock.h
