add_executable(module_math_tuples
        test_tuples.c)
target_link_libraries(module_math_tuples
    module_math
    Unity
    m)
add_test(math_tuples module_math_tuples)

add_executable(module_math_matrix
        test_matrix.c)
target_link_libraries(module_math_matrix
        module_math
        Unity
        m)
add_test(math_matrix module_math_matrix)

