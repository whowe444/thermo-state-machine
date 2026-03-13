#include "state_machine.hh"
#include <chrono>

StateMachine::StateMachine()
    :
      currentTemperature(70), // start off the thermostat at a normal temp
      desiredTemperature(70), // match the current so that the thermo is in Idle.
      currentState(State::Idle),
      running(false)
{
}

bool StateMachine::start() {
    bool result = false;
    this->running = true;

    // Launch the transition thread
    try {
        this->transitionThread = std::thread(&StateMachine::transition, this);
        result = true;
    } catch (std::system_error& e) {
	    std::cerr << "Transition Thread failed to start: " << e.what() << std::endl;
    }

    // Launch the behavior thread
    try {
        this->behaviorThread = std::thread(&StateMachine::behavior, this);
        result &= true;
    } catch (std::system_error& e) {
	    std::cerr << "Behavior Thread failed to start: " << e.what() << std::endl;
    }

    return result;
}

bool StateMachine::end() {
    bool result = false;

    // Tell the threads we are shutting down
    this->running = false;

    // First let's shut down the transition thread
    try {
        if (transitionThread.joinable()) {
            transitionThread.join();
            result = true;
        }
    } catch (const std::system_error& e) {
        std::cerr << "Unable to join Transition thread: " << e.what() << std::endl;
    }

    // Now let's shut down the behavior thread
    try {
        if (behaviorThread.joinable()) {
            this->behaviorThread.join();
            result &= true;
        }
    } catch (const std::system_error& e) {
        std::cerr << "Unable to join Behavior thread: " << e.what() << std::endl;
    }

    return result;
}

bool StateMachine::furnaceOn() {
    return this->currentState == State::HeatOn;
}

bool StateMachine::acOn() {
    return this->currentState == State::CoolOn;
}

int StateMachine::getCurrentTemperature() {
    return currentTemperature;
}

void StateMachine::setDesiredTemperature(const int desiredTemperature) {
    this->desiredTemperature = desiredTemperature;
}

void StateMachine::behavior() {
    while (this->running) {
	    switch (this->currentState) {
            case State::Idle:
	            std::cout << "Awaiting commands..." << std::endl;
		        break;
	        case State::CoolOn:
		        this->currentTemperature--;
		        break;
	        case State::HeatOn:
		        this->currentTemperature++;
		        break;
	        default:
	            break;	
	    }

	    // Sleep for 1 second.
	    std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void StateMachine::transition() {
    while (this->running) {
	    if (this->currentTemperature < this->desiredTemperature) {
		    this->currentState = State::HeatOn;
	    } else if (this->currentTemperature > this->desiredTemperature) {
		    this->currentState = State::CoolOn;
	    } else {
    		this->currentState = State::Idle;
	    }
    }
}
