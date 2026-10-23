
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

TARGET = can_diagnostics

SOURCES = src/main.cpp \
         src/socketcan/can_socket.cpp \
         src/monitor/can_monitor.cpp

all:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
