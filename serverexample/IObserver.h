#pragma once

#include <string>

class IObserver {
public:
    virtual ~IObserver() = default;

    // Method to be called when the subject notifies observers
    virtual void update(const std::string& event) = 0;
};
