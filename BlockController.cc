#include <utility>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <random>
#include <cmath>
#include <climits>

#include "../Controller.h"
#include "../../ModelContainer/ModelBoard/ModelBoard.h"

// for debugging (ashton)
// #include <climits>

const int COLUMNS = 11;  // already added in ModelBoard.cc, do I need it here?
const int ROWS = 15;  // same as above
const int RESERVE_ROWS = 3;  

using namespace std;

int Controller::BlockController::VerticalShift(int y) {
    // Vector of all the cells presently owned by the Block
    vector <ModelCell> cells = CtrlBlock->GetCells();
    ModelBoard &board = Player.GetBoard();
    int return_code = 1;
    // Compute the minimum x and max x values for the block
    int min_y = min_element(cells.begin(), cells.end(), [](auto lhs, auto rhs) {
        return lhs.GetCoordinates().second < rhs.GetCoordinates().second;})->GetCoordinates().second;
    int max_y = max_element(cells.begin(), cells.end(), [](auto lhs, auto rhs) {
        return lhs.GetCoordinates().second < rhs.GetCoordinates().second;})->GetCoordinates().second;
    if (y > 0 && max_y < ROWS + RESERVE_ROWS - 1 && [&]() { 
        // Lambda Conditional Function
        for (auto blockcell : CtrlBlock->GetCells()) {              // This block is a lambda
            pair <int,int> cc = blockcell.GetCoordinates();         // function that checks
            cc = {cc.first, cc.second + 1};                         // for any collision
            if (board.GetCell(cc).GetOwnerBlock() != CtrlBlock && board.GetCell(cc).GetOwnerBlock() != nullptr) {return_code = 0; return false;}} return true;}() ) {
        for (auto& blockcell : CtrlBlock->GetCells()) {             // Actual true condition body
            pair <int,int>& cc = blockcell.GetCoordinatesRef();
            this->Parent->s.insert(cc);
            board.GetCell(cc).Clear();
            cc = {cc.first, cc.second + 1};
            this->Parent->s.insert(cc);
        }
    }
    else if (y < 0 && min_y > 0 && [&]() { 
        // Lambda Conditional Function
        for (auto blockcell : CtrlBlock->GetCells()) {             // This block is a lambda
            pair <int,int> cc = blockcell.GetCoordinates();        // function that checks
            cc = {cc.first, cc.second - 1};                        // for any collision
            if (board.GetCell(cc).GetOwnerBlock() != CtrlBlock && board.GetCell(cc).GetOwnerBlock() != nullptr) {return_code = 0; return false;}} return true;}() )  {

        for (auto& blockcell : CtrlBlock->GetCells()) {             // Actual true condition body
            pair <int,int>& cc = blockcell.GetCoordinatesRef();
            this->Parent->s.insert(cc);
            board.GetCell(cc).Clear();
            cc = {cc.first, cc.second - 1};
            this->Parent->s.insert(cc);
        }
    }
    else return_code = 0;
    Place();

    min_y = min_element(CtrlBlock->GetCells().begin(), CtrlBlock->GetCells().end(), [](auto lhs, auto rhs) {
        return lhs.GetCoordinates().second < rhs.GetCoordinates().second;})->GetCoordinates().second;
    
    if (min_y == 0) return 0;
    for (auto blockcell : CtrlBlock->GetCells()) {             // This block is a lambda
            pair <int,int> cc = blockcell.GetCoordinates();         // function that checks
            cc = {cc.first, cc.second - 1};                           // for any collision
            if (board.GetCell(cc).GetOwnerBlock() != CtrlBlock && board.GetCell(cc).GetOwnerBlock() != nullptr) {return 0;}} 
    return return_code;
}

