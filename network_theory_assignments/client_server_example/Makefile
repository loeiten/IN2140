# Usage: make

SRC_DIR := .
BASE_DIR := ../..
BUILD_DIR := $(BASE_DIR)/build
EXEC_DIR := $(BUILD_DIR)/network_theory_assingments/client_server_example

CLIENT_BIN := $(EXEC_DIR)/client
SERVER_BIN := $(EXEC_DIR)/server

# CC and CPP are implicit rules, but we will override them here
# See https://makefiletutorial.com/#implicit-rules for details
CC := clang

# Flags to be used together with CC
CCFLAGS := -std=gnu17

.PHONY: all
all: $(CLIENT_BIN) $(SERVER_BIN)

# Make the library object file
# $@ is name of target to be created
# $< is the first prerequisite
# https://stackoverflow.com/a/3220288/2786884
$(CLIENT_BIN): $(SRC_DIR)/client1.c
	mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -o $@ $<

$(SERVER_BIN): $(SRC_DIR)/server1.c
	mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -r $(EXEC_DIR)
