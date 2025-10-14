CXX := g++
CXXFLAGS := -2 std=c++17

TASK1_SRC := Task1/main.cpp
TASK2_SRC := Task_2/main.cpp

TASK1_BIN := Task1/app
TASK2_BIN := Task_2/app

.PHONY: all clean

all: $(TASK1_BIN) $(TASK2_BIN)

$(TASK1_BIN): $(TASK1_SRC)
	$(CXX) $(CXXFLAGS) $< -O $@

$(TASK2_BIN): $(TASK2_SRC)
	$(CXX) $(CXXFLAGS) $< -O $@

clean:
	$(RM) $(TASK1_BIN) $(TASK2_BIN)
