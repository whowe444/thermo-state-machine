#include "state_machine_test.hh"
#include <chrono>

TEST_F(StateMachineTest, TestConstructor) {
    EXPECT_TRUE(stateMachine);
    EXPECT_FALSE(stateMachine->furnaceOn());
    EXPECT_FALSE(stateMachine->acOn());
}

TEST_F(StateMachineTest, TestStart) {
    // We are in an idle state
    EXPECT_FALSE(stateMachine->furnaceOn());
    EXPECT_FALSE(stateMachine->acOn());
}

TEST_F(StateMachineTest, TestHeating) {
    // Set a desired temperature.
    stateMachine->setDesiredTemperature(75);
	    
    // Sleep for 5 milliseconds
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    // Verify the furnace is on and ac off
    EXPECT_TRUE(stateMachine->furnaceOn());
    EXPECT_FALSE(stateMachine->acOn());
}

TEST_F(StateMachineTest, TestCooling) {
    // Set a desired temperature.
    stateMachine->setDesiredTemperature(64);
	    
    // Sleep for 5 milliseconds
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    // Verify the ac is on and furnace is off
    EXPECT_TRUE(stateMachine->acOn());
    EXPECT_FALSE(stateMachine->furnaceOn());
}

TEST_F(StateMachineTest, TestCoolingBackToIdle) {
    // Set a desired temperature.
    const int NEW_TEMP = 68;
    stateMachine->setDesiredTemperature(NEW_TEMP);
	    
    // Sleep for 3 seconds
    std::this_thread::sleep_for(std::chrono::seconds(3));

    // Verify the ac is on and furnace is off
    EXPECT_FALSE(stateMachine->acOn());
    EXPECT_FALSE(stateMachine->furnaceOn());

    // Verify the new temperature on the thermostat
    EXPECT_EQ(NEW_TEMP, stateMachine->getCurrentTemperature());
}

TEST_F(StateMachineTest, TestHeatingBackToIdle) {
    // Set a desired temperature.
    const int NEW_TEMP = 72;
    stateMachine->setDesiredTemperature(NEW_TEMP);
	    
    // Sleep for 3 seconds
    std::this_thread::sleep_for(std::chrono::seconds(3));

    // Verify the ac is on and furnace is off
    EXPECT_FALSE(stateMachine->acOn());
    EXPECT_FALSE(stateMachine->furnaceOn());

    // Verify the new temperature on the thermostat
    EXPECT_EQ(NEW_TEMP, stateMachine->getCurrentTemperature());
}
