CXX      := g++
CXXFLAGS := -Wall -Wextra -fPIC -std=c++17
GTESTFLAGS = -I/usr/local/include  -L/usr/local/lib64 -lgtest -lgtest_main -lpthread

SRC_DIR   := src
TEST_DIR  := test
BUILD_DIR := build

TARGET    := libStateMachine.so
TEST_BIN  := $(BUILD_DIR)/test_runner

SRCS      := $(wildcard $(SRC_DIR)/*.cc)
OBJS      := $(patsubst $(SRC_DIR)/%.cc, $(BUILD_DIR)/%.o, $(SRCS))

TEST_SRCS := $(wildcard $(TEST_DIR)/*.cc)
TEST_OBJS := $(patsubst $(TEST_DIR)/%.cc, $(BUILD_DIR)/%.o, $(TEST_SRCS))

.PHONY: all test clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -shared -o $@ $^

test: $(TARGET) $(TEST_BIN)
	./$(TEST_BIN)

$(TEST_BIN): $(TEST_OBJS) | $(BUILD_DIR)
	$(CXX) -o $@ $^ -L. $(GTESTFLAGS) -lStateMachine -Wl,-rpath,. 

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cc | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cc | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(SRC_DIR) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
