#!/bin/bash

SCRIPT_DIR=$(dirname -- "$(readlink -f -- "${BASH_SOURCE[0]}")")
source "$SCRIPT_DIR"/test_multiple_processes.sh

PORT=$1
TEST_EXEC_DIR=$2
TEST_DATA_DIR=$3

COMMANDS=()
# We need pipefail in order to report the exit status of the first command
# From the docs:
#    If set, the return value of a pipeline is the value of the last (rightmost)
#    command to exit with a non-zero status, or zero if all commands in the
#    pipeline exit successfully.
COMMANDS+=("set -o pipefail; $TEST_EXEC_DIR/test_routing_server_communication \
           populateNodeArrayAndSendRoutingTables $PORT \
           2>&1 | tee $TEST_EXEC_DIR/test_communication_2_output.txt")
COMMANDS+=("$TEST_EXEC_DIR/test_node_communication \
            sendEdgeInformationAndReceiveRoutingTable $PORT 101")
COMMANDS+=("$TEST_EXEC_DIR/test_node_communication \
            sendEdgeInformationAndReceiveRoutingTable $PORT 15")
COMMANDS+=("$TEST_EXEC_DIR/test_node_communication \
            sendEdgeInformationAndReceiveRoutingTable $PORT 42")

# Run all the commands as subprocesses
# NOTE: Does not work if one use $()
run_subprocesses "${COMMANDS[@]}"

COMMAND="diff $TEST_DATA_DIR/test_communication_2_output.txt $TEST_EXEC_DIR/test_communication_2_output.txt"
echo "$COMMAND"
echo "$COMMAND" | bash
