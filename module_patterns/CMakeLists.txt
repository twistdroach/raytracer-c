add_library(module_patterns STATIC
        pattern.h pattern.c
        uv_pattern.h uv_pattern.c)

target_include_directories(module_patterns PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        module_math
        module_shapes
        module_utilities
        CException
        )

target_link_libraries(module_patterns
        module_math
        module_shapes
        module_utilities
        CException
        m
        )
