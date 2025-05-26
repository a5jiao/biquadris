#include "subject.h"
#include "observer.h"

#include <iostream>
#include <vector>
#include <set>

using namespace std;

void Subject::attach(shared_ptr<Observer> o) {
    observers.push_back(o);
}

void Subject::detach(shared_ptr<Observer> o) {

}

void Subject::notifyObservers(set<pair<int,int>>& s) {
    
    if (observers.size() != 0) {
        for (int i = 0; i < observers.size(); ++i) {
            observers.at(i)->notify(s);
        }
    }
    // The != is not operational yet! Todo...
    /*for (auto it = observers.begin(); it != observers.end(); ++it) {
        (*it)->notify();
    }
    */
}

// Get state method goes here...
