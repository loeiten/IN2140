
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
TEST_EXEC_DIR := $(abspath $(EXEC_DIR)/test)
LIB_DIR := $(abspath $(EXEC_DIR)/lib)
BUILD_LIB_DIR := $(abspath $(EXEC_DIR)/obj_files)
BUILD_OBJ_DIR := $(abspath $(EXEC_DIR)/obj_files)
BUILD_TEST_DIR := $(abspath $(EXEC_DIR)/obj_files)

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
PROJECT_PATHS := EXEC_DIR="$(EXEC_DIR)" TEST_EXEC_DIR="$(TEST_EXEC_DIR)" LIB_DIR="$(LIB_DIR)" BUILD_LIB_DIR="$(BUILD_LIB_DIR)" BUILD_OBJ_DIR="$(BUILD_OBJ_DIR)" BUILD_TEST_DIR="$(BUILD_TEST_DIR)" SRC_DIR="$(SRC_DIR)"
PROJECT_CC_FLAGS := CC="$(CC)" CFLAGS="$(CFLAGS)" CC_LINT_FLAGS="$(CC_LINT_FLAGS)" CC_SANITIZERS="$(CC_SANITIZERS)"
PROJECT_LIB_FLAGS := ARCHIVE="$(ARCHIVE)" LIB_EXTENSION="$(LIB_EXTENSION)" LIB_FLAGS="$(LIB_FLAGS)"

# Define helper functions
define explain_build
	echo "\n\033[92m${1}\033[0m"
endef
define run_test
	echo "\n\033[94mRunning ${1}: ${2}...\033[0m"
	${2}
endef
