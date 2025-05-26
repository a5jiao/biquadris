#ifndef MODELBOARD_H
#define MODELBOARD_H
#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <memory>

#include "ModelCell/ModelCell.h"
#include "ModelBlock/ModelBlock.h"


enum class BlockType {
    IBlock,
    JBlock,
    LBlock,
    OBlock,
    SBlock,
    ZBlock,
    TBlock,
    XBlock
};

// @class ModelBoard
// @brief Owned by a Player. Contains the data pertaining to the player's board 
class ModelBoard {
    std::vector<std::vector<ModelCell>> BoardMatrix;
    std::vector<shared_ptr<ModelBlock>> BoardBlocks;
    std::map<BlockType, std::pair<std::vector<ModelCell>, char>> DefBlockMap;

    public:
        // @brief Default ctor
        ModelBoard();
        std::vector<shared_ptr<ModelBlock>> &GetBlocks();
        // @brief Get the BoardMatrix
        // @return A reference of form vector<vector<ModelCell>> (i.e., the BoardMatrix). This is modifiable.
        std::vector<std::vector<ModelCell>> &GetBoardMatrix();
        // @brief Get a Cell
        // @param c: pair<int,int> The coordinates of the cell to get
        // @return A reference to a ModelCell
        ModelCell &GetCell(std::pair<int, int> c);
        void ClearBoard();
        void PushBlock(shared_ptr<ModelBlock> mb);
        std::pair<std::vector<ModelCell>, char> GetStandardBlock (BlockType t);
};

std::ostream &operator<<(std::ostream &os, const ModelBoard mb);
std::ostream &operator<<(std::ostream &os, const BlockType bt);
std::ostream &operator<<(std::ostream &os, std::pair<ModelBoard, ModelBoard> boards);
std::ostream &operator<<(std::ostream &os, std::pair<BlockType, BlockType> blockTypes);

#endif // ModelPlayer.h
