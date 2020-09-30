# raytracer-c

[![CI Actions Status](https://github.com/twistdroach/raytracer-c/workflows/CI/badge.svg)](https://github.com/twistdroach/raytracer-c/actions)

Current executables (and sample output) are:

## main/parabola

simple demonstration of tuple implementation, shooting a projectile with gravity & wind vectors...

![](images/parabola.png)

## main/sphere_shadow

Matrix implementation allow us to cast rays...and intersect them with a sphere

![](images/sphere_shadow.png)

## main/phong_render_sphere

Phone shading!

![](images/phong_render_sphere2.png)

## main/phong_render_three_spheres

Multiple objects, but they are all spheres...Some are simply scaled to look flat.

![](images/phong_render_three_spheres.png)

## main/phong_render_three_spheres (with shadows)

Same executable but now shadows are added...

![](images/phong_render_three_spheres_with_shadows.png)

## main/phone_render_three_sphere_and_plane

Slightly newer executable but now with a plane instead of "squashed spheres"

![](images/phong_render_three_spheres_and_plane.png)

## main/phong_render_three_spheres_and_striped_plane

Very similar but now the plane has stripes.

![](images/phong_render_three_spheres_and_striped_plane.png)

## main/reflection

Now the central sphere has a reflection...

![](images/reflection.png)

## main/refraction

Added a sphere with some refraction.  Need to make a better demo image.

![](images/refraction.png)

## main/refraction_cube

Added support for cubes, also allowing an object to opt out of shadow calculation.

![](images/refraction_cube.png)

## main/refraction_cylinder

Added support for cylinders...

![](images/refraction_cylinder.png)

## main/render_obj

Added support for triangles, loading obj files...

Command takes two parameters, the first is the file to load, the second is optional and allows you to scale loaded object.

![](images/render_obj.png)

## main/render_obj

Now with normal interpolation...starting to slow down with lots of triangles/reflection/refraction...

![](images/glass_teapot.png)

An obvious performance improvement would be to add some group bounding, so we don't have to check all triangles for intersections when we aren't even close.

In any event, flamegraphs are a fun way to see what is going on.
![](images/glass_teapot_flamegraph.svg)
