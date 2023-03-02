set(source_files
    s3_assignment_2.c
    s3_assignment_3.c
    s3_assignment_5.c
)

set(link_with_utils
    s3_assignment_3
    s3_assignment_5
)

# Create executables
make_executable(SOURCE ${source_files})

# Link with libraries
link_executable(TARGETS ${link_with_utils} LIB utils)

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
do_test(NAME s3_assignment_6 NO 1 ARGS EXPECTED "\n20")
