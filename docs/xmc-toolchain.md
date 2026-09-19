# How to get the Toolchain of the XMC running on your system

This is a guide to help you getting the XMC toolchain running.
**Please note: Only native GNU/Linux systems can be fully supported, since we use Debian and Arch based Distros.**
Thus, we recommend installing GNU/Linux native stand alone or as dualboot.

Nevertheless, working with a GNU/Linux VM is fine for the most part.
A Mac OS guide for coding for the XMC is also attached (for Apple Silicon), however, the third assignment requires X-Server running, so testing it is not easily possible on Mac OS

This guide is structured as follows:

1. Instructions for Linux install as well as test, usage and debugging help
2. Tutorial to setup a VM
3. Tutorial to setup dual boot with Windows and Mac-Os (Intel Based)
4. Tutorial to install the Compiler and Libraries on Mac OS Monterey

--------------------------------------------------------------------------------------

## 1 - Installation on GNU/Linux

### For newer Debian based systems (including Ubuntu, Kubuntu, etc), Version 20.04 or newer
1. Install the toolchain
  * Open a Terminal and create the folder `/opt/gcc-arm-none-eabi`
  ```
    sudo mkdir /opt/gcc-arm-none-eabi
  ```
  * Download [AArch32 bare-metal target (arm-none-eabi) toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/downloads) for your architecture (most likely `x86_64 Linux hosted cross toolchains`) and move it to `/opt/gcc-arm-none-eabi`. Extract the archive afterwards.
  ```
    tar -xvf <filename of arm-none-eabi>
  ```
  * Include the path to the new toolchain into the PATH environment variable, by adding `PATH=$PATH:/opt/gcc-arm-none-eabi/gcc-arm-<yourVersion>-arm-none-eabi/bin/` at the end of the file `~/.profile`.

2. Download the DEB Installer for the [J-Link Software and Documentation Pack](https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack) in the correct architecture (32-bit or 64-bit).
    ```
     sudo dpkg -i <filename you just downloaded>
    ```

3. Install XMClib
  * Install the XMClib. This will extract the contents of the .deb package into your root file system, so they end up in `/opt/XMClib`.
    ```
     sudo dpkg -i <filename of package>
    ```

Make sure the folders are readable/executable by the users intended to use the toolchain.

### For older Debian based systems (including Ubuntu, Kubuntu, etc), Version 18.04 or older

1. Add the PPA from ARM for their `gcc` port and install the `gcc-arm-embedded` package:
```
   sudo add-apt-repository ppa:team-gcc-arm-embedded/ppa
   sudo apt-get update
   sudo apt-get install gcc-arm-embedded
```
2. Download the DEB Installer for the [J-Link Software and Documentation Pack](https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack) in the correct architecture (32-bit or 64-bit).
   Install `jlink` via
```
   sudo dpkg -i <filename you just downloaded>
```

3. Install the xmclib package you have received with this HowTo via
```
   sudo dpkg -i <filename of package>
```

### For arch based systems

1. Install `arm-none-eabi-gcc` and `arm-none-eabi-newlib` from official repository.
2. Install `jlink` meta package from AUR.
3. Install the XMClib .deb package
 E.g. convert with
```
debtap xmclib-<version>.deb
```
and install it with
```
sudo pacman -U xmclib-<version>.pkg.tar.zst
```
4. For access to serial interface: add your user to the group `uucp`:
```
 gpasswd -a username uucp
```
and log out and back in to apply the change.
Information on how to communicate with the device can be found in the serial interface article on the arch wiki.

### For other systems

1. Install the toolchain
  * Download [gcc arm non-eabi toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads) for your architecture and extract to appropriate folder, e.g. `/opt/gcc-arm-none-eabi`.
  * Include the path to the new toolchain into the PATH environment variable, for example by adding `export PATH=$PATH:/opt/gcc-arm-none-eabi/gcc-arm-none-eabi-6-2017-q2-update/bin/` to your `~/.profile` or adding the beforementioned path to `/etc/environment` or whatever is the preferred way to do on your system.

2. Install the SEGGER programming tools
  * Download "J-Link Software and Documentation Pack" from [their website](https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack) in your preferred compression form and install them. (the .deb packet will install at `/opt/SEGGER`)

