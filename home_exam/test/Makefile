include ../common.mk

# Comment: We could've created something like a %.o: %.c pattern target, which
#          would also have all the changes in node and routing_server as a
#          prerequisite.
#		   However, we'd like a bit more fine grained control of the build
#		   process (i.e. we don't want to build all tests anew if there is only
#          one small change in an unrelated file).
#		   Hence, we are adding the tests one by one

.PHONY: all clean test run-tests
all: test

# Build the tests
test: $(TEST_EXEC_DIR)/test_dijkstra

# @ outputs the target
# ^ outputs all prerequisites
$(TEST_EXEC_DIR)/test_dijkstra: $(BUILD_TEST_DIR)/test_dijkstra.o $(BUILD_OBJ_DIR)/dijkstra.o
	$(call explain_build,Making test_dijkstra)
	mkdir -p $(TEST_EXEC_DIR)
	$(CC) $(CCFLAGS) $(CC_LINT_FLAGS) $(CC_SANITIZERS) $^ -o $@

# < outputs the first prerequisite
$(BUILD_TEST_DIR)/test_dijkstra.o: test_dijkstra.c
	$(call explain_build,Making test_dijkstra.o)
	mkdir -p $(BUILD_TEST_DIR)
	$(CC) $(CCFLAGS) $(CC_LINT_FLAGS) $(CC_SANITIZERS) -c $< -o $@

# Run test
run-tests: run_test_get_min_distance_idx_1

.PHONY: run_test_get_min_distance_idx_1
run_test_get_min_distance_idx_1: $(TEST_EXEC_DIR)/test_dijkstra
	$(call run_test,$@,$< getMinDistanceIdx)

clean:
	rm -rf $(BUILD_TEST_DIR) $(TEST_EXEC_DIR)
