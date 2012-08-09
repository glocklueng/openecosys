Copyright (C) IntRoLab 2009-2011 
http://introlab.gel.usherbrooke.ca
Dominic Letourneau, ing. M.Sc.A.

Web Site
________
http://www.openecosys.org


Twitter
_______
@openecosys


Mailing Lists
_____________

openecosys-info@lists.sourceforge.net (Info)
openecosy-devel@lists.sourceforge.net (Developer Support)

ABOUT
_____
OpenECoSys/NetworkViewer is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.
OpenECoSys/NetworkViewer is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
OpenECoSys/NetworkViewer. If not, see http://www.gnu.org/licenses/.

RELEASE NOTES
_____________ 

1.1.16
______
* Updated to qwt 6.x
* Better time stamping of data


1.1.15
________
* Support multiple (simultaneous) interfaces at the same time
* Fixed Linux baud rate settings
* dsPIC bootloader plugin support
* Separation of the communication and core functionalities and the GUI started. This will enable
  console mode NetworkViewer.
* Minor GUI fixes

1.1.14
________

* Added delay when opening serial port with Arduino/ChipKit to avoid being stuck in the bootloader
* Added syntax highlight for Javascript / ScriptEngine 
* Updated QextSerialPort to latest version


1.1.13
________

* Added Arduino Analog Example.
* Fixed Plot performances.
* Fixed module ConfigurationView not displaying initial value when editing.
* Fixed saving & restoring variables from a module (writing back stored values).

1.1.12.1
________

* Fixed Variable Configuration View to prevent overwriting user value when editing.

1.1.12
______

* Added configuration name in XML files
* Fixed ConfigurationView to send variable update events even if it is the same value.
* Fixed QextSerialPort compilation for Qt 4.7.3
* Added preferences (will remember last used device)
* ScopeView rework. Much nicer integration with Qwt, removed space consuming variable list.
* Script Plugin can now save the current script
* Adding variables automatically in variable ConfigurationView
* Saving / configuration in variable ConfigurationView
* Putting back activate / deactivate buttons in ConfigurationView


1.1.11
______

* Fixed enable / disable variables
* Added enable / disable all
* (re)Load / Save XML configuration working 
* Layout improvements

1.1.10
______

* Fixed interface handler reloading crash
* Performance enhancement for variable view
* Fixed SerialBridge implementation
* Fixed filename when editing filename manually with the Logger plugin
* (Internal) Now using the NETV convention for names like the NETVProtocolStack.

1.1.9
_____

* Fixed configuration reload. Module Configuration View will be automatically updated. 
  Scheduler will reschedule new variables.
* Removed (some) debug information
* Fixed plot colors when changing colors
* Allowed drag & drop on Configuration Views
* Performance enhancement of Module Configuration View



NOTES FOR MAC OSX
_________________
You need to install Qt 4.7.x SDK before using NetworkViewer. 
Visit http://qt.nokia.com/downloads to download the SDK.