3. Install XMClib
  * Extract the contents of the .deb package into your root file system, so they end up in `/opt/XMClib`.

Make sure the folders are readable/executable by the users intended to use the toolchain.

### Test, Usage and Debugging

#### Test

Try compiling the given example_project.
1. Go to example_project
2. Run `make`.
   If it doesn't work, recheck your installation process and try getting help [here](https://www.startpage.com).

#### Usage

1. Write your program as you are used to. Be aware of the specialties of embedded programming, e.g. your main.c must never terminate and gets no argc, argv.

2. Copy Makefile and project.mk into project directory.

3. Set up project.mk
  * Adapt path in symbol `XMC_LIBDIR` if you chose another location during installation.
  * Choose a name for the linker description file in symbol `LDname`. What name is not important, but many people use the project's name.
  * If necessary, change remaining settings in symbol `SCFLAGS` according to your wishes.
  * Add filenames of code written by you to symbol `SRCS` and filenames of library code used in your project to symbol `LIBSRCS`. If you have to link precompiled libraries, specify them in symbol `LIBLNK`, e.g. for math operations add `-lm` there.

4. `make` will compile and link the program into an elf file, create a human readable lst file and print the size of the compiled program.

5. `make program` will download the compiled program onto the attached device, overwriting whatever program existed priorly.
   It also resets and runs the device, so the new code is executed immediately.

#### Debugging

1. `make debug` will launch the SEGGER JLinkGDBServer and connect an instance of the `arm-none-eabi-gdb` in TUI mode to it.
   If necessary, the project will be rebuilt and loaded onto the device, no prior programming required.
   A breakpoint at the beginning of main() is placed automatically.

