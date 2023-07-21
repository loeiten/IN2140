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
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/router.c"  # For setNeighbors
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/utils.c"  # For printRouter
    "test_binary_file.c"
    )
add_executable(test_router
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/router.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/utils.c"  # For printRouter
    "src/helpers.c"
    "test_router.c"
    )
add_executable(test_utils
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/utils.c"
    "test_utils.c"
    )

# Add includes
target_include_directories(test_binary_file
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include"
)
target_include_directories(test_router
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_include_directories(test_utils
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include"
)

# Add the unit tests
# Test binary file
do_test(NAME test_binary_file NO 1 ARGS "readNewline" EXPECTED "Expected newline, but got 'm'\nFailed to read from file: .*\n")

# Test router
# printNeighbors
do_test(NAME test_router NO 1 ARGS "printNeighbors" "1,2,3" EXPECTED "Neighbors: \\[1, 2, 3\\]\n")
do_test(NAME test_router NO 2 ARGS "printNeighbors" "2,1,3" EXPECTED "Neighbors: \\[2, 1, 3\\]\n")
do_test(NAME test_router NO 3 ARGS "printNeighbors" "" EXPECTED "Neighbors: \\[\\]\n")
do_test(NAME test_router NO 4 ARGS "printNeighbors" "92,105" EXPECTED "Neighbors: \\[92, 105\\]\n")
# findRouterId
do_test(NAME test_router NO 5 ARGS "findRouterId" EXPECTED "Could not find routerId 100 in routerArray\n")

# Test utils
do_test(NAME test_utils NO 1 ARGS "getBinaryString" "0" "00000000" EXPECTED "Success\n")
do_test(NAME test_utils NO 2 ARGS "getBinaryString" "1" "00000001" EXPECTED "Success\n")
do_test(NAME test_utils NO 3 ARGS "getBinaryString" "2" "00000010" EXPECTED "Success\n")
do_test(NAME test_utils NO 4 ARGS "getBinaryString" "10" "00001010" EXPECTED "Success\n")
do_test(NAME test_utils NO 5 ARGS "getBinaryString" "255" "11111111" EXPECTED "Success\n")
