# INXLib v0.5

 INXlib v0.5
 A simple skeleton framework for building X11 windowed applications with XLib's
 GLX OpenGL context for high-performance (hardware accelerated) 3D graphics.
 Copyright 2016-2023 Ioannis Nompelis

This is a very simple "library" to be used as a skeleton for building graphical
applications for the X Window environment using X11 XLib/GLX functionality. It
is meant to hide the complexity of opening up a window using XLib and getting
access to an OpenGL drawing context. The GL drawing context can be direct
(using 3D acceleration) or indirect (most likely using MESA as the software
library for the OpenGL API). A standard (very common) unix font of fixed width
is also loaded and is used to create OpenGL display lists for "drawing" text
on the GL context. The library also allows for mouse and keyboard events to
be trapped from the X server and be handled by appropriate (user-defined)
functions.

More importantly, this library is meant to guide the user through the process
of building a graphical application that requires interaction with the user.

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
both worlds.

NOTE2: Most of this would not have happened had I not had assistance from
my dear friend, ChatGPT.

The documentation can be built with Doxygen, and it contains only the very
essentials.

IN 2023/11/14
