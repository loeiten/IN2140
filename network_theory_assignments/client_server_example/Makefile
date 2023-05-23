# NOTE: This is purposfully only in make, and not in CMake as testing is best
#       performed on two separate machines
# Usage: make

SRC_DIR := .
BASE_DIR := ../..
BUILD_DIR := $(BASE_DIR)/build
EXEC_DIR := $(BUILD_DIR)/network_theory_assingments/client_server_example

CLIENT_BIN := $(EXEC_DIR)/client
SERVER_BIN := $(EXEC_DIR)/server

# CC and CPP are implicit rules, but we will override them here
# See https://makefiletutorial.com/#implicit-rules for details
ifeq (, $(shell which clang))
	CC := gcc
else
	CC := clang
endif

# Flags to be used together with CC
# -g enables debugging
CCFLAGS := -std=gnu17 -g

.PHONY: all
all: $(CLIENT_BIN) $(SERVER_BIN)

# Make the executables
# $@ is name of target to be created
# $< is the first prerequisite
# $^ is the last prerequisite
# https://stackoverflow.com/a/3220288/2786884

# Make the client executable by
# 1. Producing the .o file
# 2. Combining the .o files to an executable
# $(word 2,$^) is getting the second prerequisite
$(CLIENT_BIN): $(SRC_DIR)/client1.c $(EXEC_DIR)/helper.o
	$(CC) $(CCFLAGS) -c $< -o $@.o
	$(CC) $(CCFLAGS) -I/include -o $@ $@.o $(word 2,$^)

# Make the server executable
$(SERVER_BIN): $(SRC_DIR)/server1.c $(EXEC_DIR)/helper.o
	$(CC) $(CCFLAGS) -c $< -o $@.o
	$(CC) $(CCFLAGS) -I/include -o $@ $@.o $(word 2,$^)

# Create the helper.o file
# NOTE: The config file doesn't have to trigger the build, but it's added as a
#       a dependency to ensure that the rule is run
$(EXEC_DIR)/helper.o: helper.c include/helper.h $(EXEC_DIR)/config.txt
	$(CC) $(CCFLAGS) -I/include -c $< -o $@

# Copy the configuration file
# This will also make the directory for the executables
$(EXEC_DIR)/config.txt: config.txt
	mkdir -p $(dir $@)
	cp $< $(EXEC_DIR)/$<

.PHONY: clean
clean:
	rm -r $(EXEC_DIR)
