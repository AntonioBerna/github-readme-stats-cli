CC := clang
TARGET := github-readme-stats-cli
SRC := src/main.c

CPPFLAGS := $(shell pkg-config --cflags libcurl)
LDFLAGS := $(shell pkg-config --libs libcurl)
CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -O2

.PHONY: all clean run valgrind

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

valgrind: CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -O0 -g
valgrind: clean $(TARGET)
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes \
		./$(TARGET) AntonioBerna 3 "html,css,javascript,c%23,processing"

clean:
	rm -f $(TARGET)
