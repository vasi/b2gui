Basilisk II build with working fullscreen on Lion
====

Currently building on Lion doesn't seem to work; only building on Snow Leopard has been tested. The build is mainly [according to E-Maculation](http://www.emaculation.com/doku.php/compiling_sheepshaver_basilisk), **except**:

* Use SDL from its [Mercurial repository](http://www.libsdl.org/hg.php), since it has Lion fixes.
* Force an i386 build: `export CC="gcc -arch i386" CXX="g++ -arch i386"` before configuring.

Patches
---

* SDL breaks if SDL_UpdateRect is called on a child thread, so we make sure it's only run on the main thread.
* Video-on-SegFault (VOSF) is broken for unknown reasons, and it doesn't seem necessary, so it's disabled.
