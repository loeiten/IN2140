# Make file for home exam
# See https://makefiletutorial.com for tutorial

# Comment: A way to make compile_commands.json file with make:
# 		   bear --output ../build/compile_commands.json --append -- make -B

# The include directive tells make to suspend reading the current makefile and
# read one or more other makefiles before continuing
include common.mk

# Declaration of targets which doesn't output a file
.PHONY: all clean run-tests test

# For all, we will simply call the sub-directories
# The first $ in $$ is an escape charater.
# So `$$dir` will expand to `$dir` instead of the variable named `dir`
# NOTE: When you call $(MAKE) -C you are creating a new process
# NOTE: The order does matter: We need dynamic_memory.o as a dependency.
#       This file is created in utils, and consumed as a dependency in for
#       example route.c, hence this needs to be processed first
COMPONENTS := utils assignment_files node routing_server test
all:
	for dir in $(COMPONENTS); do \
		$(MAKE) -C $$dir; \
	done

# Run the test executables
# NOTE: This depends on the target all
#       This is in order for test/Makefile to be aware of the targets in the
#       other directories
#       An alternative to this was to make test/Makefile aware of the targets
#       through include
test: all
	$(MAKE) -C test test

clean:
	rm -rf $(EXEC_DIR) $(TEST_EXEC_DIR) $(LIB_DIR) $(BUILD_LIB_DIR) $(BUILD_OBJ_DIR) $(BUILD_TEST_DIR)
