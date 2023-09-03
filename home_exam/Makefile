# Make file for home exam
# See https://makefiletutorial.com for tutorial

# The include directive tells make to suspend reading the current makefile and
# read one or more other makefiles before continuing
include common.mk

# Declaration of targets which doesn't output a file
.PHONY: all clean run-tests test

# For all, we will simply call the sub-directories
# The first $ in $$ is an escape charater.
# So `$$dir` will expand to `$dir` instead of the variable named `dir`
COMPONENTS := assignment_files node routing_server test
all:
	for dir in $(COMPONENTS); do \
		$(MAKE) -C $$dir; \
	done

# Run the test executables
run-tests:
	$(MAKE) -C test run-tests

# Explicitly make a test target
test:
	$(MAKE) -C test

clean:
	rm -rf $(EXEC_DIR) $(TEST_EXEC_DIR) $(LIB_DIR) $(BUILD_LIB_DIR) $(BUILD_OBJ_DIR) $(BUILD_TEST_DIR)
