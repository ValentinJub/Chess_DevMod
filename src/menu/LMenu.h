#ifndef LMENU_H
#define LMENU_H

#include "utils/util.h"

class LMenu {
    public:
        virtual void free() = 0;
        virtual void render() = 0;
        virtual bool init() = 0;
};


#endif