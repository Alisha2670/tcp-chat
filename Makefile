CC = gcc
CFLAGS = -Wall -Wextra -pthread -Iinclude
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

all: server client

server: $(SRC_DIR)/server.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/server $(SRC_DIR)/server.c

client: $(SRC_DIR)/client.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/client $(SRC_DIR)/client.c

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR) logs/*.log
	@echo "Cleaned build artifacts and runtime logs."

.PHONY: all clean
