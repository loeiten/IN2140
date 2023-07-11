# Copy the test data
set(DATA_DIR "data")

file(GLOB data_files "${DATA_DIR}/*")
foreach(data_file ${data_files})
    get_filename_component(cur_file ${data_file} NAME)
    file(COPY ${data_file}
         DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${DATA_DIR}")
endforeach(data_file ${text_files})

# Make a binary of the tests
add_executable(test_binary_file
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/binary_file.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/router.c"
    "test_binary_file.c"
    )
add_executable(test_router
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/router.c"
    "test_router.c"
    )

# Add includes
target_include_directories(test_binary_file
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include"
)
target_include_directories(test_router
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include"
)

# Add the unit tests
# Test binary file
do_test(NAME test_binary_file NO 1 ARGS "readNewline" EXPECTED "Expected newline, but got 'm'\nFailed to read from file: .*\n")

# Test router
do_test(NAME test_router NO 1 ARGS "getBinaryString" "0" "00000000" EXPECTED "Success\n")
