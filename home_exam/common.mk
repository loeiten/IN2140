
# C flag options
CC := clang
CFLAGS := -g -O0 -std=gnu17
C_LINT_FLAGS := -Wall -Wextra -Wpedantic -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -Werror
C_SANITIZERS := -fsanitize=address -fsanitize=undefined

# Setting up the paths
MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
CUR_DIR := $(dir $(MKFILE_PATH))
BASE_DIR := $(abspath $(CUR_DIR)/..)
ASSIGNMENT_FILES_SRC := $(abspath $(CUR_DIR)/assignment_files)
BUILD_DIR := $(abspath $(BASE_DIR)/build)
EXEC_DIR := $(abspath $(BUILD_DIR)/home_exam)
TEST_EXEC_DIR := $(abspath $(EXEC_DIR)/test)
LIB_DIR := $(abspath $(EXEC_DIR)/lib)
BUILD_LIB_DIR := $(abspath $(EXEC_DIR)/obj_files)
BUILD_OBJ_DIR := $(abspath $(EXEC_DIR)/obj_files)
BUILD_TEST_DIR := $(abspath $(EXEC_DIR)/obj_files)
ASSIGNMENT_FILES_DIR := $(abspath $(EXEC_DIR)/assignment_files)

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

# Define helper functions
define explain_build
	echo "\033[92m${1}\033[0m"
endef
define run_test
	echo "\033[94mRunning ${1}: ${2}...\033[0m"
	${2}
	echo "\033[94m...done\033[0m"
endef
