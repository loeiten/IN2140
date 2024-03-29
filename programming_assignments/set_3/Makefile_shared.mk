# Usage: make -f Makefile_shared.mk
# See https://makefiletutorial.com for tutorial
TARGET_NAME := s3_assignment_6
LIB_BASE := my_math
LIB_TYPE := shared

SRC_DIR := .
BASE_DIR := ../..
BUILD_DIR := $(BASE_DIR)/build
EXEC_DIR := $(BUILD_DIR)/programming_assignments/set_3
LIB_DIR := $(BASE_DIR)/utils
BUILD_LIB_DIR := $(BUILD_DIR)/utils
BUILD_OBJ_DIR := $(BUILD_DIR)/obj_files_from_make

LIB_NAME := lib$(LIB_BASE)_$(LIB_TYPE)
TARGET_EXEC := $(TARGET_NAME)_$(LIB_TYPE)

# CC and CPP are implicit rules, but we will override them here
# See https://makefiletutorial.com/#implicit-rules for details
CC := clang

# Flags to be used together with CC
CCFLAGS := -std=gnu17
LINKER_FLAGS := -Wl,-rpath $(PWD)/$(BUILD_LIB_DIR)

# Detect whether we are running on Linux or OSX
# https://stackoverflow.com/a/12099167/2786884
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LIB_EXTENSION := so
	LIB_FLAGS := -shared -Wl,-soname,$(LIB_NAME).$(LIB_EXTENSION)
endif
ifeq ($(UNAME_S),Darwin)
	LIB_EXTENSION := dylib
	LIB_FLAGS := -dynamiclib -install_name @rpath/$(LIB_NAME).$(LIB_EXTENSION)
endif

# The final build step
# $(word 2,$^) is getting the second prerequisite
# NOTE: The prerequisite order matter, as it's processed in order
$(EXEC_DIR)/$(TARGET_EXEC): $(BUILD_LIB_DIR)/$(LIB_NAME).$(LIB_EXTENSION) $(BUILD_OBJ_DIR)/$(TARGET_NAME).o
	$(CC) $(CCFLAGS) $< -o $@ $(LINKER_FLAGS) $(word 2,$^)

# Make the library
# NOTE: We do NOT need to include the headers
$(BUILD_LIB_DIR)/$(LIB_NAME).$(LIB_EXTENSION): $(BUILD_OBJ_DIR)/$(LIB_BASE).o
	mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -fPIC $(LIB_FLAGS) -o $@ $<

# Make the executable object file
# NOTE: We need to include the headers
$(BUILD_OBJ_DIR)/$(TARGET_NAME).o: $(SRC_DIR)/$(TARGET_NAME).c
	$(CC) $(CCFLAGS) -I/$(LIB_DIR) -c $< -o $@

# Make the library object file
# $< is the first prerequisite
# $@ is name of target to be created
# https://stackoverflow.com/a/3220288/2786884
$(BUILD_OBJ_DIR)/$(LIB_BASE).o: $(LIB_DIR)/src/$(LIB_BASE).c $(LIB_DIR)/include/$(LIB_BASE).h
	mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -fPIC -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
