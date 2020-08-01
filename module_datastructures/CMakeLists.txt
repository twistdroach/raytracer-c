add_library(module_datastructures STATIC
        proj_exceptions.h
        tuples.c tuples.h)

target_include_directories(module_datastructures PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        CException
)

target_link_libraries(module_datastructures
        CException
)
