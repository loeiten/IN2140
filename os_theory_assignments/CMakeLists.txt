set(source_files
    set_2_os_assignment_5.c
)

# Create the executables
make_executable(SOURCE ${source_files})

# Test
# set_2_os_assignment_5
do_test(NAME set_2_os_assignment_5 NO 1 ARGS EXPECTED "Process [0-9]* going to sleep for [0-2] seconds\nProcess [0-9]* going to sleep for [0-2] seconds\nProcess [0-9]* going to sleep for [0-2] seconds\nsum\\(1,3000\\)=4501500\n")
