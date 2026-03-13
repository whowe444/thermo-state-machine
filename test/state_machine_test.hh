#pragma once

#include <gtest/gtest.h>
#include "state_machine.hh"

class StateMachineTest : public ::testing::Test {

protected:

    std::shared_ptr<StateMachine> stateMachine;

    StateMachineTest() {
        stateMachine = std::make_shared<StateMachine>();
    }

    void SetUp() override {
    }

    void TearDown() override {
    }


};
