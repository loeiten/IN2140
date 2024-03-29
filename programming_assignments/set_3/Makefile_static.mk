# Usage: make -f Makefile_static.mk
# See https://makefiletutorial.com for tutorial
TARGET_NAME := s3_assignment_6
LIB_BASE := my_math
LIB_TYPE := static

SRC_DIR := .
BASE_DIR := ../..
BUILD_DIR := $(BASE_DIR)/build
EXEC_DIR := $(BUILD_DIR)/programming_assignments/set_3
LIB_DIR := $(BASE_DIR)/utils
BUILD_LIB_DIR := $(BUILD_DIR)/utils
BUILD_OBJ_DIR := $(BUILD_DIR)/obj_files_from_make

LIB_NAME := lib$(LIB_BASE)_$(LIB_TYPE)
TARGET_EXEC := $(TARGET_NAME)_$(LIB_TYPE)

# Specify which archive to use
# We're checking if empty is equal to the output of command -v <command>
ifeq (,$(shell command -v llvm-ar))
	ARCHIVE := ar
else
	ARCHIVE := llvm-ar
endif

# CC and CPP are implicit rules, but we will override them here
# See https://makefiletutorial.com/#implicit-rules for details
CC := clang

# Flags to be used together with CC
CCFLAGS := -std=gnu17
LIB_EXTENSION := a
LIB_FLAGS := rc

# The final build step
# $(word 2,$^) is getting the second prerequisite
# NOTE: The prerequisite order matter, as it's processed in order
$(EXEC_DIR)/$(TARGET_EXEC): $(BUILD_LIB_DIR)/$(LIB_NAME).$(LIB_EXTENSION) $(BUILD_OBJ_DIR)/$(TARGET_NAME).o
	$(CC) $(CCFLAGS) $< -o $@ $(word 2,$^)

# Make the library
# NOTE: We do NOT need to include the headers
$(BUILD_LIB_DIR)/$(LIB_NAME).$(LIB_EXTENSION): $(BUILD_OBJ_DIR)/$(LIB_BASE).o
	mkdir -p $(dir $@)
	$(ARCHIVE) $(LIB_FLAGS) $@ $<

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
	$(CC) $(CCFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
