add_executable(module_datastructures_tuples
        test_tuples.c)
target_link_libraries(module_datastructures_tuples
    module_datastructures
    Unity
    m)
add_test(datastructures_tuples module_datastructures_tuples)

add_executable(module_datastructures_canvas
        test_canvas.c)
target_link_libraries(module_datastructures_canvas
        module_datastructures
        Unity
        m)
add_test(datastructures_canvas module_datastructures_canvas)

