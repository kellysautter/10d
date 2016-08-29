# Zeidon 10d 

## Compiling

### Requires
* Visual Studio 2015 Community Edition

### Environment
Run vcvarsall.bat to set VS environment (Look in "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat")
Set following environment variables
kzd = install drive.  Defaults to 'c'.
kzv = Zeidon tools version.  Defaults to '10d'.

### Compile
Run MAKEALL.BAT.

## Compiling issues
### 'Access is denied' on an executable like qspawn.exe
Check the permissions on the executable.  It looks like git might not set the permissions correctly on checkout.  Your local user must have read + execute authority
