add_executable(parabola
        parabola.c
)

target_link_libraries(parabola PRIVATE
        module_datastructures
)

add_executable(sphere_shadow
        sphere_shadow.c)

target_link_libraries(sphere_shadow PRIVATE
        module_datastructures)

add_executable(phong_render_sphere
        phong_render_sphere.c)

target_link_libraries(phong_render_sphere PRIVATE
        module_datastructures)