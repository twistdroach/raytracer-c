add_executable(parabola
        parabola.c)

target_link_libraries(parabola PRIVATE
        module_datastructures)

add_executable(sphere_shadow
        sphere_shadow.c)

target_link_libraries(sphere_shadow PRIVATE
        module_datastructures
        module_shapes)

add_executable(phong_render_sphere
        phong_render_sphere.c)

target_link_libraries(phong_render_sphere PRIVATE
        module_datastructures
        module_shapes)

add_executable(phong_render_three_spheres
        phong_render_three_spheres.c)

target_link_libraries(phong_render_three_spheres PRIVATE
        module_datastructures
        module_shapes)

add_executable(phong_render_three_spheres_and_plane
        phong_render_three_spheres_and_plane.c)

target_link_libraries(phong_render_three_spheres_and_plane PRIVATE
        module_datastructures
        module_shapes)

add_executable(phong_render_three_spheres_and_striped_plane
        phong_render_three_spheres_and_striped_plane.c)

target_link_libraries(phong_render_three_spheres_and_striped_plane PRIVATE
        module_patterns
        module_datastructures
        module_shapes)

add_executable(reflection
        reflection.c)

target_link_libraries(reflection PRIVATE
        module_patterns
        module_datastructures
        module_shapes)
