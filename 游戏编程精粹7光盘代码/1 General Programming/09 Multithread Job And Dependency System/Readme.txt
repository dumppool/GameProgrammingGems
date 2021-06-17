Project: Multithread job and dependency system
Date: JUne 5, 2007
Author: Hamaide Julien - julien.hamaide@gmail.com

Requirements:
-------------
This project is a Visual Studio .NET 2005 solution file.

The library is only implemented for windows platform.

Description:
------------
This project contains the system presented in the articles. The thread and job
type are not implemented. The job selection algorithm is only based on priority.
A single threaded version of the system is also provided. To enable it, just 
remove the __OPTION_MULTITHREAD__ in the preprocessor directives.

The code is taken from a larger code base, a simple version of reference counting 
is provided. Class name may seems strange, but they are only part of existing package.

The example is quite simple but contains everything you need to start experimenting 
with the system.

Remark:

While the code is compiling fine with Windows XP Platform XDK, it seems some problems occurs with Vista Platform SDK.
New SDK headers define some functions differently – the problem is discussed here: http://www.thescripts.com/forum/thread656230.html
Thanks to Martin Fleiz for having pointed it out