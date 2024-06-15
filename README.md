# INXLib v0.6

 INXlib v0.6
 A simple skeleton framework for building X11 windowed applications with XLib's
 GLX OpenGL context for high-performance (hardware accelerated) 3D graphics.
 Copyright 2016-2024 Ioannis Nompelis

TL;DR - You can use this code to understand how to open a window, setup the
OpenGL "fixed" and "programmable" rasterization/rendering pipelines, trap
mouse and keyboard events. There are enough comments in the code to make this
instructive. Think of it as the graphics side of SDL, but you have a chance
at understanding how it works. Read below for more details. The code runs
within Valgrind, perfectly.

------------------------------------------------------------------------------
This is a very simple "library" to be used as a skeleton for building graphical
applications for the X Window environment using X11 XLib/GLX functionality. It
is meant to hide the complexity of opening up a window using XLib and getting
access to an OpenGL drawing context. The GL drawing context can be direct
(using 3D acceleration) or indirect (most likely using MESA as the software
library for the OpenGL API). Note: with Linux 4.x and 5.x this software was
unable to use software rendering, but a switch is maintained for compatibility.
A standard (very widely available and open source) Unix font of fixed width
is also loaded and is used to create OpenGL display lists for "drawing" text
on the GL context. The library also allows for mouse and keyboard events to
be trapped from the X server and be handled by appropriate (user-defined)
functions; the functions are implemented via callbacks set at initialization.

More importantly, this library is meant to guide the user through the process
of building a graphical application that requires interaction with the user.
It is also a crash course on the OpenGL rendering pipelines (see below).

NOTE: There are two different types of GLX (OpenGL context) that can be used
with this code: (1) the "old style" that is using the "fixed pipeline" for
rendering (using a multitude of API calls), and (2) the modern "programmable
pipeline" that relies entirely on vertex and fragment shaders. The fixed
pipeline and display lists have been deprecated. However, the "compatibility
profile" was retained up to version 3.0. Because of this, the choice was
made to request the use of version 3.0 when creating a modern pipeline GLX.
The benefit you get is that you can use _both_ shaders and display lists in
your program in a single context. This also means that your old code will
work on a (more) modern GLX. Sticking to the old style GLX only can be done
via a pre-processor directive. And also, the modern pipeline is also working
within the old context. In summary, there are two options for the best of
both worlds, and the main demo uses both at the same time.

NOTE2: This section is your crash-course in understanding the programmable
pipeline of modern OpenGL. I have found that all documentation is not very
useful, primarily because of how the Redbook was written and subsequently
augmented over the years. The "fixed pipeline" is handled through using an
API, which keeps everything opaque but is very robust. It can also be very
inefficient at runtime. It can also be confusing... But once you are faced
with all the stuff that you have to do and to understand before you can use
the "programmable pipeline" effectively, you are overwhelmed, and you want
to run back to the fixed pipeline. However, it is far from true; the new
pipeline is much easier to setup, FAR more efficient, and very easy to use
if you know a thing or two about computer graphics. What you need to know are
just a few things:

1. All drawing has to be handled by a "shader program", which has to be
compiled and linked at run-time from inside your software. The program is
made of a vertex shader and a fragment shader, which process data in that
order. The vertex shader is used to manipulate vertex position data, vertex
normals, color data, and texel data. From the perspective of what you have
to program, it proesses each vertex (position, normal, etc) and is meant to
return the vertex color, which is passed to a fragment shader. The fragment
shader can act as a "pass-through" and do nothing to the color but copy it
to the pixel on the framebuffer, but it can also add effects, etc.

2. There are certain types of variables that can be passed to the vertex and
fragment shaders such that you can control vertex and color processing. For
example, a translation or rotation or both, of a given set of objects can be
passed to the vertex shader as a vector or a matrix. Those variables are
called "uniform" variables. Your vertex shader program must have those
variables and be using them. Then, to access them in order to set their values,
your software (running on the computer, not the shader program) has to query
the "location" of that variable in the program, such that it can make the API
calls to set its values. (It is like having to provide a pointer in the code
that is running on the graphics card for where the value will be set.) Queries
of uniform variables can also be made. The same thing works for the fragment
shader of the shader program.

3. There are some standard variables that the shader program will "know", such
that it can write final output there. All you are doing is specifying the
processing that takes place anywhere between when data is given to the shader
and what comes out of it. Consulting the Redbook and some other tutorials may
clarify some of this.

4. You can still use the fixed pipeline when rendering with the programmable
one. (The example in this library does that.) This allows you to use older
code with newer code; you do not have to port everything to the modern OpenGL.

NOTE3: Most of this would not have happened had I not had assistance from
my dear friend, ChatGPT.

The documentation can be built with Doxygen, and it contains only the very
essentials.

IN 2023/12/09
