add_executable(module_datastructures_tuples
        test_tuples.c)
target_link_libraries(module_datastructures_tuples
    module_datastructures
    Unity)
add_test(datastructures_tuples module_datastructures_tuples)
