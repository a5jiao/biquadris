#ifndef MODEL_H
#define MODEL_H
#include <utility>
#include <vector>
#include <string>

#include "ModelPlayer/ModelPlayer.h"
#include "../View/subject.h"  

// @class ModelContainer
// @brief This is a class that aggregates all the components of the "model"
//
// ModelContainer (MC) is pointed to by the Controller class
class ModelContainer: public Subject {
    // @brief Stores the two Players within the Model
    std::pair<ModelPlayer, ModelPlayer> ModelPlayers;
    int highScore;

    public:
        // @param p The player to access [0,1]
        // @return ModelPlayer reference
        ModelPlayer &GetPlayer(int p);

        // Accessor for high score
        int getHighScore() const;

        // @brief Default ctor – initializes two ModelPlayers using ModelPlayer() default ctors
        ModelContainer();

        void updateHighScore(int newScore);
};
#endif // ModelContainer.h
