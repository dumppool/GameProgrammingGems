Title:    Game Network Debugging with Smart Packet Sniffers
Date:    September 28, 2007
Author:    David L. Koenig
Email:    yarnhammer@hotmail.com
URL:    http://www.rancidmeat.com

//
// Basic Information
//

You should have the following files:
- PacketSniffer.exe
- plug-ins/SimProtocol.dll
- SimClient.exe
- SimServer.exe
- example1.xsnf (an example capture)
- example2.xsnf (an example capture)

Full source code to these applications can be found in the source code directory.
Visual Studio 2005 projects are included.

If you get errors regarding missing DLLs, then you may need to install the following:
- Required/vcredist_x86.exe

If you are running a 64-bit operating system, you may need to install:
- Required/x64/vcredist_x64.exe

//
// Usage:
//

Step 1: Install winpcap.

The installer for WinPcap is located in the "Required" directory.
Run WinPcap_4_0_1.exe and follow the instructions.


Step 2: Start PacketSniffer.exe

Step 3: Select a network device from the drop down menu.

**Note when running under Windows Vista:
You must run the packet sniffer as administrator in order for it to
detect your installed network capture drivers.

Step 4: Start SimServer.exe on one machine.

Step 5: Press the "Start" button on the packet sniffer.

**Note: you may need to turn off promiscuous mode if you're using a wireless
network adapter.

Step 6: Start SimClient.exe on a different machine.

You should see some packet information scrolling on the command line applications.  You should also see
traffic on the packet sniffer.

That should do it.


If you have any questions or problems, please feel free to email me.

-dave