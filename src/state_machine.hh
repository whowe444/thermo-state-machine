#pragma once

#include <iostream>
#include <thread>
#include <atomic>

enum class State {
    Idle,
    CoolOn,
    HeatOn
};

class StateMachine {

static const int DEFAULT_TEMP;

public:

    // Constructor 
    StateMachine();

    // Start up threads
    bool start();
    bool end();

    // Sense current state
    bool furnaceOn();
    bool acOn();

    // Interface with the thermostat
    int getCurrentTemperature();
    void setDesiredTemperature(const int desiredTemperature);

private:

    void transition();
    void behavior();

    std::atomic<int> currentTemperature;
    std::atomic<int> desiredTemperature;
    std::atomic<State> currentState;
    std::atomic<bool> running;
    std::thread transitionThread;
    std::thread behaviorThread;

};
