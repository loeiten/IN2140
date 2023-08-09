# Copy the assignment files
set(ASSIGNMENT_DIR "assignment_files")

file(GLOB assignment_files "${ASSIGNMENT_DIR}/*")
foreach(assignment_file ${assignment_files})
    get_filename_component(cur_file ${assignment_file} NAME)
    file(COPY ${assignment_file}
         DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${ASSIGNMENT_DIR}")
endforeach(assignment_file ${text_files})

# Make a binary of the router_operations
add_executable(router_operation
    "src/binary_file.c"
    "src/dynamic_memory.c"
    "src/command.c"
    "src/router.c"
    "src/utils.c"
    "src/router_operation.c"
    )

# Add includes
target_include_directories(router_operation
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Add the unit tests
add_subdirectory(test)

# Add the integration tests
do_test(NAME router_operation NO 1 ARGS "${ASSIGNMENT_DIR}/5_routers_fully_connected" "print 0" EXPECTED "Name: ZyXEL s4\nRouterId: 0\nActive: Yes\nWireless: Yes\n5 GHz: Yes\nModification number: 11\nFlag: 11010111 \\(215\\)\nNeighbors: \\[4, 2, 1, 3\\]\n\n")
do_test(NAME router_operation NO 2 ARGS "${ASSIGNMENT_DIR}/5_routers_fully_connected" "test/data/commands_5_routers_1.txt" EXPECTED "Name: ZyXEL s4\nRouterId: 0\nActive: Yes\nWireless: Yes\n5 GHz: Yes\nModification number: 11\nFlag: 11010111 \\(215\\)\nNeighbors: \\[4, 2, 1, 3\\]\n\nName: Cisco o1000\nRouterId: 1\nActive: Yes\nWireless: No\n5 GHz: Yes\nModification number: 0\nFlag: 00000101 \\(5\\)\nNeighbors: \\[3, 2, 4, 0\\]\n\n")
do_test(NAME router_operation NO 3 ARGS "${ASSIGNMENT_DIR}/5_routers_fully_connected" "test/data/commands_5_routers_2.txt" EXPECTED "Name: ZyXEL s4\nRouterId: 0\nActive: Yes\nWireless: Yes\n5 GHz: Yes\nModification number: 11\nFlag: 11010111 \\(215\\)\nNeighbors: \\[4, 2, 1, 3\\]\n\nName: Cisco o1000\nRouterId: 1\nActive: Yes\nWireless: No\n5 GHz: Yes\nModification number: 0\nFlag: 00000101 \\(5\\)\nNeighbors: \\[3, 2, 4, 0\\]\n\nName: Linksys j500\nRouterId: 2\nActive: Yes\nWireless: Yes\n5 GHz: No\nModification number: 8\nFlag: 00010011 \\(19\\)\nNeighbors: \\[4, 1, 3, 0\\]\n\nName: Juniper e30\nRouterId: 3\nActive: Yes\nWireless: Yes\n5 GHz: Yes\nModification number: 15\nFlag: 11110111 \\(247\\)\nNeighbors: \\[0, 2, 1, 4\\]\n\nName: Netgear y4\nRouterId: 4\nActive: No\nWireless: No\n5 GHz: No\nModification number: 2\nFlag: 01000000 \\(64\\)\nNeighbors: \\[2, 0, 1, 3\\]\n\n")
do_test(NAME router_operation NO 4 ARGS "${ASSIGNMENT_DIR}/5_routers_fully_connected" "test/data/commands_5_routers_3.txt" EXPECTED "Name: Juniper e30\nRouterId: 3\nActive: Yes\nWireless: Yes\n5 GHz: Yes\nModification number: 14\nFlag: 01110111 \\(119\\)\nNeighbors: \\[0, 2, 1, 4\\]\n\nName: ASUS RT-AX89X\nRouterId: 2\nActive: Yes\nWireless: Yes\n5 GHz: No\nModification number: 8\nFlag: 00010011 \\(19\\)\nNeighbors: \\[4, 1, 3, 0\\]\n\nRoute from router 2 to router 0 exists\n")
