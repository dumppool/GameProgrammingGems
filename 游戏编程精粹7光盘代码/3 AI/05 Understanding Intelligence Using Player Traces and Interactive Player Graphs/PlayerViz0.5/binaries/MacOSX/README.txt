Dependencies:
FLTK 1.1 (NOT FLTK 2.0) dynamic libraries (.dylib) must be installed.  

You can get FLTK at: http://fltk.org

Once you download fltk1.1.x you must do the following:
- open Terminal and run "./configure --enable-shared" in the fltk directory.  
- Once it configures successfully, you can run "make" and then "make install"

If you have problems getting it to compile in this way, you can download "CMake" (http://cmake.org) and use that as follows:
- open Terminal and run "cmake ." in the fltk directory
- then run "ccmake ." and make sure you turn ON the shared libraries option
- press 'c' and then 'g'
- now you can run "make" and "make install"

