add_library(module_shapes STATIC
        sphere.c sphere.h
        shape.c shape.h
        testshape.c testshape.h)

target_include_directories(module_shapes PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        module_datastructures
        module_utilities
        CException
)

target_link_libraries(module_shapes
        module_datastructures
        module_utilities
        CException
        m
)
