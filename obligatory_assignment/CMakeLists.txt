# Copy the assignment files
set(ASSIGNMENT_DIR "assignment_files")

file(GLOB assignment_files "${ASSIGNMENT_DIR}/*")
foreach(assignment_file ${assignment_files})
    get_filename_component(cur_file ${assignment_file} NAME)
    file(COPY ${assignment_file}
         DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${ASSIGNMENT_DIR}")
endforeach(assignment_file ${text_files})

# Make a binary of the router_operations
add_executable(router_operation router_operation.c)

# Add tests
do_test(NAME router_operation NO 1 ARGS "${ASSIGNMENT_DIR}/5_routers_fully_connected" "print 0" EXPECTED "Found 5 records\n")
