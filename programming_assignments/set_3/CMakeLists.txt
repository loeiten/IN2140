set(source_files
    s3_assignment_2.c
    s3_assignment_3.c
    s3_assignment_5.c
    s3_assignment_6.c
    s3_assignment_7.c
)

set(link_with_utils
    s3_assignment_3
    s3_assignment_5
    s3_assignment_7
)

set(link_with_my_math
    s3_assignment_6
)

# Create executables
make_executable(SOURCE ${source_files})

# Link with libraries
link_executable(TARGETS ${link_with_utils} LIB utils)
link_executable(TARGETS ${link_with_my_math} LIB my_math)

# Test
# assignment_2
do_test(NAME s3_assignment_2 NO 1 ARGS EXPECTED "hello world\nHello world\nHello world\n")

# assignment_3
do_test(NAME s3_assignment_3 NO 1 ARGS EXPECTED "I'm a string literal copied to the heap before being printed\n")

# assignment_5
# Copy files
file(GLOB text_files "s3_assignment_5_*.txt")
foreach(text_file ${text_files})
    get_filename_component(cur_file ${text_file} NAME)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/${cur_file}
         DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endforeach(text_file ${text_files})
# Run tests
do_test_with_stdin(NAME s3_assignment_5
                   NO 1
                   STDIN "\"s3_assignment_5_1.txt\ns3_assignment_5_2.txt\nthis_does_not_exist.txt\nQUIT\n\""
                   EXPECTED "Enter a path: First line\nThird line\nThis is the last line\nEnter a path: This will be printed\nEnter a path: Failed to open this_does_not_exist.txt\nEnter a path: \n"
)

# assignment_6
# With CMake
do_test(NAME s3_assignment_6 NO 1 ARGS EXPECTED "20\n")
# With Makefile
# https://stackoverflow.com/a/50204961/2786884
# https://stackoverflow.com/a/44505200/2786884
# With a static library
add_custom_command(OUTPUT s3_assignment_6_static_command
                  COMMAND make -f Makefile_static
                  DEPENDS s3_assignment_6.c my_math
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_custom_target(s3_assignment_6_static ALL DEPENDS s3_assignment_6_static_command)
do_test(NAME s3_assignment_6_static NO 1 ARGS EXPECTED "20\n")
# With a shared library
add_custom_command(OUTPUT s3_assignment_6_shared_command
                  COMMAND make -f Makefile_shared
                  DEPENDS s3_assignment_6.c my_math
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_custom_target(s3_assignment_6_shared ALL DEPENDS s3_assignment_6_shared_command)
do_test(NAME s3_assignment_6_shared NO 1 ARGS EXPECTED "20\n")

# assignment_7
do_test(NAME s3_assignment_7 NO 1 ARGS EXPECTED "Done\n")
do_test(NAME s3_assignment_7 NO 2 ARGS "DEBUG" EXPECTED "\\[DEBUG\\] The value of a is 10\n\\[INFO\\] Hello world :\\)\n\\[ERROR\\] Something is wrong here\nDone\n")
do_test(NAME s3_assignment_7 NO 3 ARGS "INFO" EXPECTED "\\[INFO\\] Hello world :\\)\n\\[ERROR\\] Something is wrong here\nDone\n")
do_test(NAME s3_assignment_7 NO 4 ARGS "ERROR" EXPECTED "\\[ERROR\\] Something is wrong here\nDone\n")
do_test(NAME s3_assignment_7 NO 5 ARGS "FOO" EXPECTED "Usage: ./s3_assignment_7 \\[DEBUG|INFO|ERROR\\]\n")
