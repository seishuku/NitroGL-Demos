GPU Particle Systems with OpenGL 3.3 (core)

Mouse Controls:
Left button - Rotate
Middle button - Pan
Right button - Zoom

Keyboard Controls:
Arrow up/down - Increase/decrease particle count by 1000
Spacebar - Pause particle system

Tech info:
Creates my burning 'NitroGL' logo using a completely GPU
generated/updated/rendered particle system.

Features a fragment shader based large scale particle system,
only limited by the graphics RAM and max allowed texture size.

Uses 4 RGBA 32bit floating point textures to hold the positional
and velocity data, processsing using the usual ping-pong method.

The particle system is rendered as simple points, sampled from
the particle system position texture using a vertex texture fetch
in the vertex shader, then passed to a geometry shader where quads
are generated and billboarded.
After that a simple fragment shader samples the particle texture
and outputs the system as you see it on the screen.

Update:
Now uses assigned materials in the 3D model to assign colors to the particle emitters!