project("Data Structures" C)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_STANDARD 11)
set(TARGET_GROUP test CACHE STRING "Group to build")
add_compile_options(-Wall -g -O0 -Wextra -pedantic -Werror)

add_subdirectory(module_datastructures)
add_subdirectory(module_utilities)

if(TARGET_GROUP STREQUAL production)
    add_subdirectory(main)
elseif(TARGET_GROUP STREQUAL test)
    include(CTest)

    add_subdirectory(external)
    add_subdirectory(test)
else()
    message(FATAL_ERROR "Given TARGET_GROUP unknown")
endif()
