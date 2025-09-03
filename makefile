CXX = g++
CXXFLAGS = -Wall -g -std=c++17
LDLIBS = -lreadline 
TARGET = my_shell

SRCS = posix.cpp ls.cpp fg_bg.cpp tokanize.cpp I_O.cpp pwd_pinfo.cpp history.cpp # list all your .cpp files
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LDLIBS)

%.o: %.cpp header.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
