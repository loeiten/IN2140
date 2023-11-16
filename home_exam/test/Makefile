include ../common.mk

# Comment: We could've created something like a %.o: %.c pattern target, which
#          would also have all the changes in node and routing_server as a
#          prerequisite.
#		   However, we'd like a bit more fine grained control of the build
#		   process (i.e. we don't want to build all tests anew if there is only
#          one small change in an unrelated file).
#		   Hence, we are adding the tests one by one

TESTS := run_test_adjacency_matrix_1 \
		 run_test_communication_1 \
		 run_test_communication_2 \
		 run_test_dijkstra_1 \
		 run_test_dijkstra_2 \
		 run_test_dijkstra_3 \
		 run_test_node_communication_1 \
		 run_test_node_communication_3 \
		 run_test_node_communication_4 \
		 run_test_node_parser_1 \
		 run_test_route_1 \
		 run_test_route_2 \
		 run_test_routing_server_communication_1 \
		 run_test_routing_server_communication_2 \
		 run_test_validation_1 \
		 run_test_validation_2 \
		 run_test_validation_3 \
		 run_test_validation_4 \
		 run_test_validation_5 \
		 run_test_validation_6 \

BINARIES := $(TEST_EXEC_DIR)/test_adjacency_matrix \
	 		$(TEST_EXEC_DIR)/test_dijkstra \
	 		$(TEST_EXEC_DIR)/test_node_communication \
	 		$(TEST_EXEC_DIR)/test_node_parser \
	 		$(TEST_EXEC_DIR)/test_route \
	 		$(TEST_EXEC_DIR)/test_routing_server_communication \
	 		$(TEST_EXEC_DIR)/test_validation

.PHONY: all clean test $(TESTS)

all: $(BINARIES)

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

# COMMUNICATION
# ==============================================================================
# Note the binaries are created in other NODE_COMMUNICATION and
# ROUTING_SERVER_COMMUNICATION

PORT_1 := 1028
PORT_2 := 1029

# Test
run_test_communication_1: $(TEST_EXEC_DIR)/test_routing_server_communication $(TEST_EXEC_DIR)/test_node_communication
	$(call run_test,$@, bash $(TEST_DATA_DIR)/../test_communication_1.sh $(PORT_2) $(TEST_EXEC_DIR) $(TEST_DATA_DIR))

run_test_communication_2: $(TEST_EXEC_DIR)/test_routing_server_communication $(TEST_EXEC_DIR)/test_node_communication
	$(call run_test,$@, bash $(TEST_DATA_DIR)/../test_communication_2.sh $(PORT_2) $(TEST_EXEC_DIR) $(TEST_DATA_DIR))
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

# NODE_COMMUNICATION
# ==============================================================================
$(TEST_EXEC_DIR)/test_node_communication: $(BUILD_TEST_DIR)/test_node_communication.o $(BUILD_OBJ_DIR)/node_communication.o $(LIB_DIR)/libprint.$(LIB_EXTENSION) $(BUILD_OBJ_DIR)/common.o $(BUILD_OBJ_DIR)/dynamic_memory.o
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
$(BUILD_TEST_DIR)/test_node_communication.o: test_node_communication.c
	$(call explain_build,Making $(notdir $@))
	mkdir -p $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(C_LINT_FLAGS) $(C_SANITIZERS) -I$(ASSIGNMENT_FILES_SRC) -c $< -o $@

# Tests
run_test_node_communication_1: $(TEST_EXEC_DIR)/test_node_communication
	$(call run_test,$@,$< getUDPSocket 2256)

# NOTE: getTCPClientSocket is tested in test_communication_1
# NOTE: sendEdgeInformation and receiveRoutingTable are tested in test_communication_2

# FIXME: Test receiveAndForwardPackets
# FIXME: Test prepareAndSendPackets

run_test_node_communication_3: $(TEST_EXEC_DIR)/test_node_communication
	$(call run_test,$@,$< extractLengthDestinationAndMessage)

