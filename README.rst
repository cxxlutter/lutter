Lutter
======
A simple raytracer using C++11, created in collaboration in a thread_ on
`c-plusplus.net`__.

.. _thread: https://www.c-plusplus.net/forum/330421-full
.. __:  https://www.c-plusplus.net

External Requirements
---------------------
For export to `.bmp`, TrippyColor's Lunch-Time-Tracer is needed in
`../Lunch-Time-Tracer`.  You can clone that from upstream_ or my fork_.

.. _upstream: https://github.com/TrippyColors/Lunch-Time-Tracer
.. _fork: https://github.com/cxxlutter/Lunch-Time-Tracer

Compilation using SCons
-----------------------
If you have SCons_ and GCC_ installed, you can compile this project with a
simple

.. _SCons: http://www.scons.org/
.. _GCC: https://gcc.gnu.org/

.. code:: bash

          $ scons

for debug mode or

.. code:: bash

          $ scons mode=release

for a release build.
