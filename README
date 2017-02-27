DB8FS RGBLightControl
=====================

1. Introduction
---------------

This project is about a hardware module for creating light effects
using the red-, green- and blue-components of light emitting diodes
(LEDs). This hardware module is based on an Atmel ATmega16
microcontroller and provides the possibility for managing 4 independent
RGB-modules. A module is a collection of one red-, one green- and one
blue channel. Each module uses a common anode and an output voltage of
12V DC. Therefore the common anode is connected to 12V DC and each
color channel has to be switched against GND for light emission at the
particular channel (we assume having the right dimensioned series
resistors for current limitation integrated in the led module).
At ATmega16 level we use pulse-width-modulation for each module's channels
to mix an emitted color light.

2. Features of the RGBLightControl PCB
--------------------------------------

• Atmel ATmega16 microcontroller with 16kB flash and bootloader
  support
• USB interface (FT232RL) for managing the colors using a PC
• parallel control of 4 independent RGB-LED modules (e.g. IKEA DIODER)
• color control using RGB- or HSI triples!
• 7-bit software PWM for seperate light intensity controlling of each
  channel!
• support for 36kHz-RC5 IR remote controls
• support for 3 pushbuttons
• integrated RGB-LED for status display
• pushbuttons are connected with ADC ports of the ATmega16 ->
  measuring of analog quantities possible (e.g. temperatures)
• Input voltage: 12V DC
• firmware completely written in AVR Assembler, therefore we have the
  features of: high speed, less memory, low crystal frequencies and
  low power consumance
• AVRISP connector (soon bootloader support via USB)

3. Requirements
---------------

3.1 PCB:

Schematic and board layout are completely designed using the freeware
layout editor Eagle (Cadsoft) which is available for MS Windows and
GNU/Linux at http://www.cadsoft.de.

3.2 Firmware:

To change and modify the firmware you may use any Atmel AVR Assembler
you want. For convenience purposes I used Atmel's AVRStudio 4,
available at http://www.atmel.com, but you are free to use any
compatible assembler.

3.3 Software

The software is completely written in C++ as a makefile project and
uses the GNU utilites for building an executable. The additional
packages you should have installed for successful compilation and
linking are some libraries of the Boost project
(http://www.boost.org), in especially Boost ASIO, Boost Spirit,
Boost Filesystem and Boost Thread. In Debian Linux you should therefore
install the packages 'libboost-system-dev' and 'libboost-thread-dev' and 
'libboost-filesystem-dev'.

For being able to communicate with the RGBLightControl pcb via USB, be
sure to enable the modules 'usbserial' and 'ftdi_sio' in your linux
kernel configuration. You should also ensure having the privileges for
accessing the /dev/ttyUSB* devices as a simple user (Debian: dialout
group).

The RGBLightControl software package also provides an integrated webserver 
for remote controlling the complete behaviour using standard web technologies.
For this purpose, the boost::asio-based HTTP server "webEm" is used, a
component written J. Bremner and C. Kohlhoff for proving include and action 
code parsing. The original "webEm" is licensed under BSD and available at 
Codeproject:
http://www.codeproject.com/KB/IP/webem.aspx

3.4 Web-Frontend

Using HTML5, JavaScript and CSS it should be possible to provide a web 
application for easy starting or programming your favorite mood light using 
the internet browser of your smartphone/tablet! At the moment the HTML5 interface 
is still at a demo-stage using jscolor and prototype.js for AJAX
requests.


4. License
----------

All files in this project are licensed under the GNU General Public
License v3 or later. For more information about this license you
should consult the Free Software Foundation at http://www.fsf.org.


5. Contact
----------

If you have questions, ideas, code, proposals or whatever, feel free
to contact the author via mail. 

Falk Schilling <falk.schilling@gmx.net>
