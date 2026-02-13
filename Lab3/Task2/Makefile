CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -finput-charset=UTF-8 -fexec-charset=UTF-8
TARGET = table_app
TEST_TARGET = test_table
MODULE = table

.PHONY: all clean

all: $(TARGET) $(TEST_TARGET)

$(TARGET): main.o $(MODULE).o
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_TARGET): test_table.o $(MODULE).o
	$(CXX) $(CXXFLAGS) -o $@ $^ -lUnitTest++

main.o: main.cpp $(MODULE).h
	$(CXX) $(CXXFLAGS) -c main.cpp

test_table.o: test_table.cpp $(MODULE).h
	$(CXX) $(CXXFLAGS) -c test_table.cpp

$(MODULE).o: $(MODULE).cpp $(MODULE).h
	$(CXX) $(CXXFLAGS) -c $(MODULE).cpp

clean:
	rm -f *.o $(TARGET) $(TARGET).exe $(TEST_TARGET) $(TEST_TARGET).exe
