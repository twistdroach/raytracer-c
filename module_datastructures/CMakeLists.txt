add_library(module_datastructures STATIC
        arrlist.c arrlist.h
        linkedlist.c linkedlist.h)

target_include_directories(module_datastructures PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        module_utilities
        module_shapes
        CException
        )

target_link_libraries(module_datastructures
        module_utilities
        module_shapes
        CException
        )
