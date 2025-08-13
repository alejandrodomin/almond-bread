SRC_DIR := src
OBJ_DIR := build
BIN := almond-bread

CXX := gcc
LIB_INCLUDE_DIRS := $(shell find lib -type d)
CXXFLAGS := -std=c2x -Wall $(addprefix -I,$(LIB_INCLUDE_DIRS)) $(shell pkg-config --cflags gtk4)
LDFLAGS := -lcurl $(shell pkg-config --libs gtk4)

SRCS := $(shell find $(SRC_DIR) $(LIB_DIR) -type f -name '*.c')
OBJS := $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRCS))


all: $(BIN)

$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN)

install-dev: 
	sudo apt install valgrind libgtk-4-dev
	brew install bear

clangd:
	bear -- make

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all ./$(BIN)

.PHONY: all clean install-dev clangd valgrind
# gcc -O2 -g src/main.c -o ./almond-bread -ldl
