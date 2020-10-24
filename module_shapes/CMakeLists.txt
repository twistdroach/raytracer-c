add_library(module_shapes STATIC
        sphere.c sphere.h
        shape.c shape.h
        testshape.c testshape.h
        plane.c plane.h
        cube.c cube.h
        cylinder.c cylinder.h
        cone.c cone.h
        group.c group.h
        triangle.c triangle.h
        csg.c csg.h
        bound.c bound.h)

target_include_directories(module_shapes PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        module_raytracer
        module_utilities
        module_datastructures
        CException
        )

target_link_libraries(module_shapes
        module_raytracer
        module_utilities
        module_datastructures
        CException
        m
        )
