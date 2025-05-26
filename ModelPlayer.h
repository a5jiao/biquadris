#ifndef MODELPLAYER_H
#define MODELPLAYER_H
#include <utility>
#include <vector>
#include <string>

#include "../ModelBoard/ModelBoard.h"

enum class SpecialAction {
    NONSPECIAL,
    EMPTY,
    HEAVY,
    BLIND,
    HEAVYBLIND
};


// @class ModelPlayer
// @brief Contains data pertaining to (and owned by) an individual player
class ModelPlayer {
    ModelBoard PlayerBoard;
    std::pair <int, int> PlayerLevelAndScore; 
    int ClearInterval;
    public:
        // @brief Default ctor
        ModelPlayer();
        // @brief Returns a reference to the Player's board
        ModelBoard &GetBoard();
        // @brief Returns a reference pair of ints containing the Level [0] and Score [1]
        std::pair <int, int> &GetPlayerLevelAndScore();
        int &GetClearInterval();
        std::pair<BlockType, SpecialAction> next;
};
#endif // ModelPlayer.h
