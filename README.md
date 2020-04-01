## IL2CPPDumper
Dump Lib libil2cpp.so from Memory of Game Process and Generate structure dump.cs

## Features
- No need of Ptrace
- Support Only arm-v7a 32bit
- Dumping of Lib from Memory of Game
- Fix and Regenerate Elf File
- Support Only IL2CPP Metadata v24 for Dumping Structure
- Dumping of Internal Game Structure as .cs file(Need to Find Pointers Manually)
- Support Both Fast & Slow Dumping

## How to Build
- Install Android NDK, if not.
- Open Shell in This Folder
- Use and Execute ndk-build of NDK Here
- Output will be in libs Folder.
 
## How to use
- Needs Root Access or Virtual Space
- Get <Root Shell through Adb or Terminal Apps> or <Normal Shell into Virtual Shell via Terminal Apps>
- For Usage Help: il2cppdumper -h
- For General Usage: il2cppdumper <option(s)> -o OutputPath

## Commands
```
./il2cppdumper -h

IL2CPPDumper v0.2 <==> Made By KMODs(kp7742)
Usage: il2cppdumper <option(s)> -o OutputPath
Dump Lib libil2cpp.so from Memory of Game Process and Generate structure dump.cs
@@Unity il2cpp v24 Supported Only@@
 Options:
--Lib Args--------------------------------------------------------------
  -l --lib                              Dump libil2cpp.so from Memory
  -p --package <packageName>            Package Name of Game
  -r --raw(Optional)                    Output Raw Lib and Not Rebuild It
  -f --fast(Optional)                   Enable Fast Dumping
                                        (May Miss Some Bytes in Dump)
--Script Args-----------------------------------------------------------
  -s --script                           Generate structure dump.cs
  -p --package <packageName>            Package Name of Game
  -g --global  <address>                GlobalMetaData Address
  -m --meta    <address>                MetadataRegistration Address
  -c --code    <address>                CodeRegistration Address
--Other Args------------------------------------------------------------
  -o --output <outputPath>              File Output path
  -h --help                             Display this information
```

## Credits
- [SoFixer](https://github.com/F8LEFT/SoFixer): So(Elf) Rebuilding
- [Il2CppDumper-Perfare](https://github.com/Perfare/Il2CppDumper): CS Dumping Code

## Technlogy Communication
> Email: patel.kuldip91@gmail.com
