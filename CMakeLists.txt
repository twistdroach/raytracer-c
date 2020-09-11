project("Data Structures" C)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_STANDARD 11)
set(TARGET_GROUP test CACHE STRING "Group to build")
# clang
#add_compile_options(-Wall -g -Ofast -march=native -DNDEBUG -Wextra -pedantic -Werror -Wno-unused-result)
# gcc
#add_compile_options(-Wall -g -Ofast -march=native -DNDEBUG -Wextra -pedantic -Werror -Wno-unused-result -Wno-clobbered -Wno-unused-parameter)

add_compile_options(-Wall -g -O0 -Wextra -pedantic -Werror)

add_subdirectory(module_math)
add_subdirectory(module_datastructures)
add_subdirectory(module_shapes)
add_subdirectory(module_patterns)
add_subdirectory(module_utilities)

add_definitions(-DUNITY_INCLUDE_DOUBLE -DUNITY_DOUBLE_PRECISION=0.0001f -DUNITY_INCLUDE_EXEC_TIME)
add_subdirectory(external)

if(TARGET_GROUP STREQUAL production)
    add_subdirectory(main)
elseif(TARGET_GROUP STREQUAL test)
    include(CTest)

    add_subdirectory(test)
    add_subdirectory(main)
else()
    message(FATAL_ERROR "Given TARGET_GROUP unknown")
endif()

add_custom_target(memcheck
    COMMAND ${CMAKE_CTEST_COMMAND} 
        --force-new-ctest-process --test-action memcheck)
