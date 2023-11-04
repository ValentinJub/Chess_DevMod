/*
LPiece.h
Sun Mar 20 2022
by Valentin
-----------
LPiece is the parent class of all chess pieces. 
*/

#ifndef HEADER_LPIECE
#define HEADER_LPIECE

#include "constantes.h"

class LPiece {
    public: 
        LPiece();
    protected: 

    private:
       bool mIsAlive,
            mHasMoved;
};

#endif
