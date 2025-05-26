#include "graphicaldisplay.h"
#include "../ModelContainer/ModelBoard/ModelCell/ModelCell.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <set>


using namespace std;

const int ROWS = 18;
const int COLUMNS = 11;
const int SPACE_BETWEEN_NEXT_BLOCKS = 22;

const int cellSize = 30; // Size of each cell in pixels
const int boardWidth = COLUMNS * cellSize; // Width of a board
const int boardHeight = ROWS * cellSize; // Height of a board
const int spaceBetweenBoards = 50; // Space between the two boards
const int player1X = 50; // X-offset for Player 1's board
const int player2X = player1X + boardWidth + spaceBetweenBoards; // X-offset for Player 2's board
const int boardY = 100; // Y-offset for both boards

GraphicalDisplay::GraphicalDisplay(shared_ptr<ModelContainer> s)
    : subject{s} {
        window = make_shared<Xwindow>(player2X + boardWidth + 50, boardY + boardHeight + 100);
        draw();
    }

void GraphicalDisplay::drawNextBlock(BlockType type, int baseX, int baseY) {
    // Size for each cell in the next block preview
    const int nextBlockCellSize = 20;
    
    // Get the color for the block type
    char blockChar;
    switch(type) {
        case BlockType::IBlock: blockChar = 'I'; break;
        case BlockType::JBlock: blockChar = 'J'; break;
        case BlockType::LBlock: blockChar = 'L'; break;
        case BlockType::OBlock: blockChar = 'O'; break;
        case BlockType::SBlock: blockChar = 'S'; break;
        case BlockType::ZBlock: blockChar = 'Z'; break;
        case BlockType::TBlock: blockChar = 'T'; break;
        default: return;
    }
    int color = getColor(blockChar);

    // Draw the block based on its type
    switch(type) {
        case BlockType::IBlock:
            // Draw horizontal I block: IIII
            for(int i = 0; i < 4; ++i) {
                window->fillRectangle(baseX + i * nextBlockCellSize, 
                                    baseY, 
                                    nextBlockCellSize, 
                                    nextBlockCellSize, 
                                    color);
            }
            break;
        
        case BlockType::JBlock:
            // Draw J block: J
            //              JJJ
            window->fillRectangle(baseX, baseY - nextBlockCellSize, 
                                nextBlockCellSize, nextBlockCellSize, color);
            for(int i = 0; i < 3; ++i) {
                window->fillRectangle(baseX + i * nextBlockCellSize, 
                                    baseY, 
                                    nextBlockCellSize, 
                                    nextBlockCellSize, 
                                    color);
            }
            break;
        
        case BlockType::LBlock:
            // Draw L block:   L
            //              LLL
            window->fillRectangle(baseX + 2 * nextBlockCellSize, 
                                baseY - nextBlockCellSize, 
                                nextBlockCellSize, 
                                nextBlockCellSize, 
                                color);
            for(int i = 0; i < 3; ++i) {
                window->fillRectangle(baseX + i * nextBlockCellSize, 
                                    baseY, 
                                    nextBlockCellSize, 
                                    nextBlockCellSize, 
                                    color);
            }
            break;
        
        case BlockType::OBlock:
            // Draw O block: OO
            //              OO
            for(int i = 0; i < 2; ++i) {
                for(int j = 0; j < 2; ++j) {
                    window->fillRectangle(baseX + i * nextBlockCellSize, 
                                        baseY + j * nextBlockCellSize - nextBlockCellSize, 
                                        nextBlockCellSize, 
                                        nextBlockCellSize, 
                                        color);
                }
            }
            break;
        
        case BlockType::SBlock:
            // Draw S block:  SS
            //              SS
            for(int i = 1; i < 3; ++i) {
                window->fillRectangle(baseX + i * nextBlockCellSize, 
                                    baseY - nextBlockCellSize, 
                                    nextBlockCellSize, 
                                    nextBlockCellSize, 
                                    color);
            }
            for(int i = 0; i < 2; ++i) {
                window->fillRectangle(baseX + i * nextBlockCellSize, 
                                    baseY, 
                                    nextBlockCellSize, 
                                    nextBlockCellSize, 
                                    color);
            }
            break;
        
        case BlockType::ZBlock:
            // Draw Z block: ZZ
            //               ZZ
            for(int i = 0; i < 2; ++i) {
                window->fillRectangle(baseX + i * nextBlockCellSize, 
                                    baseY - nextBlockCellSize, 
                                    nextBlockCellSize, 
                                    nextBlockCellSize, 
                                    color);
            }
            for(int i = 1; i < 3; ++i) {
                window->fillRectangle(baseX + i * nextBlockCellSize, 
                                    baseY, 
                                    nextBlockCellSize, 
                                    nextBlockCellSize, 
                                    color);
            }
            break;
        
        case BlockType::TBlock:
            // Draw T block: TTT
            //               T
            for(int i = 0; i < 3; ++i) {
                window->fillRectangle(baseX + i * nextBlockCellSize, 
                                    baseY - nextBlockCellSize, 
                                    nextBlockCellSize, 
                                    nextBlockCellSize, 
                                    color);
            }
            window->fillRectangle(baseX + nextBlockCellSize, 
                                baseY, 
                                nextBlockCellSize, 
                                nextBlockCellSize, 
                                color);
            break;
    }
}

