add_library(module_datastructures STATIC
        tuples.c tuples.h
        canvas.c canvas.h
        matrix.c matrix.h
        ray.c ray.h
        lights.c lights.h
        material.c material.h
        world.c world.h
        camera.c camera.h)

target_include_directories(module_datastructures PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        module_shapes
        module_utilities
        CException
)

target_link_libraries(module_datastructures
        module_shapes
        module_utilities
        CException
        m
)
