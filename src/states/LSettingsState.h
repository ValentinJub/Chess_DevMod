/*
settings.h
Sun Mar 20 2022
by Valentin
-----------
headers for settings
*/

#ifndef LSETTINGSSTATE_H
#define LSETTINGSSTATE_H

#include "utils/util.h"
#include "menu/LMenuSettings.h"
#include "states/LState.h"
#include "com/LSubject.h"


class LSettingsState : public LState {
public:
    LSettingsState() {};
    void enter(LObserver* observer);
    void exit();
    void update();
    void render();
    void free();
    void Notify();

private:
    LMenuSettings* mSettings;
    LObserver* mAppObserver;
};

#endif
