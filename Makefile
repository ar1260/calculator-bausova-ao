CC = gcc
CXX = g++

CFLAGS = -Wall -Wextra -Wpedantic -Werror -std=c11
CXXFLAGS = -Wall -Wextra -Wpedantic -Werror -std=c++17

LDFLAGS = -lstdc++ -lm
TEST_LDFLAGS = -lgtest -lgtest_main -lpthread

SRC_DIR = src
BUILD_DIR = build
UNIT_TESTS_DIR = tests/unit

# Application
APP_SRC = $(SRC_DIR)/main.c $(SRC_DIR)/stack.c $(SRC_DIR)/calculator.c
APP_OBJ = $(BUILD_DIR)/main.o $(BUILD_DIR)/stack.o $(BUILD_DIR)/calculator.o
APP_EXE = $(BUILD_DIR)/app.exe

# Unit tests
TEST_SRC = $(UNIT_TESTS_DIR)/tests.cpp
TEST_OBJ = $(BUILD_DIR)/tests.o
TEST_EXE = $(BUILD_DIR)/unit-tests.exe

# GoogleTest files
GTEST_DIR = googletest
GTEST_BUILD = $(GTEST_DIR)/build
GTEST_LIB = $(GTEST_BUILD)/lib/libgtest.a

# Formatting configuration
FORMAT_DIRS = $(SRC_DIR) $(UNIT_TESTS_DIR)
FORMAT_EXTS = *.cpp *.c *.h
CLANG_FORMAT = clang-format

# Python integration tests
VENV = venv
PYTHON = $(VENV)/bin/python
PIP = $(VENV)/bin/pip
INT_TEST_DIR = tests/integration

# Python GUI and server
GUI_SCRIPT = gui.py
SERVER_SCRIPT = server.py

.PHONY: all clean run-app run-unit-test format venv run-integration-tests run-server run-gui

all: $(APP_EXE) $(TEST_EXE)

# Build application
$(APP_EXE): $(APP_OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/stack.o: $(SRC_DIR)/stack.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/calculator.o: $(SRC_DIR)/calculator.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# GoogleTest download and build
$(GTEST_DIR)/CMakeLists.txt:
	git clone https://github.com/google/googletest.git $(GTEST_DIR)

$(GTEST_LIB): $(GTEST_DIR)/CMakeLists.txt
	mkdir -p $(GTEST_BUILD)
	cd $(GTEST_BUILD) && cmake .. && make

# Build unit tests
$(TEST_EXE): $(TEST_OBJ) $(BUILD_DIR)/stack.o $(BUILD_DIR)/calculator.o $(GTEST_LIB)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(TEST_LDFLAGS) $(LDFLAGS)

$(BUILD_DIR)/tests.o: $(UNIT_TESTS_DIR)/tests.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -DTESTING -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR)

run-int: $(APP_EXE)
	@$<

run-float: $(APP_EXE)
	@$< --float

run-unit-test: $(TEST_EXE)
	@$<

format:
	@find $(FORMAT_DIRS) -type f \( \
		-name "*.cpp" -o \
		-name "*.c" -o \
		-name "*.h" \
	\) -exec $(CLANG_FORMAT) -i -style=file {} +

$(VENV):
	@python3 -m venv $(VENV)
	@$(PIP) install --upgrade pip
	@$(PIP) install pytest PySide6 structlog requests

run-server: $(VENV)
	@$(PYTHON) $(SERVER_SCRIPT)

run-gui: $(VENV)
	@$(PYTHON) $(GUI_SCRIPT)

run-integration-tests: $(VENV)
	@$(PYTHON) -m pytest $(INT_TEST_DIR)
