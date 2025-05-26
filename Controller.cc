#include <utility>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <random>
#include <cmath>
#include <fstream>
#include <set>

#include "Controller.h"
#include "../ModelContainer/ModelBoard/ModelBoard.h"

#include <unordered_map>


const int COLUMNS = 11;  // already added in ModelBoard.cc, do I need it here?
const int ROWS = 15;  // same as above
const int RESERVE_ROWS = 3;  

using namespace std;



BlockType Controller::ReadBlock(std::istream &is) {
    string s;
    is >> s;
    char c = toupper(s[0]);
    if (c == 'I') return BlockType::IBlock;
    if (c == 'J') return BlockType::JBlock;
    if (c == 'L') return BlockType::LBlock;
    if (c == 'O') return BlockType::OBlock;
    if (c == 'S') return BlockType::SBlock;
    if (c == 'Z') return BlockType::ZBlock;
    if (c == 'T') return BlockType::TBlock;
    return BlockType::XBlock;
}


Controller::Controller(int t, int sd, int l, string _sf1, string _sf2, set<pair<int,int>>& _s)
    : s(_s)  // Initialize reference member s with the argument _s
{
    Model = make_shared<ModelContainer>();

    // Set text-based flag
    txtbased = t;

    // Set seed
    randseed = sd;
    srand(randseed);

    // Set scripts
    sf1 = _sf1;
    sf2 = _sf2;

    // Set starting Levels
    Model->GetPlayer(0).GetPlayerLevelAndScore() = {l, 0};
    Model->GetPlayer(1).GetPlayerLevelAndScore() = {l, 0};
}


Controller::~Controller() {

}

ModelBoard Controller::MakeBlind(ModelBoard blindboard){
    for (int x = 2; x < 9; ++x) {
        for (int y = 11; y > 3; --y) {
            pair<int,int> apair = {x, y};
            this->s.insert(apair);
            blindboard.GetCell(apair).GetValue() = '?';
        }
    }
    return blindboard;
}

void Controller::SmartNotify(Controller::BoardController &player) {
    ModelPlayer& p = player.GetPlayer();
    SpecialAction sp = p.next.second;
     if (sp == SpecialAction::BLIND || sp == SpecialAction::HEAVYBLIND) {
        ModelBoard org = player.GetPlayer().GetBoard();
        player.GetPlayer().GetBoard() = MakeBlind(org);
        this->Model->notifyObservers(this->s); 
        player.GetPlayer().GetBoard() = org;
    } else {
        this->Model->notifyObservers(this->s); 
    }
}

