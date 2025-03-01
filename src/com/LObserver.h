#ifndef LOBSERVER_H
#define LOBSERVER_H

class LSubject;

class LObserver {
public:
    virtual ~LObserver() {};
    virtual void poll(LSubject* subject, int value) = 0;
protected:
    LObserver() {};
};

#endif