void Controller::BlockController::HorizontalShift(int x) {
    // Vector of all the cells presently owned by the Block
    vector <ModelCell> cells = CtrlBlock->GetCells();
    ModelBoard &board = Player.GetBoard();
    // Compute the minimum x and max x values for the block
    int min_x = min_element(cells.begin(), cells.end(), [](auto lhs, auto rhs) {
        return lhs.GetCoordinates().first < rhs.GetCoordinates().first;})->GetCoordinates().first;
    int max_x = max_element(cells.begin(), cells.end(), [](auto lhs, auto rhs) {
        return lhs.GetCoordinates().first < rhs.GetCoordinates().first;})->GetCoordinates().first;
    if (x > 0 && max_x < COLUMNS - 1 && [&]() { 
        // Lambda Conditional Function
        for (auto blockcell : CtrlBlock->GetCells()) {             // This block is a lambda
            pair <int,int> cc = blockcell.GetCoordinates();         // function that checks
            cc = {cc.first+1, cc.second};                           // for any collision
            if (board.GetCell(cc).GetOwnerBlock() != CtrlBlock && board.GetCell(cc).GetOwnerBlock() != nullptr) {return false;}} return true;}() ) {
        for (auto& blockcell : CtrlBlock->GetCells()) {             // Actual true condition body
            pair <int,int>& cc = blockcell.GetCoordinatesRef();
            board.GetCell(cc).Clear();
            this->Parent->s.insert(cc);
            cc = {cc.first+1, cc.second};
            this->Parent->s.insert(cc);
        }
    }
    else if (x < 0 && min_x > 0 && [&]() { 
        // Lambda Conditional Function
        for (auto blockcell : CtrlBlock->GetCells()) {             // This block is a lambda
            pair <int,int> cc = blockcell.GetCoordinates();         // function that checks
            cc = {cc.first-1, cc.second};                           // for any collision
            if (board.GetCell(cc).GetOwnerBlock() != CtrlBlock && board.GetCell(cc).GetOwnerBlock() != nullptr) {return false;}} return true;}() )  {

        for (auto& blockcell : CtrlBlock->GetCells()) {             // Actual true condition body
            pair <int,int>& cc = blockcell.GetCoordinatesRef();
            this->Parent->s.insert(cc);
            board.GetCell(cc).Clear();
            cc = {cc.first-1, cc.second};
            this->Parent->s.insert(cc);
        }
    }
    Place();
}

void Controller::BlockController::Place() {
    // Iterate through the 
    for (auto c : CtrlBlock->GetCells()) {
        this->Parent->s.insert(c.GetCoordinates());
        Player.GetBoard().GetCell(c.GetCoordinates()).GetValue() =  Player.GetBoard().GetStandardBlock(Type).second;
        Player.GetBoard().GetCell(c.GetCoordinates()).GetOwnerBlock() = CtrlBlock;
    }
}

void Controller::BlockController::Clear() {
    for (auto c : CtrlBlock->GetCells()) {
        // This obviously needs to play the correct character. Oh well. Todo
        this->Parent->s.insert(c.GetCoordinates());
        Player.GetBoard().GetCell(c.GetCoordinates()).GetValue() = ' ';
        Player.GetBoard().GetCell(c.GetCoordinates()).GetOwnerBlock() = nullptr;
    }
}

