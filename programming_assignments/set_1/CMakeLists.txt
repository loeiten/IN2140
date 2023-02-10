SET(source_files
    assignment_1.c
)

# Create executables
make_executable(${source_files})

# Test
do_test(assignment_1 1 "Hello World" "Input: Hello World\n")
