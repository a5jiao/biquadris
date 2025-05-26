#include "textdisplay.h"

#include <iostream>
#include <iomanip> 
#include <functional> 

using namespace std;

const int SPACE_BETWEEN_PLAYER_INFO = 16;
const int SPACE_BETWEEN_INFO = 6;
const int SPACE_BETWEEN_CONTENT = 10;

const int COLUMNS = 11;  // already added in ModelBoard.cc, do I need it here?
const int ROWS = 15;  // same as above
const int RESERVE_ROWS = 3;  

TextDisplay::TextDisplay(shared_ptr<ModelContainer> mc):
    subject{mc} {}

TextDisplay::~TextDisplay() {
    subject->detach(shared_ptr<Observer>(this));  
}

void TextDisplay::notify(set<pair<int,int>> &s) {

    // Displaying high score
    cout << "High Score:" << setw(SPACE_BETWEEN_INFO) << subject->getHighScore() << endl;

    // Displaying levels
    cout << "Level:" << setw(SPACE_BETWEEN_INFO) << (subject->GetPlayer(0)).GetPlayerLevelAndScore().first
         << setw(SPACE_BETWEEN_PLAYER_INFO) 
         << "Level:" << setw(SPACE_BETWEEN_INFO) << (subject->GetPlayer(1)).GetPlayerLevelAndScore().first;
    cout << endl;

    // Displaying scores 
    cout << "Score:" << setw(SPACE_BETWEEN_INFO) << (subject->GetPlayer(0)).GetPlayerLevelAndScore().second
         << setw(SPACE_BETWEEN_PLAYER_INFO) 
         << "Score:" << setw(SPACE_BETWEEN_INFO) << (subject->GetPlayer(1)).GetPlayerLevelAndScore().second;
    cout << endl;

    // Displaying game board 
    ModelBoard mb1 = subject->GetPlayer(0).GetBoard();
    ModelBoard mb2 = subject->GetPlayer(1).GetBoard();

    cout << make_pair(ref(mb1), ref(mb2));

    // Displaying next block
    cout << "Next:      " 
         << setw(SPACE_BETWEEN_PLAYER_INFO) 
         << "Next:" << endl;

    BlockType next1 = subject->GetPlayer(0).next.first;
    BlockType next2 = subject->GetPlayer(1).next.first;

    cout << make_pair(ref(next1), ref(next2));
    
}
