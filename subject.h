#ifndef SUBJECT_H
#define SUBJECT_H
#include <vector>
#include "observer.h"
#include <set>
#include <memory>

class Observer;  // forward declaration

// What is our concrete subject class?
//  - ModelContainer?
//  - ModelBoard?

// Abstract subject
class Subject {
    std::vector<shared_ptr<Observer>> observers;

    public:
        void attach(shared_ptr<Observer> o);
        void detach(shared_ptr<Observer> o);

        void notifyObservers(set<pair<int,int>>&s);

        // Pure virtual get state method below
        
        virtual ~Subject() = default;
};

#endif
