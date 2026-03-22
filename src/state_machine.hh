#pragma once

#include <iostream>
#include <thread>
#include <atomic>
#include <condition_variable>


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

    alignas(64) std::atomic<int> currentTemperature;
    alignas(64) std::atomic<int> desiredTemperature;
    alignas(64) std::atomic<State> currentState;
    bool running;
    bool dirty;
    std::thread transitionThread;
    std::thread behaviorThread;

    std::condition_variable conditionVariable;
    std::mutex conditionVariableMutex;

};
