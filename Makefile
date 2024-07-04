CC = gcc
CFLAGS = -Wall -std=c11 -O2 -I $(INCLUDE_DIR)
LDFLAGS = -lglfw3dll -lm

LIB_DIR = lib
INCLUDE_DIR = include

SRC_DIR = src
BIN_DIR = bin

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)

TARGET = $(BIN_DIR)/app

all: $(TARGET)

$(TARGET): $(OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ -L $(LIB_DIR) $(LDFLAGS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

debug: CFLAGS += -DDEBUG -O0 -g
debug: clean $(TARGET)

clean:
	rm -rf $(BIN_DIR) $(TARGET)