add_library(module_utilities STATIC
	simple_logger.c simple_logger.h)

target_include_directories(module_utilities PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        CException
)

target_link_libraries(module_utilities
        CException
)
