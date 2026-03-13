#ifndef STATE_MACHINE_HH
#define STATE_MACHINE_HH

#include <iostream>
#include <thread>
#include <atomic>

enum class State {
    Idle,
    CoolOn,
    HeatOn
};

class StateMachine {

public:

    // Constructor 
    StateMachine();

    void start();
    void end();

    int getCurrentTemperature();
    void setDesiredTemperature(const int desiredTemperature);
    void transition();
    void behavior();

private:

    std::atomic<int> currentTemperature;
    std::atomic<int> desiredTemperature;
    std::atomic<State> currentState;
    std::atomic<bool> running;
    std::thread transitionThread;
    std::thread behaviorThread;

};

#endif // STATE_MACHINE_HH
