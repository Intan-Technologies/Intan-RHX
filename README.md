# Intan-RHX
Intan RHX is free, powerful data acquisition software that displays and records electrophysiological signals from any Intan RHD or RHS system using an RHD USB interface board, RHD recording controller, or RHS stim/recording controller.

The most recent binaries are available under Releases:

IntanRHXInstaller.exe -> Windows 64-bit installer

IntanRHX.dmg -> MacOS 64-bit disk image

IntanRHX.tar.gz -> Linux 64-bit archive file

These binaries were built with Qt 5.12.8.

STEPS TO RUN SOFTWARE

All Platforms:

Various files need to be present in the same directory as the binary executable at runtime. These include kernel.cl, ConfigRHDController.bit, ConfigRHDInterfaceBoard.bit, ConfigRHSController.bit, ConfigXEM6010Tester.bit, USBEvaluationBoard.bit.

Windows:

Opal Kelly USB drivers should be installed so that the Intan hardware can communicate via USB. These are available at: https://intantech.com/files/Intan_controller_USB_drivers.zip and are automatically installed when running IntanRHXInstaller.exe. These also rely on the Microsoft Visual C++ Redistributables (x64) from 2010, 2013, and 2015-2019, which are available from Microsoft and should also be installed prior to running IntanRHX. Finally, okFrontPanel.dll (found in the libraries directory) should be in the same directory as the binary executable at runtime.

Mac:

libokFrontPanel.dylib should be in a directory called "Frameworks" alongside the MacOS directory within the built IntanRHX.app. Running macdeployqt on this application will also populate this directory with required Qt libraries.

Linux:

A udev rules file should be added so that the Intan hardware can communicate via USB. The 60-opalkelly.rules file should be copied to /etc/udev/rules.d/, after which the system should be restarted or the command 'udevadm control --reload-rules' should be run. libokFrontPanel.so should be in the same directory as the binary executable at runtime. 
