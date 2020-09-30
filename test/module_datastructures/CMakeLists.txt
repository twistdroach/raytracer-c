add_executable(module_datastructures_canvas
        test_canvas.c)
target_link_libraries(module_datastructures_canvas
        module_datastructures
        Unity
        m)
add_test(datastructures_canvas module_datastructures_canvas)

add_executable(module_datastructures_rays
        test_rays.c)
target_link_libraries(module_datastructures_rays
        module_datastructures
        Unity
        m)
add_test(datastructures_rays module_datastructures_rays)

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
        module_patterns
        Unity
        m)
add_test(datastructures_material module_datastructures_material)

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

add_executable(module_datastructures_arrlist
        test_arrlist.c)
target_link_libraries(module_datastructures_arrlist
        module_datastructures
        Unity)
add_test(datastructures_arrlist module_datastructures_arrlist)

add_executable(module_datastructures_wavefrontobj
        test_wavefrontobj.c)
target_link_libraries(module_datastructures_wavefrontobj
        module_datastructures
        Unity)
add_test(NAME datastructures_wavefrontobj
         COMMAND module_datastructures_wavefrontobj
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/wavefrontobj)
