project("Simple Raytracer" C)
cmake_minimum_required(VERSION 3.9)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_C_STANDARD 11)

OPTION (USE_AVX2 "Use AVX2" OFF)
IF(USE_AVX2)
  add_compile_options(-mavx2)
ENDIF()

add_compile_options(-Wall -Wextra -pedantic -Werror -fno-omit-frame-pointer)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

OPTION (USE_OpenMP "Use OpenMP" ON)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_options(--coverage)
    add_link_options(--coverage)
    set( USE_OpenMP OFF )
endif()

IF(USE_OpenMP)
    FIND_PACKAGE(OpenMP)
    IF(OPENMP_C_FOUND)
        add_compile_options(${OpenMP_C_FLAGS})
        add_link_options(${OpenMP_C_FLAGS})
    ENDIF()
ENDIF()

#jemalloc seems to be quite a bit faster
OPTION (USE_jemalloc "Use jemalloc" OFF)
if (USE_jemalloc)
  find_package(PkgConfig)
  if (PKGCONFIG_FOUND)
      pkg_check_modules (JEMALLOC jemalloc)
      pkg_search_module(JEMALLOC jemalloc)
      if (JEMALLOC_FOUND)
          include_directories(${JEMALLOC_INCLUDE_DIRS})
          link_libraries(${JEMALLOC_LIBRARIES})
      endif()
  endif()
endif()

find_package( Doxygen )
if ( DOXYGEN_FOUND )
  set( DOXYGEN_EXCLUDE_PATTERNS
    */test/*
    */demo/*
    */external/*
    */main/* )

  set( DOXYGEN_OUTPUT_DIRECTORY doxygen )
  set( DOXYGEN_EXTRACT_ALL YES )
  set( DOXYGEN_OPTIMIZE_OUTPUT_FOR_C YES )

  doxygen_add_docs( doxygen "${CMAKE_CURRENT_SOURCE_DIR}" )
else()
  message( "Doxygen need to be installed to generate the doxygen documentation" )
endif()

add_definitions(-DUNITY_INCLUDE_DOUBLE -DUNITY_DOUBLE_PRECISION=0.0001f -DUNITY_INCLUDE_EXEC_TIME)

add_subdirectory(external)

add_subdirectory(module_math)
add_subdirectory(module_datastructures)
add_subdirectory(module_raytracer)
add_subdirectory(module_shapes)
add_subdirectory(module_patterns)
add_subdirectory(module_utilities)

add_subdirectory(demo)

include(CTest)
add_subdirectory(test)

