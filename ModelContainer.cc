#include "ModelContainer.h"
#include <utility>
#include <vector>
#include <string>

#include "ModelPlayer/ModelPlayer.h"


ModelPlayer &ModelContainer::GetPlayer(int p) {
    switch (p) {
        case 0:
            return ModelPlayers.first;
        case 1:
            return ModelPlayers.second;
        default:
            return ModelPlayers.first;
    }
}

int ModelContainer::getHighScore() const {
    return highScore;
}

ModelContainer::ModelContainer(): highScore{0} {
    ModelPlayers = std::pair<ModelPlayer, ModelPlayer> {ModelPlayer(), ModelPlayer()};
}

void ModelContainer::updateHighScore(int newScore) {
    if (newScore > highScore) {
        highScore = newScore;
    }
}
