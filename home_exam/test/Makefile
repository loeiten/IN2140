# NOTE:
# This Makefile takes variables passed in from the "outside" (typically from the
# parent Makefile)

# # Find the files to make the executables from
# #TEST_SRC := $(wildcard *.c)
#
# #all: %.c
# #	echo $@
#
# ## FIXME: Make a test that this is run if libprint, node or routing_server is called
# ## Make the binary files
# ## NOTE: The % is a wildcard which serves two purposes here
# ##       Firstly it matches one or more characters in the dependecy
# ##       Secondly it copies that match to the target
# #$(TEST_EXEC_DIR)/test_%: $(BUILD_TEST_DIR)/test_%.o $(LIB_DIR)/libprint.$(LIB_EXTENSION) $(EXEC_DIR)/routing_server $(EXEC_DIR)/node
# #	mkdir -p $(TEST_EXEC_DIR)
# #	$(CC) $(CCFLAGS) $(CC_LINT_FLAGS) $(CC_SANITIZERS) -c $< -o $@
#
# # FIXME: Must also get the dependencies of node, print_lib and router
#
# $(info Building target: $@)
# $(info Source file: $<)
# $(info New test: $(%.c))
# $(info CC command: $(CC) $(CCFLAGS) $(CC_LINT_FLAGS) $(CC_SANITIZERS) -c $< -o $@)


# Comment: We could've created something like a %.o: %.c pattern target, which
#          would also have all the changes in node and routing_server as a
#          prerequisite.
#		   However, we'd like a bit more fine grained control of the build
#		   process (i.e. we don't want to build all tests anew if there is only
#          one small change in an unrelated file).
#		   Hence, we are adding the tests one by one

.PHONY: test run-test

# Build the tests
test: $(TEST_EXEC_DIR)/test_dijkstra

$(TEST_EXEC_DIR)/test_dijkstra: $(BUILD_TEST_DIR)/test_dijkstra.o
	echo "Inside bin"
	$(CC) $(CCFLAGS) $(CC_LINT_FLAGS) $(CC_SANITIZERS) $< -o $@


$(BUILD_TEST_DIR)/test_dijkstra.o: test_dijkstra.c
	echo "Inside .o"
	mkdir -p $(BUILD_TEST_DIR)
	$(CC) $(CCFLAGS) $(CC_LINT_FLAGS) $(CC_SANITIZERS) -c $< -o $@

# Run test
run-tests: run_test_dijkstra

.PHONY: run_test_dijkstra
run_test_dijkstra: $(TEST_EXEC_DIR)/test_dijkstra
	$@


## FIXME:
#ROUTING_SERVER_SRC := $(wildcard *.c)
## FIXME:
#NODE_SRC := $(wildcard *.c)
#TEST_SRC := $(wildcard *.c)
#
#
#
## Make the object files
## NOTE: The % is a wildcard which serves two purposes here
##       Firstly it matches one or more characters in the dependecy
##       Secondly it copies that match to the target
##$(BUILD_TEST_DIR)/test_dijkstra.o: test_dijkstra.c
#$(BUILD_TEST_DIR)/%.o: %.c
#	mkdir -p $(BUILD_TEST_DIR)
#	$(CC) $(CCFLAGS) $(CC_LINT_FLAGS) $(CC_SANITIZERS) -c $< -o $@
#
