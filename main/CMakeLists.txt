add_executable(parabola
        parabola.c
)

target_link_libraries(parabola PRIVATE
        module_datastructures
        m
)
