set(source_files
    set_2_os_assignment_5.c
    set_4_os_assignment_7.c
    set_4_os_assignment_8.c
    set_5_os_assignment_7.c
)

# Create the executables
make_executable(SOURCE ${source_files})

# Test
# set_2_os_assignment_5
do_test(NAME set_2_os_assignment_5 NO 1 ARGS EXPECTED "Process [0-9]* going to sleep for [0-2] seconds\nProcess [0-9]* going to sleep for [0-2] seconds\nProcess [0-9]* going to sleep for [0-2] seconds\nsum\\(1,3000\\)=4501500\n")

# set_4_os_assignment_7
do_test(NAME set_4_os_assignment_7 NO 1 ARGS EXPECTED ".*\n")

# set_4_os_assignment_8
do_test(NAME set_4_os_assignment_8 NO 1 ARGS EXPECTED "Hello, world!\n")

# set_5_os_assignment_7
do_test(NAME set_5_os_assignment_7 NO 1 ARGS "Hello, world!" EXPECTED "Hello, world! from pid: [^0][0-9]*\n")
