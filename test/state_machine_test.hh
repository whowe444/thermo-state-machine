#pragma once

#include <gtest/gtest.h>
#include "state_machine.hh"

class StateMachineTest : public ::testing::Test {

protected:
    std::shared_ptr<StateMachine> stateMachine;

    StateMachineTest() = default;

    void SetUp() override {
        stateMachine = std::make_shared<StateMachine>();

        // This function should not hang.
        stateMachine->start();
    }

    void TearDown() override {
        // Now turn off the machine
        stateMachine->end();
    }

};
