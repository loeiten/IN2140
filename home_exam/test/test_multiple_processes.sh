#!/bin/bash
# From https://unix.stackexchange.com/a/436932/103583

# NOTE: Does not work if one use $()
function wait_and_get_exit_codes() {
    CHILDREN=("$@")
    for pid in "${CHILDREN[@]}"; do
       EXIT_CODE=0;
       wait "${pid}"
       EXIT_CODE=$?
       if [[ "${EXIT_CODE}" != "0" ]]; then
           echo "PID=$pid failed with exit code ${EXIT_CODE}" ;
           exit "$EXIT_CODE";
       fi
   done
}

function run_subprocesses(){
    # Capture all arguments to this function
    COMMANDS=("$@")
    CHILDREN_PIDS=()
    DELIMITER=""
    for command in "${COMMANDS[@]}"; do
        # Strip extra spaces
        command=$(echo "$command" | tr -s " ")
        # Print the command
        echo "$DELIMITER"
        echo -n "$command"
        # Pipe the command to bash
        # $! is replaced by the process identifier of the last process placed in
        # the background in the current shell, if any
        echo "$command" | bash &
        CHILDREN_PIDS+=("$!")
        DELIMITER=" & \ "
    done
    # Print newline
    echo " "

    # Wait for all the PIDs to finish
    wait_and_get_exit_codes "${CHILDREN_PIDS[@]}"
}
