add_executable(module_utilities_simple_logger
        test_simple_logger.c)
target_link_libraries(module_utilities_simple_logger
        module_utilities
        Unity)
add_test(utilities_simple_logger module_utilities_simple_logger)

add_executable(module_utilities_utilities
        test_utilities.c)
target_link_libraries(module_utilities_utilities
        module_utilities
        Unity)
add_test(utilities_utilities module_utilities_utilities)
