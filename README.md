xPL Hal (lite)
==============

The goal of this project is to get an xPL HAL server running on an embedded ARM device, such as a linux-based router or a NAS. To this end, this code is written in C++ and compiles with GCC. Additionally, to make the code portable, it uses the POCO libraries, as they should be cross platform.

Currently, it's semi-working. In the least, it supports the most common parts of the determinator spec, and the needed parts of XHCP. You should be able to talk to it using the Windows XHCP client, or using telnet. For more info, check the recent changelog.

To build, use cmake:
```bash
cd build
cmake ../src/CmakeLists.txt
make
``` 

Then try the executable with:
```bash
./xplhallite
```

What is xPL / an xPLHAL server?
-------------------------------

xPL is a home automation protocol that is medium independent, but almost all of the software using it does do with the xPL messages delivered over UDP packets. For example, I have a desk lamp that sends packets containing xPL messages when it's front buttons are pressed, and changes brightness when the right xPL messages are received.

An xPLHAL server is the 'brains' of an xPL network, providing a central place on a network to manage device configuraion, simple rules (called 'determinators'), and more complex scripting. In the example with a desk lamp, it is what might connect the bottom button on a panel with the action of turning off a lamp.


Useful Links
------------
XPL_Message_Schemas
http://xplproject.org.uk/wiki/index.php?title=XPL_Message_Schema