int Controller::ReadCommand(std::istream &is, Controller::BoardController &player) {
    ModelPlayer& p = player.GetPlayer();
    loop:
    Controller::BlockController block = player.AddBlock(p.next.first);
    if (!([&]() {
        // Lambda Conditional Function
        shared_ptr<ModelBlock> mb = block.GetBlock();
        for (auto blockcell : mb->GetCells()) {
            ModelBoard board = p.GetBoard();                         // This block is a lambda
            pair <int,int> cc = blockcell.GetCoordinates();          // function that checks
            cc = {cc.first+1, cc.second};                            // for any collision
            if (p.GetBoard().GetCell(cc).GetOwnerBlock() != mb && board.GetCell(cc).GetOwnerBlock() != nullptr) {return false;}} return true;}() )) {
        block.Clear();
        return -1;
    }
    SpecialAction sp = p.next.second;
    string s;
    ModelBoard blindboard = player.GetPlayer().GetBoard();  
    block.Place();
    SmartNotify(player);
    while (is >> s) {

        // Parse multiplier if present
        int multiplier = 1;
        size_t cmdStart = 0;
        
        if (s.empty()) block.Clear();
        if (!s.empty() && isdigit(s[0])) {
            multiplier = 0;
            while (cmdStart < s.length() && isdigit(s[cmdStart])) {
                multiplier = multiplier * 10 + (s[cmdStart] - '0');
                cmdStart++;
            }
        }
        // Extract command
        string cmdStr = s.substr(cmdStart);

        // Find the matching full command
        string fullCommand;
        auto it = commandMap.find(cmdStr);
        if (it != commandMap.end()) {
            fullCommand = it->second;
        }
        s = fullCommand;

        if (s == "left") {
            for (int i = 0; i < multiplier; i++) {
                block.HorizontalShift(-1);
            }
            if (sp == SpecialAction::HEAVY || sp == SpecialAction::HEAVYBLIND) {
                block.VerticalShift(-1);
                if (!block.VerticalShift(-1)) {
                    SmartNotify(player);
                    return 0;
                }
            }
            SmartNotify(player);
        } else if (s == "right") {
            for (int i = 0; i < multiplier; i++) {
                block.HorizontalShift(1);
            }
            if (sp == SpecialAction::HEAVY || sp == SpecialAction::HEAVYBLIND) {
                block.VerticalShift(-1);
                if (!block.VerticalShift(-1)) {
                    SmartNotify(player);
                    return 0;
                }
            }
            SmartNotify(player);
        } else if (s == "down") {
            for (int i = 0; i < multiplier; i++) {
                block.VerticalShift(-1);
            }
            SmartNotify(player);
        } else if (s == "clockwise") {
            for (int i = 0; i < multiplier; i++) {
                block.Rotate(1);
            }
            if (sp == SpecialAction::HEAVY || sp == SpecialAction::HEAVYBLIND) {
                block.VerticalShift(-1);
                if (!block.VerticalShift(-1)) {
                    SmartNotify(player);
                    return 0;
                }
            }
            
            SmartNotify(player);
        } else if (s == "counterclockwise") {
            for (int i = 0; i < multiplier; i++) {
                block.Rotate(-1);
            }
            if (sp == SpecialAction::HEAVY || sp == SpecialAction::HEAVYBLIND) {
                block.VerticalShift(-1);
                if (!block.VerticalShift(-1)) {
                    SmartNotify(player);
                    return 0;
                }
            }
            SmartNotify(player);
        } else if (s == "drop") { // multiplier not implemented
            block.Drop();
            p.GetBoard().PushBlock(block.GetBlock());
            for (int x = 2; x < 9; ++x) {
                for (int y = 11; y > 3; --y) {
                    pair<int,int> apair = {x, y};
                    this->s.insert(apair);
                }
            }
            SmartNotify(player); 
            for (int i = 0; i < multiplier - 1; ++i) {
                p.next = player.GenerateBlock(p.GetPlayerLevelAndScore().first);
                BlockController db = player.AddBlock(p.next.first);
                db.Place();
                db.Drop();
                p.GetBoard().PushBlock(db.GetBlock());
                SmartNotify(player);
            }
            return 0;
        } else if (s == "levelup") {
            pair<int, int>& ls = player.GetPlayer().GetPlayerLevelAndScore();
            for (int i = 0; i < multiplier; i++) {
                ls = {min(4, ls.first + 1), ls.second};
            }
            SmartNotify(player);
        } else if (s == "leveldown") {
            pair<int, int>& ls = player.GetPlayer().GetPlayerLevelAndScore();
            for (int i = 0; i < multiplier; i++) {
                ls = {max(0, ls.first - 1), ls.second};
            }
            SmartNotify(player);
        } else if (s == "restart") {
            this->Model->GetPlayer(0).GetBoard().ClearBoard();
            this->Model->GetPlayer(0).GetPlayerLevelAndScore() = {this->Model->GetPlayer(0).GetPlayerLevelAndScore().first,0};
            this->Model->GetPlayer(1).GetBoard().ClearBoard();
            this->Model->GetPlayer(1).GetPlayerLevelAndScore() = {this->Model->GetPlayer(1).GetPlayerLevelAndScore().first,0};
            for (int x = 0; x < COLUMNS; ++x) {
                for (int y = 0; y < ROWS + RESERVE_ROWS; ++y) {
                    pair<int,int> apair = {x, y};
                    this->s.insert(apair);
                }
            }
            block.Clear();
            SmartNotify(player);
            return -1;
        } else if (s == "norandom") {
            player.IsRandom = 0;
            is >> s;
            player.ifs = ifstream{s};
            SmartNotify(player);
        } else if (s == "random") {
            player.IsRandom = 1;
            SmartNotify(player);
        } else if (s == "sequence") {
            block.Clear();
            return 3;
        } else {
            istringstream is{cmdStr}; 
            BlockType bt = BlockType::XBlock;
            if ((bt = ReadBlock(is)) != BlockType::XBlock) {
                block.Clear();            
                player.GetPlayer().next = {bt, player.GetPlayer().next.second};
                goto loop;
            } 
        }
    }
    block.Clear();
    return 4;
}

int Controller::ReadSpecialAction(std::istream &is, BoardController otherplayer){
    string s;
    while (is >> s) {
        if (s == "blind") {
            pair<BlockType, SpecialAction>& next = otherplayer.GetPlayer().next;
            if (next.second == SpecialAction::HEAVY) {
                next = {next.first, SpecialAction::HEAVYBLIND};
            } else {
                next = {next.first, SpecialAction::BLIND};
            }
            return 1;
        } else if (s == "heavy") {
            pair<BlockType, SpecialAction>& next = otherplayer.GetPlayer().next;
            next = {next.first, SpecialAction::HEAVY};
            return 2;
        } else if (s == "force") {
            string b;
            pair<BlockType, SpecialAction>& next = otherplayer.GetPlayer().next;
            while (is >> b) {
                istringstream is{b}; 
                BlockType bt = BlockType::XBlock;
                while ((bt = ReadBlock(is)) == BlockType::XBlock);
                next = {bt, next.second};
                break;
            }
            return 3;
        }
    }
}

