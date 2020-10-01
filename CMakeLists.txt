project("Simple Raytracer" C)
cmake_minimum_required(VERSION 3.3)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_C_STANDARD 11)
set(TARGET_GROUP test CACHE STRING "Group to build")

add_compile_options(-Wall -Wextra -pedantic -Werror)
if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_options(--coverage)
    add_link_options(--coverage)
endif()

find_package( Doxygen )

if ( DOXYGEN_FOUND )
  set( DOXYGEN_EXCLUDE_PATTERNS
    */test/*
    */main/*
    */external/* )

  set( DOXYGEN_OUTPUT_DIRECTORY doxygen )
  set( DOXYGEN_EXTRACT_ALL YES )
  set( DOXYGEN_OPTIMIZE_OUTPUT_FOR_C YES )

  doxygen_add_docs( doxygen "${CMAKE_CURRENT_SOURCE_DIR}" )

else()

  message( "Doxygen need to be installed to generate the doxygen documentation" )

endif()

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