If you are unfamiliar with GDB or the TUI mode, go get a GDB quick reference card from e.g. [utexas.edu](https://users.ece.utexas.edu/~adnan/gdb-refcard.pdf).
GBD offers impressive features once you know how to use it.

--------------------------------------------------------------------------------------

## 2 - Tutorial to set up a Virtual Machine for programming assignments in Embedded Systems and Security

Following tutorial contains a short description on how to setup **Ubuntu 20.04** as a virtual machine (VM) in VirtualBox.
Tools for solving programming assignments will be installed as well.
If you already use some GNU/Linux distro there is no need to install a VM.
Follow the tutorial for direct install.

### Setup VM
#### Preparation

* Download and install the packages *VirtualBox platform package* and *VirtualBox Oracle VM VirtualBox Extension Pack* for your operating system (OS) with following link [Link](https://www.virtualbox.org/wiki/Downloads)
!Warning! Note the licence agreements of Oracle for the *VirtualBox Oracle VM VirtualBox Extension Pack* [Link](https://www.virtualbox.org/wiki/VirtualBox_PUEL), in particular that its use is only permitted for educational and personal use.

* Download Ubuntu 20.04 64.bit iso [Link](https://releases.ubuntu.com/20.04/). Please selet the "Desktop image". This is a live environment for installing Ubuntu.


#### Start Installation

In windows double-click on the downloaded Virtual-box exe. Follow the instructions of the installer.
No features need to be modified.

For other OS follow installations instructions [here](https://www.virtualbox.org/manual/UserManual.html#installation).

Do the same for the extension pack.

#### Create Ubuntu-VM

Click on New and proceed:

    Name: <as you want>
    Machine Folder: Where you want to save it
    Typ: Linux
    Version: Ubuntu (64bit)


    Memory size: Size of RAM used by VM. This depends on your PCs RAM-size. Ubuntu recomends 2GB (2048MB) but you can give more if you want.
    Hard drive: Create a virtual hard disk now. VDI and Dynamically allocated with size 10GB or more.
Click on create.

To further improve performance, you can change:
    System->Processor and increase cores
    Display->Screen:
          Increase video memory
          For better display in window mode set graphics controller to VBoxVGA

If there are further questions for setting up the VM go to [Link](https://www.virtualbox.org/manual/UserManual.html) for a very length documentation.

#### Installing Ubuntu
Now you can start the VM the first time to install Ubuntu.

* If an error pops up (Virtualization, Hardware acceleration, VT-x,..) enable Virtualization in BIOS most likely under Security tab. BIOS menus are very diverse so best google how to find the correct option on your computer.*

As start medium navigate to the directory where you saved the Ubuntu-iso and boot it.
Now the Ubuntu live environment will boot. Select a language and follow the installer.
Under *Updates and other Software* do a minimalist install to save memory.

You can tick automatic login option to save some time during bootup.
Also tick **Remember password** since it is necessary for installing software later.

Note: During installation the VM windows is typically quite small.
If you cannot see some buttons scroll the VM-screen.

    With View at the top of the VM-Window you can change to Scaled-Mode or Full-Screen-Moode


After the installation update all packages.
Open a terminal for this with Crtl+Alt+T or search for *Terminal* in application search.

Type ``sudo apt update`` to check if installed packages are up to date and ``sudo apt upgrade`` to install newer versions.


#### Installation of Guest additions
This is not necessary but offers some advantages for example to use the system clipboard from host and vm, drag-and-drop and improved mouse usage.

Install it in the VM window under Devices->Insert Guest Additions CD.
In the opening window download everything offerd (if necessary) select that software should be executed.
Next the root-passwort need to be entered (same as sudo password).

After a successfull install shutdown VM. Now it is possible to activate the standard GPU-controller VMSVGA and 3D-acceleration and restart the VM.
The drive can be removed - if not done automatically - and some more functions of the Guest addition can be selected in a the menu under devices after a successfull installation


#### Installation of required software

Use either nano or gedit as a text editor.
If you are brave, you can also install vim (Exit it with ``Escape :q`` ;)).
Other recommendet editors are Visual Studio Code, Geany, Atom or many more.

Most of them can be install via

    ``sudo apt install <package-name>``

or by downloading the *.deb package and installing it with

    ``sudo dpkg -i <*.deb>``

Also install following packages:

* **build-essential** : for make,gcc,usw.
* **python3-serial** : Python Package for serial interface required for the deliverables.

Next follow th **Direct Installation** instructions you can find on moodle to setup the XMC4500 toolchain.
If you want copy-paste the commands pasting in the terminal-emulator works with Strg+Shift+V.

#### Further changes to the VM

For one deliverable you need to change some minor things in Ubuntu.
Add your user to the dialout group with the following commands.

    ``sudo adduser $USER dialout`` to add your user.
    ``groups $USER `` to check if your user is in the dialout group.

If this did not work, replace ``$USER`` with your user name you can find with the command ``users`` as well.

#### Forward the XMC board

In the toolbar of the started VM select Devices->USB and hand over the necessary USB-device(s) to the VM.

To do this as default, select USB settings of your VM in the VirtualBox Manager. You cann add a filter for the USB-devices here.

--------------------------------------------------------------------------------------

## 3 - Dual boot on Windows

### System requirements:

  * Windows already installed
  * 4 GB system memory
  * 25 GB of free hard drive space
  * USB-drive with 4 GB or larger


To set up dual boot on a **MAC-OS** a very detailed description of the installation is given at [Link](https://www.youtube.com/watch?v=kRgKlcm1XPI).

The essential steps are:

  * Download a *bootable drive maker*.
  * Create a bootable USB-drive.
  * Browse for *Disk Utility* and create two additional partitions for Linux and Swap which meet the above given system requirements.
  * Continue the installation as described in section [Installation of Linux](#Installation_of_Linux).


### 1. Preparation of hard drive:
 1. Before starting the installation, do a backup of all important data in case of a power cut during installation.
 2. From your windows machine, enter the *disk-manager* by pressing [Win + R] and type *diskmgmt.msc* to get an overview of the disk space currently under usage.
 3. If Windows is the only operating system installed on your hard drive, you should locate the  *system reserved* and the  *data* partition on your hard drive.
 4. Make sure that there are at least 25 GB of free disk space left on your hard drive. If the current partitioning of your hard drive uses the entire disk space, do a shrinking of your data partition. This can be achieved with a right-click on the corresponding data partition followed by the selection of *Shrink Volume*.
 5. The resulting unlocked disk space will subsequently serve as the location of the Linux installation. For the sake of simplicity, the unlocked disk space does not need any further formatting and partitioning. During installation of Linux, the free disk space is recognized and formatted automatically.


### 2. Disable quick start:
 1. Disable the *quick start* option of windows by pressing [Win + R]. Enter *powercfg.cpl* to get to the windows  *power settings*.
 2. Browse for *Change settings that are currently unavailable* and get admin rights by clicking at *Change settings that are currently unavailable*.
 3. Disable *Turn on fast startup (recommended)*.


### 3. Preparation of USB-drive:
 1. In order to safely and easily flash the image of the operating system to the USB drive, download  *Balea Etcher* from [Link](https://www.balena.io/etcher/). Make sure that you download the Windows-version of Balea Etcher.
 2. Download the desired Linux operating system. We recommend Ubuntu 20.04 since the toolchain has proven to work proberly under this distribution. Download and install the latest Desktop image version of Ubuntu 20.04 LTS under [Link](https://releases.ubuntu.com/20.04/).
 3. Plug in an empty USB-drive and start Balea Etcher. In the following, the USB-drive serves as a pure boot medium and is for that reason formatted as an initial step. For that 	      reason, do a backup of all important data located on this USB-drive.
 4. Flash the image to the USB-drive. Depending on the size of the selected image, this step might take several minutes.


### 4. Installation of Linux:<a name="Installation_of_Linux"></a>
1. Plug in the USB-drive containing the Linux operation system and restart your
computer.
2. While starting up, press [F12] repeatedly to enter the boot menu (Depending on your computer the corresponding key might differ from bootloader to bootloader).
3. You should now be able to select your USB-drive as boot medium.
4. Follow the installation until you reach *Updates and other software*.
5. By default, the option *Install beside Windows* should be selected. Do not change
these settings and continue the installation.


### 5. Installation of required software

Use either nano or gedit as a text editor.
If you are brave, you can also install vim (Exit it with ``Escape :q`` ;)).
Other recommended editors are Visual Studio Code, Geany, Atom or many more.

Most of them can be installed via

    sudo apt install <package-name>

or by downloading the *.deb package and installing it with

    sudo dpkg -i <*.deb>


Also install the following packages:

* **build-essential** : for make,gcc,usw.
* **python3-serial** : Python Package for serial interface required for the deliverables.

Next follow the **Direct Installation** instructions you can find on moodle to setup the XMC4500 toolchain.

--------------------------------------------------------------------------------------

## 4 - Install the XMC Toolchain on macOS (Apple Silicon)

These instructions apply to Apple Silicon Macs, including M1, M2, M3, and M4.

### Prerequisites

Install Xcode Command Line Tools if `make` or `git` is unavailable:

```
xcode-select --install
```

Install [Homebrew](https://brew.sh/). If `brew` is not found afterwards, add it to your shell environment:

```
eval "$(/opt/homebrew/bin/brew shellenv)"
```

### Install the compiler

```zsh
brew install --cask gcc-arm-embedded
```

### Install SEGGER J-Link

Download and install the Apple Silicon or universal macOS package from the [SEGGER J-Link Software and Documentation Pack](https://www.segger.com/downloads/jlink/).

If `JLinkExe` is not on `PATH`, locate it with:

```
find /Applications -type f -name JLinkExe 2>/dev/null
```

### Install XMClib

Clone XMClib:

```
sudo mkdir -p /opt/XMClib
sudo git clone https://github.com/PromyLOPh/xmclib.git /opt/XMClib
```

Set `XMC_LIBDIR` in `project.mk`:

```
XMC_LIBDIR = /opt/XMClib
```

### Apply required XMClib fixes

The XMClib source contains an XMC4500 system-file include error. If the build stops with `fatal error: XMC4700.h: No such file or directory`, replace the XMC4700 headers with their XMC4500 equivalents:

```bash
sudo sed -i '' \
  -e 's/#include <XMC4700.h>/#include <XMC4500.h>/' \
  -e 's/#include "system_XMC4700.h"/#include "system_XMC4500.h"/' \
  /opt/XMClib/CMSIS/Infineon/XMC4500_series/Source/system_XMC4500.c
```

### Build, program, and debug

```
make
make program
make debug
```
