# Make file for home exam
# See https://makefiletutorial.com for tutorial

# C flag options
CC := clang
CFLAGS := -g -O0 -std=gnu17
CC_LINT_FLAGS := -Wall -Wextra -Wpedantic -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -Werror
CC_SANITIZERS := -fsanitize=address -fsanitize=undefined

# Setting up the paths
SRC_DIR := $(abspath .)
BASE_DIR := $(abspath ..)
BUILD_DIR := $(abspath $(BASE_DIR)/build)
EXEC_DIR := $(abspath $(BUILD_DIR)/home_exam)
LIB_DIR := $(abspath $(EXEC_DIR)/lib)
BUILD_LIB_DIR := $(abspath $(EXEC_DIR)/obj_files)
BUILD_OBJ_DIR := $(abspath $(EXEC_DIR)/obj_files)

# Setting up the libraries (we"re here using static libraries)
# Specify which archive to use
# We"re checking if empty is equal to the output of command -v <command>
ifeq (,$(shell command -v llvm-ar))
	ARCHIVE := ar
else
	ARCHIVE := llvm-ar
endif
LIB_EXTENSION := a
LIB_FLAGS := rc

# Bundle variables together to easier pass them on to children Makefiles
PROJECT_PATHS := EXEC_DIR="$(EXEC_DIR)" LIB_DIR="$(LIB_DIR)" BUILD_LIB_DIR="$(BUILD_LIB_DIR)" BUILD_OBJ_DIR="$(BUILD_OBJ_DIR)"
PROJECT_CC_FLAGS := CC="$(CC)" CFLAGS="$(CFLAGS)" CC_LINT_FLAGS="$(CC_LINT_FLAGS)" CC_SANITIZERS="$(CC_SANITIZERS)"
PROJECT_LIB_FLAGS := ARCHIVE="$(ARCHIVE)" LIB_EXTENSION="$(LIB_EXTENSION)" LIB_FLAGS="$(LIB_FLAGS)"

# Declaration of targets which doesn't output a file
.PHONY: all clean

all: $(EXEC_DIR)/routing_server $(EXEC_DIR)/node

# Make the routing_server executable
# FIXME: Have a directory specifically for routing_server
$(EXEC_DIR)/routing_server: $(LIB_DIR)/libprint.$(LIB_EXTENSION) $(EXEC_DIR)/assignment_files
	touch $(EXEC_DIR)/routing_server

# Make the node executable
# FIXME: Have directory specifically for node
$(EXEC_DIR)/node: $(LIB_DIR)/libprint.$(LIB_EXTENSION) $(EXEC_DIR)/assignment_files
	touch $(EXEC_DIR)/node

# Copy the files from assignment into the executable
ASSIGNMENT_FILES := $(wildcard assignment_files/*.*)
$(EXEC_DIR)/assignment_files: $(ASSIGNMENT_FILES)
	mkdir -p $(EXEC_DIR)/assignment_files
	cp $(ASSIGNMENT_FILES) $(EXEC_DIR)/assignment_files


# Make the library
$(LIB_DIR)/libprint.$(LIB_EXTENSION):
	$(MAKE) -C assignment_files $(PROJECT_PATHS) $(PROJECT_CC_FLAGS) $(PROJECT_LIB_FLAGS)

clean:
	rm -rf $(EXEC_DIR)