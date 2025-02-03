/*
    -- BaseState Class --

    A tweaked class from Colton Ogden
    cogden@cs50.harvard.edu

    Used as the base class for all of our states, so we don't have to
    define empty methods in each of them. StateMachine requires each
    State have a set of four "interface" methods that it can reliably call,
    so by inheriting from this base state, our State classes will all have
    at least empty versions of these methods even if we don't define them
    ourselves in the actual classes.
*/

#ifndef LSTATE_H
#define LSTATE_H

#include <com/logger.h>

class LObserver;


class LState {
public:
    virtual void enter(LObserver* observer) = 0;
    virtual void exit() = 0;
    virtual void render() = 0;
    virtual void update() = 0;
};

#endif