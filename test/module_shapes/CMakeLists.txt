
add_executable(module_shapes_sphere
        test_sphere.c)
target_link_libraries(module_shapes_sphere
        module_shapes
        Unity
        m)
add_test(shapes_sphere module_shapes_sphere)

add_executable(module_shapes_shape
        test_shape.c)
target_link_libraries(module_shapes_shape
        module_shapes
        Unity
        m)
add_test(shapes_shape module_shapes_shape)

add_executable(module_shapes_plane
        test_plane.c)
target_link_libraries(module_shapes_plane
        module_shapes
        Unity
        m)
add_test(shapes_plane module_shapes_plane)
