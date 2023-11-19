#!/bin/bash

source test_multiple_processes.sh

PORT=$1
TEST_EXEC_DIR=$2
TEST_DATA_DIR=$3

COMMANDS=()
COMMANDS+=("$TEST_EXEC_DIR/test_node_communication \
            prepareAndSendPackets $TEST_DATA_DIR/test_communication_3_input.txt $PORT 101")
COMMANDS+=("$TEST_EXEC_DIR/test_node_communication \
            receiveAndForwardPackets $PORT 15")
COMMANDS+=("$TEST_EXEC_DIR/test_node_communication \
            receiveAndForwardPackets $PORT 42")

# Run all the commands as subprocesses
# NOTE: Does not work if one use $()
run_subprocesses "${COMMANDS[@]}"

diff "$TEST_DATA_DIR"/test_communication_3_output.txt "$TEST_EXEC_DIR"/test_communication_3_output.txt
