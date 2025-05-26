#include <utility>
#include <vector>
#include <string>

#include "ModelBlock.h"

using namespace std;

ModelBlock::ModelBlock (vector<ModelCell> c, int ls, int lv) : OwnedCells{c}, LifeSpan{ls}, Level{lv} {
}  

std::vector<ModelCell> &ModelBlock::GetCells() {
    return OwnedCells;
}


