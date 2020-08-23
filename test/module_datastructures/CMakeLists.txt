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

add_executable(module_datastructures_matrix
        test_matrix.c)
target_link_libraries(module_datastructures_matrix
        module_datastructures
        Unity
        m)
add_test(datastructures_matrix module_datastructures_matrix)

add_executable(module_datastructures_rays
        test_rays.c)
target_link_libraries(module_datastructures_rays
        module_datastructures
        Unity
        m)
add_test(datastructures_rays module_datastructures_rays)

add_executable(module_datastructures_sphere
        test_sphere.c)
target_link_libraries(module_datastructures_sphere
        module_datastructures
        Unity
        m)
add_test(datastructures_sphere module_datastructures_sphere)

add_executable(module_datastructures_lights
        test_lights.c)
target_link_libraries(module_datastructures_lights
        module_datastructures
        Unity
        m)
add_test(datastructures_lights module_datastructures_lights)

add_executable(module_datastructures_material
        test_material.c)
target_link_libraries(module_datastructures_material
        module_datastructures
        Unity
        m)

add_test(datastructures_world module_datastructures_world)
add_executable(module_datastructures_world
        test_world.c)
target_link_libraries(module_datastructures_world
        module_datastructures
        Unity
        m)
add_test(datastructures_world module_datastructures_world)

add_executable(module_datastructures_camera
        test_camera.c)
target_link_libraries(module_datastructures_camera
        module_datastructures
        Unity
        m)
add_test(datastructures_camera module_datastructures_camera)
