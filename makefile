CXX = g++
CXXFLAGS = -Wall -g -std=c++17
TARGET = my_shell

SRCS = posix.cpp ls.cpp      # list all your .cpp files
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp header.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
