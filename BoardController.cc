#include <utility>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <random>
#include <cmath>

#include "../Controller.h"
#include "../../ModelContainer/ModelBoard/ModelBoard.h"

const int COLUMNS = 11;  // already added in ModelBoard.cc, do I need it here?
const int ROWS = 15;  // same as above
const int RESERVE_ROWS = 3;  

using namespace std;

Controller::BlockController Controller::BoardController::AddBlock(BlockType type) {
    ModelBoard PlayerBoard = CtrlPlayer.GetBoard();
    pair<vector<ModelCell>, char> sb = PlayerBoard.GetStandardBlock(type);
    auto nb = std::make_shared<ModelBlock>(sb.first,0,CtrlPlayer.GetPlayerLevelAndScore().first);
    return BlockController(Parent, nb, CtrlPlayer, type);
}



pair<BlockType,SpecialAction> Controller::BoardController::GenerateBlock(int level) {
    int randomValue;
    while (true) {
        int nextChar = ifs.peek(); // Peek at the next character without consuming it

        if (nextChar == EOF) {
            // Reinitialize the stream if EOF is reached
            if (this->pnum == 0) {ifs = ifstream(this->Parent->sf1);}
            else if (this->pnum == 1) {ifs = ifstream(this->Parent->sf2);}
            break;
        } else if (nextChar == '\n') {
            // Consume the newline and continue reading
            ifs.get(); // Read (and discard) the newline
        } else {
            // A valid non-newline character is found; stop looping
            break;
        }
    }
    if (!this->IsRandom) {
        return {this->Parent->ReadBlock(ifs), 
                (level >= 3) ? SpecialAction::HEAVY : SpecialAction::NONSPECIAL};
    }

    switch (level) {
        case 1: // Skewed probability: S and Z 1/12, others 1/6
            if (!this->IsRandom) return {this->Parent->ReadBlock(ifs), SpecialAction::NONSPECIAL};
            randomValue = rand() % 12;
            if (randomValue < 2) return {BlockType::SBlock, SpecialAction::NONSPECIAL};
            if (randomValue < 4) return {BlockType::ZBlock, SpecialAction::NONSPECIAL};
            return {static_cast<BlockType>((randomValue - 2) / 2), SpecialAction::NONSPECIAL};

        case 2: // Equal probability
            if (!this->IsRandom) return {this->Parent->ReadBlock(ifs), SpecialAction::NONSPECIAL};
            randomValue = rand() % 6;
            return {static_cast<BlockType>(randomValue), SpecialAction::NONSPECIAL};

        case 3: // Skewed probability: S and Z 2/9, others 1/9
            if (!this->IsRandom) return {this->Parent->ReadBlock(ifs), SpecialAction::HEAVY};
            randomValue = rand() % 9;
            if (randomValue < 2) return {BlockType::SBlock, SpecialAction::HEAVY};
            if (randomValue < 4) return {BlockType::ZBlock, SpecialAction::HEAVY};
            return {static_cast<BlockType>((randomValue - 2)),SpecialAction::HEAVY};

        case 4: // Skewed probability: S and Z 2/9, others 1/9
            if (!this->IsRandom) return {this->Parent->ReadBlock(ifs), SpecialAction::HEAVY};
            randomValue = rand() % 9;
            if (randomValue < 2) return {BlockType::SBlock, SpecialAction::HEAVY};
            if (randomValue < 4) return {BlockType::ZBlock, SpecialAction::HEAVY};
            return {static_cast<BlockType>((randomValue - 2)),SpecialAction::HEAVY};
            
        default: // Default to equal probability
            return {this->Parent->ReadBlock(ifs), SpecialAction::NONSPECIAL};
    }
}
ModelPlayer &Controller::BoardController::GetPlayer() {
    return CtrlPlayer;
}

pair<int, vector<int>> Controller::BoardController::ClearRows() {
    ModelBoard &board = CtrlPlayer.GetBoard();
    pair<int,int> apair;
    pair<int,int> bpair;
    int rc = 0;
    vector <int> cleared_levels;

    // Game Over Check:
    for (int x = 0; x < COLUMNS; ++x) {
        apair = {x,ROWS - 1};
        if (board.GetCell(apair).GetOwnerBlock() != nullptr) {
            return {-1, cleared_levels};
        }
    } 

    for (int y = 0; y < ROWS; ++y) {
        int full = 1;
        for (int x = 0; x < COLUMNS; ++x) {
            apair = {x,y};
            if (board.GetCell(apair).GetOwnerBlock() == nullptr) {
                full = 0; 
                break;
            }
        } 
        if (full) {
            ++rc;
            
            
            for (int k = 0; k < COLUMNS; ++k) {
                for (int i = y; i < ROWS - 1; ++ i) {
                    
                    apair = {k, i};     // Cell below
                    bpair = {k, i + 1}; // Cell on top
                    this->Parent->s.insert(apair); this->Parent->s.insert(bpair);


                    
                    board.GetCell(apair).GetValue() = board.GetCell(bpair).GetValue();

                    shared_ptr<ModelBlock> &mb = board.GetCell(apair).GetOwnerBlock();


                    if (mb && mb->GetCells().size() != 0) {
                        for (auto c = mb->GetCells().begin(); c !=  mb->GetCells().end(); ++c ) {
                            if ((*c).GetCoordinates() == apair) {
                                mb->GetCells().erase(c);
                                cleared_levels.push_back(mb->Level);
                                break;
                            }
                        }
                    }
                    board.GetCell(apair).SetOwnerBlock(board.GetCell(bpair).GetOwnerBlock());
                }
            }
        --y;
        } else {
        }
    }
    return {rc, cleared_levels};
}