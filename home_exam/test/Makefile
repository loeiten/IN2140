include ../common.mk

# Comment: We could've created something like a %.o: %.c pattern target, which
#          would also have all the changes in node and routing_server as a
#          prerequisite.
#		   However, we'd like a bit more fine grained control of the build
#		   process (i.e. we don't want to build all tests anew if there is only
#          one small change in an unrelated file).
#		   Hence, we are adding the tests one by one

TESTS := run_test_adjacency_matrix_1 \
		 run_test_dijkstra_1 \
		 run_test_dijkstra_2 \
		 run_test_dijkstra_3 \
		 run_test_node_parser_1 \
		 run_test_route_1 \
		 run_test_route_2 \
		 run_test_validation_1 \
		 run_test_validation_2 \
		 run_test_validation_3 \
		 run_test_validation_4 \
		 run_test_validation_5 \
		 run_test_validation_6 \

.PHONY: all clean test $(TESTS)
all: $(TEST_EXEC_DIR)/test_dijkstra \
	 $(TEST_EXEC_DIR)/test_route

test: $(TESTS)

# TOOLS
# ==============================================================================
TEST_TOOLS_SOURCE_FILES := $(wildcard $(CUR_DIR)test/src/*.c)
TEST_TOOLS_SRC := $(word 1, $(dir $(TEST_TOOLS_SOURCE_FILES)))
# Remove trailing slash
TEST_TOOLS_SRC := $(patsubst %/,%,$(TEST_TOOLS_SRC))
# NOTE:
# Substitution references
# $(var:pattern=replacement)
# are equivalent to
# $(patsubst %suffix,%replacement,$(var))
# Here we cannot use the former, and must use the latter
OBJECT_FILES := $(patsubst $(TEST_TOOLS_SRC)%.c,$(BUILD_OBJ_DIR)%.o,$(TEST_TOOLS_SOURCE_FILES))

# Building the object files
$(BUILD_OBJ_DIR)%.o: $(TEST_TOOLS_SRC)%.c
	$(call explain_build,Building the object files for test tools)
	mkdir -p $(BUILD_OBJ_DIR)
	$(CC) $(CFLAGS) $(C_LINT_FLAGS) $(C_SANITIZERS) -c $< -o $@
# ==============================================================================

# ADJACENCY_MATRIX
# ==============================================================================
# @ outputs the target
# ^ outputs all prerequisites
$(TEST_EXEC_DIR)/test_adjacency_matrix: $(BUILD_TEST_DIR)/test_adjacency_matrix.o $(BUILD_OBJ_DIR)/adjacency_matrix.o $(BUILD_OBJ_DIR)/common.o $(BUILD_OBJ_DIR)/dynamic_memory.o $(BUILD_OBJ_DIR)/test_tools.o
	$(call explain_build,Making $(notdir $@))
	mkdir -p $(TEST_EXEC_DIR)
	$(CC) $(CFLAGS) $(C_LINT_FLAGS) $(C_SANITIZERS) $^ -o $@

# < outputs the first prerequisite
# NOTE: We don't add an include path, as the path to the .h files can be found
#       relative to the .c files
$(BUILD_TEST_DIR)/test_adjacency_matrix.o: test_adjacency_matrix.c
	$(call explain_build,Making $(notdir $@))
	mkdir -p $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(C_LINT_FLAGS) $(C_SANITIZERS) -c $< -o $@

# Tests
run_test_adjacency_matrix_1: $(TEST_EXEC_DIR)/test_adjacency_matrix
	$(call run_test,$@,$< createAdjacencyMatrix)
# ==============================================================================

# DIJKSTRA
# ==============================================================================
$(TEST_EXEC_DIR)/test_dijkstra: $(BUILD_TEST_DIR)/test_dijkstra.o $(BUILD_OBJ_DIR)/dijkstra.o $(BUILD_OBJ_DIR)/dynamic_memory.o
	$(call explain_build,Making $(notdir $@))
	mkdir -p $(TEST_EXEC_DIR)
	$(CC) $(CFLAGS) $(C_LINT_FLAGS) $(C_SANITIZERS) $^ -o $@

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

# NODE_PARSER
# ==============================================================================
$(TEST_EXEC_DIR)/test_node_parser: $(BUILD_TEST_DIR)/test_node_parser.o $(BUILD_OBJ_DIR)/node_parser.o $(BUILD_OBJ_DIR)/common.o $(BUILD_OBJ_DIR)/dynamic_memory.o
	$(call explain_build,Making $(notdir $@))
	mkdir -p $(TEST_EXEC_DIR)
	$(CC) $(CFLAGS) $(C_LINT_FLAGS) $(C_SANITIZERS) $^ -o $@

# Building the object files
$(BUILD_TEST_DIR)/test_node_parser.o: test_node_parser.c
	$(call explain_build,Making $(notdir $@))
	mkdir -p $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(C_LINT_FLAGS) $(C_SANITIZERS) -c $< -o $@

# Tests
run_test_node_parser_1 : $(TEST_EXEC_DIR)/test_node_parser
	$(call run_test,$@,$< parseNodes)
# ==============================================================================

# ROUTE
# ==============================================================================
$(TEST_EXEC_DIR)/test_route: $(BUILD_TEST_DIR)/test_route.o $(BUILD_OBJ_DIR)/route.o $(LIB_DIR)/libprint.$(LIB_EXTENSION) $(BUILD_OBJ_DIR)/common.o $(BUILD_OBJ_DIR)/dynamic_memory.o
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

# VALIDATION
# ==============================================================================
$(TEST_EXEC_DIR)/test_validation: $(BUILD_TEST_DIR)/test_validation.o $(BUILD_OBJ_DIR)/validation.o $(BUILD_OBJ_DIR)/common.o $(BUILD_OBJ_DIR)/dynamic_memory.o  $(BUILD_OBJ_DIR)/test_tools.o
	$(call explain_build,Making $(notdir $@))
	mkdir -p $(TEST_EXEC_DIR)
	$(CC) $(CFLAGS) $(C_LINT_FLAGS) $(C_SANITIZERS) $^ -o $@

# Building the object files
$(BUILD_TEST_DIR)/test_validation.o: test_validation.c
	$(call explain_build,Making $(notdir $@))
	mkdir -p $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(C_LINT_FLAGS) $(C_SANITIZERS) -c $< -o $@

# Tests
run_test_validation_1: $(TEST_EXEC_DIR)/test_validation
	$(call run_test,$@,$< isEdgePresent)

run_test_validation_2: $(TEST_EXEC_DIR)/test_validation
	$(call run_test,$@,$< addEdgeToEdgeCounterArray)

run_test_validation_3: $(TEST_EXEC_DIR)/test_validation
	$(call run_test,$@,$< addInvalidEdge 2>&1 | tee $(TEST_EXEC_DIR)/test_validation_3_output.txt && diff data/test_validation_3_output.txt $(TEST_EXEC_DIR)/test_validation_3_output.txt)

run_test_validation_4: $(TEST_EXEC_DIR)/test_validation
	$(call run_test,$@,$< checkDualReport)

run_test_validation_5: $(TEST_EXEC_DIR)/test_validation
	$(call run_test,$@,$< checkIfEdgeIsValid 2>&1 | tee $(TEST_EXEC_DIR)/test_validation_5_output.txt && diff data/test_validation_5_output.txt $(TEST_EXEC_DIR)/test_validation_5_output.txt)

run_test_validation_6: $(TEST_EXEC_DIR)/test_validation
	$(call run_test,$@,$< checkAllNodesReceived 2>&1 | tee $(TEST_EXEC_DIR)/test_validation_6_output.txt && diff data/test_validation_6_output.txt $(TEST_EXEC_DIR)/test_validation_6_output.txt)
# ==============================================================================

clean:
	rm -rf $(BUILD_TEST_DIR) $(TEST_EXEC_DIR)
