# NOTE:
# This Makefile takes variables passed in from the "outside" (typically from the
# parent Makefile)

# Comment: We could've created something like a %.o: %.c pattern target, which
#          would also have all the changes in node and routing_server as a
#          prerequisite.
#		   However, we'd like a bit more fine grained control of the build
#		   process (i.e. we don't want to build all tests anew if there is only
#          one small change in an unrelated file).
#		   Hence, we are adding the tests one by one

.PHONY: test run-tests

# Build the tests
test: $(TEST_EXEC_DIR)/test_dijkstra

$(TEST_EXEC_DIR)/test_dijkstra: $(BUILD_TEST_DIR)/test_dijkstra.o
	mkdir -p $(TEST_EXEC_DIR)
	$(CC) $(CCFLAGS) $(CC_LINT_FLAGS) $(CC_SANITIZERS) $< -o $@


$(BUILD_TEST_DIR)/test_dijkstra.o: test_dijkstra.c
	mkdir -p $(BUILD_TEST_DIR)
	$(CC) $(CCFLAGS) $(CC_LINT_FLAGS) $(CC_SANITIZERS) -c $< -o $@

# Run test
run-tests: run_test_dijkstra

.PHONY: run_test_dijkstra
run_test_dijkstra: $(TEST_EXEC_DIR)/test_dijkstra
	echo "Running $<..."
	$<
	echo "...done"
