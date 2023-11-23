#!/bin/bash

# Example usage:
# bash run_1.sh 1777 ../../build/home_exam

# Get the dir of this script
SCRIPT_DIR=$(cd "$(dirname "$0")" && pwd)

MIN_PORT=1024
MAX_PORT=65500

if [ "$#" -ne 2 ]; then
    echo "Please provide base port to the script. Value between [$MIN_PORT - $MAX_PORT]"
    echo "Please provide the path to the execution directory"
    exit
fi

if [ "$1" -lt $MIN_PORT ] || [ "$1" -gt $MAX_PORT ]; then
    echo "Port $1 is not in the range [$MIN_PORT, $MAX_PORT]"
    exit
fi

BASE_PORT=$1
EXEC_DIR=$(realpath "$2")
# cd to the EXEC_DIR
# This means that the CWD of the c binary will be EXEC_DIR, and hence it can see
# the data.txt file
cd "$EXEC_DIR" || exit

MESSAGES_PATH="$SCRIPT_DIR"/messages_1.txt
LOG_BASE="$EXEC_DIR"/log_1

# Set to "valgrind" to run valgrind on nodes and server
VALGRIND=""

# Copy the messages for this scenario into the data file that own address 1 should read.
cp "$MESSAGES_PATH" "$EXEC_DIR"/data.txt

# Run routing server C
LOG_DIR="$LOG_BASE"/routing_server $VALGRIND "$EXEC_DIR"/routing_server "$BASE_PORT" 8 &

# Wait for the central server to start. If you have to wait for more than 1 seconds you
# are probably doing something wrong.
sleep 1

# Run all nodes
LOG_DIR="$LOG_BASE"/1   $VALGRIND "$EXEC_DIR"/node "$BASE_PORT" 1 11:2 103:6         &
LOG_DIR="$LOG_BASE"/11  $VALGRIND "$EXEC_DIR"/node "$BASE_PORT" 11 1:2 13:7 19:2     &
LOG_DIR="$LOG_BASE"/13  $VALGRIND "$EXEC_DIR"/node "$BASE_PORT" 13 11:7 17:3 101:4   &
LOG_DIR="$LOG_BASE"/17  $VALGRIND "$EXEC_DIR"/node "$BASE_PORT" 17 13:3 107:2        &
LOG_DIR="$LOG_BASE"/19  $VALGRIND "$EXEC_DIR"/node "$BASE_PORT" 19 11:2 101:2 103:1  &
LOG_DIR="$LOG_BASE"/101 $VALGRIND "$EXEC_DIR"/node "$BASE_PORT" 101 13:4 19:2 107:2  &
LOG_DIR="$LOG_BASE"/103 $VALGRIND "$EXEC_DIR"/node "$BASE_PORT" 103 1:6 19:1 107:4   &
LOG_DIR="$LOG_BASE"/107 $VALGRIND "$EXEC_DIR"/node "$BASE_PORT" 107 17:2 101:2 103:4 &

# Terminate all processes in case of failure
trap 'trap - SIGTERM && kill -- -$$' SIGINT SIGTERM

# Wait for processes to finish
echo "Waiting for processes to finish"
wait
exit 0
