add_library(module_datastructures STATIC
        tuples.c tuples.h)

target_include_directories(module_datastructures PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        module_utilities
        CException
)

target_link_libraries(module_datastructures
        module_utilities
        CException
)