int Controller::PlayerTurn(int turncode, Controller::BoardController &P1BC, ModelPlayer &P1, Controller::BoardController &Other) {
    int score = P1.GetPlayerLevelAndScore().second;
    int level = P1.GetPlayerLevelAndScore().first;
    
    if (turncode == 0) { // Turn has ended
        pair<int, vector<int>> rowclears = P1BC.ClearRows();
        int& cintrv = P1.GetClearInterval();
        
        if (rowclears.first == -1) return -1;

        // After a successful drop, generate only the next block for THIS player
        // This replaces their "next" that was just used
        P1.next = P1BC.GenerateBlock(P1.GetPlayerLevelAndScore().first);
        
        if (rowclears.first > 0) {
            cintrv = 0;
            int block_score = 0;

            for (auto a : rowclears.second) {
                block_score += pow((a + 1), 2);
            }
            if (rowclears.first >= 2) ReadSpecialAction(cin, Other);
            int new_score = score + pow((level + rowclears.first), 2) + block_score;
            P1.GetPlayerLevelAndScore() = {level, new_score};
            Model->updateHighScore(new_score);
            return 1;
        }
        
        if (rowclears.first == 0) ++cintrv;

        if (cintrv != 0 && cintrv % 5 == 0 && P1.GetPlayerLevelAndScore().first == 4) {
            BlockController xb = P1BC.AddBlock(BlockType::XBlock);
            xb.Drop();
            P1.GetBoard().PushBlock(xb.GetBlock());
        }
        
        rowclears = P1BC.ClearRows();
        if (rowclears.first == -1) return -1;
        if (rowclears.first > 0) {
            cintrv = 0;
            int block_score = 0;
            for (auto a : rowclears.second) {
                block_score += pow((a + 1), 2);
            }
            if (rowclears.first >= 2) ReadSpecialAction(cin, Other);
            int new_score = score + pow((level + rowclears.first), 2) + block_score;
            P1.GetPlayerLevelAndScore() = {level, new_score};
            Model->updateHighScore(new_score);
        }
        return 1;
    }
    else if (turncode == 3) {
        return 3;
    }
    else if (turncode == 4) {
        return 4;
    }
    else if (turncode == 8) {
        return 8;
    }
    else if (turncode == -1) return -1;
    
    return 1;
}

void Controller::PlayerLoop(ModelPlayer& player, BoardController& bc, BoardController& op, ifstream& sq, int& sqp) {
    while (true) {

        if (sqp) {
            if (!sq) {
                sq.clear(); 
                sqp = 0;
                cin.clear();
            }
        }

        int turncode = (sqp == 0) ? ReadCommand(cin, bc) : ReadCommand(sq, bc);
        int t = PlayerTurn(turncode, bc, player, op);
        
        if (t == -1) { 
            this->Model->GetPlayer(0).GetBoard().ClearBoard();
            this->Model->GetPlayer(0).GetPlayerLevelAndScore() = {this->Model->GetPlayer(0).GetPlayerLevelAndScore().first,0};
            this->Model->GetPlayer(1).GetBoard().ClearBoard();
            this->Model->GetPlayer(1).GetPlayerLevelAndScore() = {this->Model->GetPlayer(1).GetPlayerLevelAndScore().first,0};
            for (int x = 0; x < COLUMNS; ++x) {
                for (int y = 0; y < ROWS + RESERVE_ROWS; ++y) {
                    pair<int,int> apair = {x, y};
                    this->s.insert(apair);
                }
            }
            SmartNotify(bc);
        }
        else if (t == 1) { break; }
        else if (t == 3) {
            string s;
            cin >> s;
            sq = ifstream(s);
            if (!sq) {
                sqp = 0;
            } else {
                sqp = 1;
            }
        }
        else if (t == 4) {
        }
        else {
            break;
        }
    }
}


void Controller::GameplayLoop() {
    while (true) {
        // Text display
        
        // TextDisplay *td = new TextDisplay{Model};

        // Graphical display
        // Model->attach(new GraphicalDisplay{Model});
        // GraphicalDisplay *gd = new GraphicalDisplay{Model};

        // Call default ctors for both Players
        ModelPlayer &P1 = Model->GetPlayer(0); 
        ifstream _t1{sf1};
        ifstream &ifs1{_t1};
        BoardController P1BC = BoardController{shared_ptr<Controller>(this), P1, ifs1, 0};

        ModelPlayer &P2 = Model->GetPlayer(1);
        ifstream _t2{sf2};
        ifstream &ifs2{_t2};
        BoardController P2BC = BoardController{shared_ptr<Controller>(this), P2, ifs2, 1};

        // Initialize both players' next blocks right at the start
        P1.next = P1BC.GenerateBlock(P1.GetPlayerLevelAndScore().first);
        P2.next = P2BC.GenerateBlock(P2.GetPlayerLevelAndScore().first);


        ifstream sq1;
        int sqp1 = 0;


        ifstream sq2;
        int sqp2 = 0;
        
        Model->attach(make_shared<TextDisplay>(Model));
        if (this->txtbased != 1) {
            Model->attach(make_shared<GraphicalDisplay>(Model));
        }

        while (true) {
            PlayerLoop(P1, P1BC, P2BC, sq1, sqp1);
            PlayerLoop(P2, P2BC, P1BC, sq2, sqp2);
        }
    }
}
