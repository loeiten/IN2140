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
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/router.c" # For addLinks
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/utils.c" # For getBinaryString
    "test_binary_file.c"
)
add_executable(test_command
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/command.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/dynamic_memory.c" # For freeCommandStrCpy
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/router.c" # For printRouter
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/utils.c" # For getBinaryString
    "test_command.c"
)
add_executable(test_router
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/router.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/utils.c" # For getBinaryString
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/dynamic_memory.c" # For freeRouterArray
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
target_include_directories(test_command
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
# readBinaryFile
do_test(NAME test_binary_file NO 1 ARGS "readBinaryFile" EXPECTED "Name: ZyXEL s4\nRouterId: 0\nActive: Yes\nWireless: Yes\n5 GHz: Yes\nModification number: 11\nFlag: 11010111 \\(215\\)\nNeighbors: \\[4, 2, 1, 3\\]\n\nName: Cisco o1000\nRouterId: 1\nActive: Yes\nWireless: No\n5 GHz: Yes\nModification number: 0\nFlag: 00000101 \\(5\\)\nNeighbors: \\[3, 2, 4, 0\\]\n\nName: Linksys j500\nRouterId: 2\nActive: Yes\nWireless: Yes\n5 GHz: No\nModification number: 8\nFlag: 00010011 \\(19\\)\nNeighbors: \\[4, 1, 3, 0\\]\n\nName: Juniper e30\nRouterId: 3\nActive: Yes\nWireless: Yes\n5 GHz: Yes\nModification number: 15\nFlag: 11110111 \\(247\\)\nNeighbors: \\[0, 2, 1, 4\\]\n\nName: Netgear y4\nRouterId: 4\nActive: No\nWireless: No\n5 GHz: No\nModification number: 2\nFlag: 01000000 \\(64\\)\nNeighbors: \\[2, 0, 1, 3\\]\n\n")

# readNewline
do_test(NAME test_binary_file NO 2 ARGS "readNewline" EXPECTED "Expected newline, but got 'm'\nFailed to read from file: .*\n")

# readRouter
do_test(NAME test_binary_file NO 3 ARGS "readRouter" EXPECTED "Name: Foo Bar 42\nRouterId: 87\nActive: Yes\nWireless: Yes\n5 GHz: Yes\nModification number: 15\nFlag: 11110111 \\(247\\)\nNeighbors: \\[\\]\n\n")

# readAndSetNeighbors
do_test(NAME test_binary_file NO 4 ARGS "readAndSetNeighbors" EXPECTED "Neighbors: \\[7\\]\nNeighbors: \\[\\]\nNeighbors: \\[42, 7\\]\n")

# Test command
# getCommand
do_test(NAME test_command NO 1 ARGS "getCommand" "Foo Bar" "Foo" "1" EXPECTED "Bar\n")
do_test(NAME test_command NO 2 ARGS "getCommand" "Foo Bar Baz" "Foo" "2" EXPECTED "Bar\nBaz\n")
do_test(NAME test_command NO 3 ARGS "getCommand" "Foo Bar Baz FooBar" "Foo" "3" EXPECTED "Bar\nBaz\nFooBar\n")
do_test(NAME test_command NO 4 ARGS "getCommand" "setModel 1 Bar" "setModel" "2" EXPECTED "1\nBar\n")
do_test(NAME test_command NO 5 ARGS "getCommand" "setModel 82 Bar Baz FooBar" "setModel" "2" EXPECTED "82\nBar Baz FooBar\n")
do_test(NAME test_command NO 6 ARGS "getCommand" "setModel 104 Quux" "setModel" "2" EXPECTED "104\nQuux\n")

# Test router
# printRouter
do_test(NAME test_router NO 1 ARGS "printRouter" "2" "1" "Foo" "1,2,3" EXPECTED "Name: Foo\nRouterId: 2\nActive: Yes\nWireless: No\n5 GHz: No\nModification number: 0\nFlag: 00000001 \\(1\\)\nNeighbors: \\[1, 2, 3\\]\n\n")
do_test(NAME test_router NO 2 ARGS "printRouter" "2" "0" "Foo" "1,2,3" EXPECTED "Name: Foo\nRouterId: 2\nActive: No\nWireless: No\n5 GHz: No\nModification number: 0\nFlag: 00000000 \\(0\\)\nNeighbors: \\[1, 2, 3\\]\n\n")
do_test(NAME test_router NO 3 ARGS "printRouter" "2" "2" "Foo" "1,2,3" EXPECTED "Name: Foo\nRouterId: 2\nActive: No\nWireless: Yes\n5 GHz: No\nModification number: 0\nFlag: 00000010 \\(2\\)\nNeighbors: \\[1, 2, 3\\]\n\n")
do_test(NAME test_router NO 4 ARGS "printRouter" "2" "4" "Foo" "1,2,3" EXPECTED "Name: Foo\nRouterId: 2\nActive: No\nWireless: No\n5 GHz: Yes\nModification number: 0\nFlag: 00000100 \\(4\\)\nNeighbors: \\[1, 2, 3\\]\n\n")
do_test(NAME test_router NO 5 ARGS "printRouter" "2" "7" "Foo" "1,2,3" EXPECTED "Name: Foo\nRouterId: 2\nActive: Yes\nWireless: Yes\n5 GHz: Yes\nModification number: 0\nFlag: 00000111 \\(7\\)\nNeighbors: \\[1, 2, 3\\]\n\n")
do_test(NAME test_router NO 6 ARGS "printRouter" "2" "23" "Foo" "1,2,3" EXPECTED "Name: Foo\nRouterId: 2\nActive: Yes\nWireless: Yes\n5 GHz: Yes\nModification number: 8\nFlag: 00010111 \\(23\\)\nNeighbors: \\[1, 2, 3\\]\n\n")
do_test(NAME test_router NO 7 ARGS "printRouter" "2" "135" "Foo" "1,2,3" EXPECTED "Name: Foo\nRouterId: 2\nActive: Yes\nWireless: Yes\n5 GHz: Yes\nModification number: 1\nFlag: 10000111 \\(135\\)\nNeighbors: \\[1, 2, 3\\]\n\n")
do_test(NAME test_router NO 8 ARGS "printRouter" "2" "247" "Foo" "1,2,3" EXPECTED "Name: Foo\nRouterId: 2\nActive: Yes\nWireless: Yes\n5 GHz: Yes\nModification number: 15\nFlag: 11110111 \\(247\\)\nNeighbors: \\[1, 2, 3\\]\n\n")
do_test(NAME test_router NO 9 ARGS "printRouter" "2" "255" "Foo" "1,2,3" EXPECTED "Name: Foo\nRouterId: 2\nActive: Yes\nWireless: Yes\n5 GHz: Yes\nModification number: 15\nFlag: 11111111 \\(255\\)\nNeighbors: \\[1, 2, 3\\]\n\n")
do_test(NAME test_router NO 10 ARGS "printRouter" "42" "255" "Foo Bar 7000" "1,2,3" EXPECTED "Name: Foo Bar 7000\nRouterId: 42\nActive: Yes\nWireless: Yes\n5 GHz: Yes\nModification number: 15\nFlag: 11111111 \\(255\\)\nNeighbors: \\[1, 2, 3\\]\n\n")
do_test(NAME test_router NO 11 ARGS "printRouter" "42" "255" "Foo Bar 7000" "1" EXPECTED "Name: Foo Bar 7000\nRouterId: 42\nActive: Yes\nWireless: Yes\n5 GHz: Yes\nModification number: 15\nFlag: 11111111 \\(255\\)\nNeighbors: \\[1\\]\n\n")
do_test(NAME test_router NO 12 ARGS "printRouter" "42" "255" "Foo Bar 7000" "" EXPECTED "Name: Foo Bar 7000\nRouterId: 42\nActive: Yes\nWireless: Yes\n5 GHz: Yes\nModification number: 15\nFlag: 11111111 \\(255\\)\nNeighbors: \\[\\]\n\n")

# printNeighbors
do_test(NAME test_router NO 13 ARGS "printNeighbors" "1,2,3" EXPECTED "Neighbors: \\[1, 2, 3\\]\n")
do_test(NAME test_router NO 14 ARGS "printNeighbors" "2,1,3" EXPECTED "Neighbors: \\[2, 1, 3\\]\n")
do_test(NAME test_router NO 15 ARGS "printNeighbors" "" EXPECTED "Neighbors: \\[\\]\n")
do_test(NAME test_router NO 16 ARGS "printNeighbors" "92,105" EXPECTED "Neighbors: \\[92, 105\\]\n")

# addLink
do_test(NAME test_router NO 17 ARGS "addLink" EXPECTED "Could not find routerId 99 in routerArray\nCould not add the link as the routerId was not found\nCould not add the link as a free neighbor of routerId 42 was not found\n"
)

# findRouterId
do_test(NAME test_router NO 18 ARGS "findRouterId" EXPECTED "Could not find routerId 100 in routerArray\n")

# findFreeNeighbor
do_test(NAME test_router NO 19 ARGS "findFreeNeighbor" "9" "1" EXPECTED "Success\n")
do_test(NAME test_router NO 20 ARGS "findFreeNeighbor" "4,7" "2" EXPECTED "Success\n")
do_test(NAME test_router NO 21 ARGS "findFreeNeighbor" "4,7,95" "3" EXPECTED "Success\n")
do_test(NAME test_router NO 22 ARGS "findFreeNeighbor" "0,1,2,3,4,5,6,7,8,9" "-1" EXPECTED "Success\n")
do_test(NAME test_router NO 23 ARGS "findFreeNeighbor" "x" "0" EXPECTED "Success\n")

# setFlag (sanity)
# Args: routerId, flag, value, expected success
do_test(NAME test_router NO 24 ARGS "setFlag" "0" "-1" "0" "1" EXPECTED "Flag can only be one of \\{0, 1, 2, 4\\}\nName: Foo\nRouterId: 0\nActive: No\nWireless: No\n5 GHz: No\nModification number: 0\nFlag: 00000000 \\(0\\)\nNeighbors: \\[\\]\n\n")
do_test(NAME test_router NO 25 ARGS "setFlag" "0" "5" "0" "1" EXPECTED "Flag can only be one of \\{0, 1, 2, 4\\}\nName: Foo\nRouterId: 0\nActive: No\nWireless: No\n5 GHz: No\nModification number: 0\nFlag: 00000000 \\(0\\)\nNeighbors: \\[\\]\n\n")
do_test(NAME test_router NO 26 ARGS "setFlag" "0" "3" "0" "1" EXPECTED "Flag can only be one of \\{0, 1, 2, 4\\}\nName: Foo\nRouterId: 0\nActive: No\nWireless: No\n5 GHz: No\nModification number: 0\nFlag: 00000000 \\(0\\)\nNeighbors: \\[\\]\n\n")
do_test(NAME test_router NO 27 ARGS "setFlag" "0" "0" "-1" "1" EXPECTED "Value can only be \\{0, 1\\} for flag \\{0, 1, 2\\}\nName: Foo\nRouterId: 0\nActive: No\nWireless: No\n5 GHz: No\nModification number: 0\nFlag: 00000000 \\(0\\)\nNeighbors: \\[\\]\n\n")
do_test(NAME test_router NO 28 ARGS "setFlag" "0" "0" "2" "1" EXPECTED "Value can only be \\{0, 1\\} for flag \\{0, 1, 2\\}\nName: Foo\nRouterId: 0\nActive: No\nWireless: No\n5 GHz: No\nModification number: 0\nFlag: 00000000 \\(0\\)\nNeighbors: \\[\\]\n\n")
do_test(NAME test_router NO 29 ARGS "setFlag" "0" "4" "-1" "1" EXPECTED "Value can only be in range \\[0, 15\\] for flag 4\nName: Foo\nRouterId: 0\nActive: No\nWireless: No\n5 GHz: No\nModification number: 0\nFlag: 00000000 \\(0\\)\nNeighbors: \\[\\]\n\n")
do_test(NAME test_router NO 30 ARGS "setFlag" "0" "4" "16" "1" EXPECTED "Value can only be in range \\[0, 15\\] for flag 4\nName: Foo\nRouterId: 0\nActive: No\nWireless: No\n5 GHz: No\nModification number: 0\nFlag: 00000000 \\(0\\)\nNeighbors: \\[\\]\n\n")
do_test(NAME test_router NO 31 ARGS "setFlag" "2" "0" "0" "1" EXPECTED "Could not find routerId 2 in routerArray\nCould not set the flag as routerId 2 was not found\nCould not find routerId 2 in routerArray\nCould not print router as routerId was not found\n")

# setFlag (normal)
# Activity
do_test(NAME test_router NO 32 ARGS "setFlag" "0" "0" "0" "0" EXPECTED "Name: Foo\nRouterId: 0\nActive: No\nWireless: No\n5 GHz: No\nModification number: 0\nFlag: 00000000 \\(0\\)\nNeighbors: \\[\\]\n\n")
do_test(NAME test_router NO 33 ARGS "setFlag" "1" "0" "0" "0" EXPECTED "Name: Bar\nRouterId: 1\nActive: No\nWireless: Yes\n5 GHz: Yes\nModification number: 15\nFlag: 11111110 \\(254\\)\nNeighbors: \\[\\]\n\n")

# Wireless
do_test(NAME test_router NO 34 ARGS "setFlag" "0" "1" "1" "0" EXPECTED "Name: Foo\nRouterId: 0\nActive: No\nWireless: Yes\n5 GHz: No\nModification number: 0\nFlag: 00000010 \\(2\\)\nNeighbors: \\[\\]\n\n")
do_test(NAME test_router NO 35 ARGS "setFlag" "1" "1" "1" "0" EXPECTED "Name: Bar\nRouterId: 1\nActive: Yes\nWireless: Yes\n5 GHz: Yes\nModification number: 15\nFlag: 11111111 \\(255\\)\nNeighbors: \\[\\]\n\n")

# Modification number
do_test(NAME test_router NO 36 ARGS "setFlag" "0" "4" "7" "0" EXPECTED "Name: Foo\nRouterId: 0\nActive: No\nWireless: No\n5 GHz: No\nModification number: 7\nFlag: 11100000 \\(224\\)\nNeighbors: \\[\\]\n\n")
do_test(NAME test_router NO 37 ARGS "setFlag" "1" "4" "7" "0" EXPECTED "Name: Bar\nRouterId: 1\nActive: Yes\nWireless: Yes\n5 GHz: Yes\nModification number: 7\nFlag: 11101111 \\(239\\)\nNeighbors: \\[\\]\n\n")

# deleteRouter
do_test(NAME test_router NO 38 ARGS "deleteRouter" "0" EXPECTED "Neighbors: \\[2, 1\\]\nNeighbors: \\[87, 1, 3, 4, 5, 6, 7, 8, 9\\]\nNeighbors: \\[\\]\n")
do_test(NAME test_router NO 39 ARGS "deleteRouter" "1" EXPECTED "Neighbors: \\[2, 3\\]\nNeighbors: \\[0, 2\\]\nNeighbors: \\[0\\]\n")
do_test(NAME test_router NO 40 ARGS "deleteRouter" "2" EXPECTED "Neighbors: \\[3, 1\\]\nNeighbors: \\[87, 1, 0, 4, 5, 6, 7, 8, 9, 3\\]\nNeighbors: \\[0, 1\\]\n")
do_test(NAME test_router NO 41 ARGS "deleteRouter" "3" EXPECTED "Neighbors: \\[2, 1\\]\nNeighbors: \\[87, 1, 0, 4, 5, 6, 7, 8, 9\\]\nNeighbors: \\[0\\]\n")

# setName
do_test(NAME test_router NO 42 ARGS "setName" EXPECTED "Success\n")

# existsRoute
do_test(NAME test_router NO 43 ARGS "existsRoute" EXPECTED "Success\n")

# Test utils
do_test(NAME test_utils NO 1 ARGS "getBinaryString" "0" "00000000" EXPECTED "Success\n")
do_test(NAME test_utils NO 2 ARGS "getBinaryString" "1" "00000001" EXPECTED "Success\n")
do_test(NAME test_utils NO 3 ARGS "getBinaryString" "2" "00000010" EXPECTED "Success\n")
do_test(NAME test_utils NO 4 ARGS "getBinaryString" "10" "00001010" EXPECTED "Success\n")
do_test(NAME test_utils NO 5 ARGS "getBinaryString" "255" "11111111" EXPECTED "Success\n")
