# INXLib v0.1

/*****************************************************************************
 INXlib v0.1
 A simple skeleton framework for building X11 windowed applications with XLib.
 It includes an OpenGL context for 3D graphics.
 Copyright 2016 Ioannis Nompelis
 *****************************************************************************/

This is a very simple "library" to be used as a skeleton for building graphical
applications for the X Window environment using X11 XLib functionality. It is
meant to hide the complexity of opening up a window using XLib and getting
access to an OpenGL drawing context. The GL drawing context can be direct
(using 3D acceleration) or indirect (most likely using MESA as the software
library for the OpenGL API). A standard (very common) unix font of fixed width
is also loaded and is used to create OpenGL display lists for "drawing" text
on the GL context. The library also allows for mouse and keyboard events to
be trapped from the X server and be handled by appropriate (user-defined)
functions.

More importantly, this library is meant to guide the user through the process
of building a graphical application that requires interaction with the user.

The documentation can be built with Doxygen, and it contains only the very
essentials.

IN 2016/09/06