// Simulating a border with thin rectangles
void GraphicalDisplay::drawRectangleOutline(int x, int y, int width, int height, int color) {
    // Top border
    window->fillRectangle(x, y, width, 1, color); // 1-pixel thick
    // Bottom border
    window->fillRectangle(x, y + height - 1, width, 1, color);
    // Left border
    window->fillRectangle(x, y, 1, height, color);
    // Right border
    window->fillRectangle(x + width - 1, y, 1, height, color);
}

int GraphicalDisplay::getColor(char value) const {
    switch (value) {
        case 'I':
            // return 1;
            return Xwindow::Cyan;
        case 'J':
            // return 2;   
            return Xwindow::Blue;
        case 'L':   
            // return 3;   
            return Xwindow::Orange;
        case 'O':   
            // return 4;   
            return Xwindow::Yellow;
        case 'S':   
            // return 5;   
            return Xwindow::Green;
        case 'Z':   
            // return 7;   
            return Xwindow::Red;
        case 'T':   
            // return 6;   
            return Xwindow::Purple;
        case '*':
            // return 8;   
            return Xwindow::Brown;
        case '?':
            // return 9;   
            return Xwindow::Black;
        default:
            // return 0;   
            return Xwindow::White;
    }
}
void GraphicalDisplay::notify(set<pair<int,int>> &s) {
    auto &player1Board = subject->GetPlayer(0).GetBoard();
    auto &player2Board = subject->GetPlayer(1).GetBoard();
    for (auto it = s.begin(); it != s.end(); ++it) {
            int col = (*it).first;
            int row = (*it).second;
            int color = getColor(player1Board.GetCell(*it).GetValue());
            // Render cell
            window->fillRectangle(player1X + col * cellSize, boardY + (ROWS - 1 - row) * cellSize, cellSize, cellSize, color);
            color = getColor(player2Board.GetCell(*it).GetValue());
            window->fillRectangle(player2X + col * cellSize, boardY + (ROWS - 1 - row) * cellSize, cellSize, cellSize, color);
        }
    window->fillRectangle(0, 0, player2X + boardWidth + 50, 60, 0);
    window->fillRectangle(0, 660, player2X + boardWidth + 50, 20, 0);
    // Draw high score
    std::string highScoreString = "High Score:   " + std::to_string(subject->getHighScore());
    window->drawString(player1X - 1, 20, highScoreString);

    // Draw levels
    std::string levelString = "Player 1 Level: " + std::to_string(subject->GetPlayer(0).GetPlayerLevelAndScore().first)
                            + "                                               "
                            + "Player 2 Level: " + std::to_string(subject->GetPlayer(1).GetPlayerLevelAndScore().first);
    window->drawString(player1X - 1, 40, levelString);

    // Draw scores
    std::string scoreString = "Player 1 Score: " + std::to_string(subject->GetPlayer(0).GetPlayerLevelAndScore().second)
                              + "                                               "
                              + "Player 2 Score: " + std::to_string(subject->GetPlayer(1).GetPlayerLevelAndScore().second);
    window->drawString(player1X - 1, 60, scoreString);
    
    // Draw "Next:" labels
    std::string nextString = "Next:                                                                Next:";
    window->drawString(player1X - 1, 660, nextString);

    // Clear the specific areas where next blocks will be drawn
    // Player 1 next block area
    window->fillRectangle(player1X, 680, 100, 60, Xwindow::White);
    // Player 2 next block area
    window->fillRectangle(player2X, 680, 100, 60, Xwindow::White);

    // Draw the next blocks
    drawNextBlock(subject->GetPlayer(0).next.first, player1X, 700);
    drawNextBlock(subject->GetPlayer(1).next.first, player2X, 700);
    s.clear();
}

