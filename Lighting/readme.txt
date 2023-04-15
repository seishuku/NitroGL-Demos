Mathematical lighting models demo.

All the shaders are commented (the .tvp and .tfp files), and full source code is included.

Description:
This program demonstrates 4 different lighting models; these lighting
euqations include Diffuse, Blinn, Phong, and Anisotropic.

The calculations are ALL done in the fragment (pixel) shader, which
provides an extreamly high quality output.

For those math heads out there, the actual calcuations that are being done are these:

Diffuse:
I=Base*Kd*Falloff*(N.L)

Blinn:
I=Ka+Base*(Kd*Falloff*(N.L))+(Ks*Gloss*Self*(N.H)^n)

Phong:
I=Ka+Base*(Kd*Falloff*(N.L))+(Ks*Gloss*Self*(R.L)^n)

Anisotropic:
I=Ka+Base*(Kd*Falloff*(N.L))+(Ks*Gloss*Self*(sqrt(1-(L.T)^2)*sqrt(1-(V.T)^2)-(L.T)*(V.T))^n)

Terms:
I = Output intensity
Base = Base color texture (not really used in this demo, it just maps to white.tga)
Gloss = Gloss map (again, not really used here, it maps to alpha in white.tga)
Falloff = Light distance falloff (1-(L*(1/Radius))^2)
Self = Self shadowing ((N.L)*Falloff)
Kd = Diffuse material color
Ks = Specular material color
Ka = Ambient material color
L = Light position
V = View direction
N = Surface normal (per-pixel)
H = Half-Angle ((L+V)/2)
R = Reflection (2.0*(N.V)*N-V)
T = Surface tangnet (per-pixel, derived from normal XYZ->ZYX)


Controls:

Left mouse button + Drag = Rotate
Middle mouse button + Drag = Move
Right mouse button + Drag = Zoom

Control + Left mouse button + Drag = Move light
Control + Right mouse button + Drag = Zoom light

Ecape = Quit demo

Update (6-27-2003):
Fixed a potential bug in the Anisotropic lighting shader (negative value on RdotV.x before the POW).
Thanks to Dan Ginsburg at ATI for catching that.
