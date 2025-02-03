#ifndef LDECORATOR_H
#define LDECORATOR_H

#include "utils/util.h"

class LDecorator {
    public:
        LDecorator() {}
        virtual ~LDecorator() {}
        virtual void render() = 0;
        virtual void free() = 0;
};

#endif // LDECORATOR_H