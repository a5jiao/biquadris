#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <utility>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <fstream>
#include <set>
#include <memory>

#include <unordered_map>

#include "../ModelContainer/ModelContainer.h"
#include "../View/subject.h"
#include "../View/observer.h"
#include "../View/textdisplay.h"
#include "../View/graphicaldisplay.h"
#include "../View/window.h"



class Controller {

    class BlockController {
        shared_ptr<Controller> Parent;
        shared_ptr<ModelBlock> CtrlBlock;
        ModelPlayer &Player;
        BlockType Type;
        public: 
            shared_ptr<ModelBlock> GetBlock();
            void Rotate(int r);
            void HorizontalShift(int x);
            int VerticalShift(int y);
            void Drop();
            void Place();
            void Clear();
            BlockController(shared_ptr<Controller> parent, shared_ptr<ModelBlock> block, ModelPlayer &player, BlockType type)
            : Parent(parent), CtrlBlock(block), Player(player), Type{type} {}
            BlockController& operator=(BlockController&& other)  {
                if (this != &other) {  // Self-assignment check
                    Clear();  // Reset the current object's state if necessary
                    
                    // Move resources from `other` to `*this`
                    Parent = std::move(other.Parent);
                    CtrlBlock = std::move(other.CtrlBlock);
                    Player = other.Player;  // Assuming `ModelPlayer &Player` remains the same
                    Type = other.Type;

                    // Leave `other` in a valid state
                    other.Clear();
                }
                return *this;
            }
    };
    class BoardController {
        shared_ptr<Controller> Parent;
        ModelPlayer &CtrlPlayer;
        int pnum;
        // ifstream sf;
        public: 
            ifstream &ifs;
            BlockController AddBlock(BlockType type);
            int IsRandom;
            pair<BlockType, SpecialAction> GenerateBlock(int level);
            pair<int, vector<int>>  ClearRows();
            ModelPlayer &GetPlayer();
            BoardController(shared_ptr<Controller> parent, ModelPlayer &mp, ifstream &sf, int pnum)
            : Parent(parent), CtrlPlayer(mp), ifs(sf), pnum(pnum)  {IsRandom = 1;}
    };


    friend class BlockController;
    friend class BoardController;
    shared_ptr<ModelContainer> Model;

    public:
        int txtbased;
        int randseed;
        set<pair<int,int>> &s;
        string sf1;
        string sf2;

        Controller(int t,int sd,int l,string sf1,string sf2, set<pair<int,int>> &_s);
        ~Controller();
        void PlayerLoop(ModelPlayer& player, Controller::BoardController& bc, Controller::BoardController& op, ifstream& sq, int& sqp);
        void SmartNotify(Controller::BoardController &player);
        ModelBoard MakeBlind(ModelBoard blindboard);
        int ReadCommand(std::istream &is, BoardController &player);
        int ReadSpecialAction(std::istream &is, BoardController otherplayer); // OTHER player!
        BlockType ReadBlock(std::istream &is);
        void GameplayLoop();
        int PlayerTurn(int turncode, Controller::BoardController &P1BC, ModelPlayer &P1, Controller::BoardController &otherPlayer);
        const std::unordered_map<std::string, std::string> commandMap = {
        {"lef", "left"},
        {"left", "left"},
        {"ri", "right"},
        {"rig", "right"},
        {"righ", "right"},
        {"right", "right"},
        {"do", "down"},
        {"dow", "down"},
        {"down", "down"},
        {"dr", "drop"},
        {"dro", "drop"},
        {"drop", "drop"},
        {"cl", "clockwise"},
        {"clo", "clockwise"},
        {"cloc", "clockwise"},
        {"clock", "clockwise"},
        {"clockw", "clockwise"},
        {"clockwi", "clockwise"},
        {"clockwis", "clockwise"},
        {"clockwise", "clockwise"},
        {"co", "counterclockwise"},
        {"cou", "counterclockwise"},
        {"coun", "counterclockwise"},
        {"count", "counterclockwise"},
        {"counte", "counterclockwise"},
        {"counter", "counterclockwise"},
        {"counterc", "counterclockwise"},
        {"countercl", "counterclockwise"},
        {"counterclo", "counterclockwise"},
        {"countercloc", "counterclockwise"},
        {"counterclock", "counterclockwise"},
        {"counterclockw", "counterclockwise"},
        {"counterclockwi", "counterclockwise"},
        {"counterclockwis", "counterclockwise"},
        {"counterclockwise", "counterclockwise"},
        {"levelu", "levelup"},
        {"levelup", "levelup"},
        {"leveld", "leveldown"},
        {"leveldo", "leveldown"},
        {"leveldow", "leveldown"},
        {"leveldown", "leveldown"},
        {"n", "norandom"},
        {"no", "norandom"},
        {"nor", "norandom"},
        {"nora", "norandom"},
        {"noran", "norandom"},
        {"norand", "norandom"},
        {"norando", "norandom"},
        {"norandom", "norandom"},
        {"ra", "random"},
        {"ran", "random"},
        {"rand", "random"},
        {"rando", "random"},
        {"random", "random"},
        {"se", "sequence"},
        {"seq", "sequence"},
        {"sequ", "sequence"},
        {"seque", "sequence"},
        {"sequen", "sequence"},
        {"sequenc", "sequence"},
        {"sequence", "sequence"},
        {"re", "restart"},
        {"re", "restart"},
        {"res", "restart"},
        {"rest", "restart"},
        {"resta", "restart"},
        {"restar", "restart"},
        {"restart", "restart"},
        {"I", "I"},
        {"J", "J"},
        {"L", "L"},
        {"O", "O"},
        {"S", "S"},
        {"Z", "Z"},
        {"T", "T"}
    };
};
#endif // Controller.h
