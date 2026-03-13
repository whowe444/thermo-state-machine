#include "state_machine.hh"
#include <chrono>

StateMachine::StateMachine()
    :
    currentTemperature(0),
    desiredTemperature(0),
    currentState(State::Idle),
    running(false)
{
}

void StateMachine::start() {
    this->running = true;

    // Launch the transition thread
    try {
        this->transitionThread = std::thread(&StateMachine::transition, this);
    } catch (std::system_error& e) {
	std::cerr << "Transition Thread failed to start: " << e.what() << std::endl;
    }

    // Launch the behavior thread
    try {
        this->behaviorThread = std::thread(&StateMachine::behavior, this);
    } catch (std::system_error& e) {
	std::cerr << "Behavior Thread failed to start: " << e.what() << std::endl;
    }
}

void StateMachine::end() {
    this->running = false;
    this->transitionThread.join();
    this->behaviorThread.join();
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
