#include "ModelPlayer.h"
#include <utility>
#include <vector>
#include <string>
#include <stack>

ModelPlayer::ModelPlayer() {
    PlayerBoard = ModelBoard();
    PlayerLevelAndScore = std::pair <int, int> {0,0};
    ClearInterval = 0;
    next = {BlockType::LBlock,SpecialAction::NONSPECIAL}; 
}

ModelBoard &ModelPlayer::GetBoard() {
    return PlayerBoard;
}

std::pair <int, int> &ModelPlayer::GetPlayerLevelAndScore() {
    return PlayerLevelAndScore;
}

int &ModelPlayer::GetClearInterval() {
    return ClearInterval;
}
