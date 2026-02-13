CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -finput-charset=UTF-8 -fexec-charset=UTF-8
TARGET = gronsfeld
TEST_TARGET = test_gronsfeld
MODULE = modAlphaCipher

.PHONY: all clean

all: $(TARGET) $(TEST_TARGET)

$(TARGET): main.o $(MODULE).o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_TARGET): test.o $(MODULE).o
	$(CXX) $(CXXFLAGS) -o $@ $^ -lUnitTest++

main.o: main.cpp $(MODULE).h
	$(CXX) $(CXXFLAGS) -c main.cpp

test.o: test.cpp $(MODULE).h
	$(CXX) $(CXXFLAGS) -c test.cpp

$(MODULE).o: $(MODULE).cpp $(MODULE).h
	$(CXX) $(CXXFLAGS) -c $(MODULE).cpp

clean:
	rm -f *.o $(TARGET) $(TARGET).exe $(TEST_TARGET) $(TEST_TARGET).exe
