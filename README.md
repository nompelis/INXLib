# INXLib v0.4

 INXlib v0.4
 A simple skeleton framework for building X11 windowed applications with XLib.
 It includes an OpenGL context for 3D graphics.
 Copyright 2016-2023 Ioannis Nompelis

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


Two simple sample programs have been added to the repository to demonstrate
how to use this library and what can be achieved with it and its simplicity.
The first is a demo of what could become a simple game with a bouncing ball.
One function is missing functionality to detect simple collisions such that
the game is fully funcitonal; this is left as an exercise, and I welcome any
implementations that finish the demo, and promise an appropriate reward.
The second shows how one can use textures to display images. I have used a
set of tomographic images to build this; the images are not included in the
distribution at the moment.

IN 2017/05/25


Modified the initial "setup" function for the main window to accept arguments
to control behavriou that was hard-wired in v0.1.

IN 2018/12/26


Linux users using Nvidia drivers for the X11 setup will encounter a problem
when attempting to run the existing "test.c" (or similar demo) that invokes
the X window setup like so:

   iret = xwindow_setup( &xvars, -1, -1, -1, -1, 0, 0, 0 );

The Nvidia drivers do not operate with "indirect" (non-hardware-accelerated)
rendering, and will fail with an obscure error. Modify the driver code to setup
the X window with direct rendering enabled:

   iret = xwindow_setup( &xvars, -1, -1, -1, -1, 0, 1, 0 );

This is NOT presently the default in the driver code.
IN 2019/04/10


The AMD drivers on Slckware64 15.0 (2022 release) also fail with indirect
rendering. The following is the detault for setting up the window:

   iret = xwindow_setup( &xvars, -1, -1, -1, -1, 0, 1, 0 );

Updated the code to remove the X font handlign file, and combined everything
in a single file.

IN 2023/05/12
