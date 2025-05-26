#ifndef MODELCELL_H
#define MODELCELL_H
#include <utility>
#include <vector>
#include <string>
#include <memory>

using namespace std;


class ModelBlock;

// @class ModelCell
// @brief Contains the data of a particular (x,y) in a Player's board. Owned by a Board, which is owned by a Player
class ModelCell {
    std::pair <int, int> CellIndex;
    char Value;
    shared_ptr<ModelBlock> OwnerBlock;
    public:
        // @brief Default ctor
        ModelCell(int x, int y);
        // @brief Get the (x,y) coordinates of the cell. Note that this is assuming the bottom left is (0,0), and top left is (0, height(board))
        // @return pair <int, int> of form <x,y>
        std::pair <int, int> GetCoordinates(); // Should probably be const but idk
        std::pair <int, int> &GetCoordinatesRef(); // For special occasions
        // @brief Get the value of a cell.
        // @return char &c
        char &GetValue();
        void Clear();
        // @brief Get the owner block of a cell. Can be mutated to refer to a different ModelBlock.
        // @return ModelBlock *&mb, i.e., a reference to the pointer. 
        std::shared_ptr<ModelBlock>& GetOwnerBlock();

        void SetOwnerBlock(shared_ptr<ModelBlock> block);
};

#endif // ModelCell.h
