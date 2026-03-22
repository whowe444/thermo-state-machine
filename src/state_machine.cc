#include "state_machine.hh"
#include <chrono>

const int StateMachine::DEFAULT_TEMP(70);

StateMachine::StateMachine()
    :
      currentTemperature(StateMachine::DEFAULT_TEMP), // start off the thermostat at a normal temp
      desiredTemperature(StateMachine::DEFAULT_TEMP), // match the current so that the thermo is in Idle.
      currentState(State::Idle),
      running(false),
      dirty(false)
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
	    result = false;
    }

    // Launch the behavior thread
    try {
        this->behaviorThread = std::thread(&StateMachine::behavior, this);
        result &= true;
    } catch (std::system_error& e) {
	    std::cerr << "Behavior Thread failed to start: " << e.what() << std::endl;
	    result = false;
    }

    return result;
}

bool StateMachine::end() {
    bool result = false;

    // Tell the threads we are shutting down
    {
        std::lock_guard<std::mutex> lock(conditionVariableMutex);
        this->running = false;
    }
    conditionVariable.notify_one();

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
    {
        std::lock_guard<std::mutex> lock(conditionVariableMutex);
        this->desiredTemperature = desiredTemperature;
        this->dirty = true;
    }
    conditionVariable.notify_one();
}

void StateMachine::behavior() {
    while (this->running) {
	    switch (this->currentState) {
            case State::Idle:
	            std::cout << "Awaiting commands..." << std::endl;
		        break;
	        case State::CoolOn:
                {
                    std::lock_guard<std::mutex> lock(conditionVariableMutex);
		            this->currentTemperature--;
                    this->dirty = true;
                }
                conditionVariable.notify_one();
		        break;
	        case State::HeatOn:
                {
                    std::lock_guard<std::mutex> lock(conditionVariableMutex);
		            this->currentTemperature++;
                    this->dirty = true;
                }
                conditionVariable.notify_one();
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
        std::unique_lock<std::mutex> lock(conditionVariableMutex);
        conditionVariable.wait(lock, [this] {
            return !this->running || dirty;
        });

        if (!this->running) break;

	    if (this->currentTemperature < this->desiredTemperature) {
		    this->currentState = State::HeatOn;
	    } else if (this->currentTemperature > this->desiredTemperature) {
		    this->currentState = State::CoolOn;
	    } else {
    		this->currentState = State::Idle;
	    }

        this->dirty = false;
    }
}
