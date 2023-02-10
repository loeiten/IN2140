SET(source_files
    assignment_1.c
)

# Create executables
make_executable(${source_files})

# Test
do_test(assignment_1 1 "Hello World" "Input: Hello World\n")
do_test(assignment_1 2 "" "Usage: ./assignment_1 STR\n")
do_test(assignment_1 3 "0" "Number: 0\n")
do_test(assignment_1 4 "-1" "Input: -1\n")
do_test(assignment_1 5 "987" "Number: 987\n")
do_test(assignment_1 6 "987ab" "Input: 987ab\n")
