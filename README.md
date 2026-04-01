
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

## How to build
The shell application must be compiled for the specific target system.

### Systemrequirements
This source is based on [EDKII](https://github.com/tianocore/edk2) which is required to build this shell application.

### How to integrate into EDKII
It is recommended to place the software at **ShellPkg/Application/DumpHii/** within the EDKII source.  
Add this to your DSC file like _ShellPkg.dsc_ , _EmulatorPgk.dsc_ or _Nt32.dsc_ under the **[Components]** section:
>&nbsp;&nbsp;ShellPkg/Application/DumpHii/DumpHii.inf

:bangbang: On newer EDKII versions some checks are executed, e.g. EccCheck which failed on my environment as I just inserted the whole git repository.
To mitigate this, I think, one either have to add the repository as a submodule or as quick fix just delete the .git folder.
> PermissionError: [WinError 5] Access is denied: '<path>\\Build\\.pytool\\Plugin\\EccCheck\\ShellPkg\\Application\\DumpHii\\.git\\objects\\pack\\pack-8b7c25379d5d814f491759643322eb93e76116dd.idx'

### How to build (ShellPgk.dsc)

:bangbang: This was done in 2026 using VS2022 at EDKII commit 0049ac1793c4abd0652652286e5506f52aaaa0d4.

:bangbang: The instructions are not complete, please refert to EDKII documentation for details.
Use these steps just as a toplevel guideline of the major steps.

Get source and create python environment.

```batchfile
git clone https://github.com/tianocore/edk2.git
cd edk2
git submodule update --init
py -m venv .venv
.venv\Scripts\activate.bat
pip install -r pip-requirements.txt --upgrade
```
Copy source to directory and integrate it into the build, see "How to integrate into EDKII".
Not sure if this step is actually needed:

```batchfile
.venv\Scripts\activate.bat
stuart_setup -c .pytool/CISettings.py
python BaseTools/Edk2ToolsBuild.py
```

Run the actual build, like so:

```batchfile
stuart_ci_build -c .pytool/CISettings.py -p ShellPkg -a IA32,X64 TOOL_CHAIN_TAG=VS2022
```

Output files are at this location:  
`Build\Shell\<debug/release>_<toolchain>\<arch>\ShellPkg\Application\DumpHii\DumpHii\OUTPUT\DumpHii.efi`
> debug/release = e.g. RELEASE  
> toolchain = e.g. VS2022  
> arch = e.g. X64

On errors about pythonXXXt.lib not found, one might need to install Python including the free-threaded binaries.

To resolve uncrustify errors, run this command for the failing file, as here for _DumpHii.c_ :
```batchfile
echo ShellPkg\Application\DumpHii\DumpHii.c | .\.pytool\Plugin\UncrustifyCheck\tianocore-uncrustify-release_extdep\Windows-x86\uncrustify.exe -c .\.pytool\Plugin\UncrustifyCheck\uncrustify.cfg -F - --replace --no-backup --if-changed
```
  
  
  
> Praise the sun!  :sunny: . . . :fire: . . .  :running: :dash: 