#ifndef LOBSERVER_H
#define LOBSERVER_H

class LSubject;

class LObserver {
public:
    virtual ~LObserver() {};
    virtual void poll(LSubject* subject) = 0;
protected:
    LObserver() {};
};

#endif