void Controller::BlockController::Drop() {

    while (VerticalShift(-1));
}
void Controller::BlockController::Rotate(int r) {
    vector <ModelCell> cells = CtrlBlock->GetCells();
    ModelBoard &board = Player.GetBoard();
    int min_x = min_element(cells.begin(), cells.end(), [](auto lhs, auto rhs) {
        return lhs.GetCoordinates().first < rhs.GetCoordinates().first;})->GetCoordinates().first;
    int min_y = min_element(cells.begin(), cells.end(), [](auto lhs, auto rhs) {
        return lhs.GetCoordinates().second < rhs.GetCoordinates().second;})->GetCoordinates().second;
    int new_min_x = INT_MAX; int new_min_y = INT_MAX;
    std::vector<ModelCell> NewOwnedCells;
    if (r > 0 && [&]() {
        int _max_x = INT_MIN;
        int _max_y = INT_MIN;
        int _min_x = INT_MAX;
        int _min_y = INT_MAX;
        for (auto blockcell : CtrlBlock->GetCells()) {
            pair<int,int> cc = blockcell.GetCoordinates();
            int x = cc.first; int y = cc.second;
            int relative_x = x - min_x; int relative_y = y - min_y;
            int rot_x = relative_y; int rot_y = 2 - relative_x;
            _max_x = max(min_x + rot_x, _max_x); 
            _max_y = max(min_y + rot_y, _max_y); 
            _min_x = min(min_x + rot_x, _min_x); 
            _min_y = min(min_y + rot_y, _min_y); 
            cc = {min_x + rot_x, min_y + rot_y};
            if ( !(_max_x < COLUMNS) || !(_max_y < ROWS + RESERVE_ROWS) || !(_min_x >= 0) || !(_max_y >= 0) ) return false;
            if (board.GetCell(cc).GetOwnerBlock() != CtrlBlock && board.GetCell(cc).GetOwnerBlock() != nullptr) return false;
        }
        return true;
    }()) {
        for (auto& blockcell : CtrlBlock->GetCells()) {
            pair<int,int>& cc = blockcell.GetCoordinatesRef();
            board.GetCell(cc).Clear();
            this->Parent->s.insert(cc);
            int x = cc.first; int y = cc.second;
            int relative_x = x - min_x; int relative_y = y - min_y;
            int rot_x = relative_y; int rot_y = 2 - relative_x;
            cc = {min_x + rot_x, min_y + rot_y};
            this->Parent->s.insert(cc);
            new_min_x = min(new_min_x, min_x + rot_x);
            new_min_y = min(new_min_y, min_y + rot_y);
        }
    }
    else if (r < 0 && [&]() {
        int _max_x = INT_MIN;
        int _max_y = INT_MIN;
        int _min_x = INT_MAX;
        int _min_y = INT_MAX;
        for (auto blockcell : CtrlBlock->GetCells()) {
            pair<int,int> cc = blockcell.GetCoordinates();
            int x = cc.first; int y = cc.second;
            int relative_x = x - min_x; int relative_y = y - min_y;
            int rot_x = relative_y; int rot_y = 2 - relative_x;
            _max_x = max(min_x + rot_x, _max_x); 
            _max_y = max(min_y + rot_y, _max_y); 
            _min_x = min(min_x + rot_x, _min_x); 
            _min_y = min(min_y + rot_y, _min_y); 
            cc = {min_x + rot_x, min_y + rot_y};
            if ( !(_max_x < COLUMNS) || !(_max_y < ROWS + RESERVE_ROWS) || !(_min_x >= 0) || !(_max_y >= 0) ) return false;
            if (board.GetCell(cc).GetOwnerBlock() != CtrlBlock && board.GetCell(cc).GetOwnerBlock() != nullptr) return false;
        }
        return true;
    }()) {
        for (auto& blockcell : CtrlBlock->GetCells()) {
            pair<int,int>& cc = blockcell.GetCoordinatesRef();
            this->Parent->s.insert(cc);
            board.GetCell(cc).Clear();
            int x = cc.first; int y = cc.second;
            
            int relative_x = x - min_x; int relative_y = y - min_y;
            int rot_x = 2 - relative_y; int rot_y = relative_x;
            cc = {min_x + rot_x, min_y + rot_y};
            this->Parent->s.insert(cc);
            new_min_x = min(new_min_x, min_x + rot_x);
            new_min_y = min(new_min_y, min_y + rot_y);
        }
    }
    else {
        Place();
        return;
    }
    if ( (new_min_x != min_x) || (new_min_y != min_y) ) {
        int hshift = min_x - new_min_x;
        int vshift = min_y - new_min_y;
        for (auto& blockcell : CtrlBlock->GetCells()) {
            pair<int,int>& cc = blockcell.GetCoordinatesRef();
            this->Parent->s.insert(cc);
            cc = {cc.first + hshift, cc.second + vshift};
            this->Parent->s.insert(cc);
        }
    }
    Place();
}
shared_ptr<ModelBlock> Controller::BlockController::GetBlock(){
    return CtrlBlock;
}
