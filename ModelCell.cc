#include <utility>
#include <vector>
#include <string>

#include "ModelCell.h"
#include "../ModelBlock/ModelBlock.h"


ModelCell::ModelCell(int x, int y) {
    Value = ' ';
    CellIndex = std::pair <int, int> {x, y};
    OwnerBlock = nullptr;
}

std::pair <int, int> ModelCell::GetCoordinates() {
    return CellIndex;
}

std::pair <int, int> &ModelCell::GetCoordinatesRef() {
    return CellIndex;
}

char &ModelCell::GetValue() {
    return Value;
}
shared_ptr<ModelBlock>& ModelCell::GetOwnerBlock() {
    return OwnerBlock;
}

void ModelCell::SetOwnerBlock(shared_ptr<ModelBlock> block) {
    OwnerBlock = std::move(block);
}

void ModelCell::Clear() {
    this->OwnerBlock.reset();
    this->Value = ' ';
}