#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include "observer.h"
#include "../ModelContainer/ModelContainer.h"
#include "subject.h"

#include <iostream>
#include <memory>

class TextDisplay: public Observer {
    // Assuming we're using the ModelContainer as the concrete subject
    // This should be the only field
    // Scores and levels of Player 1 and 2 can be accessed through methods
    shared_ptr<ModelContainer> subject;

    public:
        TextDisplay(shared_ptr<ModelContainer> s);
        void notify(set<pair<int,int>> &s) override;
        ~TextDisplay();

};

#endif
