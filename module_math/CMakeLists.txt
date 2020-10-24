add_library(module_math STATIC
        tuples.c tuples.h
        matrix.c matrix.h
        )

target_include_directories(module_math PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        module_utilities
        CException
        )

target_link_libraries(module_math
        module_utilities
        CException
        m
        )
