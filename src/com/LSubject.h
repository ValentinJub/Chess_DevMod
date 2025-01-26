#ifndef LSUBJECT_H
#define LSUBJECT_H

#include <vector>
#include "LObserver.h"

class LSubject {
public:
    virtual void Attach(LObserver* observer);
    virtual void Detach(LObserver* observer);
    virtual void Notify();
protected:
    LSubject();
private:
    std::vector<LObserver*> mObservers;
};

#endif