#ifndef OBSERVER_H
#define OBSERVER_H
#include <set>
#include <utility>
#include <memory>

using namespace std;

class Observer {
    public:
        // Pure virtual notify method
        virtual void notify(set<pair<int,int>> &s) = 0;
        virtual ~Observer() = default;
};

#endif
