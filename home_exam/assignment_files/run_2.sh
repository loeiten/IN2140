#!/bin/bash

# Example usage:
# bash run_2.sh 2345 ../../build/home_exam

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

MESSAGES_PATH="$SCRIPT_DIR"/messages_2.txt
LOG_BASE="$EXEC_DIR"/log_2

# Set to "valgrind" to run valgrind on nodes and server
VALGRIND=""

# Copy the messages for this scenario into the data file that own address 1 should read.
cp "$MESSAGES_PATH" "$EXEC_DIR"/data.txt

# Run routing server C
LOG_DIR="$LOG_BASE"/routing_server $VALGRIND "$EXEC_DIR"/routing_server "$BASE_PORT" 9 &

# Wait for the central server to start. If you have to wait for more than 1 seconds you
# are probably doing something wrong.
sleep 1

# Run all nodes
LOG_DIR="$LOG_BASE"/1   $VALGRIND "$EXEC_DIR"/node "$BASE_PORT" 1 2:4 4:2 16:8                 &
LOG_DIR="$LOG_BASE"/2   $VALGRIND "$EXEC_DIR"/node "$BASE_PORT" 2 1:4 8:1                      &
LOG_DIR="$LOG_BASE"/4   $VALGRIND "$EXEC_DIR"/node "$BASE_PORT" 4 1:2 16:5                     &
LOG_DIR="$LOG_BASE"/8   $VALGRIND "$EXEC_DIR"/node "$BASE_PORT" 8 2:1 16:3 32:1                &
LOG_DIR="$LOG_BASE"/16  $VALGRIND "$EXEC_DIR"/node "$BASE_PORT" 16 1:8 4:5 8:3 32:2 64:5 256:2 &
LOG_DIR="$LOG_BASE"/32  $VALGRIND "$EXEC_DIR"/node "$BASE_PORT" 32 8:1 16:2 128:2 256:4        &
LOG_DIR="$LOG_BASE"/64  $VALGRIND "$EXEC_DIR"/node "$BASE_PORT" 64 16:5 256:5                  &
LOG_DIR="$LOG_BASE"/128 $VALGRIND "$EXEC_DIR"/node "$BASE_PORT" 128 32:2 256:3                 &
LOG_DIR="$LOG_BASE"/256 $VALGRIND "$EXEC_DIR"/node "$BASE_PORT" 256 16:2 32:4 64:5 128:3       &

# Terminate all processes in case of failure
trap 'trap - SIGTERM && kill -- -$$' SIGINT SIGTERM

# Wait for processes to finish
echo "Waiting for processes to finish"
wait
exit 0
