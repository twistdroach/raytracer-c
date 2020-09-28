add_library(module_datastructures STATIC
        canvas.c canvas.h
        ray.c ray.h
        lights.c lights.h
        material.c material.h
        world.c world.h
        camera.c camera.h
        arrlist.c arrlist.h
        wavefrontobj.c wavefrontobj.h)

target_include_directories(module_datastructures PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        module_shapes
        module_patterns
        module_utilities
        CException
)

target_link_libraries(module_datastructures
        module_shapes
        module_patterns
        module_utilities
        CException
        m
)
