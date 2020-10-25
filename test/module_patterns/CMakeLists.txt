
add_executable(module_patterns_pattern
        test_pattern.c)
target_link_libraries(module_patterns_pattern
        module_patterns
        Unity
        m)
add_test(patterns_pattern module_patterns_pattern)

add_executable(module_patterns_uv_pattern
        test_uv_pattern.c)
target_link_libraries(module_patterns_uv_pattern
        module_patterns
        Unity
        m)
add_test(patterns_pattern module_patterns_uv_pattern)

