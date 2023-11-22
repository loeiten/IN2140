#!/bin/bash

set -e

PORT=$1
EXEC_DIR=$2
TEST_EXEC_DIR=$3
TEST_DATA_DIR=$4

SCRIPT_DIR=$(dirname -- "$(readlink -f -- "${BASH_SOURCE[0]}")")
source "$SCRIPT_DIR"/test_multiple_processes.sh

# NOTE: We are working with the following graph
#    7   22
#  o - o - o
#  1  15   42

rm -rf "$TEST_EXEC_DIR"/data.txt "$TEST_EXEC_DIR"/test_integration_1_routing_server "$TEST_EXEC_DIR"/test_integration_1_15 "$TEST_EXEC_DIR"/test_integration_1_42
cp "$TEST_DATA_DIR"/test_messages.txt "$TEST_EXEC_DIR"/data.txt

# Change directory so that data.txt can be read
cd "$TEST_EXEC_DIR"

# Start the routing server
# NOTE: As the nodes can arrive an arbitrary order, the log should not be diffed
COMMAND="LOG_DIR=$TEST_EXEC_DIR/test_integration_1_routing_server $EXEC_DIR/routing_server $PORT 3 & sleep 1"
echo "$COMMAND"
echo "$COMMAND" | bash

# Start the nodes
COMMANDS=()
COMMANDS+=("$EXEC_DIR/node $PORT 1 15:7")
COMMANDS+=("LOG_DIR=$TEST_EXEC_DIR/test_integration_1_15 $EXEC_DIR/node \
            $PORT 15 1:7 42:22")
COMMANDS+=("LOG_DIR=$TEST_EXEC_DIR/test_integration_1_42 $EXEC_DIR/node \
            $PORT 42 15:22")

# Run all the commands as subprocesses
# NOTE: Does not work if one use $()
run_subprocesses "${COMMANDS[@]}"

COMMAND="diff $TEST_DATA_DIR/test_integration_1_15_output.txt $TEST_EXEC_DIR/test_integration_1_15/logfile.txt"
echo "$COMMAND"
echo "$COMMAND" | bash
COMMAND="diff $TEST_DATA_DIR/test_integration_1_42_output.txt $TEST_EXEC_DIR/test_integration_1_42/logfile.txt"
echo "$COMMAND"
echo "$COMMAND" | bash
