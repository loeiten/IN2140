# Copy the test data
set(DATA_DIR "data")

file(GLOB data_files "${DATA_DIR}/*")
foreach(data_file ${data_files})
    get_filename_component(cur_file ${data_file} NAME)
    file(COPY ${data_file}
         DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${DATA_DIR}")
endforeach(data_file ${text_files})

# Make a binary of the router_operations
add_executable(test_binary_file
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/binary_file.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/router.c"
    "test_binary_file.c"
    )

# Add includes
target_include_directories(router_operation
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include"
)

# Add the unit tests
do_test(NAME test_binary_file NO 1 ARGS "binary_file" EXPECTED "Found 5 records\n")