void GraphicalDisplay::draw() {
    window->fillRectangle(0, 0, window->getWidth(), window->getHeight(), Xwindow::White);
    
    // Draw Player 1's Board Border
    
    // Draw Player 1's Board
    auto &player1Board = subject->GetPlayer(0).GetBoard();
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLUMNS; ++col) {
            char value = player1Board.GetCell(pair{col, row}).GetValue();
            int color = getColor(value);
            

            // Render cell
            window->fillRectangle(player1X + col * cellSize, boardY + (ROWS - 1 - row) * cellSize, cellSize, cellSize, color);
        }
    }
    
    // Draw Player 2's Board Border
    
    // Draw Player 2's Board
    auto &player2Board = subject->GetPlayer(1).GetBoard();
    // for (int row = ROWS - 1; row >= 0; --row) {
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLUMNS; ++col) {
            char value = player2Board.GetCell(pair{col, row}).GetValue();
            int color = getColor(value); 

            // Render cell 
            window->fillRectangle(player2X + col * cellSize, boardY + (ROWS - 1 - row) * cellSize, cellSize, cellSize, color);
        }
    }

    // Render the high scores
    std::string highScoreString = "High Score:   " + std::to_string(subject->getHighScore());
    window->drawString(player1X - 1, 20, highScoreString);

    // Render the levels
    std::string levelString = "Player 1 Level: " + std::to_string(subject->GetPlayer(0).GetPlayerLevelAndScore().first)
                            + "                                               "  // Space between player1 and player2
                            + "Player 2 Level: " + std::to_string(subject->GetPlayer(1).GetPlayerLevelAndScore().first);
    window->drawString(player1X - 1, 40, levelString);

    // Render the scores
    std::string scoreString = "Player 1 Score: " + std::to_string(subject->GetPlayer(0).GetPlayerLevelAndScore().second)
                              + "                                               "  // Space between player1 and player2
                              + "Player 2 Score: " + std::to_string(subject->GetPlayer(1).GetPlayerLevelAndScore().second);
    window->drawString(player1X - 1, 60, scoreString);
    
    // Render next blocks (for both players)
    std::string nextString = std::string("Next:") + std::string("                                                          ") + std::string("Next:"); 
    window->drawString(player1X - 1, 660, nextString);
    drawRectangleOutline(player1X - 1, boardY - 1, boardWidth + 2, boardHeight + 2, Xwindow::Black);
    drawRectangleOutline(player2X - 1, boardY - 1, boardWidth + 2, boardHeight + 2, Xwindow::Black);


}

GraphicalDisplay::~GraphicalDisplay() {
    subject->detach(shared_ptr<GraphicalDisplay>(this));
}
