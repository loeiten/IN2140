include ../common.mk

# Comment: We could've created something like a %.o: %.c pattern target, which
#          would also have all the changes in node and routing_server as a
#          prerequisite.
#		   However, we'd like a bit more fine grained control of the build
#		   process (i.e. we don't want to build all tests anew if there is only
#          one small change in an unrelated file).
#		   Hence, we are adding the tests one by one

TESTS := run_test_get_min_distance_idx_1 \
		 run_test_get_min_distance_idx_2

.PHONY: all clean test $(TESTS)
all: $(TEST_EXEC_DIR)/test_dijkstra

test: $(TESTS)

# @ outputs the target
# ^ outputs all prerequisites
$(TEST_EXEC_DIR)/test_dijkstra: $(BUILD_TEST_DIR)/test_dijkstra.o $(BUILD_OBJ_DIR)/dijkstra.o
	$(call explain_build,Making test_dijkstra)
	mkdir -p $(TEST_EXEC_DIR)
	$(CC) $(CFLAGS) $(C_LINT_FLAGS) $(C_SANITIZERS) $^ -o $@

# < outputs the first prerequisite
# NOTE: We don't add an include path, as the path to the .h files can be found
#       relative to the .c files
$(BUILD_TEST_DIR)/test_dijkstra.o: test_dijkstra.c
	$(call explain_build,Making test_dijkstra.o)
	mkdir -p $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(C_LINT_FLAGS) $(C_SANITIZERS) -c $< -o $@

# Run tests
run_test_get_min_distance_idx_1: $(TEST_EXEC_DIR)/test_dijkstra
	$(call run_test,$@,$< getMinDistanceIdx)

run_test_get_min_distance_idx_2: $(TEST_EXEC_DIR)/test_dijkstra
	$(call run_test,$@,$< dijkstra)

clean:
	rm -rf $(BUILD_TEST_DIR) $(TEST_EXEC_DIR)
