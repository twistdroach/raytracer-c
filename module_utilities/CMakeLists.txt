add_library(module_utilities STATIC
        exceptions.h exceptions.c
        logger.c logger.h utilities.h utilities.c)

target_include_directories(module_utilities PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        CException
        )

target_link_libraries(module_utilities
        CException
        )
