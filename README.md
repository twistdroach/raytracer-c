# raytracer-c

[![CI](https://github.com/twistdroach/raytracer-c/workflows/CI/badge.svg?branch=master)](https://github.com/twistdroach/raytracer-c/actions?query=workflow%3ACI)
[![codecov](https://codecov.io/gh/twistdroach/raytracer-c/branch/master/graph/badge.svg)](https://codecov.io/gh/twistdroach/raytracer-c)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/twistdroach/raytracer-c.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/twistdroach/raytracer-c/context:cpp)

Current executables (and sample output) are:

## demo/parabola

simple demonstration of tuple implementation, shooting a projectile with gravity & wind vectors...

![](images/parabola.png)

## demo/sphere_shadow

Matrix implementation allow us to cast rays...and intersect them with a sphere

![](images/sphere_shadow.png)

## demo/phong_render_sphere

Phone shading!

![](images/phong_render_sphere2.png)

## demo/phong_render_three_spheres

Multiple objects, but they are all spheres...Some are simply scaled to look flat.

![](images/phong_render_three_spheres.png)

## demo/phong_render_three_spheres (with shadows)

Same executable but now shadows are added...

![](images/phong_render_three_spheres_with_shadows.png)

## demo/phone_render_three_sphere_and_plane

Slightly newer executable but now with a plane instead of "squashed spheres"

![](images/phong_render_three_spheres_and_plane.png)

## demo/phong_render_three_spheres_and_striped_plane

Very similar but now the plane has stripes.

![](images/phong_render_three_spheres_and_striped_plane.png)

## demo/reflection

Now the central sphere has a reflection...

![](images/reflection.png)

## demo/refraction

Added a sphere with some refraction.  Need to make a better demo image.

![](images/refraction.png)

## demo/refraction_cube

Added support for cubes, also allowing an object to opt out of shadow calculation.

![](images/refraction_cube.png)

## demo/refraction_cylinder

Added support for cylinders...

![](images/refraction_cylinder.png)

## demo/render_obj

Added support for triangles, loading obj files...

Command takes two parameters, the first is the file to load, the second is optional and allows you to scale loaded object.

![](images/render_obj.png)

## demo/render_obj

Now with normal interpolation...starting to slow down with lots of triangles/reflection/refraction...

![](images/glass_teapot.png)

An obvious performance improvement would be to add some group bounding, so we don't have to check all triangles for intersections when we aren't even close.

In any event, flamegraphs are a fun way to see what is going on.
![](images/glass_teapot_flamegraph.svg)

## demo/csg

Add some constructive solid geometry - aka CSG...this is a sphere with a cubice bite taken out of it.

![](images/csg.png)
