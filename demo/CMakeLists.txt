add_executable(parabola
        parabola.c)

target_link_libraries(parabola PRIVATE
        module_raytracer)

add_executable(sphere_shadow
        sphere_shadow.c)

target_link_libraries(sphere_shadow PRIVATE
        module_raytracer
        module_shapes)

add_executable(phong_render_sphere
        phong_render_sphere.c)

target_link_libraries(phong_render_sphere PRIVATE
        module_raytracer
        module_shapes)

add_executable(phong_render_three_spheres
        phong_render_three_spheres.c)

target_link_libraries(phong_render_three_spheres PRIVATE
        module_raytracer
        module_shapes)

add_executable(phong_render_three_spheres_and_plane
        phong_render_three_spheres_and_plane.c)

target_link_libraries(phong_render_three_spheres_and_plane PRIVATE
        module_raytracer
        module_shapes)

add_executable(phong_render_three_spheres_and_striped_plane
        phong_render_three_spheres_and_striped_plane.c)

target_link_libraries(phong_render_three_spheres_and_striped_plane PRIVATE
        module_patterns
        module_raytracer
        module_shapes)

add_executable(reflection
        reflection.c)

target_link_libraries(reflection PRIVATE
        module_patterns
        module_raytracer
        module_shapes)

add_executable(refraction
        refraction.c)

target_link_libraries(refraction PRIVATE
        module_patterns
        module_raytracer
        module_shapes)

add_executable(refraction_cube
        refraction_cube.c)

target_link_libraries(refraction_cube PRIVATE
        module_patterns
        module_raytracer
        module_shapes)

add_executable(refraction_cylinder
        refraction_cylinder.c)

target_link_libraries(refraction_cylinder PRIVATE
        module_patterns
        module_raytracer
        module_shapes)

add_executable(random_spheres
        random_spheres.c)

target_link_libraries(random_spheres PRIVATE
        module_patterns
        module_raytracer
        module_shapes)

add_executable(render_obj
        render_obj.c)

target_link_libraries(render_obj PRIVATE
        module_patterns
        module_raytracer
        module_shapes)

add_executable(csg
        csg.c)

target_link_libraries(csg PRIVATE
        module_patterns
        module_raytracer
        module_shapes)

add_executable(sphere_spin sphere_spin.c)
target_link_libraries(sphere_spin PRIVATE
        module_raytracer
        module_patterns
        module_shapes)

add_executable(texture_map texture_map.c)
target_link_libraries(texture_map PRIVATE
  module_raytracer
  module_patterns
  module_shapes)
