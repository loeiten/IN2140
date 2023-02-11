# Add library
# We can here specify whether we'd like a static, shared or module library
add_library(utils SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/src/string_helpers.c
)

# NOTE: It's also possible to do interface libraries like so
# add_library(my_big_bunch_of_libs INTERFACE)
# target_link_libraries(my_big_bunch_of_libs INTERFACE lib1 lib2 ...)

# Add includes
# Anybody linking to `utils` needs to include the current include directory
# INTERFACE means things that consumers require, but producers doesn't
target_include_directories(utils
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
)