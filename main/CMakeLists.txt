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