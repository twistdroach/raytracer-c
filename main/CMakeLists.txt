add_executable(render
        render.c)

target_link_libraries(render PRIVATE
        module_raytracer)
