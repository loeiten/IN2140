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
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/router.c" # For setNeighbors
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/utils.c" # For printRouter
    "test_binary_file.c"
)
add_executable(test_router
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/router.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/utils.c" # For printRouter
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
# printRouter
do_test(NAME test_router NO 1 ARGS "printRouter" "2" "1" "Foo" "1,2,3" EXPECTED "Name: Foo\nRouterId: 2\nActive: Yes\nWireless: No\n5 GHz: No\nModification number: 0\nFlag: 00000001 \\(1\\)\nNeighbors: \\[1, 2, 3\\]\n")
do_test(NAME test_router NO 2 ARGS "printRouter" "2" "0" "Foo" "1,2,3" EXPECTED "Name: Foo\nRouterId: 2\nActive: No\nWireless: No\n5 GHz: No\nModification number: 0\nFlag: 00000000 \\(0\\)\nNeighbors: \\[1, 2, 3\\]\n")
do_test(NAME test_router NO 3 ARGS "printRouter" "2" "2" "Foo" "1,2,3" EXPECTED "Name: Foo\nRouterId: 2\nActive: No\nWireless: Yes\n5 GHz: No\nModification number: 0\nFlag: 00000010 \\(2\\)\nNeighbors: \\[1, 2, 3\\]\n")
do_test(NAME test_router NO 4 ARGS "printRouter" "2" "4" "Foo" "1,2,3" EXPECTED "Name: Foo\nRouterId: 2\nActive: No\nWireless: No\n5 GHz: Yes\nModification number: 0\nFlag: 00000100 \\(4\\)\nNeighbors: \\[1, 2, 3\\]\n")
do_test(NAME test_router NO 5 ARGS "printRouter" "2" "7" "Foo" "1,2,3" EXPECTED "Name: Foo\nRouterId: 2\nActive: Yes\nWireless: Yes\n5 GHz: Yes\nModification number: 0\nFlag: 00000111 \\(7\\)\nNeighbors: \\[1, 2, 3\\]\n")
do_test(NAME test_router NO 6 ARGS "printRouter" "2" "23" "Foo" "1,2,3" EXPECTED "Name: Foo\nRouterId: 2\nActive: Yes\nWireless: Yes\n5 GHz: Yes\nModification number: 8\nFlag: 00010111 \\(23\\)\nNeighbors: \\[1, 2, 3\\]\n")
do_test(NAME test_router NO 7 ARGS "printRouter" "2" "135" "Foo" "1,2,3" EXPECTED "Name: Foo\nRouterId: 2\nActive: Yes\nWireless: Yes\n5 GHz: Yes\nModification number: 1\nFlag: 10000111 \\(135\\)\nNeighbors: \\[1, 2, 3\\]\n")
do_test(NAME test_router NO 8 ARGS "printRouter" "2" "247" "Foo" "1,2,3" EXPECTED "Name: Foo\nRouterId: 2\nActive: Yes\nWireless: Yes\n5 GHz: Yes\nModification number: 15\nFlag: 11110111 \\(247\\)\nNeighbors: \\[1, 2, 3\\]\n")
do_test(NAME test_router NO 9 ARGS "printRouter" "2" "255" "Foo" "1,2,3" EXPECTED "Name: Foo\nRouterId: 2\nActive: Yes\nWireless: Yes\n5 GHz: Yes\nModification number: 15\nFlag: 11111111 \\(255\\)\nNeighbors: \\[1, 2, 3\\]\n")
do_test(NAME test_router NO 10 ARGS "printRouter" "42" "255" "Foo Bar 7000" "1,2,3" EXPECTED "Name: Foo Bar 7000\nRouterId: 42\nActive: Yes\nWireless: Yes\n5 GHz: Yes\nModification number: 15\nFlag: 11111111 \\(255\\)\nNeighbors: \\[1, 2, 3\\]\n")
do_test(NAME test_router NO 11 ARGS "printRouter" "42" "255" "Foo Bar 7000" "1" EXPECTED "Name: Foo Bar 7000\nRouterId: 42\nActive: Yes\nWireless: Yes\n5 GHz: Yes\nModification number: 15\nFlag: 11111111 \\(255\\)\nNeighbors: \\[1\\]\n")
do_test(NAME test_router NO 12 ARGS "printRouter" "42" "255" "Foo Bar 7000" "" EXPECTED "Name: Foo Bar 7000\nRouterId: 42\nActive: Yes\nWireless: Yes\n5 GHz: Yes\nModification number: 15\nFlag: 11111111 \\(255\\)\nNeighbors: \\[\\]\n")

# printNeighbors
do_test(NAME test_router NO 13 ARGS "printNeighbors" "1,2,3" EXPECTED "Neighbors: \\[1, 2, 3\\]\n")
do_test(NAME test_router NO 14 ARGS "printNeighbors" "2,1,3" EXPECTED "Neighbors: \\[2, 1, 3\\]\n")
do_test(NAME test_router NO 15 ARGS "printNeighbors" "" EXPECTED "Neighbors: \\[\\]\n")
do_test(NAME test_router NO 16 ARGS "printNeighbors" "92,105" EXPECTED "Neighbors: \\[92, 105\\]\n")

# setNeighbor
do_test(NAME test_router NO 17 ARGS "setNeighbor" EXPECTED "Could not find routerId 99 in routerArray\nCould not set the neighbor as the routerId was not found\nCould not set the neighbor as a free neighbor of routerId 42 was not found\n"
)

# findRouterId
do_test(NAME test_router NO 18 ARGS "findRouterId" EXPECTED "Could not find routerId 100 in routerArray\n")

# findFreeNeighbor
do_test(NAME test_router NO 19 ARGS "findFreeNeighbor" "9" "1" EXPECTED "Success\n")
do_test(NAME test_router NO 20 ARGS "findFreeNeighbor" "4,7" "2" EXPECTED "Success\n")
do_test(NAME test_router NO 21 ARGS "findFreeNeighbor" "4,7,95" "3" EXPECTED "Success\n")
do_test(NAME test_router NO 22 ARGS "findFreeNeighbor" "0,1,2,3,4,5,6,7,8,9" "-1" EXPECTED "Success\n")
do_test(NAME test_router NO 23 ARGS "findFreeNeighbor" "x" "0" EXPECTED "Success\n")

# Test utils
do_test(NAME test_utils NO 1 ARGS "getBinaryString" "0" "00000000" EXPECTED "Success\n")
do_test(NAME test_utils NO 2 ARGS "getBinaryString" "1" "00000001" EXPECTED "Success\n")
do_test(NAME test_utils NO 3 ARGS "getBinaryString" "2" "00000010" EXPECTED "Success\n")
do_test(NAME test_utils NO 4 ARGS "getBinaryString" "10" "00001010" EXPECTED "Success\n")
do_test(NAME test_utils NO 5 ARGS "getBinaryString" "255" "11111111" EXPECTED "Success\n")
