
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

add_executable(module_shapes_cube
        test_cube.c)
target_link_libraries(module_shapes_cube
        module_shapes
        Unity
        m)
add_test(shapes_cube module_shapes_cube)

add_executable(module_shapes_cylinder
        test_cylinder.c)
target_link_libraries(module_shapes_cylinder
        module_shapes
        Unity
        m)
add_test(shapes_cylinder module_shapes_cylinder)

add_executable(module_shapes_cone
        test_cone.c)
target_link_libraries(module_shapes_cone
        module_shapes
        Unity
        m)
add_test(shapes_cone module_shapes_cone)
set_tests_properties(shapes_cone PROPERTIES DISABLED True)

add_executable(module_shapes_group
        test_group.c)
target_link_libraries(module_shapes_group
        module_shapes
        Unity
        m)
add_test(shapes_group module_shapes_group)

add_executable(module_shapes_triangle
        test_triangle.c)
target_link_libraries(module_shapes_triangle
        module_shapes
        Unity
        m)
add_test(shapes_triangle module_shapes_triangle)
