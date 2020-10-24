add_library(module_raytracer STATIC
        canvas.c canvas.h
        ray.c ray.h
        lights.c lights.h
        material.c material.h
        world.c world.h
        camera.c camera.h
        wavefrontobj.c wavefrontobj.h
        sequences.c sequences.h)

target_include_directories(module_raytracer PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        module_shapes
        module_patterns
        module_utilities
        CException
        )

target_link_libraries(module_raytracer
        module_shapes
        module_patterns
        module_utilities
        module_datastructures
        CException
        m
        )