run_test_node_communication_4: $(TEST_EXEC_DIR)/test_node_communication
	$(call run_test,$@,$< createPacket)
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
run_test_node_parser_1: $(TEST_EXEC_DIR)/test_node_parser
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
	$(call run_test,$@,LOG_DIR=$(TEST_EXEC_DIR) $< printEdges && diff $(TEST_DATA_DIR)/test_route_1_logfile.txt $(TEST_EXEC_DIR)/logfile.txt)

run_test_route_2: $(TEST_EXEC_DIR)/test_route
	$(call run_test,$@,$< createRoutingTableArray)
# ==============================================================================

# ROUTING_SERVER_COMMUNICATION
# ==============================================================================
$(TEST_EXEC_DIR)/test_routing_server_communication: $(BUILD_TEST_DIR)/test_routing_server_communication.o $(BUILD_OBJ_DIR)/routing_server_communication.o $(LIB_DIR)/libprint.$(LIB_EXTENSION) $(BUILD_OBJ_DIR)/common.o $(BUILD_OBJ_DIR)/dynamic_memory.o $(BUILD_OBJ_DIR)/test_tools.o
	$(call explain_build,Making $(notdir $@))
	mkdir -p $(TEST_EXEC_DIR)
	$(CC) $(CFLAGS) $(C_LINT_FLAGS) $(C_SANITIZERS) $^ -o $@

# Building the object files
$(BUILD_TEST_DIR)/test_routing_server_communication.o: test_routing_server_communication.c
	$(call explain_build,Making $(notdir $@))
	mkdir -p $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS) $(C_LINT_FLAGS) $(C_SANITIZERS) -c $< -o $@

# Tests
run_test_routing_server_communication_1: $(TEST_EXEC_DIR)/test_routing_server_communication
	$(call run_test,$@,$< getTCPServerSocket 1025)

# NOTE: populateNodeArray and sendRoutingTables are tested in test_communication_2

run_test_routing_server_communication_2: $(TEST_EXEC_DIR)/test_routing_server_communication
	$(call run_test,$@,$< translateTableFromIdxToAddress)
# ==============================================================================

# VALIDATION
# ==============================================================================
$(TEST_EXEC_DIR)/test_validation: $(BUILD_TEST_DIR)/test_validation.o $(BUILD_OBJ_DIR)/validation.o $(BUILD_OBJ_DIR)/common.o $(BUILD_OBJ_DIR)/dynamic_memory.o $(BUILD_OBJ_DIR)/test_tools.o
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
	$(call run_test,$@,$< addInvalidEdge 2>&1 | tee $(TEST_EXEC_DIR)/test_validation_3_output.txt && \
	  diff $(TEST_DATA_DIR)/test_validation_3_output.txt $(TEST_EXEC_DIR)/test_validation_3_output.txt)

run_test_validation_4: $(TEST_EXEC_DIR)/test_validation
	$(call run_test,$@,$< checkDualReport)

run_test_validation_5: $(TEST_EXEC_DIR)/test_validation
	$(call run_test,$@,$< checkIfEdgeIsValid 2>&1 | tee $(TEST_EXEC_DIR)/test_validation_5_output.txt && \
	  diff $(TEST_DATA_DIR)/test_validation_5_output.txt $(TEST_EXEC_DIR)/test_validation_5_output.txt)

run_test_validation_6: $(TEST_EXEC_DIR)/test_validation
	$(call run_test,$@,$< checkAllNodesReceived 2>&1 | tee $(TEST_EXEC_DIR)/test_validation_6_output.txt && \
	  diff $(TEST_DATA_DIR)/test_validation_6_output.txt $(TEST_EXEC_DIR)/test_validation_6_output.txt)
# ==============================================================================

clean:
	rm -rf $(BUILD_TEST_DIR) $(TEST_EXEC_DIR)
