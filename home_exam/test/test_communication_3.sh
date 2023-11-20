#!/bin/bash

source test_multiple_processes.sh

PORT=$1
TEST_EXEC_DIR=$2
TEST_DATA_DIR=$3

# NOTE: We prepareAndSendPackets the last as recv is blocking and the ports
#       of the forwarding nodes needs to be ready to receive when we are sending
COMMANDS=()
COMMANDS+=("LOG_DIR=$TEST_EXEC_DIR/test_communication_3_15 $TEST_EXEC_DIR/test_node_communication \
            receiveAndForwardPackets $PORT 15")
COMMANDS+=("LOG_DIR=$TEST_EXEC_DIR/test_communication_3_42 $TEST_EXEC_DIR/test_node_communication \
            receiveAndForwardPackets $PORT 42")
COMMANDS+=("$TEST_EXEC_DIR/test_node_communication \
            prepareAndSendPackets $TEST_DATA_DIR/test_communication_3_input.txt $PORT 101")

# Run all the commands as subprocesses
# NOTE: Does not work if one use $()
run_subprocesses "${COMMANDS[@]}"

diff "$TEST_DATA_DIR"/test_communication_3_15_output.txt "$TEST_EXEC_DIR"/test_communication_3_15/logfile.txt
diff "$TEST_DATA_DIR"/test_communication_3_42_output.txt "$TEST_EXEC_DIR"/test_communication_3_42/logfile.txt
