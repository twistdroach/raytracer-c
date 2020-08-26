
add_executable(module_datastructures_sphere
        test_sphere.c)
target_link_libraries(module_datastructures_sphere
        module_datastructures
        Unity
        m)
add_test(datastructures_sphere module_datastructures_sphere)

add_executable(module_datastructures_shape
        test_shape.c)
target_link_libraries(module_datastructures_shape
        module_datastructures
        Unity
        m)
add_test(datastructures_shape module_datastructures_shape)

add_executable(module_datastructures_plane
        test_plane.c)
target_link_libraries(module_datastructures_plane
        module_datastructures
        Unity
        m)
add_test(datastructures_plane module_datastructures_plane)
