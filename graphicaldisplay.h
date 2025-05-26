#ifndef GRAPHICALDISPLAY_H
#define GRAPHICALDISPLAY_H

#include "observer.h"
#include "../ModelContainer/ModelContainer.h"
#include "subject.h"
#include "window.h"

#include <iostream>

class GraphicalDisplay: public Observer {
    shared_ptr<ModelContainer> subject;
    shared_ptr<Xwindow> window;

    public:
        GraphicalDisplay(shared_ptr<ModelContainer> s);
        void drawRectangleOutline(int x, int y, int width, int height, int color);
        int getColor(char value) const;
        void notify(set<pair<int,int>> &redraw) override;
        void draw();
        void drawNextBlock(BlockType type, int baseX, int baseY); 
        ~GraphicalDisplay();

};

#endif
