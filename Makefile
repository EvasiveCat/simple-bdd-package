# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -pthread

# Include all subdirectories of the include folder
INC_DIRS := $(shell find include -type d)
CXXFLAGS += $(addprefix -I, $(INC_DIRS))

# Debug flags
DEBUG_FLAGS := -g -O0

# Aggressive optimization flags
RELEASE_FLAGS := -O3 -march=native -flto

# Directories
SRC_DIR := src
TEST_DIR := test
BUILD_DIR := build
TEST_BUILD_DIR := $(BUILD_DIR)/test
LOG_DIR := log

# Source files
MAIN_SRC := main.cpp
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
TEST_SRCS := $(shell find $(TEST_DIR) -name '*.cpp')

# Object files
MAIN_OBJ := $(BUILD_DIR)/main.o
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))
TEST_OBJS := $(patsubst $(TEST_DIR)/%.cpp, $(TEST_BUILD_DIR)/%.o, $(TEST_SRCS))

# Targets
TARGET := main
DEBUG_TARGET := main_debug
RELEASE_TARGET := main_release
TEST_TARGET := run_tests

# Google Test linker flags
GTEST_LIBS := -lgtest -lgtest_main -lpthread

# Create build and log directories if they don't exist
$(shell mkdir -p $(BUILD_DIR) $(TEST_BUILD_DIR) $(LOG_DIR))

# Default target
all: $(TARGET)

# Debug target
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: $(DEBUG_TARGET)

# Release target
release: CXXFLAGS += $(RELEASE_FLAGS)
release: $(RELEASE_TARGET)

# Test target
test: $(TEST_TARGET)

# Link main executable
$(TARGET): $(OBJS) $(MAIN_OBJ)
	$(CXX) $^ -o $@

# Link debug executable
$(DEBUG_TARGET): $(OBJS) $(MAIN_OBJ)
	$(CXX) $^ -o $@

# Link release executable
$(RELEASE_TARGET): $(OBJS) $(MAIN_OBJ)
	$(CXX) $^ -o $@

# Link test executable
$(TEST_TARGET): $(OBJS) $(TEST_OBJS)
	$(CXX) $^ $(GTEST_LIBS) -o $@
	./$@

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile test files to object files
$(TEST_BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile main.cpp separately
$(MAIN_OBJ): $(MAIN_SRC)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf $(BUILD_DIR)/* $(TARGET) $(DEBUG_TARGET) $(RELEASE_TARGET) $(TEST_TARGET)

clean_logs:
	rm -rf $(LOG_DIR)/*

# Phony targets
.PHONY: all clean clean_logs debug release test