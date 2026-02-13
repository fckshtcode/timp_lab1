CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -finput-charset=UTF-8 -fexec-charset=UTF-8
TARGET = gronsfeld
SRCS = main.cpp modAlphaCipher.cpp
OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean doc

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp modAlphaCipher.h
	$(CXX) $(CXXFLAGS) -c $<

doc:
	doxygen Doxyfile

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe
	rm -rf html latex
