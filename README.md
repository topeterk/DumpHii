
  # UEFI EDKII Shell application **DumpHii**

UEFI EDKII Shell application that stores all available HII packages

## Features
To run this shell application just execute it without command line options:
> DumpHii

A folder named **HPK** will be created to store the HII dump.  
For each HII package that is found a ***.hpk** file gets created.  
The filename is the GUID of that HII package.  
> Note: Only the loaded packages that are installed on the system can be dumped.  
> Means you may have to enter the Setup menu first, boot the shell afterwards and then run the application.  
> Otherwise the Setup may not get loaded and the HII database has not installed the desired forms.

## Get the software
There are no special releases available yet. Just go ahead and get latest source.

## Installation
The shell application must be compiled for the specific target system.

### Systemrequirements
This source is based on [EDKII](https://github.com/tianocore/edk2) which is required to build this shell application.

### How to integrate into EDKII
It is recommended to place the software at **ShellPkg/Application/DumpHii/** within the EDKII source.  
Add this to your DSC file like _ShellPkg.dsc_ , _EmulatorPgk.dsc_ or _Nt32.dsc_ under the **[Components]** section:
>&nbsp;&nbsp;ShellPkg/Application/DumpHii/DumpHii.inf
  
  
  
> Praise the sun!  :sunny: . . . :fire: . . .  :running: :dash: 