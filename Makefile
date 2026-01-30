CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = chess
SRCS = main.cpp game.cpp board.cpp pieces.cpp player.cpp move.cpp ai.cpp
OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Зависимости заголовков
main.o: main.cpp game.h board.h pieces.h move.h player.h
game.o: game.cpp game.h board.h pieces.h move.h player.h ai.h
ai.o: ai.cpp ai.h board.h pieces.h move.h
board.o: board.cpp board.h pieces.h move.h
pieces.o: pieces.cpp pieces.h board.h move.h
player.o: player.cpp player.h pieces.h move.h
move.o: move.cpp move.h

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean
