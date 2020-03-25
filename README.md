## IL2CPPDumper
Dump Lib libil2cpp.so from Memory of Game Process and Generate structure dump.cs

## Features
- No need of Ptrace
- Support Only arm-v7a 32bit
- Support Only IL2CPP Metadata v24
- Dumping of Lib from Memory of Game
- Dumping of Internal Game Structure as .cs file(Need to Find Pointers Manually)
- Support Both Fast & Slow Dumping

## How to Build
- Install Android NDK, if not.
- Open Shell in This Folder
- Use and Execute ndk-build of NDK Here
- Output will be in libs Folder.
 
## How to use
- Needs Root Access
- Get Root Shell through Adb or Terminal Apps
- For Usage Help: il2cppdumper -h
- For General Usage: il2cppdumper <option(s)> -o FilePath

## Commands
```
./il2cppdumper -h

IL2CPPDumper v0.1 <==> Made By KMODs(kp7742)
Usage: il2cppdumper <option(s)> -o FilePath
Dump Lib libil2cpp.so from Memory of Game Process and Generate structure dump.cs
@@Unity il2cpp v24 Supported Only@@
 Options:
--Lib Args--------------------------------------------------------------
  -l --lib                              Dump libil2cpp.so from Memory
  -p --package <packageName>            Package Name of Game
  -f --fast                             Enable Fast Dumping
                                        (May Miss Some Bytes in Dump)
--Script Args-----------------------------------------------------------
  -s --script                           Generate structure dump.cs
  -p --package <packageName>            Package Name of Game
  -g --global  <address>                GlobalMetaData Address
  -m --meta    <address>                MetadataRegistration Address
  -c --code    <address>                CodeRegistration Address
--Other Args------------------------------------------------------------
  -o --output <outputFilePath>          Generate file path
  -h --help                             Display this information
```

## Credits 
- [Il2CppDumper-Perfare](https://github.com/Perfare/Il2CppDumper): CS Dumping Code

## Technlogy Communication
> Email: patel.kuldip91@gmail.com
