include ../common.mk

# Comment: We could've created something like a %.o: %.c pattern target, which
#          would also have all the changes in node and routing_server as a
#          prerequisite.
#		   However, we'd like a bit more fine grained control of the build
#		   process (i.e. we don't want to build all tests anew if there is only
#          one small change in an unrelated file).
#		   Hence, we are adding the tests one by one

TESTS := run_test_dijkstra_1 \
		 run_test_dijkstra_2 \
		 run_test_dijkstra_3 \
		 run_test_receiver_1 \
		 run_test_receiver_2 \
		 run_test_route_1 \
		 run_test_route_2

.PHONY: all clean test $(TESTS)
all: $(TEST_EXEC_DIR)/test_dijkstra \
	 $(TEST_EXEC_DIR)/test_route

test: $(TESTS)

# DIJKSTRA
# ==============================================================================
# @ outputs the target
# ^ outputs all prerequisites
$(TEST_EXEC_DIR)/test_dijkstra: $(BUILD_TEST_DIR)/test_dijkstra.o $(BUILD_OBJ_DIR)/dijkstra.o $(BUILD_OBJ_DIR)/dynamic_memory.o
	$(call explain_build,Making $(notdir $@))
	mkdir -p $(TEST_EXEC_DIR)
	$(CC) $(CFLAGS) $(C_LINT_FLAGS) $(C_SANITIZERS) $^ -o $@

# < outputs the first prerequisite
# NOTE: We don't add an include path, as the path to the .h files can be found
#       relative to the .c files
$(BUILD_TEST_DIR)/test_dijkstra.o: test_dijkstra.c
	$(call explain_build,Making $(notdir $@))
	mkdir -p $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(C_LINT_FLAGS) $(C_SANITIZERS) -c $< -o $@

# Tests
run_test_dijkstra_1: $(TEST_EXEC_DIR)/test_dijkstra
	$(call run_test,$@,$< getMinDistanceIdx)

run_test_dijkstra_2: $(TEST_EXEC_DIR)/test_dijkstra
	$(call run_test,$@,$< registerRoute)

run_test_dijkstra_3: $(TEST_EXEC_DIR)/test_dijkstra
	$(call run_test,$@,$< dijkstra)
# ==============================================================================

# RECEIVER
# ==============================================================================
$(TEST_EXEC_DIR)/test_receiver: $(BUILD_TEST_DIR)/test_receiver.o $(BUILD_OBJ_DIR)/receiver.o
	$(call explain_build,Making $(notdir $@))
	mkdir -p $(TEST_EXEC_DIR)
	$(CC) $(CFLAGS) $(C_LINT_FLAGS) $(C_SANITIZERS) $^ -o $@

# Building the object files
$(BUILD_TEST_DIR)/test_receiver.o: test_receiver.c
	$(call explain_build,Making $(notdir $@))
	mkdir -p $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(C_LINT_FLAGS) $(C_SANITIZERS) -c $< -o $@

# Tests
run_test_receiver_1: $(TEST_EXEC_DIR)/test_receiver
	$(call run_test,$@,LOG_DIR=$(TEST_EXEC_DIR) $< checkAllNodesReceived)

run_test_receiver_2: $(TEST_EXEC_DIR)/test_receiver
	$(call run_test,$@,$< createAdjacencyMatrix)
# ==============================================================================

# ROUTE
# ==============================================================================
$(TEST_EXEC_DIR)/test_route: $(BUILD_TEST_DIR)/test_route.o $(BUILD_OBJ_DIR)/route.o $(BUILD_OBJ_DIR)/dynamic_memory.o $(LIB_DIR)/libprint.$(LIB_EXTENSION)
	$(call explain_build,Making $(notdir $@))
	mkdir -p $(TEST_EXEC_DIR)
	$(CC) $(CFLAGS) $(C_LINT_FLAGS) $(C_SANITIZERS) $^ -o $@

# Building the object files
# NOTE: The only reason why we add an include path is because we have pretended
#       that print_lib.h comes from an external library
#       Since we use statements like
#       #include "print_lib/include/print_lib.h", and since print_lib doesn't
#       reside in "this" directory, we need to make sure that the compiler can
#       find it
$(BUILD_TEST_DIR)/test_route.o: test_route.c
	$(call explain_build,Making $(notdir $@))
	mkdir -p $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(C_LINT_FLAGS) $(C_SANITIZERS) -I$(ASSIGNMENT_FILES_SRC) -c $< -o $@

# Tests
run_test_route_1: $(TEST_EXEC_DIR)/test_route
	$(call run_test,$@,LOG_DIR=$(TEST_EXEC_DIR) $< printEdges && diff data/test_route_1_logfile.txt $(TEST_EXEC_DIR)/logfile.txt)

run_test_route_2: $(TEST_EXEC_DIR)/test_route
	$(call run_test,$@,$< createRoutingTables)
# ==============================================================================

clean:
	rm -rf $(BUILD_TEST_DIR) $(TEST_EXEC_DIR)
