#pragma once

#include <memory>

#include "IObserver.h"

class ISubject {
public:
    virtual ~ISubject() = default;

    // Methods for managing observers
    virtual void attachObserver(std::shared_ptr<IObserver> observer) = 0;
    virtual void detachObserver(std::shared_ptr<IObserver> observer) = 0;

    // Notify all attached observers of an event
    virtual void notifyObservers(const std::string& event) = 0;
};
