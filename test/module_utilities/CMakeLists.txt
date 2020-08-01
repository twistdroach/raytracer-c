#linkedlist
add_executable(module_utilities_simple_logger
        test_simple_logger.c)
target_link_libraries(module_utilities_simple_logger
    module_utilities
    Unity)
add_test(utilities_simple_logger module_utilities_simple_logger)
