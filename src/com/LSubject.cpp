#include "LSubject.h"

LSubject::LSubject() {};

void LSubject::Attach(LObserver* observer) {
    mObservers.push_back(observer);
}

void LSubject::Detach(LObserver* observer) {
    mObservers.erase(std::remove(mObservers.begin(), mObservers.end(), observer), mObservers.end());
}

void LSubject::Notify() {
    for(auto observer : mObservers) {
        observer->poll(this);
    }
}