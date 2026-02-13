CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -finput-charset=UTF-8 -fexec-charset=UTF-8
TARGET = table
SRCS = main.cpp table.cpp
OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp table.h
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe
